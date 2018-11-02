/**
 * Represents the behavior of single nodes.
 */
#include "nodefunc.h"

nodestate_t process(nodeval_t act_old, nodeval_t slope_old, int number_d_neighbors, nodeval_t *d_neighbors,
                    int number_id_neighbors, nodeval_t *id_neighbors) {
    // calculate mean over all madn nodes
    nodeval_t madn = 0;
    for (int i = 0; i < number_d_neighbors; ++i) {
        madn = madn + d_neighbors[i];
    }
    madn = madn / number_d_neighbors;
    // add direct neighbor factor
    madn = madn * D_NEIGHBORFACTOR;

    // calculate mean over all maidn nodes
    nodeval_t maidn = 0;
    for (int i = 0; i < number_id_neighbors; ++i) {
        maidn = maidn + id_neighbors[i];
    }
    maidn = maidn / number_id_neighbors;
    // add indirect neighbor factor
    maidn = maidn * ID_NEIGHBORFACTOR;

    // add factor
    nodeval_t act_old_factored = act_old * ENERGY_FACTOR;

    // calculate slope
    nodeval_t slope_d = madn - act_old_factored;
    nodeval_t slope_id = maidn - act_old_factored;
    nodeval_t slope_vector = slope_d + slope_id;

    // add factors
    slope_vector = slope_vector * DELTA_FACTOR;
    slope_old = slope_old * SLOPE_FACTOR;

    // calculate new slope
    nodeval_t slope_new = slope_old + slope_vector;

    // add factors
    nodeval_t slope_new_weighted = slope_new * SLOPE_WEIGHT;
    nodeval_t act_old_weighted = act_old * ENERGY_WEIGHT;

    // calculate new energy levels
    nodeval_t act_new = act_old_weighted + slope_new_weighted;

    // store results
    nodestate_t res;
    res.act = act_new;
    res.slope = slope_new;

    return res;
}