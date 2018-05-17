//
// Created by Johannes on 11.05.2018.
// Supports different types of kernel-function, which can be chosen live.
// All kernel functions have to conform to the same interface, in order to be executable.
// All kernels should, but do not need to come in pairs (d_kernel and id_kernel).
//

#ifndef BRAINSIMULATION_KERNELS_H
#define BRAINSIMULATION_KERNELS_H

#include "definitions.h"

/**
 * Returns a function pointer to function of the direct neighborhood kernel.
 *
 * @param name The name of the required function. (Currently not implemented.)
 * @return A function pointer to the specific function.
 */
kernelfunc_t d_kernel_function_factory(char *name);

/**
 * Returns a function pointer to function of the indirect neighborhood kernel.
 *
 * @param name The name of the required function. (Currently not implemented.)
 * @return A function pointer to the specific function.
 */
kernelfunc_t id_kernel_function_factory(char *name);

/**
 * Calculates the direct kernel for one specific node of the node grid, i.e., the direct neighborhood.
 *
 * @param result The array to store the kernel into. Length 4 required.
 * @param number_nodes_x The number of nodes in the first dimension of nodes.
 * @param number_nodes_y The number of nodes in the second dimension of nodes.
 * @param nodegrid 2D array of nodes with their current energy level. Size number_nodes_x * number_nodes_y.
 * @param x The x-Coordinate of the specific node for which the kernel is to be executed for.
 * @param y The y-Coordinate of the specific node for which the kernel is to be executed for.
 *
 * @return The number of neighbors generated, i.e., the length of the given result array.
 */
int d_kernel_4neighbors(nodeval_t *result, int number_nodes_x, int number_nodes_y, nodeval_t **nodegrid, int x, int y);

/**
 * Calculates the indirect kernel for one specific node of the node grid, i.e., the indirect neighborhood.
 *
 * @param result The array to store the kernel into. Length 4 required.
 * @param number_nodes_x The number of nodes in the first dimension of nodes.
 * @param number_nodes_y The number of nodes in the second dimension of nodes.
 * @param nodegrid 2D array of nodes with their current energy level. Size number_nodes_x * number_nodes_y.
 * @param x The x-Coordinate of the specific node for which the kernel is to be executed for.
 * @param y The y-Coordinate of the specific node for which the kernel is to be executed for.
 *
 * @return The number of neighbors generated, i.e., the length of the given result array.
 */
int id_kernel_4neighbors(nodeval_t *result, int number_nodes_x, int number_nodes_y, nodeval_t **nodegrid, int i, int j);

#endif //BRAINSIMULATION_KERNELS_H
