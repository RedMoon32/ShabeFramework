//
// Created by rinat on 27.04.2020.
//

#ifndef SHABEFRAMEWORK_URL_PATTERNS_H
#define SHABEFRAMEWORK_URL_PATTERNS_H

#include <dispatcher.h>
#include <http_structures.h>

void api_funcs_init();

void api_funcs_add(api_url *new_api);

void api_funcs_remove_and_free(api_url *api_to_remove);

api_url *api_funcs_get(char *url);

void api_funcs_deinit();

#endif //SHABEFRAMEWORK_URL_PATTERNS_H
