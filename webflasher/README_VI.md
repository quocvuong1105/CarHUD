# CarHUD Firmware Flasher

Website tĩnh dùng GitHub Pages + ESP Web Tools. Website đọc GitHub Releases để hiển thị phiên bản mới nhất và các bản cũ.

## Cấu hình repository

Trong `app.js`, xác nhận:
- `OWNER = "quocvuong1105"`
- `REPO = "CarHUD"`

Nếu repository đổi tên hoặc owner, cập nhật hai giá trị này.

## Quy ước asset bắt buộc trong mỗi GitHub Release

Mỗi Release cần đính kèm các file merged binary đúng tên:
- `CarHUD-ES3C28P-merged.bin`
- `CarHUD-ES3N28P-merged.bin`

Chỉ đính kèm file cho bo đã được build và kiểm tra. Không đổi tên một binary chung thành hai biến thể nếu chưa xác nhận pinout và phần cứng hai mẫu tương thích.

`merged.bin` phải là image đã merge đúng offset và cấu hình Flash của ESP32-S3. Website không tự build hoặc tự chuyển đổi file `.bin`.

## Bật GitHub Pages

1. Đưa thư mục `webflasher/` lên thư mục gốc repository.
2. Đưa workflow `deploy-pages.yml` vào `.github/workflows/`.
3. GitHub → Settings → Pages → Build and deployment → Source: GitHub Actions.
4. Actions → Deploy CarHUD Flasher to GitHub Pages → Run workflow (hoặc push commit lên main).
5. Sau khi deploy xanh, mở URL Pages hiển thị trong Settings → Pages.

## Lưu ý

- ESP Web Tools yêu cầu HTTPS và Web Serial; dùng Chrome/Edge trên máy tính.
- Repository cần public để trang có thể đọc GitHub Releases bằng API không cần token, hoặc phải thiết kế cơ chế xác thực riêng.
- ESP Web Tools chọn chip family ESP32-S3; người dùng vẫn phải chọn ES3C28P/ES3N28P trong giao diện vì chip không tự nhận diện tên model.
- Hiện tại đây là website flasher scaffold. Chưa có Release firmware hợp lệ cho tới khi pipeline build tạo và đính kèm các merged binary.
