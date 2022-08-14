
// Includes.

#include <base.hpp>

#include "tusb_option.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include "device/dcd.h"
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"
#pragma GCC diagnostic pop

#ifndef MAX_EP_COUNT
#define MAX_EP_COUNT 8U
#endif

namespace {

constinit std::uint16_t s_last_in_size{};
constinit std::uint16_t s_last_out_size{};

void dcd_open_ep0(PCD_HandleTypeDef* usb)
{
    HAL_StatusTypeDef rc{};

    rc = HAL_PCD_EP_Open(usb, 0x00U, CFG_TUD_ENDPOINT0_SIZE, TUSB_XFER_CONTROL);
    debug_assert(rc == HAL_OK);

    rc = HAL_PCD_EP_Open(usb, 0x80U, CFG_TUD_ENDPOINT0_SIZE, TUSB_XFER_CONTROL);
    debug_assert(rc == HAL_OK);
}

} // namespace

namespace sys::hal {

PCD_HandleTypeDef g_pcd_handle;

// Initialize controller to device mode

extern "C" void dcd_init(std::uint8_t rhport)
{
    HAL_StatusTypeDef rc{};

    debug_assert(rhport == 0);

    g_pcd_handle.pData                        = nullptr;
    g_pcd_handle.Instance                     = USB_OTG_FS;
    g_pcd_handle.Init.dev_endpoints           = MAX_EP_COUNT;
    g_pcd_handle.Init.phy_itface              = PCD_PHY_EMBEDDED;
    g_pcd_handle.Init.speed                   = PCD_SPEED_FULL;
    g_pcd_handle.Init.Sof_enable              = DISABLE;
    g_pcd_handle.Init.low_power_enable        = DISABLE;
    g_pcd_handle.Init.lpm_enable              = DISABLE;
    g_pcd_handle.Init.battery_charging_enable = DISABLE;
    g_pcd_handle.Init.use_dedicated_ep1       = DISABLE;
    g_pcd_handle.Init.vbus_sensing_enable     = DISABLE;
    g_pcd_handle.Init.dma_enable              = DISABLE;

    rc = HAL_PCD_Init(std::addressof(g_pcd_handle));
    debug_assert(rc == HAL_OK);

    HAL_PCDEx_SetRxFiFo(std::addressof(g_pcd_handle),    0x80);         // shared rx buffer
    HAL_PCDEx_SetTxFiFo(std::addressof(g_pcd_handle), 0, 0x20);         // ep0 tx buffer
    HAL_PCDEx_SetTxFiFo(std::addressof(g_pcd_handle), 1, 0x10);         // ep1 (keyboard) tx buffer
    HAL_PCDEx_SetTxFiFo(std::addressof(g_pcd_handle), 2, 0x10);         // ep2 (mouse, system, consumer) tx buffer
    HAL_PCDEx_SetTxFiFo(std::addressof(g_pcd_handle), 3, 0x80);         // ep3 (webusb or msc) tx buffer

    rc = HAL_PCD_Start(std::addressof(g_pcd_handle));
    debug_assert(rc == HAL_OK);
}

// Enable device interrupt

extern "C" void dcd_int_enable(std::uint8_t rhport)
{
    static_cast<void>(rhport);
}

// Disable device interrupt

extern "C" void dcd_int_disable(std::uint8_t rhport)
{
    static_cast<void>(rhport);
}

// Receive Set Address request, mcu port must also include status IN response

extern "C" void dcd_set_address(std::uint8_t rhport, std::uint8_t dev_addr)
{
    HAL_StatusTypeDef rc;

    static_cast<void>(rhport);

    rc = HAL_PCD_SetAddress(std::addressof(g_pcd_handle), dev_addr);
    debug_assert(rc == HAL_OK);

    rc = HAL_PCD_EP_Transmit(std::addressof(g_pcd_handle), 0, nullptr, 0);
    debug_assert(rc == HAL_OK);
}

// Wake up host

extern "C" void dcd_remote_wakeup(std::uint8_t rhport)
{
    HAL_StatusTypeDef rc;

    static_cast<void>(rhport);

    rc = HAL_PCD_ActivateRemoteWakeup(std::addressof(g_pcd_handle));
    debug_assert(rc == HAL_OK);

    HAL_Delay(5);

    rc = HAL_PCD_DeActivateRemoteWakeup(std::addressof(g_pcd_handle));
    debug_assert(rc == HAL_OK);
}

// Connect by enabling internal pull-up resistor on D+/D-

extern "C" void dcd_connect(std::uint8_t rhport)
{
    static_cast<void>(rhport);

    const auto rc = HAL_PCD_DevConnect(std::addressof(g_pcd_handle));
    debug_assert(rc == HAL_OK);
}

// Disconnect by disabling internal pull-up resistor on D+/D-

extern "C" void dcd_disconnect(std::uint8_t rhport)
{
    static_cast<void>(rhport);

    const auto rc = HAL_PCD_DevDisconnect(std::addressof(g_pcd_handle));
    debug_assert(rc == HAL_OK);
}

//--------------------------------------------------------------------+
// Endpoint API
//--------------------------------------------------------------------+

// Configure endpoint's registers according to descriptor

extern "C" bool dcd_edpt_open(
    std::uint8_t                rhport,
    tusb_desc_endpoint_t const* ep_desc
)
{
    static_cast<void>(rhport);

    debug_assert(ep_desc != nullptr);

    /* Translate USB standard type codes to STM32. */

    constexpr std::uint8_t k_type_lut[] =
    {
        [TUSB_XFER_CONTROL]     = EP_TYPE_CTRL,
        [TUSB_XFER_ISOCHRONOUS] = EP_TYPE_ISOC,
        [TUSB_XFER_BULK]        = EP_TYPE_BULK,
        [TUSB_XFER_INTERRUPT]   = EP_TYPE_INTR,
    };

    const auto rc = HAL_PCD_EP_Open(
        std::addressof(g_pcd_handle),
        ep_desc->bEndpointAddress,
        ep_desc->wMaxPacketSize,
        k_type_lut[ep_desc->bmAttributes.xfer]
    );

    debug_assert(rc == HAL_OK);

    return rc == HAL_OK ? true : false;
}

extern "C" void dcd_edpt_close(
    std::uint8_t rhport,
    std::uint8_t ep_addr
)
{
    static_cast<void>(rhport);

    const auto rc = HAL_PCD_EP_Close(std::addressof(g_pcd_handle), ep_addr);
    debug_assert(rc == HAL_OK);
}

extern "C" void dcd_edpt_close_all (std::uint8_t rhport)
{
    static_cast<void>(rhport);
}

// Submit a transfer, When complete dcd_event_xfer_complete() is invoked to notify the stack

extern "C" bool dcd_edpt_xfer(
    std::uint8_t  rhport,
    std::uint8_t  ep_addr,
    std::uint8_t* buffer,
    std::uint16_t total_bytes
)
{

    static_cast<void>(rhport);

    const auto epnum     = tu_edpt_number(ep_addr);
    const auto direction = tu_edpt_dir   (ep_addr);

    if ( direction == TUSB_DIR_IN )
    {
        const auto rc = HAL_PCD_EP_Transmit(
            std::addressof(g_pcd_handle),
            epnum,
            buffer,
            static_cast<std::uint32_t>(total_bytes)
        );
        debug_assert(rc == HAL_OK);

        s_last_in_size = total_bytes;

        return rc == HAL_OK;
    }

    const auto rc = HAL_PCD_EP_Receive(
        std::addressof(g_pcd_handle),
        epnum,
        buffer,
        static_cast<std::uint32_t>(total_bytes)
    );
    debug_assert(rc == HAL_OK);

    s_last_out_size = total_bytes;

    return rc == HAL_OK;
}

// Submit a transfer where is managed by FIFO, When complete dcd_event_xfer_complete() is invoked to notify the stack - optional, however, must be listed in usbd.c

extern "C" bool dcd_edpt_xfer_fifo(
    std::uint8_t  rhport,
    std::uint8_t  ep_addr,
    tu_fifo_t*    ff,
    std::uint16_t total_bytes
)
{
    static_cast<void>(rhport);
    static_cast<void>(ep_addr);
    static_cast<void>(ff);
    static_cast<void>(total_bytes);

    return false;
}

// Stall endpoint

extern "C" void dcd_edpt_stall(
    std::uint8_t rhport,
    std::uint8_t ep_addr
)
{
    static_cast<void>(rhport);

    const auto rc = HAL_PCD_EP_SetStall(std::addressof(g_pcd_handle), ep_addr);
    debug_assert(rc == HAL_OK);
}

// clear stall, data toggle is also reset to DATA0

extern "C" void dcd_edpt_clear_stall(
    std::uint8_t rhport,
    std::uint8_t ep_addr
)
{
    static_cast<void>(rhport);

    const HAL_StatusTypeDef rc = HAL_PCD_EP_ClrStall(std::addressof(g_pcd_handle), ep_addr);
    debug_assert(rc == HAL_OK);
}

extern "C" void HAL_PCD_MspInit(PCD_HandleTypeDef* pcdHandle)
{
    static_cast<void>(pcdHandle);
}

extern "C" void HAL_PCD_MspDeInit(PCD_HandleTypeDef* pcdHandle)
{
    static_cast<void>(pcdHandle);
}

extern "C" void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef* hpcd)
{
    dcd_event_setup_received(0, (std::uint8_t*) hpcd->Setup, true);
}

extern "C" void HAL_PCD_DataOutStageCallback(
    PCD_HandleTypeDef* hpcd,
    std::uint8_t       epnum
)
{
    const auto count = HAL_PCD_EP_GetRxCount(hpcd, epnum);
    dcd_event_xfer_complete(0, epnum, count, XFER_RESULT_SUCCESS, true);
}

extern "C" void HAL_PCD_DataInStageCallback(
    PCD_HandleTypeDef* hpcd,
    std::uint8_t       epnum
)
{
    if ( epnum == 0 )
    {
        dcd_event_xfer_complete(0, 0x80 | epnum, hpcd->IN_ep[epnum & EP_ADDR_MSK].xfer_count, XFER_RESULT_SUCCESS, true);
    }

    else
    {
        dcd_event_xfer_complete(0, 0x80 | epnum, s_last_in_size, XFER_RESULT_SUCCESS, true);
    }
}

extern "C" void HAL_PCD_SOFCallback(PCD_HandleTypeDef* hpcd)
{
    static_cast<void>(hpcd);

    dcd_event_bus_signal(0, DCD_EVENT_SOF, true);
}

extern "C" void HAL_PCD_ResetCallback(PCD_HandleTypeDef* hpcd)
{
    static_cast<void>(hpcd);

    dcd_open_ep0(hpcd);
    dcd_event_bus_signal(0, DCD_EVENT_BUS_RESET, true);
}

extern "C" void HAL_PCD_SuspendCallback(PCD_HandleTypeDef* hpcd)
{
    static_cast<void>(hpcd);

    dcd_event_bus_signal(0, DCD_EVENT_SUSPEND, true);
}

extern "C" void HAL_PCD_ResumeCallback(PCD_HandleTypeDef* hpcd)
{
    static_cast<void>(hpcd);

    dcd_event_bus_signal(0, DCD_EVENT_RESUME, true);
}

extern "C" void HAL_PCD_ISOOUTIncompleteCallback(
    PCD_HandleTypeDef* hpcd,
    std::uint8_t       epnum
)
{
    static_cast<void>(hpcd);
    static_cast<void>(epnum);
}

extern "C" void HAL_PCD_ISOINIncompleteCallback(
    PCD_HandleTypeDef* hpcd,
    std::uint8_t       epnum
)
{
    static_cast<void>(hpcd);
    static_cast<void>(epnum);
}

extern "C" void HAL_PCD_ConnectCallback(PCD_HandleTypeDef* hpcd)
{
    static_cast<void>(hpcd);
}

extern "C" void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef* hpcd)
{
    static_cast<void>(hpcd);

    dcd_event_bus_signal(0, DCD_EVENT_UNPLUGGED, true);
}

} // namespace sys::hal
