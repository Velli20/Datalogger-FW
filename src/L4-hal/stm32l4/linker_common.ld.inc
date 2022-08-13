SECTIONS
{
    /* Section for the interrupt vectors */
    .vector_table : SUBALIGN(4)
    {
        PROVIDE_HIDDEN(__vector_table_start = .);
        KEEP(*(.vector_table) )
        KEEP(*(.vector_table*))
        PROVIDE_HIDDEN(__vector_table_end = .);
    } > VECTOR_TABLE

    /* Main stack. */
    .main_stack (NOLOAD) : ALIGN(8)
    {
        PROVIDE(MainStackBottom = .);
        . += 2K;
        PROVIDE(MainStackTop = .);
    } > RAM

    .application_heap (NOLOAD) : ALIGN(8)
    {
        /* Define a global symbol at application heap start */
        PROVIDE(__application_heap_start = .);
        . += 20K;
         /* Define a global symbol at application heap end */
        PROVIDE(__application_heap_end = .);
    } > RAM

    /* The program code and other data goes into FLASH */
    .text : ALIGN(4)
    {
        /* .text sections (executable code) */
        /* .gnu.linkonce hold C++ elements with vague linkage
            https://gcc.gnu.org/onlinedocs/gcc/Vague-Linkage.html */
        *(.text .text.* .gnu.linkonce.t.*)

        /**
         *  @ref https://github.com/bjornfor/stm32-test/blob/master/stm32.ld
         *  @ref https://en.wikipedia.org/wiki/ARM_architecture#Cores
         *  @ref https://developer.arm.com/docs/dui0471/k/interworking-arm-and-thumb/about-interworking
         *
         *  glue_7 and glue_7t are for thumb/arm interworking, which is automatically
         *  assumed for ARM architecture later than ARMv5T, the architecture for
         *  cortex-m series is ARMv6 ~ v8-M, which is later than ARMv5T.
         */

        /* glue_7 and glue_7t hold helper functions emitted by the compiler to
           support interworking (linking between functions in ARM and THUMB
           mode). Note that Cortex-M's do not support ARM mode, but this is left
           here to save someone headache if they ever attempt to port Tock to a
           Cortex-A core.  */

        /* ARM-Thumb interworking veneers (arm to thumb code) */
        *(.glue_7)
        /* ARM-Thumb interworking veneers (thumb to arm code) */
        *(.glue_7t)
        /* Section unwinding and source language info */
        /**
         *  @ref https://ppt.cc/fX4N4x
         *  @ref https://github.com/bjornfor/stm32-test/blob/master/stm32.ld#L71
         *  @ref document [EHABI](http://docplayer.net/51529403-Exception-handling-abi-for-the-arm-architecture.html)
         *
         *  .ARM.extab (exception table) is similar to .eh_frame (exception handling),
         *  both are used for exception handling, except that the former is only used
         *  on 32-bit machine. For architecture after ARMv8, .eh_frame is used instead
         *  of .ARM.extab.
         */
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        *(.eh_frame)

        /* Create a global symbol for stack section */
        // PROVIDE(__stack_start      = ADDR(RAM) + SIZEOF(RAM) - 8K;
        // PROVIDE(__stack_end        =  ADDR(RAM) + SIZEOF(RAM);

        /* Create a global symbol for data section */
        PROVIDE(__data_start       = ADDR(.data));
        PROVIDE(__data_end         = ADDR(.data) + SIZEOF(.data));
        PROVIDE(__data_load_start  = LOADADDR(.data));
        PROVIDE(__data_load_end    = LOADADDR(.data) + SIZEOF(.data));

        /* Create a global symbol for bss section */

        /* Constructor and destructor sections:

           - init/fini
              Defined by ELF as sections that hold `process
              initialization/termination code`
           - {pre}{init/fini}_array_{start/end}
              Symbols used by the C runtime for initialization / termination
           - ctors/dtors
              Symbols used by the C++ runtime for initialization / termination
        */

        KEEP(*(.init))
        KEEP(*(.fini))

    } > FLASH

    /*
     * RODATA section hold read-only data (constants, strings, ...)
     */
    .rodata : ALIGN(4)
    {
        *(.rodata)
        *(.rodata*)
        *(.gnu.linkonce.r.*)
    } > FLASH

    /* ARM Exception support
     *
     * This contains compiler-generated support for unwinding the stack,
     * consisting of key-value pairs of function addresses and information on
     * how to unwind stack frames.
     * https://wiki.linaro.org/KenWerner/Sandbox/libunwind?action=AttachFile&do=get&target=libunwind-LDS.pdf
     *
     * .ARM.exidx is sorted, so has to go in its own output section.
     *
     * @ref   https://github.com/gcc-mirror/gcc/blob/master/libgcc/unwind-arm-common.inc#L104
     * @ref   https://stackoverflow.com/questions/9752000/exidx-start-and-exidx-end-what-do-they-do
     *
     * __exidx_start and __exidx_end symbols are used internally by call-chain unwinding
     *  mechanism in GCC that takes control during exception handling phase.
     */

    /* ARM executables use an .ARM.exidx section to look up an exception
     * handling record for the current instruction pointer.
     */

    .ARM.exidx : ALIGN(8)
    {
        PROVIDE_HIDDEN(__exidx_start = .);
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        PROVIDE_HIDDEN(__exidx_end = .);
    } > FLASH

    /*
     * The preinit code, i.e. an array of pointers to initialization
     * functions to be performed before constructors.
     * SORT is used for sections that have strict
     * init/de-init ordering requirements.
    */
    .preinit_array :  ALIGN(4)
    {
        PROVIDE(__preinit_array_start = .);
        KEEP(*(.preinit_array))
        KEEP(*(SORT(.preinit_array.*)))
        PROVIDE(__preinit_array_end = .);
    } > FLASH

    /*
     * The init code, i.e. an array of pointers to static constructors.
     *
     * INIT_ARRAY section holds an array of function pointers to call for
     * C++ static constructors and C library initialization code.
     */
    .constructors :  ALIGN(4)
    {
        PROVIDE(__init_array_start = .);
        KEEP(*(.init_array))
        KEEP(*(SORT(.init_array)))
        PROVIDE(__init_array_end = .);
    } > FLASH

    /*
     * The fini code, i.e. an array of pointers to static destructors.
     *
     * FINI_ARRAY section holds an array of function pointers to call for
     * C++ static destructors C library finalization code.
     */
    .destructors : ALIGN(4)
    {
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(.fini_array))
        KEEP(*(SORT(.fini_array.*)))
        PROVIDE_HIDDEN(__fini_array_end = .);
    } > FLASH

    /*
     * DATA section holds initialized variables. Initialisation values are
     * stored in FLASH (LMA) and copied to SRAM (VMA) on startup.
     */
     /* Data that needs to be initialized to a value different than 0 */
    .data : ALIGN(8)
    {
        /* .data sections (initialized variables) */
        *(.data)
        *(.data*)
        . = ALIGN(8);
     } > RAM AT > FLASH

    /* BSS section holds uninitialized variables. It is set to zero on startup. */
    /* Data that needs to be initialized to 0 */
    .bss (NOLOAD) : ALIGN(8)
    {
        /* define a global symbol at bss start */
        PROVIDE(__bss_start = .);

        *(.bss)
        *(.bss*)
        /*
         * Collect symbols that do not have a particular input section. See
         * https://sourceware.org/binutils/docs/ld/Input-Section-Common.html
         */
        *(COMMON)
        PROVIDE(__bss_end = .);
    } > RAM

    .segger_rtt :
    {
        . = ALIGN(4);
        KEEP(*(.segger_rtt))
        . = ALIGN(4);
    } > RAM_SEGGER_RTT

    /*
     * NOINIT section holds uninitialized variables that should not be set to
     * zero on startup.
     */
    .noinit (NOLOAD) :
    {
        . = ALIGN(4);
        KEEP(*(.noinit))
        . = ALIGN(4);
    } > RAM

    /* Stabs debugging sections.  */
    .stab          0 : { *(.stab) }
    .stabstr       0 : { *(.stabstr) }
    .stab.excl     0 : { *(.stab.excl) }
    .stab.exclstr  0 : { *(.stab.exclstr) }
    .stab.index    0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment       0 : { *(.comment) }

    /*
     * DWARF debug sections.
     * Symbols in the DWARF debugging sections are relative to the beginning
     * of the section so we begin them at 0.
     */
    /* DWARF 1 */
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    /* GNU DWARF 1 extensions */
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    /* DWARF 1.1 and DWARF 2 */
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    /* DWARF 2 */
    .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    .debug_macinfo  0 : { *(.debug_macinfo) }
    /* SGI/MIPS DWARF 2 extensions */
    .debug_weaknames 0 : { *(.debug_weaknames) }
    .debug_funcnames 0 : { *(.debug_funcnames) }
    .debug_typenames 0 : { *(.debug_typenames) }
    .debug_varnames  0 : { *(.debug_varnames) }

    /* Remove information from the standard libraries */
    /*
     * DISCARD section.
     * Discard input sections. See [1], �3.6.7 "Output Section Discarding"
     * https://sourceware.org/binutils/docs/ld/Output-Section-Discarding.html#Output-Section-Discarding
     */
    /* /DISCARD/ :
    {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    } */

    .ARM.attributes 0 :
    {
        *(.ARM.attributes)
    }

    .freertos_heap (NOLOAD) : ALIGN(4) SUBALIGN(4)
    {
        *(.freertos_heap*);
    } > RAM

    .freertos_kernel (NOLOAD) : ALIGN(8) SUBALIGN(8)
    {
        *(.freertos_kernel*);
    } > RAM

    .application_heap (NOLOAD) : ALIGN(4) SUBALIGN(4)
    {
        *(.application_heap*);
    } > RAM
}
