#ifndef _FILE_H_
#define _FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

int file_not_exist(FILE *f); 
int file_get_size(FILE *f); 
int exist_file(const char *filename);
void read_filename_index(int index, char *&filename);
void truncate(const char *filename, int start, int bytes);
void open_notepad(const char *path);
void list_file_to_index(const char *path, const char *index);
int check_exist_folder(const char *path);
bool is_utf16_file(FILE *f);
int count_file(FILE *f);

#endif