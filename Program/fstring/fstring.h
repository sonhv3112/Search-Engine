#ifndef _FSTRING_H_
#define _FSTRING_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "../file/file.h"

void read_file_utf16(const char *filename, wchar_t *&text);
bool is_char(wchar_t ch); 
void build_vnm_map(); 
void get_stopword();
void standardized(wchar_t *text);
void remove_stopword(wchar_t *text);
void remove_vnm_accents(wchar_t *text, char *&utf8);
void safe_delete(); 
void split_text_to_word(char *text, char **&word, int &cnt_word, int word_len);
void find_keyword(char **&word, int &cnt_word, int total_word, int *frequency, int num, int den);
int count_word(const char *text);
void read_utf16(const wchar_t *mess, wchar_t *&text);
void read_data_metadata(FILE *metadata, int &n, char **&keyword, int *&frequency);
int binary_search_keyword(char *word, int n, char **keyword, int *frequency);

#endif