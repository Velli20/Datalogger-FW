#pragma once

namespace sys::hal {

enum class sdmmc_clock_edge_type : std::uint32_t
{
    k_clock_rising_edge = SDMMC_CLOCK_EDGE_RISING
};

enum class sdmmc_bus_width_type : std::uint32_t
{
    k_bus_width_one_bit = SDMMC_BUS_WIDE_1B,
};

enum class sdmmc_hardware_flow_control_type : std::uint32_t
{
    k_disable = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE
};

enum class sdmmc_tranceiver_type : std::uint32_t
{
    k_disable = SDMMC_TRANSCEIVER_DISABLE
};

struct sdmmc_phy_config_type
{
    sdmmc_clock_edge_type            clock_edge;
    bool                             power_save;
    sdmmc_bus_width_type             bus_width;
    sdmmc_hardware_flow_control_type hw_flow_control;
    sdmmc_tranceiver_type            tranceiver;
    device_node                      pins[6];
};

} // namespace sys::hal
