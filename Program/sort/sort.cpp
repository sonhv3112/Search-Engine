#include "sort.h" 

int cmp_keyword(const char *str1, const char *str2) { 
    return !(count_word(str1) <= count_word(str2)); 
}

void merge_sorted_string_arr(char **arr1, int n1, char **arr2, int n2, char **&arr, int (*cmp)(const char *, const char *)) { 
    int i1 = 0, i2 = 0; 
    for (int i = 0; i < n1 + n2; ++i) 
        if (i2 == n2)  
            arr[i] = arr1[i1++]; 
        else if (i1 == n1 || cmp(arr1[i1], arr2[i2]) > 0)  
            arr[i] = arr2[i2++]; 
        else 
            arr[i] = arr1[i1++]; 
}

void sort_string_arr(char **&arr, int n, int (*cmp)(const char *, const char *)) { 
    char **temp = (char**)(malloc(n * sizeof(char*)));  
    for (int i = 0; (1 << i) < n; ++i) { 
        for (int l = 0; l < n; l += (2 << i)) { 
            if (l + (1 << i) >= n) 
                break; 
            int r = l + (2 << i) - 1; 
            if (r >= n) r = n - 1; 
            merge_sorted_string_arr(arr + l, 1 << i, arr + l + (1 << i), r - l - (1 << i) + 1, temp, cmp); 
            for (int ii = 0; ii < r - l + 1; ++ii) 
                arr[l + ii] = temp[ii]; 
        }
    }
    free(temp); 
}   

void unique_string_arr(char **&arr, int &n, int *&frequency) { 
    frequency = (int*)(malloc(n * sizeof(int))); 
    memset(frequency, 0, sizeof(frequency));
    int m = 0; 
    for (int i = 0; i < n; ++i) { 
        frequency[m] = 1; 
        arr[m] = arr[i];  
        for (int j = i + 1; j < n; ++j, ++i) 
            if (!strcmp(arr[j], arr[j - 1])) { 
                free(arr[j]); 
                frequency[m]++; 
            } else 
                break; 
        m++; 
    }
    n = m; 
}

void merge_sorted_index_arr(int *arr1, int n1, int *arr2, int n2, int *arr, int *f, int *score) { 
    int i1 = 0, i2 = 0; 
    for (int i = 0; i < n1 + n2; ++i) 
        if (i2 == n2)  
            arr[i] = arr1[i1++]; 
        else if (i1 == n1 || (score[arr2[i2]] > score[arr1[i1]] || (score[arr2[i2]] == score[arr1[i1]] && f[arr2[i2]] > f[arr1[i1]])))  
            arr[i] = arr2[i2++]; 
        else 
            arr[i] = arr1[i1++]; 
}

void sort_index_arr(int *arr, int n, int *f, int *score) { 
    int *temp = (int*)(malloc(n * sizeof(int)));  
    for (int i = 0; (1 << i) < n; ++i) { 
        for (int l = 0; l < n; l += (2 << i)) { 
            if (l + (1 << i) >= n) 
                break; 
            int r = l + (2 << i) - 1; 
            if (r >= n) r = n - 1; 
            merge_sorted_index_arr(arr + l, 1 << i, arr + l + (1 << i), r - l - (1 << i) + 1, temp, f, score); 
            for (int ii = 0; ii < r - l + 1; ++ii) 
                arr[l + ii] = temp[ii]; 
        }
    }
    free(temp); 
}  