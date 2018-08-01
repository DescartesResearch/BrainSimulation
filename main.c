#include "definitions.h"
#include "utils.h"
#include "brainsimulation.h"
#include "brainsetup.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/** Command line flag for y indices of frequency generating nodes (multiple integer paramters).*/
#define FLAG_HELP "--help"


// for file input
const int FILE_NUM_INPUTNODES_DEFAULT = 4;
const int FILE_INPUT_NODES_X_INDICES_DEFAULT[] = {10, 20, 30, 100};
const int FILE_INPUT_NODES_Y_INDICES_DEFAULT[] = {10, 20, 30, 100};
const int FILE_INPUT_NUMBER_OF_ELEMENTS_DEFAULT[] = {50, 50, 50, 63};
const char *FILE_INPUTNODES_PATHS[] = {"./testinput/input10-10.csv", "./testinput/input20-20.csv",
                                       "./testinput/input30-30.csv",
                                       "./testinput/input100-100.csv"};

static void print_help() {
	printf("Brainsimulation. Simulates the energy transfer over time between nodes in the human brain.\n");
	printf("\tRun with: brainsimulation %s X_NODES %s Y_NODES %s SIMULATION_TICKS [OPTIONAL PARAMETERS]\n",
		FLAG_X_NODES, FLAG_Y_NODES, FLAG_TICKS);
	printf("Mandatory parameters:\n");
	printf("\t%s X_NODES: Size of the simulated node grid on the X axis. Single integer parameter.\n",
		FLAG_X_NODES);
	printf("\t%s Y_NODES: Size of the simulated node grid on the Y axis. Single integer parameter.\n",
		FLAG_Y_NODES);
	printf("\t%s SIMULATION_TICKS: Number of time ticks to simulate. Single integer parameter.\n",
		FLAG_TICKS);
	printf("Recommended parameters (technically optional, but doesn't really make sense not to use them):\n");
	printf("\t%s OBS_X_INDICES: X indices of the observed nodes. One or multiple integer parameters.\n",
		FLAG_X_OBSERVATIONNODES);
	printf("\t%s OBS_Y_INDICES: Y indices of the observed nodes. One or multiple integer parameters.\n",
		FLAG_Y_OBSERVATIONNODES);
	printf("\t\t Must have the same number of parameters as %s.\n", FLAG_X_OBSERVATIONNODES);
	printf("Optional parameters:\n");
	printf("\t%s STARTING_ENERGY_LEVELS: Initial energy levels of nodes with starting energy. One or multiple floating point parameters.\n",
		FLAG_START_LEVELS);
	printf("\t%s STARTING_ENERGY_X_INDICES: X indices of nodes with starting energy. One or multiple integer parameters.\n",
		FLAG_START_NODES_X);
	printf("\t\t Must have the same number of parameters as %s.\n", FLAG_START_LEVELS);
	printf("\t%s STARTING_ENERGY_Y_INDICES: Y indices of nodes with starting energy. One or multiple integer parameters.\n",
		FLAG_START_NODES_Y);
	printf("\t\t Must have the same number of parameters as %s.\n", FLAG_START_LEVELS);
	printf("\t%s FREQUENCIES: Frequencies of nodes generating energy using sin-frequencies. One or multiple integer parameters.\n",
		FLAG_FREQUENCIES);
	printf("\t%s FREQUENCY_NODES_X_INDICES: X indices of nodes generating energy using frequencies. One or multiple integer parameters.\n",
		FLAG_FREQ_NODES_X);
	printf("\t\t Must have the same number of parameters as %s.\n", FLAG_FREQUENCIES);
	printf("\t%s STARTING_ENERGY_Y_INDICES: Y indices of nodes generating energy using frequencies. One or multiple integer parameters.\n",
		FLAG_FREQ_NODES_Y);
	printf("\t\t Must have the same number of parameters as %s.\n", FLAG_FREQUENCIES);
	printf("\n");
	printf("Example:\nbrainsimulation %s 200 %s 200 %s 5000 %s 50 51 %s 50 51 %s 10 11 %s 10 11 %s 10 11 %s 3 5 %s 25 26 %s 25 26\n",
		FLAG_X_NODES, FLAG_Y_NODES, FLAG_TICKS, FLAG_X_OBSERVATIONNODES, FLAG_Y_OBSERVATIONNODES, FLAG_START_LEVELS,
		FLAG_START_NODES_X, FLAG_START_NODES_Y, FLAG_FREQUENCIES, FLAG_FREQ_NODES_X, FLAG_FREQ_NODES_Y);
	printf("\n");
}

int main(const int argc, const char *argv[]) {
	if (contains_flag(argc, argv, FLAG_HELP)) {
		print_help();
		return 0;
	} else {
		printf("Brainsimulation: Run with --help for help.\n");
	}
    double tick_ms = 1;
    //int num_ticks = 5000;
	int num_observationnodes = 0;
	int num_inputnodes = 0;
	nodetimeseries_t *observationnodes = init_observation_timeseries_from_sh(argc, argv, &num_observationnodes);

    //nodetimeseries_t *observationnodes = init_observation_timeseries(NUM_OBERSERVATIONNODES_DEFAULT,
    //                                                                 OBSERVATION_X_INDICES_DEFAULT,
    //                                                                 OBSERVATION_Y_INDICES_DEFAULT,
    //                                                                 num_ticks);
    int number_nodes_x = parse_int_arg(argc, argv, FLAG_X_NODES);
	int number_nodes_y = parse_int_arg(argc, argv, FLAG_Y_NODES);
    nodeval_t **nodegrid = alloc_2d(number_nodes_x, number_nodes_y);
	init_start_time_state_from_sh(argc, argv, number_nodes_x, number_nodes_y, nodegrid);
	nodeinputseries_t *inputs = generate_input_frequencies_from_sh(argc, argv, &num_inputnodes, tick_ms);
//    nodeinputseries_t *inputs = read_input_behavior(FILE_NUM_INPUTNODES_DEFAULT, FILE_INPUT_NODES_X_INDICES_DEFAULT,
//                                                    FILE_INPUT_NODES_Y_INDICES_DEFAULT, FILE_INPUTNODES_PATHS,
//                                                    FILE_INPUT_NUMBER_OF_ELEMENTS_DEFAULT);
    simulate(tick_ms, observationnodes->timeseries_ticks, number_nodes_x, number_nodes_y, nodegrid,
		num_observationnodes, observationnodes, num_inputnodes, inputs);
    printf("Output:\n");
	for (int j = 0; j < num_observationnodes; ++j) {
        //printf("    Node %d: (%d|%d):\n", j, observationnodes[j].x_index, observationnodes[j].y_index);
        //int i;
        //for (i = 0; i < num_ticks; ++i) {
        //    printf("    Time: %d, Energy-Value: %f.\n", i, observationnodes[j].timeseries[i]);
        //}
        char str[12] = {""};
        char filename[100] = "./testoutput/output";
        sprintf(str, "%d", observationnodes[j].x_index);
        strcat(filename, str);
        strcat(filename, "-");
        sprintf(str, "%d", observationnodes[j].y_index);
        strcat(filename, str);
        strcat(filename, ".csv");
        printf("filename: %s\n", filename);
        output_to_csv(filename, observationnodes[j].timeseries_ticks, observationnodes[j].timeseries);
    }
    printf("Finished.\n");
    return 0;
}
