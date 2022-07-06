#ifndef _SORT_H_ 
#define _SORT_H_ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../fstring/fstring.h"

int cmp_keyword(const char *str1, const char *str2);
void merge_sorted_string_arr(char **arr1, int n1, char **arr2, int n2, char **&arr, int (*cmp)(const char *, const char *) = strcmp); 
void sort_string_arr(char **&arr, int n, int (*cmp)(const char *, const char *) = strcmp);
void unique_string_arr(char **&arr, int &n, int *&frequency);
void merge_sorted_index_arr(int *arr1, int n1, int *arr2, int n2, int *arr, int *f, int *score);
void sort_index_arr(int *arr, int n, int *f, int *score);

#endif