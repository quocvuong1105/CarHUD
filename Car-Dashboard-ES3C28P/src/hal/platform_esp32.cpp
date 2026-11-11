/**
 * @file platform_esp32.cpp
 * Implementation of the platform port (core/platform.h) on the CYD.
 * restart() and factory_reset() live in app.cpp because they have to persist
 * application data first.
 */
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEClient.h>
#include <string.h>

#include "../config/board_config.h"
#include "../core/platform.h"
#include "../core/settings.h"
#include "backlight.h"
#include "buzzer.h"
#include "lcd.h"
#include "light_sensor.h"
#include "touch.h"

namespace platform {
namespace {

constexpr uint8_t kBleMaxDevices = 16;
BleDeviceInfo g_ble_devices[kBleMaxDevices] = {};
uint8_t g_ble_device_count = 0;
volatile bool g_ble_scan_active = false;
volatile bool g_ble_scan_completed = false;
bool g_ble_initialized = false;
BLEClient* g_ble_client = nullptr;
bool g_ble_connected = false;
portMUX_TYPE g_ble_mux = portMUX_INITIALIZER_UNLOCKED;

void ble_scan_complete(BLEScanResults results) {
  BleDeviceInfo found[kBleMaxDevices] = {};
  const uint32_t total = results.getCount();
  const uint8_t count = static_cast<uint8_t>(total < kBleMaxDevices ? total : kBleMaxDevices);

  for (uint8_t i = 0; i < count; ++i) {
    BLEAdvertisedDevice device = results.getDevice(i);
    const String address = device.getAddress().toString();
    const String name = device.haveName() ? device.getName() : String();
    snprintf(found[i].address, sizeof(found[i].address), "%s", address.c_str());
    // Use the MAC address as the visible name for unnamed peripherals so
    // users can distinguish multiple adapters without advertised names.
    snprintf(found[i].name, sizeof(found[i].name), "%s",
             name.length() == 0 ? address.c_str() : name.c_str());
    found[i].rssi = static_cast<int16_t>(device.getRSSI());
  }

  // Sort by RSSI descending: values closer to 0 dBm are stronger.
  // Insertion sort keeps the original order for devices with equal RSSI.
  for (uint8_t i = 1; i < count; ++i) {
    const BleDeviceInfo current = found[i];
    int8_t j = static_cast<int8_t>(i) - 1;
    while (j >= 0 && found[j].rssi < current.rssi) {
      found[j + 1] = found[j];
      --j;
    }
    found[j + 1] = current;
  }

  portENTER_CRITICAL(&g_ble_mux);
  memcpy(g_ble_devices, found, sizeof(found));
  g_ble_device_count = count;
  g_ble_scan_active = false;
  g_ble_scan_completed = true;
  portEXIT_CRITICAL(&g_ble_mux);
}

}  // namespace

uint32_t millis() { return ::millis(); }

bool ble_scan_start() {
  if (g_ble_scan_active) return false;
  if (!g_ble_initialized) {
    BLEDevice::init("");
    g_ble_initialized = true;
  }

  BLEScan* scanner = BLEDevice::getScan();
  if (scanner == nullptr) return false;
  scanner->setActiveScan(true);
  scanner->setInterval(100);
  scanner->setWindow(99);
  scanner->clearResults();

  portENTER_CRITICAL(&g_ble_mux);
  g_ble_device_count = 0;
  g_ble_scan_completed = false;
  g_ble_scan_active = true;
  portEXIT_CRITICAL(&g_ble_mux);

  if (!scanner->start(6, ble_scan_complete, false)) {
    portENTER_CRITICAL(&g_ble_mux);
    g_ble_scan_active = false;
    g_ble_scan_completed = true;
    portEXIT_CRITICAL(&g_ble_mux);
    return false;
  }
  return true;
}

bool ble_connect(const char* address) {
  if (address == nullptr || address[0] == 0) return false;
  if (!g_ble_initialized) {
    BLEDevice::init("");
    g_ble_initialized = true;
  }

  g_ble_connected = false;
  // Arduino-ESP32 BLE 3.2.1 has no client-deletion API.
  // Reuse the client object; disconnect it before attempting another device.
  if (g_ble_client != nullptr && g_ble_client->isConnected()) {
    g_ble_client->disconnect();
  }
  if (g_ble_client == nullptr) {
    g_ble_client = BLEDevice::createClient();
  }
  if (g_ble_client == nullptr) return false;
  if (!g_ble_client->connect(BLEAddress(address))) return false;

  // A successful GATT connection is distinct from a successful OBD-II
  // command exchange; the latter is verified by the data-link layer.
  g_ble_connected = g_ble_client->isConnected();
  return g_ble_connected;
}

void ble_disconnect() {
  g_ble_connected = false;
  if (g_ble_client != nullptr && g_ble_client->isConnected()) {
    g_ble_client->disconnect();
  }
}

bool ble_connected() {
  g_ble_connected = g_ble_client != nullptr && g_ble_client->isConnected();
  return g_ble_connected;
}

bool ble_scan_active() { return g_ble_scan_active; }

bool ble_scan_completed() { return g_ble_scan_completed; }

uint8_t ble_scan_count() {
  portENTER_CRITICAL(&g_ble_mux);
  const uint8_t count = g_ble_device_count;
  portEXIT_CRITICAL(&g_ble_mux);
  return count;
}

bool ble_scan_get(uint8_t index, BleDeviceInfo& out) {
  portENTER_CRITICAL(&g_ble_mux);
  const bool valid = index < g_ble_device_count;
  if (valid) memcpy(&out, &g_ble_devices[index], sizeof(out));
  portEXIT_CRITICAL(&g_ble_mux);
  return valid;
}

void system_info(SystemInfo& out) {
  out.chip_model = ESP.getChipModel();
  out.chip_revision = ESP.getChipRevision();
  out.cores = ESP.getChipCores();
  out.cpu_mhz = ESP.getCpuFreqMHz();
  out.flash_bytes = ESP.getFlashChipSize();
  out.heap_free = ESP.getFreeHeap();
  out.heap_min_free = ESP.getMinFreeHeap();
  out.heap_total = ESP.getHeapSize();
  out.sdk_version = ESP.getSdkVersion();
  out.board_name = CYD_BOARD_NAME;
  out.panel_name = hal::lcd_panel_name();
}

uint16_t light_sensor_raw() { return hal::light_sensor::raw(); }

uint8_t ambient_level() { return hal::light_sensor::level(); }

uint8_t backlight_level() { return hal::backlight::level(); }

void play(Sound sound) {
  const core::Settings& s = core::settings::get();
  switch (sound) {
    case Sound::Click:
      if (s.touch_sound) hal::buzzer::play(hal::buzzer::Sound::Click);
      break;
    case Sound::Confirm:
      if (s.touch_sound) hal::buzzer::play(hal::buzzer::Sound::Confirm);
      break;
    case Sound::Error:
      if (s.touch_sound) hal::buzzer::play(hal::buzzer::Sound::Error);
      break;
    case Sound::Startup:
      if (s.touch_sound) hal::buzzer::play(hal::buzzer::Sound::Startup);
      break;
    case Sound::Warning:
      if (s.alert_sound) hal::buzzer::play(hal::buzzer::Sound::Warning);
      break;
    case Sound::Critical:
      if (s.alert_sound) hal::buzzer::play(hal::buzzer::Sound::Critical);
      break;
  }
}

bool touch_read_raw(TouchSample& out) { return hal::touch::read_raw(out); }

void touch_apply_calibration(const core::TouchCalibration& cal) { hal::touch::apply_calibration(cal); }

void touch_input_enable(bool enable) { hal::touch::set_enabled(enable); }

}  // namespace platform
