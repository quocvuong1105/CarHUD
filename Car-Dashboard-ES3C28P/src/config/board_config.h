/**
 * @file board_config.h
 * ESP32-S3 ES3C28P / ES3N28P board definition.
 * ESP32-S3 N16R8, ILI9341V 240x320 SPI LCD, FT6336G capacitive touch.
 * Pin allocation is based on the ES3C28P/ES3N28P hardware documentation.
 */
#pragma once

#include <stdint.h>

#define ESP32S3_ES3C28P 1
#define ESP32S3_ES3N28P 2
#ifndef ESP32S3_BOARD
#define ESP32S3_BOARD ESP32S3_ES3C28P
#endif

#ifndef ESP32S3_LCD_SPI_HZ
#define ESP32S3_LCD_SPI_HZ 40000000
#endif

// Compatibility macro retained for existing application references.// Kept as a compatibility macro because the application log uses this name.
#if ESP32S3_BOARD == ESP32S3_ES3N28P
#define CYD_BOARD_NAME "ESP32-S3 ES3N28P"
#else
#define CYD_BOARD_NAME "ESP32-S3 ES3N28P"
#endif

namespace board {
// LCD: ILI9341V, 4-wire SPI. RESET is tied to ESP32-S3 CHIP_PU / EN.
constexpr int kLcdSclk = 12;
constexpr int kLcdMosi = 11;
constexpr int kLcdMiso = 13;
constexpr int kLcdCs = 10;
constexpr int kLcdDc = 46;
constexpr int kLcdRst = -1;
constexpr int kLcdBacklight = 45;  // active HIGH
constexpr int kLcdWidth = 240;    // native portrait
constexpr int kLcdHeight = 320;

// Capacitive touch controller FT6336G, I2C address 0x38.
constexpr int kTouchSda = 16;
constexpr int kTouchScl = 15;
constexpr int kTouchRst = 18;  // active LOW
constexpr int kTouchIrq = 17;  // active LOW; polling is used by this port
constexpr uint8_t kTouchAddress = 0x38;

// ES3C28P has a single-wire RGB indicator, not the CYD's three PWM LED pins.
// The original CYD RGB driver is therefore disabled in this initial port.
constexpr int kLedRed = -1;
constexpr int kLedGreen = -1;
constexpr int kLedBlue = -1;

// This board has no CYD-style ambient-light sensor or PWM buzzer input.
constexpr int kLightSensor = -1;
constexpr bool kHasLightSensor = false;
constexpr int kSpeaker = -1;

// BOOT key is GPIO0, active LOW.
constexpr int kBootButton = 0;

// No CYD CN1 UART mapping is assumed for the ESP32-S3 board.
constexpr int kLinkRx = -1;
constexpr int kLinkTx = -1;

// LEDC channel assignments used by the existing HAL.
constexpr uint8_t kLedcBacklight = 0;
constexpr uint8_t kLedcSpeaker = 2;
constexpr uint8_t kLedcRed = 4;
constexpr uint8_t kLedcGreen = 5;
constexpr uint8_t kLedcBlue = 6;

// Legacy constants referenced by the existing light-sensor implementation.
constexpr uint16_t kLightRawBright = 40;
constexpr uint16_t kLightRawDark = 1500;
}  // namespace board
