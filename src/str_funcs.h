//
// Created by rinat on 01.05.2020.
//

#ifndef SHABEFRAMEWORK_STR_FUNCS_H
#define SHABEFRAMEWORK_STR_FUNCS_H

#include <http_structures.h>

static void* concat_str(const char *str1, const char *str2, char *res) {
    memset(res, 0, PATH_MAX);
    strcpy(res, str1);
    strcpy(res + strlen(str1), str2);
    return res;
}

#endif //SHABEFRAMEWORK_STR_FUNCS_H
