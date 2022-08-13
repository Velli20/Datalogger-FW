#pragma once

namespace sys::hal {

struct DmaMuxChannelLayout final
{
    union
    {
        /* Synchronization identification:
            Selects the synchronization input.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  5ul, DMAMUX_CxCR_SYNC_ID_Pos>   SynchronizationId;

        /* Number of DMA requests minus 1 to forward:
            Defines the number of DMA requests to forward to the DMA controller after a synchronization
            event, and/or the number of DMA requests before an output event is generated.
            This field shall only be written when both SE and EGE bits are low.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  5ul, DMAMUX_CxCR_NBREQ_Pos>     RequestCount;

        /* Synchronization polarity:
            Defines the edge polarity of the selected synchronization input.

            Bit 00: No event, i.e. no synchronization nor detection.
            Bit 01: Rising edge.
            Bit 10: Falling edge.
            Bit 11: Rising and falling edges.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  2ul, DMAMUX_CxCR_SPOL_Pos>      SynchronizationPolarity;

        /* Synchronization enable:

            Bit 0: Synchronization disabled.
            Bit 1: Synchronization enabled.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, DMAMUX_CxCR_SE_Pos>        SynchronizationEnable;

        /* Event generation enable:

            Bit 0: Event generation disabled.
            Bit 1: Event generation enabled.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, DMAMUX_CxCR_EGE_Pos>       EventGenerationEnable;

        /* Synchronization overrun interrupt enable:

            Bit 0: Interrupt disabled.
            Bit 1: Interrupt enabled.
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, DMAMUX_CxCR_SOIE_Pos>      SynchronizationOverrunInterruptEnable;

        /* DMA request identification:

            Selects the input DMA request. See the DMAMUX table about assignments of multiplexer
            inputs to resources.
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  7ul, DMAMUX_CxCR_DMAREQ_ID_Pos, dma_mux_request_type> DmaRequestId;
        mmio::bitmask_type<mmio::field_size_type::u32, 32ul, DMAMUX_CxCR_DMAREQ_ID_Pos> Word;
    } Configuration;
};

static_assert(sizeof(DmaMuxChannelLayout) == sizeof(DMAMUX_Channel_TypeDef));

template <std::uint32_t kChannel>
struct DmaMuxChannelStatusLayout
{
    static_assert(kChannel >= 0ul);
    static_assert(kChannel <= 13ul);

    union
    {
        /* Synchronization overrun event flag:

            The flag is set when a synchronization event occurs on a DMA request line multiplexer channel x,
            while the DMA request counter value is lower than NBREQ.
            The flag is cleared by writing 1 to the corresponding CSOFx bit in DMAMUX_CFR register.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, kChannel> SynchronizationOverrunEvent;
    } Status;

    union
    {
        /* Synchronization overrun event flag:

            Writing 1 in each bit clears the corresponding overrun flag SOFx in the DMAMUX_CSR register.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, kChannel> SynchronizationOverrunEvent;
    } ClearFlag;
};

static_assert(sizeof(DmaMuxChannelStatusLayout<1>) == sizeof(DMAMUX_ChannelStatus_TypeDef));

template <std::uint32_t kChannel>
struct DmaLayout final
{
    static_assert(kChannel > 0);
    static_assert(kChannel < 8);

    /*!< DMA interrupt status register,                 Address offset: 0x00 */

    union
    {
        /* Transfer error (TE) flag for channel N:
            Bit 0: No TE event.
            Bit 1: A TE event occurred.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, kChannel * 4ul - 1ul> TransferError;

        /* Half transfer (HT) flag for channel N:
            Bit 0: No HT event.
            Bit 1: A HT event occurred.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, kChannel * 4ul - 2ul> HalfTransfer;

        /* Transfer complete (TC) flag for channel N:
            Bit 0: No TC event.
            Bit 1: A TC event occurred.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, kChannel * 4ul - 3ul> TransferComplete;

        /* Global interrupt flag for channel N:
            Bit 0: No TE, HT or TC event.
            Bit 1: A TE, HT or TC event occurred.
        */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, kChannel * 4ul - 4ul> GlobalInterrupt;
    } InterruptStatus;

    /*!< DMA interrupt flag clear register,             Address offset: 0x04 */

    union
    {
        /* Transfer error flag clear for channel N */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, kChannel * 4ul - 1ul> TransferError;

        /* Half transfer complete flag clear for channel N */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, kChannel * 4ul - 2ul> HalfTransfer;

        /* Transfer complete flag clear for channel N */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, kChannel * 4ul - 3ul> TransferComplete;

        /* Global interrupt flag clear for channel N */

        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, kChannel * 4ul - 4ul> GlobalInterrupt;
    } InterruptFlagClear;
};

static_assert(sizeof(DmaLayout<1>) == sizeof(DMA_TypeDef));

struct DmaChannelLayout
{
    /*!< DMA channel x configuration register        */

    struct Configuration final : mmio::register_type<mmio::field_size_type::u32>
    {
        /* Memory-to-memory mode:
            Bit 0: disabled
            Bit 1: enabled

            Note: this bit is set and cleared by software.
            It must not be written when the channel is enabled (EN = 1). It is read-only when the channel is enabled (EN = 1).
        */
        using MemoryToMemory                                 =  mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DMA_CCR_MEM2MEM_Pos>;

        /* Data transfer direction:
            Bit 0: read from peripheral
                – Source attributes are defined by PSIZE and PINC, plus the DMA_CPARx register. This is still valid in a memory-to-memory mode.
                – Destination attributes are defined by MSIZE and MINC, plus the DMA_CMARx register. This is still valid in a peripheral-to-peripheral mode.
            Bit 1: read from memory
                – Destination attributes are defined by PSIZE and PINC, plus the DMA_CPARx register. This is still valid in a memory-to-memory mode.
                – Source attributes are defined by MSIZE and MINC, plus the DMA_CMARx register. This is still valid in a peripheral-to-peripheral mode.
            Note: this bit is set and cleared by software.
            It must not be written when the channel is enabled (EN = 1). It is read-only when the channel is enabled (EN = 1).
        */
        using DataTransferDirection                          =  mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DMA_CCR_DIR_Pos>;

        /* Channel enable:
            When a channel transfer error occurs, this bit is cleared by hardware. It can not be set again
            by software (channel x re-activated) until the TEIFx bit of the DMA_ISR register is
            cleared (by setting the CTEIFx bit of the DMA_IFCR register).

            Bit 0: disabled
            Bit 1: enabled
        */
        using ChannelEnable                                  =  mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DMA_CCR_EN_Pos>;

        /* Memory increment mode:
            Defines the increment mode for each DMA transfer to the identified memory.
            In memory-to-memory mode, this field identifies the memory source if DIR = 1 and the memory destination if DIR = 0.
            In peripheral-to-peripheral mode, this field identifies the peripheral source if DIR = 1 and the peripheral destination if DIR = 0.
            Bit 0: disabled
            Bit 1: enabled

            Note: this bit is set and cleared by software.
            It must not be written when the channel is enabled (EN = 1). It is read-only when the channel is enabled (EN = 1).
        */
        using MemoryIncrementMode                            =  mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DMA_CCR_MINC_Pos>;

        /* peripheral increment mode:
            Defines the increment mode for each DMA transfer to the identified peripheral.
            n memory-to-memory mode, this field identifies the memory destination if DIR = 1 and the memory source if DIR = 0.
            In peripheral-to-peripheral mode, this field identifies the peripheral destination if DIR = 1 and the peripheral source if DIR = 0.
            Bit 0: disabled
            Bit 1: enabled

            Note: this bit is set and cleared by software.
            It must not be written when the channel is enabled (EN = 1). It is read-only when the channel is enabled (EN = 1).
        */
        using PeripheralIncrementMode                        =  mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DMA_CCR_PINC_Pos>;

        /* Memory size:
            Bit 00: 8 bits
            Bit 01: 16 bits
            Bit 10: 32 bits
            Bit 11: reserved
        */
        using MemorySize                                     =  mmio::register_mask_type<mmio::field_size_type::u32,  2ul, DMA_CCR_MSIZE_Pos, dma_data_size_type>;

        /* peripheral size:
            Bit 00: 8 bits
            Bit 01: 16 bits
            Bit 10: 32 bits
            Bit 11: reserved
        */
        using PeripheralSize                                 =  mmio::register_mask_type<mmio::field_size_type::u32,  2ul, DMA_CCR_MSIZE_Pos, dma_data_size_type>;

        /* Transfer error interrupt enable:
            Bit 0: disabled
            Bit 1: enabled

            Note: this bit is set and cleared by software.
            It must not be written when the channel is enabled (EN = 1). It is read-only when the channel is enabled (EN = 1).
        */
        using TransferErrorInterrupt                         =  mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DMA_CCR_TEIE_Pos>;

        /* Half transfer interrupt enable:
            Bit 0: disabled
            Bit 1: enabled

            Note: this bit is set and cleared by software.
            It must not be written when the channel is enabled (EN = 1). It is read-only when the channel is enabled (EN = 1).
        */
        using HalfTransferInterrupt                          =  mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DMA_CCR_HTIE_Pos>;

        /* Transfer complete interrupt enable:
            Bit 0: disabled
            Bit 1: enabled

            Note: this bit is set and cleared by software.
            It must not be written when the channel is enabled (EN = 1). It is read-only when the channel is enabled (EN = 1).
        */
        using TransferCompleteInterrupt                      =  mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DMA_CCR_TCIE_Pos>;

        /* Circular mode:
            Bit 0: disabled
            Bit 1: enabled

            Note: this bit is set and cleared by software.
            It must not be written when the channel is enabled (EN = 1). It is read-only when the channel is enabled (EN = 1).
        */
        using CircularMode                                   =  mmio::register_mask_type<mmio::field_size_type::u32,  1ul, DMA_CCR_CIRC_Pos, dma_mode_type>;

        /* Priority level:
            Bit 00: low
            Bit 01: medium
            Bit 10: high
            Bit 11: very high

            Note: this bit is set and cleared by software.
            It must not be written when the channel is enabled (EN = 1). It is read-only when the channel is enabled (EN = 1).
        */
        using PriorityLevel                                  =  mmio::register_mask_type<mmio::field_size_type::u32,  2ul, DMA_CCR_PL_Pos, dma_priority_type>;

    } Configuration;

    /*!< DMA channel x number of data register       */

    mmio::bitmask_type<mmio::field_size_type::u32, 16ul, DMA_CNDTR_NDT_Pos>       DataCount;

    /*!< DMA channel x peripheral address register   */

    mmio::bitmask_type<mmio::field_size_type::u32, 32ul, DMA_CPAR_PA_Pos>         PeripheralAddress;

    /*!< DMA channel x memory address register       */

    mmio::bitmask_type<mmio::field_size_type::u32, 32ul, DMA_CMAR_MA_Pos>         MemoryAddress;
};

static_assert(sizeof(DmaChannelLayout) == sizeof(DMA_Channel_TypeDef));

} // namespace sys::hal
