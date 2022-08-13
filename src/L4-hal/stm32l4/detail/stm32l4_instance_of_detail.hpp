#pragma once

namespace sys::hal::detail {

template <device_node k_node>
struct instance_of_dma_channel_node
{
    static constexpr bool value = instance_of_node<k_node, "/chip/dma1-channel@1">::value ||
                                  instance_of_node<k_node, "/chip/dma1-channel@2">::value ||
                                  instance_of_node<k_node, "/chip/dma1-channel@3">::value ||
                                  instance_of_node<k_node, "/chip/dma1-channel@4">::value ||
                                  instance_of_node<k_node, "/chip/dma1-channel@5">::value ||
                                  instance_of_node<k_node, "/chip/dma1-channel@6">::value ||
                                  instance_of_node<k_node, "/chip/dma1-channel@7">::value ||

                                  instance_of_node<k_node, "/chip/dma2-channel@1">::value ||
                                  instance_of_node<k_node, "/chip/dma2-channel@2">::value ||
                                  instance_of_node<k_node, "/chip/dma2-channel@3">::value ||
                                  instance_of_node<k_node, "/chip/dma2-channel@4">::value ||
                                  instance_of_node<k_node, "/chip/dma2-channel@5">::value ||
                                  instance_of_node<k_node, "/chip/dma2-channel@6">::value ||
                                  instance_of_node<k_node, "/chip/dma2-channel@7">::value;

};

template <device_node k_node>
struct instance_of_uart_node
{
    static constexpr bool value = instance_of_node<k_node, "/chip/usart@1">::value ||
                                  instance_of_node<k_node, "/chip/usart@2">::value ||
                                  instance_of_node<k_node, "/chip/usart@3">::value ||
                                  instance_of_node<k_node, "/chip/uart@4">::value;
};

template <device_node k_node>
struct instance_of_gpio_node
{
    static constexpr bool value = instance_of_node<k_node, "/chip/gpio-a@0">::value ||
                                  instance_of_node<k_node, "/chip/gpio-b@0">::value ||
                                  instance_of_node<k_node, "/chip/gpio-c@0">::value ||
                                  instance_of_node<k_node, "/chip/gpio-d@0">::value ||
                                  instance_of_node<k_node, "/chip/gpio-e@0">::value ||
                                  instance_of_node<k_node, "/chip/gpio-f@0">::value ||
                                  instance_of_node<k_node, "/chip/gpio-g@0">::value ||
                                  instance_of_node<k_node, "/chip/gpio-h@0">::value;

};

template <device_node k_node>
struct instance_of_i2c_node
{
    static constexpr bool value = instance_of_node<k_node, "/chip/i2c@1">::value ||
                                  instance_of_node<k_node, "/chip/i2c@2">::value ||
                                  instance_of_node<k_node, "/chip/i2c@3">::value ||
                                  instance_of_node<k_node, "/chip/i2c@4">::value;
};

} // namespace sys::hal::detail
