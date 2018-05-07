#ifndef NODEFUNC_H
#define NODEFUNC_H

#include "definitions.h"

/**
 * Represents the behavior of single nodes.
 */

/**
* This is the process done by the each agent.
* Takes the old state and information about the neighbors and calculated the new energy level.
*
* @param actOld: The old state information.
* @param slopeOld: The old slope.
* @param number_d_neighbors The number of direct neighbors.
* @param d_neighbors: Array of direct neighboring states. Length: number_d_neighbors.
* @param number_id_neighbors The number of indirect neighbors.
* @param id_neighbors: Array of indirect neighboring states. Length: number_id_neighbors.
*
* @returns The new resulting energy level.
*/
nodeval_t process(nodeval_t act_old, nodeval_t slope_old, int number_d_neighbors, nodeval_t *d_neighbors,
                  int number_id_neighbors, nodeval_t *id_neighbors);

#endif
