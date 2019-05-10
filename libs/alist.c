/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <beluckydaf@gmail.com> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Vladislav Smirnov
 * ----------------------------------------------------------------------------
 */

#include "alist.h"

p_array_list create_array_list(size_t size) {
    void** array = (void**)malloc(sizeof(void*) * size);
    p_array_list alist = (p_array_list)malloc(sizeof(array_list));
    memset(array, 0, sizeof(void*) * size);
    alist->size = size;
    alist->count = 0;
    alist->array = array;
    return alist;
}

void delete_array_list(p_array_list alist) {
    free(alist->array);
    free(alist);
}

size_t expand_array_list(p_array_list alist) {
    size_t size = alist->size;
    size_t new_size = size * 2;
    void** array = (void**)malloc(sizeof(void*) * new_size);
    memset(array, 0, sizeof(void*) * new_size);
    memcpy(array, alist->array, sizeof(void*) * size);
    free(alist->array);
    alist->array = array;
    alist->size = new_size;
    return new_size;
}

int array_list_add(p_array_list alist, void* item) {
    for (int i = 0; i < alist->size; i++) {
        if (alist->array[i] == 0) {
            alist->array[i] = item;
            alist->count++;
            return i;
        }
    }
    int index = (int) ((expand_array_list(alist) / 2) + 1);
    alist->array[index] = item;
    alist->count++;
    return index;
}

int array_list_remove(p_array_list alist, void* item) {
    for (int i = 0; i < alist->size; i++) {
        if (alist->array[i] == item) {
            alist->array[i] = 0;
            alist->count--;
            return i;
        }
    }
    return -1;
}

int array_list_remove_at(p_array_list alist, int index) {
    if (alist->array[index] == 0) return -1;
    alist->array[index] = 0;
    alist->count--;
    return index;
}

int array_list_iter(p_array_list alist) {
    if (alist->count == 0) return -1;
    for (int i = 0; i < alist->size; i++) {
        if (alist->array[i] != 0) return i;
    }
    return -1;
}

int array_list_next(p_array_list alist, int iter) {
    for (int i = iter + 1; i < alist->size; i++) {
        if (alist->array[i] != 0) return i;
    }
    return -1;
}

void* array_list_get(p_array_list alist, int index) {
    if (index > alist->size) return (void*)(-1);
    return alist->array[index];
}


void array_list_free_all(p_array_list alist){
    for (int i = array_list_iter(alist);i!=-1;i = array_list_next(alist,i)){
        void* current = array_list_get(alist, i);
        free(current);
        array_list_remove_at(alist, i);
    }
}