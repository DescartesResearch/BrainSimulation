#ifndef HELPERS_H
#define HELPERS_H

#include "definitions.h"

/**
 * Common helper functions.
 */


/**
* Allocates a new 2d array with m pointers, pointing to a list of n elements.
* @param m The number of nodes in the first dimension (x-axis).
* @param n The number of nodes in the second dimension (y-axis).
*
* @return A two-dimensional array of size m*n.
*/
t_nodeval **alloc_2d(const int m, const int n);


/**
* Allocates a new 4d array with m pointers, pointing to a list of n elements, pointing to a list of o elements,
* pointing to a list of p elements.
* @param m The number of nodes in the first dimension.
* @param n The number of nodes in the second dimension.
* @param o The number of nodes in the third dimension.
* @param p The number of nodes in the fourth dimension.
*
* @return A four-dimensional array of size m*n*o*p.
*/
t_nodeval ****alloc_4d(const int m, const int n, const int o, const int p);

/**
 * Returns the number of processor cores online in the system.
 * @return The number of processors online in the system.
 */
int system_processor_online_count();

#endif
