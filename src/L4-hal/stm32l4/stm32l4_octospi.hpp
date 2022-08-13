#pragma once

namespace sys::hal {

namespace detail {

struct flash_geometry_type
{
    constexpr flash_geometry_type() = default;

    std::size_t        m_flash_size{};
    std::size_t        m_erase_sector_size{std::numeric_limits<std::size_t>::max()};
    std::size_t        m_write_granuality{};

    jedec::cfi_command m_erase_sector_command{};
    jedec::cfi_command m_write_enable_command{};

    std::byte          m_erased_pattern{0xFF};
    bool               m_is_non_volatile{};
};

struct [[gnu::packed]] status_register_type
{
    [[nodiscard]] auto is_write_in_progress() noexcept
    {
        const std::bitset<8> bits{data[0]};

        /* WIP bit.
            The write in Progress (WIP) bit indicates whether the memory is busy in program/erase/write status
            register progress. When WIP bit sets to 1, means the device is busy in program/erase/write
            status register progress, when WIP bit sets 0, means the device is not in
            program/erase/write status register progress.
        */

        return bits.test(0);
    }

    [[nodiscard]] auto is_write_enabled() noexcept
    {
        const std::bitset<8> bits{data[0]};

        /* WEL bit.

            The write Enable Latch (WEL) bit indicates the status of the internal write Enable Latch. When set
            to 1 the internal write Enable Latch is set, when set to 0 the internal write Enable Latch is reset
            and no write Status mmio::register_type, Program or Erase command is accepted.
        */

        return bits.test(1);
    }

    std::uint8_t data[3];
};

static_assert(sizeof(status_register_type) == 3);

} // namespace detail

template <>
struct peripheral_type<"/chip/octospi@1"> final
{
    static constexpr auto k_page_size = std::size_t{256};
    static constexpr auto k_node      = device_node{"/chip/octospi@1"};

    static auto read_status() noexcept -> std::optional<detail::status_register_type>
    {
        detail::status_register_type result{};

        auto command = hal::ospi_command
        {
            jedec::cfi_command::k_read_status_register1,
            HAL_OSPI_DATA_1_LINE,
            1
        };

        if ( HAL_OSPI_Command(m_ospi_handle, command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK )
        {
            m_error = make_error_code(m_ospi_handle.error());
            return {};
        }

        if ( HAL_OSPI_Receive(m_ospi_handle, reinterpret_cast<std::uint8_t*>(std::addressof(result)), HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK )
        {
            m_error = make_error_code(m_ospi_handle.error());
            return {};
        }

        return result;
    }

    enum class status_flag_type
    {
        k_write_complete,
        k_write_enabled,
    };

    template <status_flag_type k_flag>
    static auto wait_status() noexcept
    {
        do
        {
            auto status = read_status();

            if ( status.has_value() == false )
            {
                return false;
            }

            if constexpr ( k_flag == status_flag_type::k_write_complete )
            {
                if ( status.value().is_write_in_progress() == false )
                {
                    return true;
                }
            }

            if constexpr ( k_flag == status_flag_type::k_write_enabled )
            {
                if ( status.value().is_write_enabled() == true )
                {
                    return true;
                }
            }
        }
        while ( true );
    }

    static auto write_enable(bool enable = true) noexcept
    {
        // TODO: Enable only once.

        // if ( m_geometry.m_is_non_volatile )
        //     return true;

        auto command = hal::ospi_command{enable ? jedec::cfi_command::k_write_enable : jedec::cfi_command::k_write_disable};

        if ( HAL_OSPI_Command(m_ospi_handle, command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK )
        {
            m_error = make_error_code(m_ospi_handle.error());
            return false;
        }

        return wait_status<status_flag_type::k_write_enabled>();
    }

    static auto quad_mode_enable() noexcept
    {
        if ( write_enable(true) == false )
        {
            return false;
        }

        auto quad_mode_command = []()
        {
            auto command = hal::ospi_command{jedec::cfi_command::k_write_status_register2};

            command.Address     = 0x02;
            command.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
            command.AddressSize = HAL_OSPI_ADDRESS_8_BITS;

            return command;
        }();

        if ( HAL_OSPI_Command(m_ospi_handle, quad_mode_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK )
        {
            m_error = make_error_code(m_ospi_handle.error());
            return false;
        }

        return wait_status<status_flag_type::k_write_complete>();
    }

    static auto erase_sector(std::uint32_t sector) noexcept -> bool
    {
        if ( write_enable(true) == false )
        {
            return false;
        }

        auto erase_command = [sector]()
        {
            auto command = hal::ospi_command{m_geometry.m_erase_sector_command};

            command.AddressMode = HAL_OSPI_ADDRESS_1_LINE;
            command.Address     = sector * m_geometry.m_erase_sector_size;

            return command;
        }();

        if ( HAL_OSPI_Command(m_ospi_handle, erase_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK )
        {
            m_error = make_error_code(m_ospi_handle.error());
            return false;
        }

        return wait_status<status_flag_type::k_write_complete>();
    }

    static auto erase_chip() noexcept
    {
        std::uint32_t sector_count = m_geometry.m_flash_size / 8u / m_geometry.m_erase_sector_size;
        std::uint32_t page_count   = m_geometry.m_erase_sector_size / k_page_size;

        auto buffer = std::array<std::byte, k_page_size>{};

        const auto is_erased = [](std::span<const std::byte> region)
        {
            for ( auto b : region )
            {
                if ( b != m_geometry.m_erased_pattern )
                {
                    return false;
                }
            }

            return true;
        };

        for ( std::uint32_t sector{}; sector != sector_count; ++sector )
        {
            for ( std::uint32_t page{}; page != page_count; ++page )
            {
                auto read_address = [&]() -> std::uint32_t
                {
                    return (sector * m_geometry.m_erase_sector_size) + (page * k_page_size);
                }();

                if ( auto rc = read(std::span{buffer}, read_address); rc != k_page_size )
                {
                    return false;
                }

                if ( is_erased(std::span{buffer}) )
                {
                    continue;
                }

                if ( erase_sector(sector) != true )
                {
                    return false;
                }

                break;
            }
        }

        return true;
    }

    static auto memorymapped_mode(bool enable = true)
    {
        OSPI_MemoryMappedTypeDef config{};

        config.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;

        if ( HAL_OSPI_MemoryMapped(m_ospi_handle, std::addressof(config)) != HAL_OK )
        {
            return false;
        }

        return true;
    }

    static auto write(
        const std::span<const std::byte> buffer,
        std::uint32_t                    write_address
    ) noexcept -> std::size_t
    {
        if ( buffer.empty() || m_initialized.load() == false )
        {
            return {};
        }

        std::size_t written{};

        auto command = hal::ospi_command
        {
            jedec::cfi_command::k_page_program,
            HAL_OSPI_DATA_1_LINE
        };

        while ( written < buffer.size_bytes() )
        {
            auto to_write = std::clamp(buffer.size_bytes() - written, std::size_t{}, m_geometry.m_write_granuality);
            if ( to_write == 0u )
            {
                break;
            }

            auto write_buffer = buffer.subspan(written, to_write);
            if ( write_buffer.empty() )
            {
                break;
            }

            command.set_address_mode(HAL_OSPI_ADDRESS_1_LINE);
            command.set_address(static_cast<std::uint32_t>(write_address) + written);
            command.set_data_count(write_buffer.size_bytes());

            // Enable write operations.

            if ( write_enable(true) == false )
            {
                return written;
            }

            // Configure the command.

            if ( HAL_OSPI_Command(m_ospi_handle, command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK )
            {
                m_error = make_error_code(m_ospi_handle.error());
                return written;
            }

            // Transfer data.

            if ( HAL_OSPI_Transmit(m_ospi_handle, (std::uint8_t*) (write_buffer.data()), HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK )
            {
                m_error = make_error_code(m_ospi_handle.error());
                return written;
            }

            // Poll for completion.

            if ( wait_status<status_flag_type::k_write_complete>() == false )
            {
                return written;
            }

            written += to_write;
        }

        return written;
    }

    static auto read(
        std::span<std::byte> buffer,
        std::uint32_t        read_address
    ) noexcept -> std::size_t
    {
        if ( buffer.empty() || m_initialized.load() == false )
        {
            return {};
        }

        auto read_command = hal::ospi_command
        {
            jedec::cfi_command::k_read_data,
            HAL_OSPI_DATA_1_LINE,
            buffer.size_bytes()
        };

        read_command.set_address_mode(HAL_OSPI_ADDRESS_1_LINE);
        read_command.set_address(read_address);

        if ( HAL_OSPI_Command(m_ospi_handle, read_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK )
        {
            m_error = make_error_code(m_ospi_handle.error());
            return {};
        }

        if ( HAL_OSPI_Receive(m_ospi_handle, reinterpret_cast<std::uint8_t*>(buffer.data()), HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK )
        {
            m_error = make_error_code(m_ospi_handle.error());
            return {};
        }

        return buffer.size_bytes();
    }

    static auto read_serial_flash_discoverable_parameter() noexcept
    {
        auto&& sfdp_header = []() -> std::optional<jedec::sfdp_header>
        {
            union
            {
                std::uint8_t       buffer[sizeof(jedec::sfdp_header)]{};
                jedec::sfdp_header header;
            } erased;

            auto command = hal::ospi_command
            {
                jedec::cfi_command::k_jedec_sfdp,
                HAL_OSPI_DATA_1_LINE,
                sizeof(jedec::sfdp_header),
                HAL_OSPI_ADDRESS_1_LINE
            };

            if ( HAL_OSPI_Command(m_ospi_handle, command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK )
            {
                return {};
            }

            if ( HAL_OSPI_Receive(m_ospi_handle, erased.buffer, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK )
            {
                return {};
            }

            return std::move(erased.header);
        }();

        if ( sfdp_header.has_value() == false || sfdp_header.value().is_valid() == false )
        {
            m_error = make_error_code(m_ospi_handle.error());

            log::error() << "Failed to read Flash parameter table." << log::endl;
            return false;
        }

        auto&& params_opt = [address = sfdp_header.value().param_table_pointer()]() -> std::optional<jedec::parameter_table>
        {
            union
            {
                std::uint8_t           buffer[sizeof(jedec::parameter_table)];
                jedec::parameter_table table{};
            } erased;

            auto command = hal::ospi_command
            {
                jedec::cfi_command::k_jedec_sfdp,
                HAL_OSPI_DATA_1_LINE,
                sizeof(jedec::parameter_table),
                HAL_OSPI_ADDRESS_1_LINE
            };

            command.set_address(address);

            if ( HAL_OSPI_Command(m_ospi_handle, command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK )
            {
                return {};
            }

            if ( HAL_OSPI_Receive(m_ospi_handle, erased.buffer, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK )
            {
                return {};
            }

            return std::move(erased.table);
        }();

        if ( params_opt.has_value() == false )
        {
            m_error = make_error_code(m_ospi_handle.error());
            return false;
        }

        auto params = params_opt.value();

        m_geometry.m_is_non_volatile  = params.is_non_volatile();
        m_geometry.m_flash_size       = params.density();
        m_geometry.m_write_granuality = params.write_granuality();

        if ( auto op = params.write_voltile_status_register_enable_op(); op.has_value() )
        {
            m_geometry.m_write_enable_command = static_cast<jedec::cfi_command>(op.value());
            m_geometry.m_is_non_volatile      = false;
        }

        static constexpr jedec::sector_type k_sector_type[] =
        {
            jedec::sector_type::k_sector1,
            jedec::sector_type::k_sector2,
            jedec::sector_type::k_sector3,
            jedec::sector_type::k_sector4
        };

        // Find smallest erasable sector size.

        for ( auto type : k_sector_type )
        {
            if ( auto sector = params.sector(type); sector.has_value() == true )
            {
                if ( auto [op, size] = sector.value(); size < m_geometry.m_erase_sector_size )
                {
                    m_geometry.m_erase_sector_size    = size;
                    m_geometry.m_erase_sector_command = op;
                }
            }
        }

        if ( m_geometry.m_erase_sector_size == std::numeric_limits<std::size_t>::max() )
        {
            return false;
        }

        log::info() << "Jedec params"                                                                      << log::endl
                    << "\t\tNon-volatile:      " << m_geometry.m_is_non_volatile                             << log::endl
                    << "\t\tErase sector size: " << m_geometry.m_erase_sector_size                           << log::endl
                    << "\t\tErase sector op:   " << static_cast<unsigned>(m_geometry.m_erase_sector_command) << log::endl
                    << "\t\tWrite granuality:  " << m_geometry.m_write_granuality                            << log::endl
                    << "\t\tTotal size:        " << m_geometry.m_flash_size                                  << log::endl;

        return true;
    }

    static auto init() noexcept -> bool
    {
        if ( auto was_initialized = m_initialized.exchange(true); was_initialized == true )
        {
            return true;
        }

        m_error = make_error_code(hal_ospi_error::no_error);

        // Initialize peripheral pins.

        [] <typename T, T... Is> (std::integer_sequence<T, Is...>)
        {
            constexpr auto k_configuration = std::get<ospi_phy_config_type>(hal::peripheral_of(k_node).config);

            ([&]
            {
                constexpr auto k_pin = k_configuration.pins[Is];
                hal::peripheral_type<k_pin>::init();
            }(), ...);

        }(std::make_integer_sequence<std::size_t, sizeof(ospi_phy_config_type::pins) / sizeof(device_node) >{});

        // Enable peripheral clock.

        peripheral_type<"/chip/rcc">::enable<k_node>();

        m_ospi_handle.Init.FifoThreshold          = 8;
        m_ospi_handle.Init.DualQuad               = HAL_OSPI_DUALQUAD_DISABLE;
        m_ospi_handle.Init.MemoryType             = 0;
        m_ospi_handle.Init.DeviceSize             = POSITION_VAL((16 * 1024 * 1024)); // FIXME.
        m_ospi_handle.Init.ChipSelectHighTime     = 1;
        m_ospi_handle.Init.FreeRunningClock       = HAL_OSPI_FREERUNCLK_DISABLE;
        m_ospi_handle.Init.ClockMode              = HAL_OSPI_CLOCK_MODE_0;
        m_ospi_handle.Init.ClockPrescaler         = 4;
        m_ospi_handle.Init.SampleShifting         = HAL_OSPI_SAMPLE_SHIFTING_NONE;
        m_ospi_handle.Init.DelayHoldQuarterCycle  = HAL_OSPI_DHQC_DISABLE;
        m_ospi_handle.Init.ChipSelectBoundary     = 0;
        m_ospi_handle.Init.DelayBlockBypass       = HAL_OSPI_DELAY_BLOCK_BYPASSED;

        if ( HAL_OSPI_Init(m_ospi_handle) != HAL_OK )
        {
            m_error = make_error_code(m_ospi_handle.error());
            return m_initialized.exchange(false) == false;
        }

        OSPIM_CfgTypeDef OSPIM_Cfg_Struct = {};

        OSPIM_Cfg_Struct.ClkPort   = 1;
        OSPIM_Cfg_Struct.NCSPort   = 1;
        OSPIM_Cfg_Struct.IOLowPort = HAL_OSPIM_IOPORT_1_LOW;

        if ( HAL_OSPIM_Config(m_ospi_handle, std::addressof(OSPIM_Cfg_Struct), HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK )
        {
            m_error = make_error_code(m_ospi_handle.error());
            return m_initialized.exchange(false) == false;
        }

        if ( auto rc = read_serial_flash_discoverable_parameter(); rc == false )
        {
            m_error = make_error_code(m_ospi_handle.error() );
            return m_initialized.exchange(false) == false;
        }

        return true;
    }

    [[nodiscard]] static auto block_size() noexcept -> std::optional<std::size_t>
    {
        if ( m_initialized.load() == true )
        {
            return m_geometry.m_erase_sector_size;
        }

        return {};
    }

    [[nodiscard]] static auto block_count() noexcept -> std::optional<std::size_t>
    {
        if ( m_initialized.load() == true )
        {
            return m_geometry.m_flash_size / 8u / m_geometry.m_erase_sector_size;
        }

        return {};
    }

    [[nodiscard]] static auto GetError() noexcept
    {
        return m_error;
    }

    private:

    static inline           auto m_ospi_handle = hal_handle<OSPI_HandleTypeDef, k_node>{};
    static inline constinit auto m_geometry    = detail::flash_geometry_type{};
    static inline constinit auto m_initialized = std::atomic<bool>{};
    static inline           auto m_error       = sys::error_code{};
};

} // namespace sys::hal
