#pragma once

// Font Freetype data struct.

typedef struct liboled_font_fft_type
{
    uint8_t width;
    uint8_t height;
    uint8_t stride;
    uint8_t advance;
    uint8_t bearing_x;
    uint8_t bearing_y;

    const uint8_t glyph[];
}
LIB_OLED_FONT_FFT;

// liboled_font

typedef struct liboled_font
{
    const uint8_t*  font;
    const uint16_t* offset_table;

    // Char width and height.

    uint16_t width;
    uint16_t height;

    // Fisrt and last char unicodes.

    uint16_t first_char;
    uint16_t char_count;
}
LIB_OLED_FONT;

typedef struct liboled_bitmap_type
{
    const uint8_t* bytes;

    uint16_t stride;
    uint16_t width;
    uint16_t height;
}
LIB_OLED_BITMAP;

extern "C" {
#include "graphics/font/resources.h"
#include "graphics/font/roboto.h"
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wunused-variable"

// Defines

#define SSD1306_SEGMENT_VERTICAL_ADRESSING_MODE   0xA1
#define SSD1306_SEGMENT_HORIZONTAL_ADRESSING_MODE 0xA0

// In normal mode scan from COM0 to COM8. In reversed mode
// scan mode is done from COM8 to COM0.

#define SSD1306_SCAN_MODE_REMAPPED                0xC8
#define SSD1306_SCAN_MODE_NORMAL                  0xC0

#define SSD1306_DISPLAY_OFF                       0xAE
#define SSD1306_DISPLAY_ON                        0xAF

#define SSD1306_SET_CHARGE_PUMP                   0x8D
#define SSD1306_CHAGE_PUMP_ENABLE                 0x14

#define SSD1306_NORMAL_DISPLAY                    0xA6
#define SSD1306_INVERTED_DISPLAY                  0xA7
#define SSD1306_PAGE_ADDRESS_START                0xB0
#define SSD1306_CONTRAST                          0x81

#define OLED_DRIVER_STATUS_INITIALIZED            0x01
#define OLED_DRIVER_STATUS_DISPLAY_INVERTED       0x02
#define OLED_DRIVER_STATUS_DISPLAY_NORMAL         0x04
#define OLED_DRIVER_STATUS_DISPLAY_ON             0x08

#define DISPLAY_BITS_PER_PIXEL      8


namespace sys::dev {

template <std::size_t k_display_width, std::size_t k_display_height, device_node k_i2c_bus>
struct monochrome_graphics
{
    private:

    static constexpr auto wordlen(const char* word_start) -> std::uint32_t
    {
        if ( word_start == nullptr )
        {
            return {};
        }

        std::uint32_t len{};

        // Return char count until next white space.

        while ( (*word_start) != ' ' && (*word_start) != '\0' )
        {
            word_start++;
            len++;
        }

        return len;
    }

    [[nodiscard]] constexpr auto buffer_pixel(
        std::size_t y_position,
        std::size_t x_position
    ) -> std::uint8_t&
    {
        return m_frame_buffer[ ((((y_position) / DISPLAY_BITS_PER_PIXEL) * k_display_width) + x_position)];
    }

    public:

    constexpr monochrome_graphics() = default;

    constexpr auto draw_pixel(
        std::size_t y_position,
        std::size_t x_position
    ) -> sys::expected<void, graphics::errc>
    {
        // Check display bounds.

        if ( x_position >= k_display_width || y_position >= k_display_height )
        {
            return tl::make_unexpected(graphics::errc::k_out_of_bounds);
        }

        buffer_pixel(y_position, x_position) |= (1 << ((y_position) % 8));

        return {};
    }

    constexpr auto clear_pixel(
        std::size_t y_position,
        std::size_t x_position
    ) -> sys::expected<void, graphics::errc>
    {
        // Check display bounds.

        if ( x_position >= k_display_width || y_position >= k_display_height )
        {
            return tl::make_unexpected(graphics::errc::k_out_of_bounds);
        }

        buffer_pixel(y_position, x_position) &= ~(1 << ((y_position) % 8));

        return {};
    }

    template <typename T>
    [[nodiscard]] constexpr auto g_abs(T value)
    {
        return value > 0 ? (value) : -(value);
    }

    auto draw_line(
        std::uint16_t y1,
        std::uint16_t x1,
        std::uint16_t y2,
        std::uint16_t x2
    ) -> sys::expected<void, graphics::errc>
    {
        if ( x1 > k_display_width || y1 > k_display_height )
        {
            return tl::make_unexpected(graphics::errc::k_out_of_bounds);
        }

        std::int16_t xinc1{};
        std::int16_t xinc2{};
        std::int16_t yinc1{};
        std::int16_t yinc2{};

        std::int16_t deltax = g_abs(x2 - x1);
        std::int16_t deltay = g_abs(y2 - y1);

        auto x = static_cast<std::int16_t>(x1);
        auto y = static_cast<std::int16_t>(y1);

        if ( x2 >= x1 )
        {
            xinc1 = 1;
            xinc2 = 1;
        }

        else
        {
            xinc1 -= 1;
            xinc2 -= 1;
        }

        if ( y2 >= y1 )
        {
            yinc1 = 1;
            yinc2 = 1;
        }

        else
        {
            yinc1 -= 1;
            yinc2 -= 1;
        }

        std::int16_t den;
        std::int16_t num;
        std::int16_t num_add;
        std::int16_t num_pixels;

        if ( deltax >= deltay )
        {
            xinc1      = 0;
            yinc2      = 0;
            den        = deltax;
            num        = deltax / 2;
            num_add    = deltay;
            num_pixels = deltax;
        }

        else
        {
            xinc2      = 0;
            yinc1      = 0;
            den        = deltay;
            num        = deltay / 2;
            num_add    = deltax;
            num_pixels = deltay;
        }

        for ( std::int16_t curpixel{}; curpixel <= num_pixels; ++curpixel )
        {
            draw_pixel(y, x);

            num += num_add;

            if ( num >= den )
            {
                num -= den;
                x   += xinc1;
                y   += yinc1;
            }

            x += xinc2;
            y += yinc2;
        }

        return {};
    }

    // draw_circle

    auto draw_circle(
        std::uint16_t x_position,
        std::uint16_t y_position,
        std::uint16_t radius
    ) -> sys::expected<void, graphics::errc>
    {
        std::int32_t  decision;
        std::uint16_t current_x;
        std::uint16_t current_y;

        decision =  3 - (radius << 1);
        for ( current_x = 0, current_y = radius; current_x <= current_y; current_x++ )
        {
            draw_pixel((y_position - current_y), (x_position + current_x));
            draw_pixel((y_position - current_y), (x_position - current_x));
            draw_pixel((y_position - current_x), (x_position + current_y));
            draw_pixel((y_position - current_x), (x_position - current_y));
            draw_pixel((y_position + current_y), (x_position + current_x));
            draw_pixel((y_position + current_y), (x_position - current_x));
            draw_pixel((y_position + current_x), (x_position + current_y));
            draw_pixel((y_position + current_x), (x_position - current_y));

            if ( decision < 0 )
            {
                decision+= (current_x << 2) + 6;
            }

            else
            {
                decision+= ((current_x - current_y) << 2) + 10;
                current_y--;
            }
        }

        return {};
    }

    // draw_filled_circle

    auto draw_filled_circle(
        std::uint16_t x_position,
        std::uint16_t y_position,
        std::uint16_t radius
    ) -> sys::expected<void, graphics::errc>
    {
        std::int32_t  decision  = 3 - (radius << 1);
        std::uint16_t current_y = radius;

        for ( std::uint16_t current_x{}; current_x <= current_y; current_x++ )
        {
            for ( std::uint16_t n{}; n < (2 * current_y); ++n )
            {
                draw_pixel((y_position + current_x), (x_position - current_y + n));
                draw_pixel((y_position - current_x), (x_position - current_y + n));
            }

            for ( std::uint16_t n{}; n < (2 * current_x); ++n )
            {
                draw_pixel((y_position - current_y), (x_position - current_x + n));
                draw_pixel((y_position + current_y), (x_position - current_x + n));
            }

            if ( decision < 0 )
            {
                decision += (current_x << 2) + 6;
            }

            else
            {
                decision += ((current_x - current_y) << 2) + 10;
                current_y--;
            }
        }

        return {};
    }

    // draw_filled_rect

    auto draw_filled_rect(
        std::uint16_t x_position,
        std::uint16_t y_position,
        std::uint16_t width,
        std::uint16_t height
    ) -> sys::expected<void, graphics::errc>
    {
        if ( x_position > k_display_width || y_position > k_display_height )
        {
            return tl::make_unexpected(graphics::errc::k_out_of_bounds);
        }

        for ( std::uint16_t n{}; n != height; ++n )
        {
            for ( std::uint16_t i{}; i != width; ++i )
            {
                draw_pixel(y_position + n, x_position + i);
            }
        }

        return {};
    }

    // draw_bitmap

    auto draw_bitmap(
        const LIB_OLED_BITMAP* bitmap,
        std::uint16_t          x_position,
        std::uint16_t          y_position
    ) -> sys::expected<void, graphics::errc>
    {
        for ( std::int16_t height_pos{}; height_pos != bitmap->height; ++height_pos )
        {
            if ( (height_pos + y_position) >= (k_display_height - 1) )
            {
                break;
            }

            // Draw bitmap row.

            for ( std::int16_t n{}; n < bitmap->stride; n++ )
            {
                auto pixel_byte = bitmap->bytes[(height_pos * bitmap->stride) + n];

                // Draw byte pixels.

                for ( std::int16_t i{}; i < 8; i++ )
                {
                    std::int16_t width_pos = x_position + i + (n * 8);

                    // Continue to next row if are about to draw outside of the screen bounds.

                    if ( width_pos >= (k_display_width - 1) )
                    {
                        break;
                    }

                    if ( pixel_byte & (1 << i) )
                    {
                        draw_pixel(y_position + height_pos, width_pos);
                    }
                }
            }
        }

        return {};
    }

    // draw_string

    auto draw_string(
        std::int16_t         x_start,
        std::int16_t         y_start,
        const LIB_OLED_FONT* font,
        const char*          text
    ) -> sys::expected<void, graphics::errc>
    {
        // Check parameters.

        auto x_position = x_start;
        auto y_position = y_start;

        while ( *text != '\0' )
        {
            std::uint16_t char_index = text[0] - font->first_char;
            if ( text[0] < font->first_char ||char_index > font->char_count )
            {
                text++;
                continue;
            }

            // Read font data osset from offset table.

            auto font_offset = font->offset_table[char_index];

            auto* fft = (const LIB_OLED_FONT_FFT*) &font->font[font_offset];
            if ( fft == nullptr )
            {
                return tl::make_unexpected(graphics::errc::k_invalid_argument);
            }

            // Handle new line or text overflow.

            if ( x_position >= (k_display_width - fft->advance) || *text == '\n' )
            {
                x_position  = x_start;
                y_position += font->height;

                // Skip any spaces after new line.

                if ( *text == '\n' || *text == ' ' )
                {
                    text++;
                    continue;
                }
            }

            // Check that next word fits on the display.

            else if ( *text == ' ' )
            {
                auto width = wordlen(&text[1]) * fft->width;
                if ( (x_position + (fft->width * 2) + (std::int16_t) width) >= k_display_width && width < k_display_width )
                {
                    text++;

                    // Switch to new line to fit word on the screen without splitting it
                    // in two parts.

                    x_position  = x_start;
                    y_position += font->height;
                }
            }

            std::int16_t pixel_x = (x_position + fft->bearing_x);
            std::int16_t pixel_y = (y_position + font->height - fft->bearing_y);

            // Draw glyph pixel by pixel.

            for ( std::int32_t y{}; y < fft->height; y++ )
            {
                if ( (y + y_position) >= k_display_height )
                {
                    break;
                }

                // Draw glyph row.

                for ( std::int32_t x{}; x < fft->stride; x++ )
                {
                    if ( ((x * 8) + x_position) >= (k_display_width - 1) )
                    {
                        break;
                    }

                    auto pixel_byte = fft->glyph[(y * fft->stride) + x];

                    // Each bit represents pixel state (on/off).

                    for ( std::int32_t i{}; i < 8; i++)
                    {
                        if ( !(pixel_byte & (1 << i)) )
                        {
                            continue;
                        }

                        draw_pixel((pixel_y + y), pixel_x + i + (x * 8));
                    }
                }

            }

            text++;
            x_position += fft->advance;
        }

        return {};
    }

    private:

    auto driver_refresh() -> sys::expected<void, graphics::errc>
    {
        using namespace sys::dev;
        using namespace std::chrono_literals;

        for ( std::uint8_t page{} ; page != (k_display_height / DISPLAY_BITS_PER_PIXEL); ++page )
        {
            // Sets display column end address.

            std::uint8_t command = 0x80;

            device_type<k_i2c_bus>::write(std::uint16_t{m_i2c_address}, 0, std::as_bytes(std::span{std::addressof(command), 1}), 1000ms);

            // Set SSD1306 GDDRAM page start address.

            command = (SSD1306_PAGE_ADDRESS_START | page);

            device_type<k_i2c_bus>::write(std::uint16_t{m_i2c_address}, 0, std::as_bytes(std::span{std::addressof(command), 1}), 1000ms);

            // Write data.

            device_type<k_i2c_bus>::write(std::uint16_t{m_i2c_address}, 0x40, std::as_bytes(std::span{std::addressof(m_frame_buffer[(page * k_display_width)]), k_display_width}), 1000ms);
        }

        return{};
    }

    public:

    // buffer_refresh

    auto buffer_refresh() -> sys::expected<void, graphics::errc>
    {
        driver_refresh();

        return {};
    }

    // clear

    auto buffer_clear_area(
        std::uint16_t x_position,
        std::uint16_t y_position,
        std::uint16_t width,
        std::uint16_t height
    ) -> sys::expected<void, graphics::errc>
    {
        if ( x_position > k_display_width || y_position > k_display_height )
        {
            return tl::make_unexpected(graphics::errc::k_out_of_bounds);
        }

        // Needs some optimization :D

        for ( std::uint16_t n{}; n != height; ++n )
        {
            for ( std::uint16_t i{}; i != width; ++i )
            {
                clear_pixel(x_position + i, y_position + n);
            }
        }

        return {};
    }

    // clear

    auto buffer_clear() -> sys::expected<void, graphics::errc>
    {
        // Clear display buffer.

        memset(m_frame_buffer, 0, k_display_width * k_display_height * sizeof(std::uint8_t));

        return {};
    }

    // init

    auto init() -> sys::expected<void, graphics::errc>
    {
        using namespace sys::dev;
        using namespace std::chrono_literals;

        static constexpr std::uint8_t k_startup_sequence[]=
        {
            // Enable charge pump regulator.

            SSD1306_SET_CHARGE_PUMP, SSD1306_CHAGE_PUMP_ENABLE,

            // Set segment re-map.

            SSD1306_SEGMENT_VERTICAL_ADRESSING_MODE,

            // Set COM Output Scan direction.

            SSD1306_SCAN_MODE_REMAPPED,

            // Set display to mode.

            SSD1306_NORMAL_DISPLAY,

            // Display on.

            SSD1306_DISPLAY_ON,
        };

        device_type<k_i2c_bus>::write(std::uint16_t{m_i2c_address}, 0, std::as_bytes(std::span{k_startup_sequence, sizeof(k_startup_sequence)}), 1000ms);

        m_driver_status = (OLED_DRIVER_STATUS_INITIALIZED | OLED_DRIVER_STATUS_DISPLAY_NORMAL);

        return {};
    }

    // display_driver_invert

    auto display_driver_invert() -> sys::expected<void, graphics::errc>
    {
        using namespace sys::dev;
        using namespace std::chrono_literals;

        std::uint8_t pixel_byte;

        // Set display inverted.

        if ( m_driver_status & OLED_DRIVER_STATUS_DISPLAY_NORMAL )
        {
            m_driver_status &= ~OLED_DRIVER_STATUS_DISPLAY_NORMAL;
            m_driver_status |=  OLED_DRIVER_STATUS_DISPLAY_INVERTED;

            pixel_byte = SSD1306_INVERTED_DISPLAY;
        }

        // Set display mode to normal.

        else if ( m_driver_status & OLED_DRIVER_STATUS_DISPLAY_INVERTED )
        {
            m_driver_status &= ~OLED_DRIVER_STATUS_DISPLAY_INVERTED;
            m_driver_status |=  OLED_DRIVER_STATUS_DISPLAY_NORMAL;

            pixel_byte = SSD1306_NORMAL_DISPLAY;
        }

        // Return error if mode is unknown or display is unitialized.

        else
        {
            return tl::make_unexpected(graphics::errc::k_invalid_argument);
        }

        // Write display mode command.

        device_type<k_i2c_bus>::write(std::uint16_t{m_i2c_address}, 0x40, std::as_bytes(std::span{std::addressof(pixel_byte), 1}), 1000ms);

        return {};
    }

    // display_driver_set_contrast

    auto display_driver_set_contrast(std::uint8_t contrast) -> sys::expected<void, graphics::errc>
    {
        using namespace sys::dev;
        using namespace std::chrono_literals;

        std::uint8_t data = ((contrast << 8) | (SSD1306_CONTRAST));

        // Write display contrast command followed by contrast byte.

        device_type<k_i2c_bus>::write(std::uint16_t{m_i2c_address}, 0x40, std::as_bytes(std::span{std::addressof(data), 2}), 1000ms);

        return {};
    }

    // display_driver_set_display_off

    auto display_driver_set_display_off() -> sys::expected<void, graphics::errc>
    {
        using namespace sys::dev;
        using namespace std::chrono_literals;

        // Set display off.

        m_driver_status &= ~OLED_DRIVER_STATUS_DISPLAY_ON;

        std::uint8_t data = SSD1306_DISPLAY_OFF;

        // Write display mode command.

        device_type<k_i2c_bus>::write(std::uint16_t{m_i2c_address}, 0, std::as_bytes(std::span{std::addressof(data), 1}), 1000ms);

        return {};
    }

    // display_driver_set_display_on

    auto display_driver_set_display_on() -> sys::expected<void, graphics::errc>
    {
        using namespace sys::dev;
        using namespace std::chrono_literals;

        // Set display on.

        m_driver_status |= OLED_DRIVER_STATUS_DISPLAY_ON;

        std::uint8_t data = SSD1306_DISPLAY_ON;

        // Write display mode command.

        device_type<k_i2c_bus>::write(std::uint16_t{m_i2c_address}, 0, std::as_bytes(std::span{std::addressof(data), 1}), 1000ms);

        return {};
    }

    private:

    [[gnu::section(".noinit")]]
    static inline std::uint8_t  m_frame_buffer[k_display_width * k_display_height]{};
    std::uint8_t  m_i2c_address{0x3C};
    std::uint16_t m_driver_status{};

};

} // namespace sys::dev

#pragma GCC diagnostic pop
