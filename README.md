# 🚀 Cờ cá ngựa

## 📌 Giới thiệu
Phân tích chiến lược trờ chơi cờ cá ngựa.

-> Slide: https://canva.link/76a6jfhn41r5cft

## 📌Luật chơi
1. Bàn cờ và Quân cờ
  - Cấu trúc: 4 màu (Xanh dương, Đỏ, Xanh lá, Vàng). Mỗi màu có 4 quân ngựa.
  - Lộ trình: 48 ô vòng tròn chung. Sau khi đi hết 1 vòng, ngựa tiến vào đường dẫn 6 ô bậc thang để về đích.
3. Quy tắc Di chuyển & Tương tác
  - Xuất quân: Chỉ khi tung được xúc xắc 6.
  - Di chuyển: Đi theo số nút xúc xắc theo chiều kim đồng hồ.
  - Vật cản (Blocking): Nếu trên đường đi (trong phạm vi số nút) có ngựa khác đứng chặn, quân đang đi không thể vượt qua (phải chọn quân khác hoặc bỏ lượt).
  - Đá ngựa (Kicking): Nếu ô đích trùng với vị trí quân đối phương, quân đối phương bị đá về chuồng.
4. Cơ chế Thêm lượt 
-> Người chơi được tung xúc xắc tiếp ngay lập tức nếu rơi vào 1 trong 3 trường hợp:
  1.Tung được xúc xắc 6.
  2.Đá được ngựa của đối phương về chuồng.
  3.Đưa được ngựa về đích (vào một trong các ô bậc thang thành công).

## 📌Chiến thuật
1. Chiến thuật "Hung hăng" (Aggressive)
  - Ưu tiên: Luôn chọn con ngựa có thể Đá đối thủ.
  - Lý do: Tận dụng luật "Thêm lượt" khi đá ngựa để đi được quãng đường dài nhất và kìm hãm đối phương.
  - Đặc điểm: Tỷ lệ va chạm cực cao, tạo ra nhiều biến số trên bàn cờ.
2. Chiến thuật "Ưu tiên về đích" (Sprinter)
  - Ưu tiên: Luôn chọn con ngựa đang ở vị trí cao nhất (gần chuồng nhất).
  - Lý do: Tận dụng luật "Thêm lượt" khi về đích để nhanh chóng kết thúc ván đấu.
  - Đặc điểm: Tập trung vào tốc độ, cố gắng đưa từng con ngựa vào vùng an toàn sớm nhất.
3. Chiến thuật "Dàn quân - Rào chắn" (The Wall)
  - Ưu tiên: Luôn di chuyển con ngựa ở vị trí thấp nhất (gần điểm xuất phát nhất).
  - Lý do: Giữ đội hình san sát nhau để tạo rào chắn, khiến đối thủ phía sau không thể đi xuyên qua.
  - Đặc điểm: Chơi chắc chắn, phòng thủ tốt, đợi đối thủ sơ hở để đá.
4. Chiến thuật "Ngẫu nhiên" (Random)
  - Ưu tiên: Chọn đại một con ngựa bất kỳ có thể đi.
  - Lý do: Dùng làm nhóm đối chứng để so sánh sức mạnh của 3 chiến thuật trên.


