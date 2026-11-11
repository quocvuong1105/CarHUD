#include "lcd.h"

#include "../config/board_config.h"

namespace hal {
namespace {

/** LCDWiki ES3C28P/ES3N28P: ILI9341V, 240x320, 4-wire SPI. */
class LcdWikiDevice final : public lgfx::LGFX_Device {
 public:
  LcdWikiDevice() {
    auto bus_cfg = bus_.config();
    bus_cfg.spi_host = SPI2_HOST;
    bus_cfg.spi_mode = 0;
    bus_cfg.freq_write = ESP32S3_LCD_SPI_HZ;
    bus_cfg.freq_read = 16000000;
    bus_cfg.spi_3wire = false;
    bus_cfg.use_lock = true;
    bus_cfg.pin_sclk = board::kLcdSclk;
    bus_cfg.pin_mosi = board::kLcdMosi;
    bus_cfg.pin_miso = board::kLcdMiso;
    bus_cfg.pin_dc = board::kLcdDc;
    bus_.config(bus_cfg);
    panel_.setBus(&bus_);

    auto panel_cfg = panel_.config();
    panel_cfg.pin_cs = board::kLcdCs;
    panel_cfg.pin_rst = board::kLcdRst;
    panel_cfg.pin_busy = -1;
    panel_cfg.panel_width = board::kLcdWidth;
    panel_cfg.panel_height = board::kLcdHeight;
    panel_cfg.memory_width = board::kLcdWidth;
    panel_cfg.memory_height = board::kLcdHeight;
    panel_cfg.offset_x = 0;
    panel_cfg.offset_y = 0;
    panel_cfg.offset_rotation = 0;
    panel_cfg.readable = true;
    // LCDWiki ILI9341V default. If colors are inverted on the actual batch,
    // change this to true after the first display test.
    panel_cfg.invert = false;
    panel_cfg.rgb_order = false;
    panel_cfg.dlen_16bit = false;
    panel_cfg.bus_shared = false;
    panel_.config(panel_cfg);
    setPanel(&panel_);
  }

 private:
  lgfx::Bus_SPI bus_;
  lgfx::Panel_ILI9341 panel_;
};

LcdWikiDevice g_device;
}  // namespace

lgfx::LGFX_Device& lcd() { return g_device; }
const char* lcd_panel_name() { return "ILI9341V IPS TFT 240x320"; }
uint32_t lcd_orientation_id() { return 0x45533301; }  // "ES3", driver revision 1

}  // namespace hal
