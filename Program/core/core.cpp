#include "core.h"

int read_choice() { 
    int ret; 
    wprintf(L"Nhập sự lựa chọn của bạn: "); 
    wscanf(L"%d%*lc", &ret); 
    wprintf(L"-------------------------------------\n"); 
    return ret; 
}

void delay() { 
    wprintf(L"-------------------------------------\n"); 
    wprintf(L"Mời bạn nhấn Enter để tiếp tục.\n");  
    rewind(stdin);
    getwchar();
}

void print_utf8(const char *str) { 
    _setmode(_fileno(stdout), 0x4000);
    printf("%s\n", str); 
    _setmode(_fileno(stdout), 0x20000);
}

void print_search_menu(int total_result, int cur_result) { 
    wprintf(L"Các chức năng trong phần tìm kiếm:\n"); 
    if (cur_result < total_result)
        wprintf(L"1. In ra thêm %d văn bản liên quan.\n", total_result - cur_result < 5 ? total_result - cur_result : 5); 
    wprintf(L"2. In văn bản ra console.\n"); 
    wprintf(L"3. Trở lại menu chính.\n");  
    wprintf(L"-------------------------------------\n"); 
}

void print_list_file_search(int total_result, int &cur_result, int *sorted_index, int *score, int *frequency, double time1) {   
    wprintf(L"Tìm thấy %d kết quả trong %f giây.\n", total_result, time1);
    wprintf(L"-------------------------------------\n");
    wprintf(L"Top %d văn bản có từ khóa liên quan:\n", cur_result); 
    for (int i = 0; i < cur_result; ++i) { 
        char *filename;
        read_filename_index(sorted_index[i], filename); 
        wprintf(L"%d. ", i + 1); 
        print_utf8(filename); 
        wprintf(L"Số điểm: %d, ", score[sorted_index[i]]); 
        wprintf(L"Số lần xuất hiện của các từ khóa trong văn bản: %d\n\n", frequency[sorted_index[i]]); 
        free(filename); 
    }
    wprintf(L"-------------------------------------\n");
}

void print_file(const char *filename) { 
    FILE *temp = fopen(filename, "r"); 
    if (file_not_exist(temp)) { 
        wprintf(L"File không tồn tại trên máy.\n"); 
        fclose(temp); 
        return; 
    }
    fclose(temp); 
    wprintf(L"Văn bản tại đường dẫn: ", filename);  
    print_utf8(filename); 
    wprintf(L"\n"); 
    wchar_t *text; 
    read_file_utf16(filename, text); 
    wprintf(L"%ls\n", text); 
    free(text); 
}

////////////////////////////////////////////////////////////////////////////////

int add_file_to_metadata(const char *filename, int type, FILE *metadata) { 
    FILE *f = fopen(filename, "r"); 
    if (file_not_exist(f)) 
        return 1; 
    fclose(f);
        
    if (type) { 
        if (exist_file(filename) != -1) 
            return 2; 
        int temp, cnt_file; 

        FILE *fi = fopen("index.inp", "a+"); 
        fseek(fi, 0, SEEK_END); 
        temp = ftell(fi); 
        fprintf(fi, "%s\n", filename); 
        fclose(fi); 

        fi = fopen("position.txt", "r+b"); 
        fseek(fi, -4, SEEK_END); 
        fread(&cnt_file, sizeof(int), 1, fi); 
        fseek(fi, -4, SEEK_END); 
        fwrite(&temp, sizeof(int), 1, fi); 
        temp = ftell(metadata); 
        fwrite(&temp, sizeof(int), 1, fi); 
        cnt_file++; 
        fwrite(&cnt_file, sizeof(int), 1, fi); 
        fclose(fi); 
    }

    wchar_t  *text; 
    char *text_utf8; 

    read_file_utf16(filename, text);
    standardized(text); 
    remove_stopword(text); 
    remove_vnm_accents(text, text_utf8);   

    for (int len = 1; len <= 3; ++len) { 
        int cnt_word = 0, total_word = 0; 
        char **word = NULL; 
        int *frequency; 

        split_text_to_word(text_utf8, word, cnt_word, len); 
        total_word = cnt_word; 
        sort_string_arr(word, cnt_word); 
        unique_string_arr(word, cnt_word, frequency); 
        find_keyword(word, cnt_word, total_word, frequency, 3, 100 * len); 

        fprintf(metadata, "%d\n", cnt_word); 
        for (int i = 0; i < cnt_word; ++i)  
            fprintf(metadata, "%d %s\n", frequency[i], word[i]); 

        for (int i = 0; i < cnt_word; ++i) 
            free(word[i]); 
        free(word); 
        free(frequency); 
    }

    free(text); 
    free(text_utf8);

    return 0; 
}

int delete_file_from_metadata(const char *filename) {
    int index = exist_file(filename); 
    if (index == -1) 
        return 0; 

    int cnt_file = 0; 

    FILE *f = fopen("position.txt", "rb"); 
    fseek(f, -4, SEEK_END); 
    fread(&cnt_file, sizeof(int), 1, f); 
    fseek(f, 0, SEEK_SET); 
    int *postition_arr = (int*)(malloc(cnt_file * 2 * sizeof(int)));
    fread(postition_arr, sizeof(int), cnt_file * 2, f); 
    fclose(f); 

    int bytes_mdata = index < cnt_file - 1 ? postition_arr[index * 2 + 3] - postition_arr[index * 2 + 1] : -1; 
    int bytes_idata = index < cnt_file - 1 ? postition_arr[index * 2 + 2] - postition_arr[index * 2] : -1;
    truncate("metadata.txt", postition_arr[index * 2 + 1], bytes_mdata); 
    truncate("index.inp", postition_arr[index * 2], bytes_idata); 
    
    remove("position.txt"); 
    for (int i = index + 1; i < cnt_file; ++i) { 
        postition_arr[(i - 1) * 2] = postition_arr[i * 2] - bytes_idata; 
        postition_arr[(i - 1) * 2 + 1] = postition_arr[i * 2 + 1] - bytes_mdata; 
    }

    f = fopen("position.txt", "wb");  
    cnt_file--; 
    if (cnt_file) 
        fwrite(postition_arr, sizeof(int), cnt_file * 2, f);  
    fwrite(&cnt_file, sizeof(int), 1, f); 
    fclose(f); 

    free(postition_arr); 

    return 1; 
}

double build_metadata() { 
    wchar_t *path_utf16; 
    char *path; 

    read_utf16(L"Nhập đường dẫn thư mục chứa tập văn bản muốn xây dựng siêu văn bản:\n", path_utf16); 

    double time1 = clock(); 

    remove_vnm_accents(path_utf16, path); 

    if (!check_exist_folder(path)) { 
        wprintf(L"Đường dẫn thư mục nhập vào không tồn tại.\n");  
        free(path_utf16); 
        free(path); 
        return (clock() - time1) / CLOCKS_PER_SEC; 
    }

    wprintf(L"-------------------------------------\n");
    wprintf(L"Đang xây dựng tập siêu văn bản...\n"); 

    _setmode(_fileno(stdout), 0x4000); 

    list_file_to_index(path, "index.inp");
    FILE *findex = fopen("index.inp", "r"); 
    FILE *fmdata = fopen("metadata.txt", "w"); 
    FILE *fpos = fopen("position.txt", "wb"); 

    int cnt_file = 0, total_file = count_file(findex); 
    char filename[1000]; 

    int cnt_loading = 50 > total_file ? total_file : 50; 

    for (int i = 0; i < cnt_loading; i++)
        printf("%c", 177);
    printf("\r");

    while (1) { 
        int pos = ftell(findex); 
        fgets(filename, 1000, findex); 
        if (feof(findex)) 
            break; 
        fwrite(&pos, sizeof(int), 1, fpos); 
        pos = ftell(fmdata); 
        fwrite(&pos, sizeof(int), 1, fpos); 
        cnt_file++; 
        filename[strlen(filename) - 1] = '\0'; 
        add_file_to_metadata(filename, 0, fmdata); 

        if (cnt_file * cnt_loading / total_file != (cnt_file - 1) * cnt_loading / total_file) 
            printf("%c", 219); 
    }
    fwrite(&cnt_file, sizeof(int), 1, fpos); 

    printf("\n"); 

    _setmode(_fileno(stdout), 0x20000); 

    fclose(fpos); 
    fclose(findex); 
    fclose(fmdata); 
    free(path_utf16); 
    free(path); 

    wprintf(L"Đã thêm thành công %d file.\n", cnt_file); 

    return (clock() - time1) / CLOCKS_PER_SEC; 
}

double search() { 
    wchar_t *text; 
    char *text_utf8;

    read_utf16(L"Nhập chuỗi từ muốn tìm kiếm: \n", text); 

    double time1 = clock(); 

    standardized(text); 
    remove_stopword(text); 
    remove_vnm_accents(text, text_utf8);   

    
    int cnt_word[3] = {0, 0, 0}; 
    char ***word = (char***)(malloc(3 * sizeof(char**)));  

    for (int len = 0; len < 3; ++len) 
        split_text_to_word(text_utf8, word[len], cnt_word[len], len + 1); 

    int cnt_file = 0; 

    FILE *f = fopen("position.txt", "rb"); 
    fseek(f, -4, SEEK_END); 
    fread(&cnt_file, sizeof(int), 1, f); 
    fclose(f); 

    int *frequency, *score; 

    frequency = (int*)(malloc(cnt_file * sizeof(int))); 
    score = (int*)(malloc(cnt_file * sizeof(int))); 

    f = fopen("metadata.txt", "r"); 

    for (int iFile = 0; iFile < cnt_file; ++iFile) {
        frequency[iFile] = score[iFile] = 0; 
        for (int len = 0; len < 3; ++len) { 
            int cnt = 0, *frequency_text; 
            char **keyword; 
            read_data_metadata(f, cnt, keyword, frequency_text); 

            for (int i = 0; i < cnt_word[len]; ++i) { 
                int x = binary_search_keyword(word[len][i], cnt, keyword, frequency_text); 
                score[iFile] += (x > 0) * (len * 2 + 1); 
                frequency[iFile] += x; 
            }

            for (int i = 0; i < cnt; ++i) 
                free(keyword[i]); 
            free(keyword); 
            free(frequency_text);
        }
    }

    fclose(f); 

    int *sorted_index = (int*)(malloc(cnt_file * sizeof(int*))); 
    for (int i = 0; i < cnt_file; ++i) 
        sorted_index[i] = i; 
    sort_index_arr(sorted_index, cnt_file, frequency, score);

    int total_result = 0, cur_result = 0; 
    for (int i = 0; i < cnt_file; ++i) 
        total_result += score[sorted_index[i]] > 0; 

    time1 = (clock() - time1) / CLOCKS_PER_SEC; 

    // print result

    // if (strlen(text_utf8) == 0) { 
    //     total_result = cnt_file;
    // }
    if (total_result == 0) { 
        wprintf(L"-------------------------------------\n");
        wprintf(L"Không tìm thấy văn bản nào liên quan.\n");  
        delay(); 
        return time1; 
    }

    int type = 1, id; 
    while (1) { 
        // print search menu 
        system("cls"); 
        switch (type) {
            case 1:
                cur_result += 5; 
                if (cur_result > total_result) 
                    cur_result = total_result; 

                break;

            case 2:
                if (id <= 0 || total_result < id) { 
                    wprintf(L"Số thứ tự của file nhập vào không nằm trong danh sách văn bản liên quan.\n"); 
                } else { 
                    char *filename; 
                    read_filename_index(sorted_index[id - 1], filename);
                    print_file(filename);   
                    free(filename);  
                }
                delay();
                system("cls");
                
                break;
            
            default:
                break;
        }
        print_list_file_search(total_result, cur_result, sorted_index, score, frequency, time1); 
        print_search_menu(total_result, cur_result);
        type = read_choice();  
        if (type == 2) { 
            wprintf(L"Nhập số thứ tự văn bản muốn in ra console: ");  
            wscanf(L"%d%*lc", &id);  
        }
        if (type == 1 && cur_result == total_result) 
            break; 
        if (type != 1 && type != 2) 
            break; 
    }

    // safe delete dynamic array
    free(frequency); 
    free(score); 
    free(text); 
    free(text_utf8); 
    free(sorted_index);
    for (int len = 0; len < 3; ++len) { 
        for (int i = 0; i < cnt_word[len]; ++i) 
            free(word[len][i]); 
        free(word[len]);
    }
    free(word); 

    return time1; 
}

double add_file() { 
    wchar_t *filename_utf16; 
    char *filename; 

    read_utf16(L"Nhập đường dẫn tuyệt đối file muốn thêm vào: \n", filename_utf16); 
    
    double time1 = clock(); 

    remove_vnm_accents(filename_utf16, filename); 

    FILE *fmdata = fopen("metadata.txt", "a+"); 
    fseek(fmdata, 0, SEEK_END); 
    int type = add_file_to_metadata(filename, 1, fmdata); 
    fclose(fmdata); 

    switch (type) {
        case 0:
            wprintf(L"Thêm file thành công.\n"); 
            break;
        case 1:
            wprintf(L"File không tồn tại.\n"); 
            break;
        case 2:
            wprintf(L"File muốn thêm vào đã được thêm vào trước đó.\n"); 
            break;
    }

    free(filename_utf16); 
    free(filename); 

    return (clock() - time1) / CLOCKS_PER_SEC; 
}

double add_folder() { 
    wchar_t *path_utf16; 
    char *path; 

    read_utf16(L"Nhập đường dẫn thư mục chứa tập văn bản muốn thêm vào:\n", path_utf16); 

    double time1 = clock(); 

    remove_vnm_accents(path_utf16, path); 

    if (!check_exist_folder(path)) { 
        wprintf(L"Đường dẫn thư mục nhập vào không tồn tại.\n");  
        free(path_utf16); 
        free(path); 
        return (clock() - time1) / CLOCKS_PER_SEC; 
    }

    list_file_to_index(path, "temp_index.inp");

    FILE *fmdata = fopen("metadata.txt", "a+"); 
    FILE *findex = fopen("temp_index.inp", "r"); 
    fseek(fmdata, 0, SEEK_END); 

    int cnt = 0; 
    int cnt_file = 0, total_file = count_file(findex); 

    int cnt_loading = 50 > total_file ? total_file : 50;

    wprintf(L"-------------------------------------\n");
    wprintf(L"Đang thêm file từ folder...\n"); 

    _setmode(_fileno(stdout), 0x4000); 
    
    for (int i = 0; i < cnt_loading; i++)
        printf("%c", 177);
    printf("\r");

    char filename[1000]; 

    while (1) { 
        fgets(filename, 1000, findex); 
        if (feof(findex)) 
            break; 
        filename[strlen(filename) - 1] = '\0'; 
        cnt += add_file_to_metadata(filename, 1, fmdata) == 0; 
        cnt_file++; 
        if (cnt_file * cnt_loading / total_file != (cnt_file - 1) * cnt_loading / total_file) 
            printf("%c", 219); 
    }

    fclose(findex); 
    fclose(fmdata); 
    free(path_utf16); 
    free(path); 
    remove("temp_index.inp"); 

    printf("\n"); 

    _setmode(_fileno(stdout), 0x20000);
 
    wprintf(L"Đã thêm thành công %d file từ folder nhập vào.\n", cnt); 

    return (clock() - time1) / CLOCKS_PER_SEC; 
}

double delete_file() { 
    wchar_t *filename_utf16; 
    char *filename; 

    read_utf16(L"Nhập đường dẫn tuyệt đối file muốn xóa: \n", filename_utf16); 

    double time1 = clock(); 

    remove_vnm_accents(filename_utf16, filename); 
    if (!delete_file_from_metadata(filename)) { 
        wprintf(L"File muốn xóa chưa được thêm vào trước đó.\n"); 
        free(filename_utf16); 
        free(filename); 
        return (clock() - time1) / CLOCKS_PER_SEC; 
    } 

    wprintf(L"Xóa file thành công.\n");
    
    free(filename_utf16); 
    free(filename); 

    return (clock() - time1) / CLOCKS_PER_SEC; 
}

double edit_file() { 
    wchar_t *filename_utf16; 
    char *filename; 

    read_utf16(L"Nhập đường dẫn tuyệt đối file muốn chỉnh sửa: \n", filename_utf16); 
    
    double time1 = clock(); 

    remove_vnm_accents(filename_utf16, filename); 

    // delete file 
    if (!delete_file_from_metadata(filename)) { 
        wprintf(L"File không tồn tại.\n"); 
        free(filename_utf16); 
        free(filename); 
        return (clock() - time1) / CLOCKS_PER_SEC;
    }

    // edit file 
    open_notepad(filename); 

    // add file 
    FILE *fmdata = fopen("metadata.txt", "a+"); 
    fseek(fmdata, 0, SEEK_END); 
    add_file_to_metadata(filename, 1, fmdata); 

    fclose(fmdata); 
    free(filename_utf16); 
    free(filename); 

    wprintf(L"Chỉnh sửa file thành công.\n"); 

    return (clock() - time1) / CLOCKS_PER_SEC;
}