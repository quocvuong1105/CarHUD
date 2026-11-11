/**
 * CarHUD - Vehicle dashboard for LCDWiki ES3C28P / ES3N28P
 *
 * Board:     ESP32S3 Dev Module (esp32 by Espressif Systems, core 3.x)
 * Libraries: lvgl 9.x, LovyanGFX 1.2.x
 * Config:    lv_conf.h (this folder), src/config/board_config.h, src/config/app_config.h
 *
 * The sketch file only hands over to the application; see README.md for the
 * architecture and build instructions.
 */
#include "src/app/app.h"

void setup() { app::setup(); }

void loop() { app::loop(); }
