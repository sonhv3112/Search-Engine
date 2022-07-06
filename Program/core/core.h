#ifndef _CORE_H_
#define _CORE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <io.h>
#include <time.h>

#include "../fstring/fstring.h"
#include "../sort/sort.h"
#include "../file/file.h"

int read_choice();
void delay();
void print_utf8(const char *str);
void print_search_menu(int total_result, int cur_result);
void print_list_file_search(int total_result, int &cur_result, int *sorted_index, int *score, int *frequency, double time1);
void print_file(const char *filename);

int delete_file_from_metadata(const char *filename);
int add_file_to_metadata(const char *filename, int type, FILE *metadata);
double build_metadata();
double search();
double add_file();
double add_folder();
double delete_file();
double edit_file();

#endif