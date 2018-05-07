/**
 * Represents the behavior of single nodes.
 */
#include "nodefunc.h"

t_nodeval process(t_nodeval act_old, t_nodeval slope_old, int number_d_neighbors, t_nodeval *d_neighbors,
                  int number_id_neighbors, t_nodeval *id_neighbors){
    // calculate mean over all madn nodes
    t_nodeval madn = 0;
    for (int i = 0; i < number_d_neighbors; ++i) {
        madn = madn + d_neighbors[i];
    }
    madn = madn/number_d_neighbors;

    // calculate mean over all maidn nodes
    t_nodeval maidn = 0;
    for (int i = 0; i < number_id_neighbors; ++i) {
        maidn = maidn + id_neighbors[i];
    }
    maidn = maidn/number_id_neighbors;

    // rest of the algorithm
    t_nodeval slope3 = madn - slope_old;
    t_nodeval slope4 = maidn - slope_old;
    t_nodeval slope_vector = slope3 + slope4;
    t_nodeval slope_new = slope_old + slope_vector;
    t_nodeval act_new = act_old + slope_new;

    return act_new;
}