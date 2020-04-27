//
// Created by rinat on 27.04.2020.
//

#include <api_funcs.h>
#include <map.h>
#include <stdlib.h>

map_void_t url_patterns;

/**
 * Init api container
 */
void api_funcs_init() {
    map_init(&url_patterns);
}

/**
 * Replace some url with new function processor
 */
void api_funcs_add(api_url *new_api) {
    void *cur_past = api_funcs_get(new_api->url);
    api_funcs_remove_and_free(cur_past);
    map_set(&url_patterns, new_api->url, new_api);
}

/**
 * Remove and free memory of some api processor
 */
void api_funcs_remove_and_free(api_url *api_to_remove) {
    if (api_to_remove != NULL) {
        map_remove(&url_patterns, api_to_remove->url);
        free(api_to_remove);
    }
}

/*
 * Get api processor from the url
 */
api_url *api_funcs_get(char *url) {
    void **res = map_get(&url_patterns, url);

    if (res != NULL) {
        api_url *api = *res;
        return api;
    }

    return NULL;
}

/**
 * Free all url functions from the container
 */
void api_funcs_deinit() {
    map_free_all(&url_patterns);
    map_deinit(&url_patterns);
}


