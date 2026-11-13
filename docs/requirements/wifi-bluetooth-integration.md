# Đặc tả yêu cầu: Wi-Fi và Bluetooth BLE cho CarHUD

- Nhánh: `feature/wifi-bluetooth-integration`
- Trạng thái: Cập nhật theo bố cục Settings → Connection đã thống nhất; chờ duyệt trước khi sửa mã nguồn triển khai.
- Phạm vi triển khai giai đoạn hiện tại: Wi-Fi và Bluetooth BLE.
- USB được thể hiện trong thiết kế giao diện/kiến trúc nguồn dữ liệu, nhưng chưa triển khai giao tiếp phần cứng trong giai đoạn này.

## 1. Mục tiêu

Hoàn thiện trang hiện có `Settings → Connection` theo bố cục thống nhất bên dưới. Không tạo mục Connection cấp cao mới và không thay đổi cấu trúc điều hướng Settings ngoài phần cần thiết.

## 2. Bố cục Settings → Connection

### 2.1. OBD-II

- **Cấu hình nguồn dữ liệu**
- **Nguồn dữ liệu**:
  - Auto (tự động lựa chọn nguồn khả dụng).
  - Bluetooth BLE.
  - Wi-Fi.
  - USB.
- Nguồn được chọn thủ công phải được giữ lại; chế độ Auto mới thực hiện lựa chọn tự động theo các nguồn đã hỗ trợ.
- Không báo nguồn đang hoạt động nếu chưa có kết nối hợp lệ và chưa xác nhận giao tiếp OBD-II.

### 2.2. Wi-Fi

- Bật/tắt Wi-Fi.
- Quét và hiển thị các mạng Wi-Fi tìm thấy.
- Cho phép người dùng tự chọn SSID.
- Nhập mật khẩu và kết nối.
- Hiển thị trạng thái kết nối.
- Wi-Fi trên ESP32-S3 là Wi-Fi 2.4 GHz.

### 2.3. Bluetooth

- Bật/tắt BLE.
- Quét và hiển thị danh sách thiết bị BLE tìm thấy.
- Người dùng tự chọn thiết bị; không cố định tên hoặc model ELM327.
- Kết nối/ngắt kết nối.
- Hiển thị trạng thái kết nối.
- Không giả định mọi thiết bị BLE dùng cùng dịch vụ/giao thức. Giao tiếp ELM327 thực tế phải nhận diện dịch vụ và characteristic phù hợp.
- ESP32-S3 hỗ trợ BLE, không hỗ trợ Bluetooth Classic.

### 2.4. USB

- Tự nhận thiết bị.
- Hiển thị thông tin thiết bị.
- Hiển thị trạng thái kết nối.
- Trong giai đoạn hiện tại, đây là mục giao diện/định hướng kiến trúc. Chưa triển khai USB OBD-II, USB Host hoặc USB-UART; không được hiển thị USB là nguồn khả dụng/đã kết nối khi chưa có hỗ trợ thực tế.

### 2.5. Trạng thái OBD-II

Hiển thị tối thiểu:
- **Nguồn đang sử dụng** (ví dụ: Bluetooth BLE).
- **Thiết bị** (hiển thị tên thiết bị thực tế, ví dụ: ELM327; không hard-code tên).
- **Trạng thái** (ví dụ: Đã kết nối).

Trạng thái cần phân biệt rõ:
- Radio Wi-Fi/BLE đang bật hay tắt.
- Đang quét hoặc đã tìm thấy thiết bị.
- Đang kết nối, đã kết nối hoặc mất kết nối.
- Đã xác nhận giao tiếp OBD-II và đang nhận dữ liệu hợp lệ.

Không coi việc tìm thấy thiết bị hoặc chỉ thiết lập được liên kết BLE/Wi-Fi là bằng chứng đã nhận được dữ liệu OBD-II.

## 3. Quy tắc chọn nguồn dữ liệu

- **Auto**: tự lựa chọn trong số nguồn đã được triển khai và khả dụng; không chọn USB trong giai đoạn này khi USB chưa được triển khai.
- **Bluetooth BLE**: sử dụng thiết bị BLE do người dùng chọn.
- **Wi-Fi**: sử dụng kết nối Wi-Fi và thiết bị OBD-II tương thích được cấu hình.
- **USB**: chỉ là lựa chọn giao diện/placeholder ở giai đoạn hiện tại; chưa thực hiện kết nối.
- Giao diện phải phản ánh đúng nguồn thực sự đang hoạt động, không chỉ nguồn người dùng đã chọn.

## 4. Lưu cấu hình

- Dùng cơ chế Preferences/NVS hiện có của dự án.
- Lưu chế độ nguồn dữ liệu, trạng thái bật/tắt cần thiết, SSID và định danh thiết bị BLE cùng các tùy chọn cần thiết để hỗ trợ kết nối lại.
- Không hiển thị mật khẩu Wi-Fi đã lưu và không ghi mật khẩu vào log.
- Không lưu trạng thái kết nối giả định như thể thiết bị vẫn đang kết nối sau khi khởi động lại; phải kiểm tra kết nối thực tế.

## 5. Giới hạn phần cứng trong giai đoạn này

- Chỉ triển khai Wi-Fi và Bluetooth BLE tích hợp trên ESP32-S3.
- Không tích hợp CAN/TJA1050 hoặc TWAI.
- Không thay đổi chân GPIO, sơ đồ phần cứng hoặc đường USB hiện tại.
- Không triển khai USB OBD-II / USB Host / USB-UART.
- Cổng USB hiện tại không được mặc định xem là giao tiếp OBD-II; việc cấp nguồn và truyền dữ liệu là hai chức năng riêng biệt.

## 6. Bảo toàn chức năng hiện có

- Giữ nguyên trang Settings và cấu trúc điều hướng hiện tại.
- Giữ nguyên chế độ Demo/Live hiện có nếu chưa có thay đổi được duyệt; khi triển khai phải xác định rõ Live chỉ được báo hoạt động khi có dữ liệu OBD-II thực tế.
- Không thay đổi giao diện Dashboard ngoài phần cần thiết để hiển thị trạng thái kết nối đã được thống nhất.
- Giữ hỗ trợ các cấu hình bo mạch ES3C28P/ES3N28P hiện có.

## 7. Quy trình

1. Commit tài liệu yêu cầu và bố cục giao diện lên nhánh tính năng.
2. Gửi mã commit để người dùng kiểm tra.
3. Chờ xác nhận đặc tả cập nhật trước khi sửa mã nguồn triển khai.
4. Sau khi được duyệt mới triển khai theo từng phần, tạo commit riêng và kiểm tra GitHub Actions.
5. Không sửa nhánh `main`, không tự tạo PR hoặc hợp nhất nếu chưa được yêu cầu.

## 8. Tiêu chí kiểm thử dự kiến

- Wi-Fi có thể bật/tắt, quét mạng, chọn SSID, nhập mật khẩu, kết nối và phục hồi cấu hình sau khởi động.
- BLE có thể bật/tắt, quét, chọn thiết bị, kết nối/ngắt kết nối và phục hồi lựa chọn sau khởi động.
- Các mục OBD-II, Wi-Fi, Bluetooth, USB và Trạng thái OBD-II xuất hiện đúng bố cục đã thống nhất.
- USB được đánh dấu/chỉ thể hiện là chưa khả dụng trong giai đoạn hiện tại, không báo kết nối giả.
- Giao diện phân biệt rõ thiết bị được tìm thấy, liên kết đã kết nối và dữ liệu OBD-II đang nhận.
- Không làm hỏng chế độ Demo hiện có.
- Firmware build thành công cho cấu hình ES3C28P/ES3N28P được dự án hỗ trợ.
