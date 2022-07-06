=============================================================================
                    Đồ án kỹ thuật lập trình: Search Engine
                            Sinh viên thực hiện: 
                         Hồ Văn Sơn, Trần Hữu Thiên 
=============================================================================

Nội dung cài đặt của các file: 

sort\sort.h
	Header file: Khai báo các hàm sắp xếp chuỗi và sắp xếp mảng. 

sort\sort.cpp 
	Cài đặt các hàm đã được khai báo tại file header sort.h

file\file.h
	Header file: Khai báo các hàm thao tác trên file như kiểm tra file 
	tồn tại, lấy kích thước của file, cắt file,... 

file\file.cpp 
	Cài đặt các hàm đã được khai báo tại file header file.h

fstring\fstring.h 
	Header file: Khai báo các hàm xử lý văn bản như đọc file text UTF16,
	UTF8, chuẩn hóa văn bản, tìm keyword cho văn bản,... 

fstring\fstring.cpp
	Cài đặt các hàm đã được khai báo tại file header fstring.h

core\core.h 
	Header file: Khai báo các hàm xử lí cho các chức năng của chương 
	trình bao gồm: Tìm kiếm bằng chuỗi, xóa file, thêm file, thêm 
	folder, xây tập metadata,... 

core\core.cpp 
	Cài đặt các hàm đã được khai báo tại file header core.h 

menu\menu.h 
	Header file: Thiết kế giao diện trên console cho người dùng chọn 
	các chức năng để thao tác. 

menu\menu.cpp 
	Cài đặt các hàm đã được khai báo tại file menu.h

main.cpp 	
	Chương trình chính: Chạy chương trình. 

//////////////////////////////////////////////////////////////////////////////

Biên dịch chương trình: 

- Mở Command Prompt tại thư mục chứa các file mã nguồn của chương trình (folder 
Program). 
- Dùng lệnh dịch: "g++ -std=gnu++11 -o 20120021_20120584.exe *.cpp *\*.cpp" 
(không chứa dấu ngoặc kép). 

//////////////////////////////////////////////////////////////////////////////

Sử dụng chương trình dạng Command line: 

- Mở Command Prompt tại thư mục chứa các file thực thi của chương trình
(20120021_20120584.exe). 
- Dùng lệnh: "20120021_20120584.exe", chương trình sẽ đưa ra menu các chức năng 
cho người dùng lựa chọn để thao tác. 

//////////////////////////////////////////////////////////////////////////////

Các chức năng chính của chương trình: 

1. Tìm kiếm bằng chuỗi từ: 
	Cho phép người dùng nhập vào một chuỗi từ muốn tìm kiếm trong tập tin
	đã được xây dựng thành siêu văn bản, trả về danh sách các văn bản liên 
	quan được sắp xếp giảm dần theo mức độ liên quan. 
1.1. In thêm danh sách văn bản liên quan 
1.2. In văn bản ra console  

2. Thêm file vào file metadata. 
	Người dùng nhập vào đường dẫn tuyệt đối của file muốn thêm vào siêu 
	văn bản, nếu file chưa được thêm vào trước đó thì siêu văn bản sẽ 
	được cập nhật. 

3. Thêm các file text từ 1 folder vào metadata. 
	Tương tự chức năng thêm file tuy nhiên ở đây người dùng sẽ thêm tất cả
	file text có trong thư mục mà trước đó chưa được thêm vào siêu văn bản. 

4. Chỉnh sửa file từ file metadata. 
	Cho phép người dùng nhập vào đường dẫn tuyệt đối của một file muốn 
	chỉnh sửa đã có sẵn, mở bằng notepad và chỉnh sửa sau đó chương trình sẽ
	tự động cập nhật siêu văn bản. 

5. Xóa file từ file metadata.
	Xóa một file đã thêm trước đó ở siêu văn bản.

6. Xây dựng lại file metadata.
	Xây lại siêu văn bản từ các văn bản ở folder được nhập vào. 

//////////////////////////////////////////////////////////////////////////////



	



