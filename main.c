#include "definitions.h"
#include "utils.h"
#include "brainsimulation.h"
#include "brainsetup.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const int OBSERVATION_X_INDICES_DEFAULT[] = {20, 21, 22, 100};
const int OBSERVATION_Y_INDICES_DEFAULT[] = {20, 20, 20, 100};
const int NUM_OBERSERVATIONNODES_DEFAULT = 4;

const int START_NODES_X_INDICES_DEFAULT[] = { 20, 40, 50, 100 };
const int START_NODES_Y_INDICES_DEFAULT[] = { 20, 40, 50, 100 };
const int NUM_START_NODES_DEFAULT = 4;
const nodeval_t START_NODE_LEVELS_DEFAULT[] = { 304, 12, 3, 100 };

int main(int argc, char *argv[]) {
    double tick_ms = 1;
    int num_ticks = 500;
    nodetimeseries_t *observationnodes = init_observation_timeseries(NUM_OBERSERVATIONNODES_DEFAULT,
                                                                     OBSERVATION_X_INDICES_DEFAULT,
                                                                     OBSERVATION_Y_INDICES_DEFAULT,
																	 num_ticks);
    int number_nodes_x = 120;
    int number_nodes_y = 120;
    nodeval_t **nodegrid = alloc_2d(number_nodes_x, number_nodes_y);
	init_start_time_state(number_nodes_x, number_nodes_y, nodegrid,
		NUM_START_NODES_DEFAULT, START_NODE_LEVELS_DEFAULT,
		START_NODES_X_INDICES_DEFAULT, START_NODES_Y_INDICES_DEFAULT);
    simulate(tick_ms, num_ticks, number_nodes_x, number_nodes_y, nodegrid,
             NUM_OBERSERVATIONNODES_DEFAULT, observationnodes);
    printf("Output:\n");
	for (int j = 0; j < NUM_OBERSERVATIONNODES_DEFAULT; ++j) {
        //printf("    Node %d: (%d|%d):\n", j, observationnodes[j].x_index, observationnodes[j].y_index);
        //int i;
        //for (i = 0; i < num_ticks; ++i) {
        //    printf("    Time: %d, Energy-Value: %f.\n", i, observationnodes[j].timeseries[i]);
        //}
        char str[12] = {""};
	    char filename[100] = "./testoutput/output";
        sprintf(str, "%d", observationnodes[j].x_index);
        strcat(filename,str);
        strcat(filename,"-");
        sprintf(str, "%d", observationnodes[j].y_index);
        strcat(filename,str);
        strcat(filename,".csv");
        printf("filename: %s\n", filename);
	    output_to_csv(filename, observationnodes[j].timeseries_ticks, observationnodes[j].timeseries);
    }
}
