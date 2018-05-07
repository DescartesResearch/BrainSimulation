#include "definitions.h"
#include "helpers.h"
#include "brainsimulation.h"
#include "brainsetup.h"
#include <stdio.h>
#include <stdlib.h>

const int OBSERVATION_X_INDICES_DEFAULT[] = {20, 21, 22, 100};
const int OBSERVATION_Y_INDICES_DEFAULT[] = {20, 20, 20, 100};
const int NUM_OBERSERVATIONNODES_DEFAULT = 4;

const int START_NODES_X_INDICES_DEFAULT[] = { 20, 40, 50, 100 };
const int START_NODES_Y_INDICES_DEFAULT[] = { 20, 40, 50, 100 };
const int NUM_START_NODES_DEFAULT = 4;
const t_nodeval START_NODE_LEVELS_DEFAULT[] = { 304, 12, 3, 100 };

int main(int argc, char *argv[]) {
    int tick_ms = 1;
    int num_ticks = 1000;
    int num_observationnodes = 4;
    t_nodetimeseries *observationnodes = init_observation_timeseries(num_observationnodes,
                                                                     OBSERVATION_X_INDICES_DEFAULT,
                                                                     OBSERVATION_Y_INDICES_DEFAULT,
																	 num_ticks);
    int number_nodes_x = 1200;
    int number_nodes_y = 1200;
    t_nodeval **nodegrid = alloc_2d(number_nodes_x, number_nodes_y);
	init_start_time_state(number_nodes_x, number_nodes_y, nodegrid,
		NUM_START_NODES_DEFAULT, START_NODE_LEVELS_DEFAULT,
		START_NODES_X_INDICES_DEFAULT, START_NODES_Y_INDICES_DEFAULT);
    simulate(tick_ms, num_ticks, number_nodes_x, number_nodes_y, nodegrid,
             num_observationnodes, observationnodes);
    printf("Output:\n");
//	for (int j = 0; j < num_observationnodes; ++j) {
//		printf("    Node %d: (%d|%d):\n", j, observationnodes[j].x_index, observationnodes[j].y_index);
//        int i;
//        for (i = 0; i < num_ticks; ++i) {
//			printf("    Time: %d, Energy-Value: %f.\n", i, observationnodes[j].timeseries[i]);
//        }
//    }
}
