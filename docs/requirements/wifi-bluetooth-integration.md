# Đặc tả yêu cầu: Wi-Fi và Bluetooth BLE cho CarHUD

- Nhánh: `feature/wifi-bluetooth-integration`
- Trạng thái: Đã ghi nhận yêu cầu; chờ duyệt đặc tả trước khi sửa mã nguồn.
- Phạm vi hiện tại: chỉ Wi-Fi và Bluetooth BLE.

## 1. Mục tiêu

Bổ sung quản lý Wi-Fi và Bluetooth BLE bên trong trang hiện có `Settings → Connection`. Không tạo mục Connection cấp cao mới và không thay đổi cấu trúc điều hướng Settings ngoài phần cần thiết.

## 2. Wi-Fi

- Bật/tắt Wi-Fi.
- Quét và hiển thị các mạng Wi-Fi tìm thấy.
- Cho phép người dùng tự chọn SSID.
- Cho phép nhập mật khẩu và kết nối.
- Hiển thị trạng thái: tắt, đang quét, đang kết nối, đã kết nối, mất kết nối hoặc lỗi.
- Lưu SSID và cấu hình cần thiết bằng Preferences để có thể kết nối lại sau khi khởi động.
- Không hiển thị mật khẩu đã lưu trong giao diện và không ghi mật khẩu vào log.
- Wi-Fi trên ESP32-S3 là Wi-Fi 2.4 GHz.

## 3. Bluetooth

- Sử dụng Bluetooth Low Energy (BLE) tích hợp trên ESP32-S3; không yêu cầu module Bluetooth ngoài.
- Bật/tắt BLE.
- Quét và hiển thị danh sách thiết bị BLE tìm thấy.
- Người dùng tự chọn thiết bị, không cố định tên hoặc model ELM327.
- Cho phép kết nối/ngắt kết nối và hiển thị trạng thái.
- Lưu định danh thiết bị đã chọn để hỗ trợ kết nối lại.
- Không giả định mọi thiết bị BLE đều dùng cùng một dịch vụ/giao thức. Phần giao tiếp ELM327 thực tế phải nhận diện dịch vụ và characteristic phù hợp.
- ESP32-S3 không hỗ trợ Bluetooth Classic.

## 4. Trang Settings → Connection

- Giữ trang Connection hiện tại.
- Bổ sung các thiết lập Wi-Fi và Bluetooth BLE bên trong trang này.
- Rà soát hàng Nguồn dữ liệu và Trạng thái hiện có để phản ánh đúng chế độ dữ liệu và tình trạng kết nối.
- Phân biệt trạng thái radio (Wi-Fi/BLE bật hay tắt), trạng thái liên kết (đã kết nối hay chưa) và trạng thái nhận dữ liệu thực tế.
- Không hiển thị “đã kết nối” chỉ vì quét thấy thiết bị.
- Giữ nguyên chế độ Demo/Live hiện có nếu chưa có thay đổi được duyệt; xác định rõ ý nghĩa của Live khi triển khai kết nối thực tế.

## 5. Lưu cấu hình

- Dùng cơ chế Preferences/NVS hiện có của dự án.
- Lưu trạng thái bật/tắt, SSID và lựa chọn thiết bị BLE cùng các tùy chọn cần thiết.
- Không ghi mật khẩu Wi-Fi ra log hoặc hiển thị lại dưới dạng văn bản.

## 6. Ngoài phạm vi giai đoạn này

- Không tích hợp CAN/TJA1050 hoặc TWAI.
- Không thay đổi chân GPIO, sơ đồ phần cứng hoặc đường USB hiện tại.
- Không triển khai USB OBD-II / USB Host / USB-UART.
- Không thay đổi giao diện Dashboard ngoài phần cần thiết để hiển thị trạng thái kết nối đã được thống nhất.
- Không sửa nhánh `main`, không tự tạo PR hoặc hợp nhất.

## 7. Quy trình

1. Commit tài liệu đặc tả này để ghi nhận yêu cầu.
2. Gửi mã commit cho người dùng xem xét.
3. Chờ xác nhận đặc tả trước khi sửa bất kỳ mã nguồn triển khai nào.
4. Sau khi được duyệt mới triển khai theo từng phần, tạo commit riêng và kiểm tra GitHub Actions.
5. Không hợp nhất vào `main` nếu chưa được yêu cầu.

## 8. Tiêu chí kiểm thử dự kiến

- Wi-Fi có thể bật/tắt, quét mạng, chọn SSID, nhập mật khẩu, kết nối và phục hồi cấu hình sau khởi động.
- BLE có thể bật/tắt, quét, chọn thiết bị, kết nối/ngắt kết nối và phục hồi lựa chọn sau khởi động.
- Giao diện phân biệt rõ quét thấy thiết bị, đã kết nối và đang nhận dữ liệu.
- Không làm hỏng chế độ Demo hiện có.
- Firmware build thành công cho cấu hình ES3C28P/ES3N28P được dự án hỗ trợ.
