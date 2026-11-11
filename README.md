# CarHUD – ESP32-S3 (ES3C28P / ES3N28P)

CarHUD là firmware bảng đồng hồ xe sử dụng ESP32-S3 và bo ESP32-S3 phiên bản ES3C28P/ES3N28P.

Mã nguồn được phát triển từ dự án nền tảng [Car Dashboard ESP32 CYD](https://github.com/muki01/Car-Dashboard-ESP32-CYD), sau đó điều chỉnh phần cứng và phát triển theo định hướng CarHUD.

## Phần cứng hỗ trợ

- Nền tảng ESP32-S3, biến thể bo ES3C28P / ES3N28P.
- LCD ILI9341V: SPI SCK=GPIO12, MOSI=GPIO11, MISO=GPIO13, CS=GPIO10, DC=GPIO46; RESET dùng chung EN.
- Đèn nền LCD: GPIO45, active HIGH.
- Cảm ứng FT6336G: I2C SDA=GPIO16, SCL=GPIO15, địa chỉ 0x38; RESET=GPIO18, INT=GPIO17.
- LVGL 9.x và LovyanGFX 1.2.x.

## Biên dịch firmware

Mã nguồn yêu cầu ESP32 Arduino core 3.x, LVGL 9.6.0 và LovyanGFX 1.2.x. Dùng board ESP32S3 Dev Module, Flash 16 MB và PSRAM OPI. Sketch Arduino nằm trong thư mục `Car-Dashboard-ES3C28P/`.

GitHub Actions tự động biên dịch firmware khi push/PR theo cấu hình workflow. Bản phát hành chuẩn dùng tên `CarHUD-ES3C28P-merged.bin`.

## Web Flasher

- Repository: https://github.com/quocvuong1105/CarHUD
- Firmware Flasher: https://quocvuong1105.github.io/CarHUD/
- Hướng dẫn: [webflasher/README_VI.md](webflasher/README_VI.md)

Web Flasher hỗ trợ chọn file firmware `.bin` từ máy tính để nạp trực tiếp bằng Chrome/Edge qua USB. Chỉ nạp firmware đúng với bo mạch.

## Trạng thái và giới hạn

- Cần kiểm tra firmware trên bo ES3C28P/ES3N28P thực tế sau mỗi thay đổi phần cứng.
- Khả năng kết nối ELM327 phụ thuộc loại adapter và giao thức Bluetooth/Wi-Fi mà adapter hỗ trợ.
- ESP32-S3 không hỗ trợ Bluetooth Classic SPP; adapter chỉ có Classic SPP không thể kết nối trực tiếp theo kiểu này.

## Tài liệu phần cứng

- https://www.lcdwiki.com/2.8inch_ESP32-S3_Display
- https://www.lcdwiki.com/res/ES3C28P/2.8inch_ES3C28P_ES3N28P_arduino_Demo_Instructions.pdf
