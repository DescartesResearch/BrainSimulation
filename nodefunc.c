/**
 * Represents the behavior of single nodes.
 */
#include "nodefunc.h"

tNodeval process(t_nodeval actOld, t_nodeval slopeOld, t_nodeval * dNeighbors, t_nodeval * idNeighbors) {
    t_nodeval madn = (dNeighbors[0] + dNeighbors[1] + dNeighbors[2]+ dNeighbors[3])/4;
    t_nodeval maidn = (idNeighbors[0] + idNeighbors[1] + idNeighbors[2] + idNeighbors[3])/4;
    t_nodeval slope3 = madn - oldState;
    t_nodeval slope4 = maidn - oldState;
    t_nodeval slopeVector = slope3 + slope4;
    t_nodeval slopeNew = slopeOld + slopeVector;
    t_nodeval actNew = actOld + slopeNew;

    return actNew;
}