#include "fstring.h"

const int NUM_VNM = 67;
const int NUM_SWORD = 1942; 
const int NUM_MAP = 1 << 16; 
const wchar_t VNM_UPPER[] = {L'À',L'Á',L'Ạ',L'Ả',L'Ã',L'Â',L'Ầ',L'Ấ',L'Ậ',L'Ẩ',L'Ẫ',L'Ă',L'Ằ',L'Ắ',L'Ặ',L'Ẳ',L'Ẵ',L'È',L'É',L'Ẹ',L'Ẻ',L'Ẽ',L'Ê',L'Ề',L'Ế',L'Ệ',L'Ể',L'Ễ',L'Ì',L'Í',L'Ị',L'Ỉ',L'Ĩ',L'Ò',L'Ó',L'Ọ',L'Ỏ',L'Õ',L'Ô',L'Ồ',L'Ố',L'Ộ',L'Ổ',L'Ỗ',L'Ơ',L'Ờ',L'Ớ',L'Ợ',L'Ở',L'Ỡ',L'Ù',L'Ú',L'Ụ',L'Ủ',L'Ũ',L'Ư',L'Ừ',L'Ứ',L'Ự',L'Ử',L'Ữ',L'Ỳ',L'Ý',L'Ỵ',L'Ỷ',L'Ỹ',L'Đ'};
const wchar_t VNM_LOWER[] = {L'à',L'á',L'ạ',L'ả',L'ã',L'â',L'ầ',L'ấ',L'ậ',L'ẩ',L'ẫ',L'ă',L'ằ',L'ắ',L'ặ',L'ẳ',L'ẵ',L'è',L'é',L'ẹ',L'ẻ',L'ẽ',L'ê',L'ề',L'ế',L'ệ',L'ể',L'ễ',L'ì',L'í',L'ị',L'ỉ',L'ĩ',L'ò',L'ó',L'ọ',L'ỏ',L'õ',L'ô',L'ồ',L'ố',L'ộ',L'ổ',L'ỗ',L'ơ',L'ờ',L'ớ',L'ợ',L'ở',L'ỡ',L'ù',L'ú',L'ụ',L'ủ',L'ũ',L'ư',L'ừ',L'ứ',L'ự',L'ử',L'ữ',L'ỳ',L'ý',L'ỵ',L'ỷ',L'ỹ',L'đ'}; 
const wchar_t VNM_NACCENT[] = {L'a',L'a',L'a',L'a',L'a',L'a',L'a',L'a',L'a',L'a',L'a',L'a',L'a',L'a',L'a',L'a',L'a',L'e',L'e',L'e',L'e',L'e',L'e',L'e',L'e',L'e',L'e',L'e',L'i',L'i',L'i',L'i',L'i',L'o',L'o',L'o',L'o',L'o',L'o',L'o',L'o',L'o',L'o',L'o',L'o',L'o',L'o',L'o',L'o',L'o',L'u',L'u',L'u',L'u',L'u',L'u',L'u',L'u',L'u',L'u',L'u',L'u',L'u',L'u',L'u',L'u',L'd'};

int *VNM_UPPER_MAP, *VNM_LOWER_MAP; 
wchar_t **STOPWORD; 

void convert_utf8_to_utf16(char *utf8, wchar_t *&utf16) { 
	int len = 0; 
	for (int i = 0; i < strlen(utf8); ++i, ++len) { 
		if (((unsigned char)(utf8[i]) >> 5) == 6)
			++i; 
		else if (((unsigned char)(utf8[i]) >> 4) == 14)
			i += 2; 
	}
	utf16 = (wchar_t*)(malloc((len + 1) * sizeof(wchar_t))); 
	utf16[len - 1] = L'\0'; 
	len = 0; 
	for (int i = 0; i < strlen(utf8); ++i, ++len) { 
		utf16[len] = 0; 
		if (((unsigned char)(utf8[i]) >> 5) == 6) { 
			utf16[len] |= ((unsigned char)(utf8[i++]) ^ (3 << 6)) << 6;
			utf16[len] |= (unsigned char)(utf8[i]) ^ (1 << 7);
		} else if (((unsigned char)(utf8[i]) >> 4) == 14) { 
			utf16[len] |= ((unsigned char)(utf8[i++]) ^ (7 << 5)) << 12;
			utf16[len] |= ((unsigned char)(utf8[i++]) ^ (1 << 7)) << 6;
			utf16[len] |= (unsigned char)(utf8[i]) ^ (1 << 7);
		} else 
			utf16[len] = (unsigned char)(utf8[i]); 
	}
}

// void read_file_utf16(const char *filename, wchar_t *&text) { 
//     FILE *f = fopen(filename, "rb"); 

//     int size = file_get_size(f);  
//     text = (wchar_t*)(malloc(size + sizeof(wchar_t)));
//     fread(text, sizeof(wchar_t), size / 2, f); 
//     text[size / 2] = L'\0'; 

//     fclose(f); 
// }

void read_file_utf16(const char *filename, wchar_t *&text) { 
    FILE *f = fopen(filename, "rb"); 

	if (f == NULL) 
		return; 

    int size = file_get_size(f);  

	if (!is_utf16_file(f)) { 
		// read utf8 file
		char *buffer; 
		buffer = (char*)(malloc(size + 1));
		fread(buffer, sizeof(char), size, f); 
		buffer[size] = '\0'; 
		convert_utf8_to_utf16(buffer, text); 
		free(buffer); 
	} else { 
		// read utf16 file
		text = (wchar_t*)(malloc(size + sizeof(wchar_t)));
		fread(text, sizeof(wchar_t), size / 2, f); 
		text[size / 2] = L'\0'; 
	}

    fclose(f); 
}

bool is_char(wchar_t ch) { 
    return 
        VNM_UPPER_MAP[(unsigned int)(ch)] != -1 || 
        VNM_LOWER_MAP[(unsigned int)(ch)] != -1 || 
        (L'a' <= ch && ch <= L'z') ||
        (L'Z' <= ch && ch <= L'Z') ||
        (L'0' <= ch && ch <= L'9');
}

void build_vnm_map() {
    VNM_UPPER_MAP = (int*)(malloc(NUM_MAP * sizeof(int)));  
    VNM_LOWER_MAP = (int*)(malloc(NUM_MAP * sizeof(int))); 
    memset(VNM_UPPER_MAP, -1, NUM_MAP * sizeof(int)); 
    memset(VNM_LOWER_MAP, -1, NUM_MAP * sizeof(int)); 
    for (int i = 0; i < NUM_VNM; ++i) { 
        VNM_UPPER_MAP[(unsigned int)(VNM_UPPER[i])] = i; 
        VNM_LOWER_MAP[(unsigned int)(VNM_LOWER[i])] = i; 
    }
}

void get_stopword() { 
    STOPWORD = (wchar_t**)(malloc(NUM_SWORD * sizeof(wchar_t*))); 
    wchar_t *buffer; 
    read_file_utf16("stopword.txt", buffer); 

    int j = 0, n = 0; 
    for (int i = 0; i < wcslen(buffer); ++i) 
        if (buffer[i] == 0xfffe || buffer[i] == 0xfeff || buffer[i] == 0xd || buffer[i] == 0xa) { 
            if (j != i) { 
                STOPWORD[n] = (wchar_t*)(malloc((i - j + 1) * sizeof(wchar_t))); 
                wcsncpy(STOPWORD[n], buffer + j, i - j); 
                STOPWORD[n][i - j] = L'\0'; 
                n++; 
            }
            j = i + 1;
        }

    free(buffer); 
}

void standardized(wchar_t *text) { 
    int n = 0, len = wcslen(text); 
    for (int i = 0; i < len; ++i) { 
        if (L'A' <= text[i] && text[i] <= L'Z') 
            text[i] = (wchar_t)(text[i] + 32); 
        else if (VNM_UPPER_MAP[(unsigned int)(text[i])] != -1) 
            text[i] = VNM_LOWER[VNM_UPPER_MAP[(unsigned int)(text[i])]];  
        if (!is_char(text[i])) { 
            if (n > 0 && text[n - 1] != L' ') 
                text[n++] = L' '; 
            continue; 
        }
        text[n++] = text[i]; 
    }
    text[n] = L'\0';         
}

void remove_stopword(wchar_t *text) { 
    int n = 0, len = wcslen(text), slen; 
    for (int i = 0; i < len; ++i) { 
        int flag = 1; 
        if (i == 0 || text[i - 1] == L' ') { 
            // binary search 
            int l = 0, r = NUM_SWORD - 1; 
            while (l <= r) { 
                int mid = l + r >> 1;  
                if (wcsncmp(STOPWORD[mid], text + i, wcslen(STOPWORD[mid])) <= 0)
                    l = mid + 1; 
                else 
                    r = mid - 1; 
            }

            // brute force
            for (int j = l - 1; j >= 0 && j >= l - 10; --j) { 
                slen = wcslen(STOPWORD[j]);
                if (i + slen >= len || text[i + slen] != L' ')
                    continue; 
                if (wcsncmp(text + i, STOPWORD[j], slen) == 0) { 
                    flag = 0; 
                    i += slen; 
                    break; 
                }
            }
        } 
        if (flag) 
            text[n++] = text[i]; 
    }         
    text[n] = L'\0'; 
} 

void remove_vnm_accents(wchar_t *text, char *&utf8) { 
    utf8 = (char*)(malloc(wcslen(text) + 1)); 
    for (int i = 0; i < wcslen(text); ++i) { 
        if (VNM_LOWER_MAP[text[i]] != -1)   
            utf8[i] = (char)(VNM_NACCENT[VNM_LOWER_MAP[text[i]]]); 
        else 
            utf8[i] = (char)(text[i]); 
    }
    utf8[wcslen(text)] = '\0';
}

void safe_delete() { 
    for (int i = 0; i < NUM_SWORD; ++i) 
        free(STOPWORD[i]); 
    free(VNM_UPPER_MAP); 
    free(VNM_LOWER_MAP); 
    free(STOPWORD); 
}

void split_text_to_word(char *text, char **&word, int &cnt_word, int word_len) { 
    int len = strlen(text); 

    cnt_word = 0; 
    for (int i = 0; i < len; ++i) 
        cnt_word += (i == len - 1 || text[i] == ' '); 
    cnt_word -= word_len - 1; 

    word = (char**)(malloc(cnt_word * sizeof(char*))); 

    int l = 0, r = 0; 
    while (word_len > 1) { 
        word_len -= (text[r] == ' ' || r == len - 1); 
        r++; 
    }
    for (int i = 0; i < cnt_word; ++i, ++l, ++r) { 
        while (r < len && text[r] != ' ') 
            r++; 
        word[i] = (char*)(malloc(r - l + 1)); 
        memcpy(word[i], text + l, r - l); 
        word[i][r - l] = '\0'; 
        while (l < len && text[l] != ' ') 
            l++; 
    }
}

void find_keyword(char **&word, int &cnt_word, int total_word, int *frequency, int num, int den) {
    int n = 0; 
    for (int i = 0; i < cnt_word; ++i) 
        if (frequency[i] * den >= total_word * num) { 
            frequency[n] = frequency[i]; 
            word[n++] = word[i]; 
        } else 
            free(word[i]); 
    cnt_word = n; 
}

int count_word(const char *text) { 
    int ret = 0; 
    for (int i = 0; i < strlen(text); ++i) 
        ret += text[i] == ' '; 
    ret += text[strlen(text) - 1] != ' '; 
    return ret; 
}

void read_utf16(const wchar_t *mess, wchar_t *&text) { 
    wprintf(L"%ls", mess); 
    wchar_t *buffer = (wchar_t*)(malloc((int)(1e5) * sizeof(wchar_t)));  
    wscanf(L"%*lc"); 
    fgetws(buffer, (int)(1e5), stdin); 
    wscanf(L"%*lc"); 
    text = (wchar_t*)(malloc(wcslen(buffer) * sizeof(wchar_t))); 
    memcpy(text, buffer, (wcslen(buffer) - 1) * sizeof(wchar_t)); 
    text[wcslen(buffer) - 1] = L'\0'; 
    free(buffer); 
}

void read_data_metadata(FILE *metadata, int &n, char **&keyword, int *&frequency) { 
    fscanf(metadata, "%d\n", &n); 
    keyword = (char**)(malloc(n * sizeof(char*))); 
    frequency = (int*)(malloc(n * sizeof(int))); 
    char *buffer = (char*)(malloc((int)(1e5) * sizeof(char))); 
    for (int i = 0; i < n; ++i) { 
        fscanf(metadata, "%d ", frequency + i); 
        fgets(buffer, (int)(1e5), metadata); 
        keyword[i] = (char*)(malloc(strlen(buffer) * sizeof(char))); 
        memcpy(keyword[i], buffer, (strlen(buffer) - 1) * sizeof(char)); 
        keyword[i][strlen(buffer) - 1] = '\0'; 
    }
    free(buffer); 
}

int binary_search_keyword(char *word, int n, char **keyword, int *frequency) { 
    int l = 0, r = n - 1; 
    while (l <= r) { 
        int mid = l + r >> 1, value = strcmp(word, keyword[mid]); 
        if (value == 0) 
            return frequency[mid]; 
        if (value < 0) 
            r = mid - 1; 
        else 
            l = mid + 1; 
    }
    return 0; 
}