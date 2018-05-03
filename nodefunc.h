/**
 * Represents the behavior of single nodes.
 */
#include "brainsimulation.h"
/**
* This is the process done by the each agent.
* Takes the old state and information about the neighbors and calculated the new energy level.
*
* @param actOld: The old state information.
* @param slopeOld: The old slope.
* @param dNeighbors: Array of direct neighboring states. Length 4.
* @param idNeighbors: Array of indirect neighboring states. Length 4.
*
* @returns The new resulting energy level.
*/
t_nodeval process(t_nodeval actOld, t_nodeval slopeOld, t_nodeval * dNeighbors, t_nodeval * idNeighbors);