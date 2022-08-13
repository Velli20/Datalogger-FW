#pragma once

namespace sys::hal {

struct UsartLayout
{
    /*!< USART Control register 1,                 Address offset: 0x00 */

    union
    {
        mmio::bitmask_type<mmio::field_size_type::u32, 32ul, 0ul>                            Word;
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_RXFFIE_Pos>           RxFifoFullInterrupt;
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_TXFEIE_Pos>           TxFifoEmptyInterrupt;
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_FIFOEN_Pos>           FifoMode;
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_M1_Pos>               WordLengthBit1;     /* This bit must be used in conjunction with bit 12 (M0) to determine the word length. This bit can only be written when the USART is disabled (UE = 0). */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_M0_Pos>               WordLengthBit0;     /* This bit is used in conjunction with bit 28 (M1) to determine the word length. This bitfield can only be written when the USART is disabled (UE = 0). */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_EOBIE_Pos>            EndOfBlockInterrupt;
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_RTOIE_Pos>            ReceiverTimeoutInterrupt;
        mmio::bitmask_type<mmio::field_size_type::u32,  5ul, USART_CR1_DEAT_Pos>             DriverEnableAssertionTime;
        mmio::bitmask_type<mmio::field_size_type::u32,  5ul, USART_CR1_DEDT_Pos>             DriverEnableDeassertionTime;

        /* Oversampling mode
            Bit 0: Oversampling by 16.
            Bit 1: Oversampling by 8.
            This bitfield can only be written when the USART is disabled (UE = 0).
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_OVER8_Pos>             OversamplingMode;
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_CMIE_Pos>              CharacterMatchInterrupt;
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_MME_Pos>               MuteMode;    /* This bit enables the USART Mute mode function. When set, the USART can switch between active and Mute mode, as defined by the WAKE bit. It is set and cleared by software. 0: Receiver in active mode permanently 1: Receiver can switch between Mute mode and active mode. */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_WAKE_Pos>              ReceiverWakeUp;   /* This bit determines the USART wakeup method from Mute mode. It is set or cleared by software. 0: Idle line 1: Address mark  */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_PCE_Pos>               ParityControl;
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_PS_Pos>                ParitySelection;     /* Bit 0: Even parity. Bit 1: Odd parity. This bitfield can only be written when the USART is disabled (UE = 0).*/
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_PEIE_Pos>              ParityErrorInterrupt;
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_TXEIE_TXFNFIE_Pos>     TxFifoNotFullInterrupt;
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_TCIE_Pos>              TransmissionCompleteInterrupt;
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_RXNEIE_RXFNEIE_Pos>    RxFifoNotEmptyInterrupt;
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_IDLEIE_Pos>            IdleInterrupt;
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_TE_Pos>                TransmitterEnable;
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_RE_Pos>                ReceiverEnable;
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_UESM_Pos>              LowPowerMode;   /* Bit 0: USART not able to wake up the MCU from low-power mode. Bit 1: USART able to wake up the MCU from low-power mode. */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR1_UE_Pos>                UsartEnable;     /* Bit 0: USART prescaler and outputs disabled, low-power mode. Bit 1: USART enabled */
    } Control1;

    /*!< USART Control register 2,                 Address offset: 0x04 */

    union
    {
        mmio::bitmask_type<mmio::field_size_type::u32, 32ul, 0ul>                             Word;
        mmio::bitmask_type<mmio::field_size_type::u32,  8ul, USART_CR2_ADD_Pos>               UsartNodeAddress;
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR2_RTOEN_Pos>             ReceiverTimeoutEnable;

        /* Auto baud rate mode:
            00: Measurement of the start bit is used to detect the baud rate.
            01: Falling edge to falling edge measurement (the received frame must start with a single bit = 1 and Frame = Start10xxxxxx).
            10: 0x7F frame detection.
            11: 0x55 frame detection.
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  2ul, USART_CR2_ABRMODE_Pos>           AutoBaudRateMode;
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR2_ABREN_Pos>             AutoBaudRateEnable;

        /* Most significant bit first:
            Bit 0: data is transmitted/received with data bit 0 first, following the start bit.
            Bit 1: data is transmitted/received with the MSB (bit 7/8) first, following the start bit.
            This bitfield can only be written when the USART is disabled (UE = 0).
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR2_MSBFIRST_Pos>          MostSignificantBitFirst;

        /* Binary data inversion:
            Bit 0: Logical data from the data register are send/received in positive/direct logic. (1 = H, 0 = L).
            Bit 1: Logical data from the data register are send/received in negative/inverse logic. (1 = L, 0 = H).
            This bitfield can only be written when the USART is disabled (UE = 0).
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR2_DATAINV_Pos>           BinaryDataInversion;

        /* TX pin active level inversion:
            Bit 0: TX pin signal works using the standard logic levels (VDD =1/idle, Gnd = 0/mark).
            Bit 1: TX pin signal values are inverted (VDD =0/mark, Gnd = 1/idle).
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR2_TXINV_Pos>             TxPinActiveLevelInversion;

        /* RX pin active level inversion:
            Bit 0: RX pin signal works using the standard logic levels (VDD =1/idle, Gnd = 0/mark).
            Bit 1: RX pin signal values are inverted (VDD =0/mark, Gnd = 1/idle).
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR2_RXINV_Pos>             RxPinActiveLevelInversion;

        /* Swap TX/RX pins:
            Bit 0: TX/RX pins are used as defined in standard pinout.
            Bit 1: The TX and RX pins functions are swapped. This enables to work in the case of a cross-wired.
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR2_SWAP_Pos>              SwapPins;

        /* LIN mode enable:
            Bit 0: LIN mode disabled
            Bit 1: LIN mode enabled.
            This bitfield can only be written when the USART is disabled (UE = 0).
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR2_LINEN_Pos>             LinModeEnable;

        /* Stop bits:
            Bit 00: 1 stop bit
            Bit 01: 0.5 stop bit.
            Bit 10: 2 stop bits
            Bit 11: 1.5 stop bits
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  2ul, USART_CR2_STOP_Pos>              stop_bits;

        /* Clock parity:
            Bit 0: Steady low value on SCLK pin outside transmission window.
            Bit 1: Steady high value on SCLK pin outside transmission window.
            This bitfield can only be written when the USART is disabled (UE = 0).
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR2_CPOL_Pos>              ClockParity;

        /* Clock phase:
            Bit 0: The first clock transition is the first data capture edge.
            Bit 1: The second clock transition is the first data capture edge.
            This bitfield can only be written when the USART is disabled (UE = 0).
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR2_CPOL_Pos>              ClockPhase;
    } Control2;

    /*!< USART Control register 3,                 Address offset: 0x08 */

    union
    {
        mmio::bitmask_type<mmio::field_size_type::u32, 32u, 0u>                              Word;

        /* TX FIFO threshold configuration:
            Bit 000: TXFIFO reaches 1/8 of its depth.
            Bit 001: TXFIFO reaches 1/4 of its depth.
            Bit 010: TXFIFO reaches 1/2 of its depth.
            Bit 011: TXFIFO reaches 3/4 of its depth.
            Bit 100: TXFIFO reaches 7/8 of its depth.
            Bit 101: TXFIFO becomes empty.
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  3ul, USART_CR3_TXFTCFG_Pos>           TxFifoThreshold;

        /* RX FIFO threshold interrupt enable:
            Bit 0: Interrupt inhibited.
            Bit 1: USART interrupt generated when Receive FIFO reaches the threshold programmed in RXFTCFG.
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR3_RXFTIE_Pos>            RxFifoThresholdInterruptEnable;

        /* RX FIFO threshold configuration:
            Bit 000: Receive FIFO reaches 1/8 of its depth.
            Bit 001: Receive FIFO reaches 1/4 of its depth.
            Bit 010: Receive FIFO reaches 1/2 of its depth.
            Bit 011: Receive FIFO reaches 3/4 of its depth.
            Bit 100: Receive FIFO reaches 7/8 of its depth.
            Bit 101: Receive FIFO becomes full.
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  3ul, USART_CR3_RXFTCFG_Pos>           RxFifoThreshold;

        /* TX FIFO threshold interrupt enable:
            Bit 0: Interrupt inhibited.
            Bit 1: USART interrupt generated when TXFIFO reaches the threshold programmed in TXFTCFG.
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR3_TXFTIE_Pos>            TxFifoThresholdInterruptEnable;

        /* Wakeup from low-power mode interrupt enable:
            Bit 0: Interrupt inhibited.
            Bit 1: USART interrupt generated whenever WUF = 1 in the USART_ISR register.
            Note: WUFIE must be set before entering in low-power mode.
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR3_WUFIE_Pos>             WakeupFromLowPowerInterruptEnable;

        /* Wakeup from low-power mode interrupt flag selection:
            Bit 00: WUF active on address match (as defined by ADD[7:0] and ADDM7).
            Bit 01: Reserved.
            Bit 10: WUF active on start bit detection.
            Bit 11: WUF active on RXNE/RXFNE.
            This bitfield can only be written when the USART is disabled (UE = 0).
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  2ul, USART_CR3_WUS_Pos>               WakeupFromLowPowerInterruptMode;

        /* DMA Disable on Reception Error:
            Bit 0: DMA is not disabled in case of reception error. The corresponding error flag is set but RXNE is kept 0
                   preventing from overrun. As a consequence, the DMA request is not asserted, so the erroneous data
                   is not transferred (no DMA request), but next correct received data is transferred (used for Smartcard mode).
            Bit 1: DMA is disabled following a reception error. The corresponding error flag is set, as well as RXNE.
                   The DMA request is masked until the error flag is cleared. This means that the software must first disable
                   the DMA request (DMAR = 0) or clear RXNE/RXFNE is case FIFO mode is enabled) before clearing the error flag.
            This bitfield can only be written when the USART is disabled (UE = 0).
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR3_DDRE_Pos>              DmaDisableOnReceptionError;

        /* Overrun Disable:
            Bit 0: Overrun Error Flag, ORE, is set when received data is not read before receiving new data.
            Bit 1: Overrun functionality is disabled. If new data is received while the RXNE flag is still set the ORE flag is
                   not set and the new received data overwrites the previous content of the USART_RDR register.
                   When FIFO mode is enabled, the RXFIFO is bypassed and data is written directly in USART_RDR register.
                   Even when FIFO management is enabled, the RXNE flag is to be used.
            This bitfield can only be written when the USART is disabled (UE = 0).
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR3_OVRDIS_Pos>            OverrunDisable;

        /* DMA enable transmitter:
            Bit 0: DMA mode is enabled for transmission.
            Bit 1: DMA mode is disabled for transmission.
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR3_DMAT_Pos>              TransmitterDmaEnable;

        /* DMA enable receiver:
            Bit 0: DMA mode is enabled for reception.
            Bit 1: DMA mode is disabled for reception.
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR3_DMAR_Pos>              ReceiverDmaEnable;

        /* Error interrupt enable:
            Error Interrupt Enable Bit is required to enable interrupt generation in case of a framing error, overrun
            error noise flag or SPI slave underrun error (FE = 1 or ORE = 1 or NE = 1 or UDR = 1 in the USART_ISR register).

            Bit 0: Interrupt inhibited
            Bit 1: interrupt generated when FE = 1 or ORE = 1 or NE = 1 or UDR = 1 (in SPI slave mode) in the USART_ISR register.
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  1ul, USART_CR3_EIE_Pos>               ErrorInterruptEnable;
    } Control3;

    /*!< USART Baud rate register,                 Address offset: 0x0C */

    union
    {
        mmio::bitmask_type<mmio::field_size_type::u32, 16ul, 0ul>                             Word;
        mmio::bitmask_type<mmio::field_size_type::u32,  4ul, USART_BRR_DIV_FRACTION_Pos>      Fraction;
        mmio::bitmask_type<mmio::field_size_type::u32, 12ul, USART_BRR_DIV_MANTISSA_Pos>      Mantissa;
        mmio::bitmask_type<mmio::field_size_type::u32, 16ul, 0ul>                             Value;
    } BaudRate;

    mmio::register_type<mmio::field_size_type::u16> GuardTimeAndPrescaler; /*!< USART Guard time and prescaler register,  Address offset: 0x10 */
    mmio::register_type<mmio::field_size_type::u16> Reserved2;             /*!< Reserved, 0x12                                                 */
    mmio::register_type<mmio::field_size_type::u32> ReceiverTimeout;       /*!< USART Receiver Time Out register,         Address offset: 0x14 */
    mmio::register_type<mmio::field_size_type::u16> Request;               /*!< USART Request register,                   Address offset: 0x18 */
    mmio::register_type<mmio::field_size_type::u16> Reserved3;             /*!< Reserved, 0x1A                                                 */

    /*!< USART Interrupt and status register,      Address offset: 0x1C */

    union
    {
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, USART_ISR_PE_Pos>                 ParityError;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, USART_ISR_FE_Pos>                 FramingError;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, USART_ISR_NE_Pos>                 NoiseDetection;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, USART_ISR_ORE_Pos>                OverrunError;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, USART_ISR_IDLE_Pos>               IdleLineDetected;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, USART_ISR_RXNE_RXFNE_Pos>         RxFifoNotEmpty;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, USART_ISR_TC_Pos>                 TransmissionComplete;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, USART_ISR_TXE_TXFNF_Pos>          TxFifoNotEmpty;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, USART_ISR_LBDF_Pos>               LinBreakDetection;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, USART_ISR_RTOF_Pos>               ReceiverTimeout;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, USART_ISR_ABRE_Pos>               AutoBaudRateError;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, USART_ISR_BUSY_Pos>               TransmissionOngoing;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, USART_ISR_CMF_Pos>                CharacterMatch;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, USART_ISR_RWU_Pos>                ReceiverWakeUpFromMute;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul, USART_ISR_WUF_Pos>                ReceiverWakeUpFromLowPower;
    } InterruptStatus;

    /*!< USART Interrupt flag Clear register,      Address offset: 0x20 */

    union
    {
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul,  USART_ICR_PECF_Pos>              ParityError;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul,  USART_ICR_FECF_Pos>              FramingError;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul,  USART_ICR_NECF_Pos>              NoiseDetection;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul,  USART_ICR_ORECF_Pos>             OverrunError;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul,  USART_ICR_IDLECF_Pos>            IdleLineDetected;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul,  USART_ICR_TXFECF_Pos>            TxFifoNotEmpty;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul,  USART_ICR_TCCF_Pos>              TransmissionComplete;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul,  USART_ICR_LBDCF_Pos>             LinBreakDetection;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul,  USART_ICR_RTOCF_Pos>             ReceiverTimeout;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul,  USART_ICR_CMCF_Pos>              CharacterMatch;
        mmio::bitmask_type<mmio::field_size_type::u32, 1ul,  USART_ICR_WUCF_Pos>              WakeUpFromLowPower;
        mmio::bitmask_type<mmio::field_size_type::u32, 32ul, 0ul>                             All;
    } InterruptFlagClear;

    /*!< USART Receive Data register,              Address offset: 0x24 */

    mmio::register_type<mmio::field_size_type::u16>                                               ReceiveData;

    /*!< Reserved, 0x26                                                 */

    mmio::register_type<mmio::field_size_type::u16>                                               Reserved4;

    /*!< USART Transmit Data register,             Address offset: 0x28 */

    union
    {
        mmio::bitmask_type<mmio::field_size_type::u16, 9ul, USART_TDR_TDR_Pos, std::uint16_t> DataEightBit;
    } Transmit;

    /*!< Reserved, 0x2A                                                 */

    mmio::register_type<mmio::field_size_type::u16> Reserved5;

    /*!< USART Prescaler register,                 Address offset: 0x2C */

    union
    {
        mmio::bitmask_type<mmio::field_size_type::u32, 32ul, 0ul>                              Word;

        /* Clock prescaler:

            The USART input clock can be divided by a prescaler factor:

            Bit 0000: input clock not divided.
            Bit 0001: input clock divided by 2.
            Bit 0010: input clock divided by 4.
            Bit 0011: input clock divided by 6.
            Bit 0100: input clock divided by 8.
            Bit 0101: input clock divided by 10.
            Bit 0110: input clock divided by 12.
            Bit 0111: input clock divided by 16.
            Bit 1000: input clock divided by 32.
            Bit 1001: input clock divided by 64.
            Bit 1010: input clock divided by 128.
            Bit 1011: input clock divided by 256.
        */
        mmio::bitmask_type<mmio::field_size_type::u32,  4ul, USART_PRESC_PRESCALER_Pos, uart_clock_prescaler_type>       Value;
    } Prescaler;
};

static_assert(sizeof(UsartLayout) == sizeof(USART_TypeDef));

} // namespace sys::hal
