#include "menu.h"

void print_menu() { 
    wprintf(L"Đồ án: SEARCH ENGINE\n"); 
    wprintf(L"-------------------------------------\n"); 
    wprintf(L"Các chức năng chính của chương trình:\n"); 
    wprintf(L"1. Tìm kiếm bằng chuỗi từ.\n"); 
    wprintf(L"2. Thêm file vào file metadata.\n"); 
    wprintf(L"3. Thêm các file text từ 1 folder vào metadata.\n"); 
    wprintf(L"4. Chỉnh sửa file từ file metadata.\n"); 
    wprintf(L"5. Xóa file từ file metadata.\n"); 
    wprintf(L"6. Xây dựng lại file metadata.\n"); 
    wprintf(L"7. Thoát chương trình.\n"); 
    wprintf(L"-------------------------------------\n"); 
}

void run() {   
    build_vnm_map(); 
    get_stopword(); 
    _setmode(_fileno(stdin), 0x20000);
    _setmode(_fileno(stdout), 0x20000);

    while (1) { 
        system("cls"); 
        int type; 
        print_menu(); 
        type = read_choice(); 
        // wprintf(L"%d\n", type); 
        if (type > 6 || type < 1) 
            break; 
        double t;
        switch (type) {
            case 1:
                t = search(); 
                break;
            case 2:
                t = add_file();
                wprintf(L"-------------------------------------\n"); 
                wprintf(L"Thời gian thực hiện thao tác thêm file khoảng: %fs.\n", t);
                break;
            case 3:
                t = add_folder();
                wprintf(L"-------------------------------------\n"); 
                wprintf(L"Thời gian thực hiện thao tác thêm file từ folder khoảng: %fs.\n", t);
                break;
            case 4:
                t = edit_file();
                wprintf(L"-------------------------------------\n"); 
                wprintf(L"Thời gian thực hiện thao tác chỉnh sửa file khoảng: %fs.\n", t);
                break;
            case 5:
                t = delete_file(); 
                wprintf(L"-------------------------------------\n"); 
                wprintf(L"Thời gian thực hiện thao tác xóa file khoảng: %fs.\n", t);
                break;
            case 6:
                t = build_metadata(); 
                wprintf(L"-------------------------------------\n"); 
                wprintf(L"Thời gian thực hiện thao tác xây dựng tập siêu văn bản khoảng: %fs.\n", t);
                break;
            default:
                break;
        }
        if (type != 1) 
            delay(); 
    }

    _setmode(_fileno(stdin), 0x4000);
    _setmode(_fileno(stdout), 0x4000);
    safe_delete(); 
}