#include "file.h"

int file_not_exist(FILE *f) { 
    return f == NULL; 
}

int file_get_size(FILE *f) { 
    fseek(f, 0, SEEK_END); 
    int ret = ftell(f); 
    fseek(f, 0, SEEK_SET); 
    return ret; 
}

int exist_file(const char *filename) { 
    FILE *f = fopen("index.inp", "r"); 
    char e_filename[1000]; 
    memset(e_filename, 0x00, sizeof(e_filename)); 
    int ret = 0; 
    while (1) { 
        fgets(e_filename, 1000, f); 
        if (feof(f)) 
            break; 
        e_filename[strlen(e_filename) - 1] = '\0'; 
        if (!strcmp(e_filename, filename)) { 
            fclose(f); 
            return ret; 
        }
        memset(e_filename, 0, 1000); 
        ret++;
    }
    fclose(f); 
    return -1;
}

void read_filename_index(int index, char *&filename) { 
    FILE *f = fopen("position.txt", "rb"); 
    fseek(f, index * 8, SEEK_SET);
    fread(&index, sizeof(int), 1, f); 
    fclose(f); 
    f = fopen("index.inp", "r"); 
    fseek(f, index, SEEK_SET); 
    char *buffer = (char*)(malloc((int)(1e4) * sizeof(char))); 
    fgets(buffer, (int)(1e4), f); 
    filename = (char*)(malloc(strlen(buffer) * sizeof(char))); 
    memcpy(filename, buffer, strlen(buffer) - 1); 
    filename[strlen(buffer) - 1] = '\0'; 
    free(buffer); 
    fclose(f); 
}

void truncate(const char *filename, int start, int bytes) { 
    start++; 
    FILE *f = fopen(filename, "rb"); 
    FILE *ft = fopen("temp_file.txt", "wb"); 
    int len_buffer = 1000, tlen; 
    char *buffer = (char*)(malloc((int)(len_buffer) * sizeof(char))); 

    while (start) { 
        tlen = fread(buffer, sizeof(char), start < len_buffer ? start - 1 : len_buffer, f);
        if (!tlen)
            break; 
        fwrite(buffer, sizeof(char), tlen, ft); 
        start -= tlen; 
    }
    if (bytes != -1) { 
        fseek(f, bytes, SEEK_CUR); 
        do { 
            tlen = fread(buffer, sizeof(char), len_buffer, f); 
            fwrite(buffer, sizeof(char), tlen, ft); 
        } while (tlen == len_buffer); 
    }

    free(buffer); 
    fclose(ft); 
    fclose(f); 

    remove(filename); 
    rename("temp_file.txt", filename); 
}

void open_notepad(const char *path) { 
	char *command = (char*)(malloc(strlen(path) + 9)); 
	memcpy(command, "notepad ", 8); 
	memcpy(command + 8, path, strlen(path)); 
    command[strlen(path) + 8] = '\0'; 
	system(command); 
	free(command); 
}

void list_file_to_index(const char *path, const char *index) { 
	char *command = (char*)(malloc(strlen(path) + strlen(index) + 22)); 
	memcpy(command, "dir /s /b \"", 11); 
	memcpy(command + 11, path, strlen(path)); 
	memcpy(command + 11 + strlen(path), "\\*.txt\" > ", 10); 
    memcpy(command + 21 + strlen(path), index, strlen(index)); 
	command[strlen(path) + strlen(index) + 21] = '\0'; 
	system(command); 
	free(command); 
}

int check_exist_folder(const char *path) { 
	char *command = (char*)(malloc(strlen(path) + 32)); 
	memcpy(command, "if exist \"", 10); 
	memcpy(command + 10, path, strlen(path)); 
	memcpy(command + 10 + strlen(path), "\\\" exit 1 else exit 0", 21); 
	command[strlen(path) + 31] = '\0'; 
	int ret = system(command); 
	free(command); 
    return ret; 
}

bool is_utf16_file(FILE *f) { 
	wchar_t temp; 
	fread(&temp, sizeof(wchar_t), 1, f); 
	fseek(f, 0, SEEK_SET); 
	return temp == 0xfffe || temp == 0xfeff; 
}

int count_file(FILE *f) { 
    int ret = 0; 
    char filename[1000]; 
    while (1) { 
        fgets(filename, 1000, f); 
        if (feof(f)) 
            break;
        ret++;
    }
    fseek(f, 0, SEEK_SET); 
    return ret; 
}