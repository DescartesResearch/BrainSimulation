//
// Created by Johannes on 11.05.2018.
//
#include "kernels.h"

#include <stdio.h>
#include <stdlib.h>


kernelfunc_t d_kernel_function_factory(char *name) {
    // we can later on link the given name to a kernel function
    // for now, we just return the static function
    kernelfunc_t functionPtr = &d_kernel_4neighbors;
    return functionPtr;
}

kernelfunc_t id_kernel_function_factory(char *name) {
    // we can later on link the given name to a kernel function
    // for now, we just return the static function
    kernelfunc_t functionPtr = &id_kernel_4neighbors;
    return functionPtr;
}

int d_kernel_4neighbors(nodeval_t *result, int number_nodes_x, int number_nodes_y, nodeval_t **nodegrid, int x, int y) {
    // we require 4 elements in the given array
    if (x - 1 >= 0) {
        result[0] = nodegrid[x - 1][y];
    } else {
        // border behavior
        result[0] = 0;
    }
    if (y - 1 >= 0) {
        result[1] = nodegrid[x][y - 1];
    } else {
        // border behavior
        result[1] = 0;
    }
    if (y + 1 < number_nodes_y) {
        result[2] = nodegrid[x][y + 1];
    } else {
        // border behavior
        result[2] = 0;
    }
    if (x + 1 < number_nodes_x) {
        result[3] = nodegrid[x + 1][y];
    } else {
        // border behavior
        result[3] = 0;
    }
    return 4;
}

int
id_kernel_4neighbors(nodeval_t *result, int number_nodes_x, int number_nodes_y, nodeval_t **nodegrid, int x, int y) {
    // we require 4 elements in the given array
    if (x - 1 >= 0 && y - 1 >= 0) {
        result[0] = nodegrid[x - 1][y - 1];
    } else {
        // border behavior
        result[0] = 0;
    }
    if (x - 1 >= 0 && y + 1 < number_nodes_y) {
        result[1] = nodegrid[x - 1][y + 1];
    } else {
        // border behavior
        result[1] = 0;
    }
    if (x + 1 < number_nodes_x && y - 1 >= 0) {
        result[2] = nodegrid[x + 1][y - 1];
    } else {
        // border behavior
        result[2] = 0;
    }
    if (x + 1 < number_nodes_x && y + 1 < number_nodes_y) {
        result[3] = nodegrid[x + 1][y + 1];
    } else {
        // border behavior
        result[3] = 0;
    }
    return 4;
}