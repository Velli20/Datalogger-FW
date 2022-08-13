#pragma once

namespace sys::arch::arm {

enum class vendor_irqn_type : std::int32_t
{
    /******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
    k_reset                          = -15,
    k_non_maskable_interrupt         = -14,    /*!< 2 Cortex-M4 Non Maskable Interrupt                                */
    k_hard_fault                     = -13,    /*!< 3 Cortex-M4 Hard Fault Interrupt                                  */
    k_memory_managment               = -12,    /*!< 4 Cortex-M4 Memory Management Interrupt                           */
    k_bus_fault                      = -11,    /*!< 5 Cortex-M4 Bus Fault Interrupt                                   */
    k_usage_fault                    = -10,    /*!< 6 Cortex-M4 Usage Fault Interrupt                                 */
    k_service_call                   = -5,     /*!< 11 Cortex-M4 SV Call Interrupt                                    */
    k_debug_monitor                  = -4,     /*!< 12 Cortex-M4 Debug Monitor Interrupt                              */
    k_pending_service_call           = -2,     /*!< 14 Cortex-M4 Pend SV Interrupt                                    */
    k_system_tick                    = -1,     /*!< 15 Cortex-M4 System Tick Interrupt                                */

    /*****  STM32 specific Interrupt Numbers **********************************************************************/
    k_window_watchdog                = 0,      /*!< Window WatchDog Interrupt                                         */
    k_k_pvd_pvm                      = 1,      /*!< PVD/PVM1/PVM2/PVM3/PVM4 through EXTI Line detection Interrupts    */
    k_tamper_and_timestamp           = 2,      /*!< Tamper and TimeStamp interrupts through the EXTI line             */
    k_rtc_wakeup                     = 3,      /*!< RTC Wakeup interrupt through the EXTI line                        */
    k_flash                          = 4,      /*!< FLASH global Interrupt                                            */
    k_rcc_global                     = 5,      /*!< RCC global Interrupt                                              */
    k_external_line0                 = 6,      /*!< EXTI Line0 Interrupt                                              */
    k_external_line1                 = 7,      /*!< EXTI Line1 Interrupt                                              */
    k_external_line2                 = 8,      /*!< EXTI Line2 Interrupt                                              */
    k_external_line3                 = 9,      /*!< EXTI Line3 Interrupt                                              */
    k_external_line4                 = 10,     /*!< EXTI Line4 Interrupt                                              */
    k_dma1_channel1                  = 11,     /*!< DMA1 Channel 1 global Interrupt                                   */
    k_dma1_channel2                  = 12,     /*!< DMA1 Channel 2 global Interrupt                                   */
    k_dma1_channel3                  = 13,     /*!< DMA1 Channel 3 global Interrupt                                   */
    k_dma1_channel4                  = 14,     /*!< DMA1 Channel 4 global Interrupt                                   */
    k_dma1_channel5                  = 15,     /*!< DMA1 Channel 5 global Interrupt                                   */
    k_dma1_channel6                  = 16,     /*!< DMA1 Channel 6 global Interrupt                                   */
    k_dma1_channel7                  = 17,     /*!< DMA1 Channel 7 global Interrupt                                   */
    k_adc1                           = 18,     /*!< ADC1 global Interrupt                                             */
    k_can1_tx                        = 19,     /*!< CAN1 TX Interrupt                                                 */
    k_can1_rx0                       = 20,     /*!< CAN1 RX0 Interrupt                                                */
    k_can1_rx1                       = 21,     /*!< CAN1 RX1 Interrupt                                                */
    k_can1_sce                       = 22,     /*!< CAN1 SCE Interrupt                                                */
    k_external_line9to5              = 23,     /*!< External Line[9:5] Interrupts                                     */
    k_tim1_break_and_tim15_global    = 24,     /*!< TIM1 Break interrupt and TIM15 global interrupt                   */
    k_tim1_update_and_tim16_global   = 25,     /*!< TIM1 Update Interrupt and TIM16 global interrupt                  */
    k_tim1_trigger_and_tim17_global  = 26,     /*!< TIM1 Trigger and Commutation Interrupt and TIM17 global interrupt */
    k_tim1_capture                   = 27,     /*!< TIM1 Capture Compare Interrupt                                    */
    k_tim2                           = 28,     /*!< TIM2 global Interrupt                                             */
    k_tim3                           = 29,     /*!< TIM3 global Interrupt                                             */
    k_tim4                           = 30,     /*!< TIM4 global Interrupt                                             */
    k_i2c1_event                     = 31,     /*!< I2C1 Event Interrupt                                              */
    k_i2c1_error                     = 32,     /*!< I2C1 Error Interrupt                                              */
    k_i2c2_event                     = 33,     /*!< I2C2 Event Interrupt                                              */
    k_i2c2_error                     = 34,     /*!< I2C2 Error Interrupt                                              */
    k_spi_global1                    = 35,     /*!< SPI1 global Interrupt                                             */
    k_spi_global2                    = 36,     /*!< SPI2 global Interrupt                                             */
    k_usart_global1                  = 37,     /*!< USART1 global Interrupt                                           */
    k_usart_global2                  = 38,     /*!< USART2 global Interrupt                                           */
    k_usart_global3                  = 39,     /*!< USART3 global Interrupt                                           */
    k_external_line15to10            = 40,     /*!< External Line[15:10] Interrupts                                   */
    k_rtc_alarm                      = 41,     /*!< RTC Alarm (A and B) through EXTI Line Interrupt                   */
    k_dfsdm1_filter3                 = 42,     /*!< DFSDM1 Filter 3 global Interrupt                                  */
    k_tim8_BRK                       = 43,     /*!< TIM8 Break Interrupt                                              */
    k_tim8_UP                        = 44,     /*!< TIM8 Update Interrupt                                             */
    k_tim8_TRG_COM                   = 45,     /*!< TIM8 Trigger and Commutation Interrupt                            */
    k_tim8_CC                        = 46,     /*!< TIM8 Capture Compare Interrupt                                    */
    k_fmc_global                     = 48,     /*!< FMC global Interrupt                                              */
    k_sdmmc_global                   = 49,     /*!< SDMMC1 global Interrupt                                           */
    k_tim5                           = 50,     /*!< TIM5 global Interrupt                                             */
    k_spi_global3                    = 51,     /*!< SPI3 global Interrupt                                             */
    k_uart_global4                   = 52,     /*!< UART4 global Interrupt                                            */
    k_uart_global5                   = 53,     /*!< UART5 global Interrupt                                            */
    k_tim6_DAC                       = 54,     /*!< TIM6 global and DAC1&2 underrun error  interrupts                 */
    k_tim7                           = 55,     /*!< TIM7 global interrupt                                             */
    k_dma2_channel1                  = 56,     /*!< DMA2 Channel 1 global Interrupt                                   */
    k_dma2_channel2                  = 57,     /*!< DMA2 Channel 2 global Interrupt                                   */
    k_dma2_channel3                  = 58,     /*!< DMA2 Channel 3 global Interrupt                                   */
    k_dma2_channel4                  = 59,     /*!< DMA2 Channel 4 global Interrupt                                   */
    k_dma2_channel5                  = 60,     /*!< DMA2 Channel 5 global Interrupt                                   */
    k_dfsdm1_filter                  = 61,     /*!< DFSDM1 Filter 0 global Interrupt                                  */
    k_dfsdm1_filter1                 = 62,     /*!< DFSDM1 Filter 1 global Interrupt                                  */
    k_dfsdm1_filter2                 = 63,     /*!< DFSDM1 Filter 2 global Interrupt                                  */
    k_comparator                     = 64,     /*!< COMP1 and COMP2 Interrupts                                        */
    k_low_power_tim1                 = 65,     /*!< LP TIM1 interrupt                                                 */
    k_low_power_tim2                 = 66,     /*!< LP TIM2 interrupt                                                 */
    k_otg_filesystem                 = 67,     /*!< USB OTG FS global Interrupt                                       */
    k_dma2_channel6                  = 68,     /*!< DMA2 Channel 6 global interrupt                                   */
    k_dma2_channel7                  = 69,     /*!< DMA2 Channel 7 global interrupt                                   */
    k_low_power_uart1                = 70,     /*!< LP UART1 interrupt                                                */
    k_octospi1                       = 71,     /*!< OctoSPI1 global interrupt                                         */
    k_i2c3_event                     = 72,     /*!< I2C3 event interrupt                                              */
    k_i2c3_error                     = 73,     /*!< I2C3 error interrupt                                              */
    k_serial_audio_interface1        = 74,     /*!< Serial Audio Interface 1 global interrupt                         */
    k_serial_audio_interface2        = 75,     /*!< Serial Audio Interface 2 global interrupt                         */
    k_octospi2                       = 76,     /*!< OctoSPI2 global interrupt                                         */
    k_touch_sense_controller         = 77,     /*!< Touch Sense Controller global interrupt                           */
    k_aes                            = 79,     /*!< AES global interrupt                                              */
    k_random_number_generator        = 80,     /*!< RNG global interrupt                                              */
    k_floating_point_unit            = 81,     /*!< FPU global interrupt                                              */
    k_hash_crs                       = 82,     /*!< HASH and CRS interrupt                                            */
    k_i2c4_error                     = 83,     /*!< I2C4 Error interrupt                                              */
    k_i2c4_event                     = 84,     /*!< I2C4 Event interrupt                                              */
    k_dcmi                           = 85,     /*!< DCMI global interrupt                                             */
    k_dma2d                          = 90,     /*!< DMA2D global interrupt                                            */
    k_dmamux1_overrun                = 94,     /*!< DMAMUX1 overrun global interrupt                                  */
    k_min                            = k_reset,
    k_max                            = k_dmamux1_overrun,
    k_none
};

} // namespace sys::arch::arm
