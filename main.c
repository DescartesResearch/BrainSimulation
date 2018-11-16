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
	printf("\t%s X_NODES: Size of the simulated node grid on the X axis.\n",
		FLAG_X_NODES);
	printf("\t\t Single integer parameter.\n");
	printf("\t%s Y_NODES: Size of the simulated node grid on the Y axis.\n",
		FLAG_Y_NODES);
	printf("\t\t Single integer parameter.\n");
	printf("\t%s SIMULATION_TICKS: Number of time ticks to simulate.\n",
		FLAG_TICKS);
	printf("\t\t Single integer parameter.\n");
	printf("Recommended parameters (technically optional, but doesn't really make sense not to use them):\n");
	printf("\t%s OBS_X_INDICES: X indices of the observed nodes.\n",
		FLAG_X_OBSERVATIONNODES);
	printf("\t\t One or multiple integer parameters.\n");
	printf("\t%s OBS_Y_INDICES: Y indices of the observed nodes.\n",
		FLAG_Y_OBSERVATIONNODES);
	printf("\t\t Must have the same number of parameters as %s.\n", FLAG_X_OBSERVATIONNODES);
	printf("\t\t One or multiple integer parameters.\n");
	printf("Optional parameters:\n");
	printf("\t%s: Observes all nodes instead of only a few specified nodes.\n", FLAG_ALL_OBSERVATIONNODES);
	printf("\t\t Causes %s and %s to be ignored (can thus be ommited).\n", FLAG_X_OBSERVATIONNODES, FLAG_Y_OBSERVATIONNODES);
	printf("\t\t Needs no additional parameters.\n");
	printf("\t\t WARNING: Leads to very slow execution times. Use only when absolutely needed.\n");
	printf("\t%s STARTING_ENERGY_LEVELS: Initial energy levels of nodes with starting energy.\n",
		FLAG_START_LEVELS);
	printf("\t\t One or multiple floating point parameters.\n");
	printf("\t%s STARTING_ENERGY_X_INDICES: X indices of nodes with starting energy.\n",
		FLAG_START_NODES_X);
	printf("\t\t Must have the same number of parameters as %s.\n", FLAG_START_LEVELS);
	printf("\t\t One or multiple floating point parameters.\n");
	printf("\t%s STARTING_ENERGY_Y_INDICES: Y indices of nodes with starting energy.\n",
		FLAG_START_NODES_Y);
	printf("\t\t Must have the same number of parameters as %s.\n", FLAG_START_LEVELS);
	printf("\t\t One or multiple integer parameters.\n");
	printf("\t%s FREQUENCIES: Frequencies of nodes generating energy using sin-frequencies.\n",
		FLAG_FREQUENCIES);
	printf("\t\t Cannot be used together with %s and its associated flags.\n", FLAG_FREQ_BITMAPS);
	printf("\t\t One or multiple integer parameters.\n");
	printf("\t%s FREQUENCY_NODES_X_INDICES: X indices of nodes generating energy using frequencies.\n",
		FLAG_FREQ_NODES_X);
	printf("\t\t Must have the same number of parameters as %s.\n", FLAG_FREQUENCIES);
	printf("\t\t One or multiple integer parameters.\n");
	printf("\t%s STARTING_ENERGY_Y_INDICES: Y indices of nodes generating energy using frequencies.\n",
		FLAG_FREQ_NODES_Y);
	printf("\t\t Must have the same number of parameters as %s.\n", FLAG_FREQUENCIES);
	printf("\t\t One or multiple integer parameters.\n");
	printf("\t%s FILENAMES: File paths of bitmap images to be used for specifying sin-frequencies.\n",
		FLAG_FREQ_BITMAPS);
	printf("\t\t Bitmaps must be uncompressed, 24-bit (MS-Paint default). Multiple bitmaps must have the same dimensions.\n");
	printf("\t\t Cannot be used together with %s and its associated flags.\n", FLAG_FREQUENCIES);
	printf("\t\t One or multiple parameters.\n");
	printf("\t%s MIN_FREQUENCY: The minimum frequency to generate, mapped to the minimum non-0 bitmap color (1).\n",
		FLAG_MIN_BITMAP_FREQ);
	printf("\t\t Single integer parameter.\n");
	printf("\t%s MAX_FREQUENCY: The maximum frequency to generate, mapped to the maximum bitmap color (765).\n",
		FLAG_MAX_BITMAP_FREQ);
	printf("\t\t Single integer parameter.\n");
	printf("\t%s DURATION_TICKS: The generation duration (in ticks) for a bitmap's signal.\n", FLAG_BITMAP_DURATION);
	printf("\t\t (analogous to a frame's duration in a movie)\n");
	printf("\t\t Single integer parameter.\n");
	printf("\n");
	printf("Example:\nbrainsimulation %s 200 %s 200 %s 5000 %s 50 51 %s 50 51 %s 10 11 %s 10 11 %s 10 11 %s 3 5 %s 25 26 %s 25 26\n",
		FLAG_X_NODES, FLAG_Y_NODES, FLAG_TICKS, FLAG_X_OBSERVATIONNODES, FLAG_Y_OBSERVATIONNODES, FLAG_START_LEVELS,
		FLAG_START_NODES_X, FLAG_START_NODES_Y, FLAG_FREQUENCIES, FLAG_FREQ_NODES_X, FLAG_FREQ_NODES_Y);
	printf("Example using bitmaps:\nbrainsimulation %s 200 %s 200 %s 3000 %s 50 51 %s 50 51"
		" %s testinput/input0.bmp testinput/input1.bmp testinput/input2.bmp %s 10 %s 40 %s 1000\n",
		FLAG_X_NODES, FLAG_Y_NODES, FLAG_TICKS, FLAG_X_OBSERVATIONNODES, FLAG_Y_OBSERVATIONNODES,
		FLAG_FREQ_BITMAPS, FLAG_MIN_BITMAP_FREQ, FLAG_MAX_BITMAP_FREQ, FLAG_BITMAP_DURATION);
	printf("\n");
}

int main(const int argc, const char *argv[]) {
	int num_observationnodes = 0;
	int num_inputnodes = 0;
	double tick_ms = 1;
	int number_nodes_x = 0;
	int number_nodes_y = 0;
	int num_ticks = 0;
	nodetimeseries_t *observationnodes;
	nodeval_t **nodegrid;
	nodeinputseries_t *inputs;

	//unsigned int size_x;
	//unsigned int size_y;
	//unsigned int *bitmap = read_bitmap_contents("/mnt/c/Users/w-man/Pictures/test.png", &size_x, &size_y);
	//for (int y = 0; y < size_y; y++) {
	//	for (int x = 0; x < size_x; x++) {
	//		printf("%d ", bitmap[y * size_x + x]);
	//	}
	//	printf("\n");
	//}

	if (contains_flag(argc, argv, FLAG_HELP)) {
		print_help();
		return 0;
	} else if (argc == 1){
		// no arguments were given
		printf("Brainsimulation: Run with --help for help.\n");
		printf("No input parameters given. Using default values...\n");
		num_ticks=5000;
		observationnodes = init_observation_timeseries_default(&num_observationnodes, num_ticks);

		nodegrid = init_nodegrid_default(&number_nodes_x, &number_nodes_y);

		inputs = generate_input_frequencies_default(&num_inputnodes, tick_ms);
	} else {
		printf("Brainsimulation: Run with --help for help.\n");
		printf("Parsing input parameters.\n");
		if (contains_flag(argc, argv, FLAG_TICKS)){
			printf("Number of ticks defined.");
			num_ticks = parse_int_arg(argc, argv, FLAG_TICKS);
			printf("Parsing %d ticks.\n", num_ticks);
		}
		else {
			printf("Number of ticks not defined. Using default values.\n");
			num_ticks = 5000;
		}
        if(contains_flag(argc, argv, FLAG_X_NODES) && contains_flag(argc, argv, FLAG_Y_NODES)) {
            printf("Parsing gridsize input.\n");
            number_nodes_x = parse_int_arg(argc, argv, FLAG_X_NODES);
            number_nodes_y = parse_int_arg(argc, argv, FLAG_Y_NODES);
            nodegrid = alloc_2d(number_nodes_x, number_nodes_y);
            if(contains_flag(argc, argv, FLAG_START_LEVELS) && contains_flag(argc, argv, FLAG_START_NODES_X) && contains_flag(argc, argv, FLAG_START_NODES_Y)){
                printf("Parsing start-state input.\n");
                init_start_time_state_from_sh(argc, argv, number_nodes_x, number_nodes_y, nodegrid);
            } else {
                printf("No information about start-state found. Using empty grid.\n");
                init_zeros_2d(nodegrid,number_nodes_x, number_nodes_y);
            }
        } else {
            printf("No input about Grid-size found. Using default values.\n");
            nodegrid = init_nodegrid_default(&number_nodes_x, &number_nodes_y);
		}
		if (contains_flag(argc, argv, FLAG_ALL_OBSERVATIONNODES)) {
			printf("Observing all nodes.\n");
			printf("WARNING: Observing all nodes is very slow, it is recommended to only observe specific nodes.\n");
			observationnodes = init_all_observation_timeseries(number_nodes_x, number_nodes_y, num_ticks); //init_observation_timeseries_from_sh(argc, argv, &num_observationnodes);
			num_observationnodes = number_nodes_x * number_nodes_y;
		} else if (contains_flag(argc, argv, FLAG_X_OBSERVATIONNODES) && contains_flag(argc, argv, FLAG_Y_OBSERVATIONNODES)){
			printf("Parsing observation input.\n");
			observationnodes = init_observation_timeseries_from_sh(argc, argv, &num_observationnodes);
			num_ticks = observationnodes->timeseries_ticks;
		} else {
			printf("No observation input found. Using default values.\n");
			observationnodes = init_observation_timeseries_default(&num_observationnodes, num_ticks);
		}
		if(contains_flag(argc, argv, FLAG_FREQUENCIES) && contains_flag(argc, argv, FLAG_FREQ_NODES_X) && contains_flag(argc, argv, FLAG_FREQ_NODES_Y)) {
            printf("Parsing input of frequency nodes from command line.\n");
			if (contains_flag(argc, argv, FLAG_FREQ_BITMAPS)) {
				printf("WARNING: \"%s\" and \"%s\" were set at the same time. This is not supported.\n", FLAG_FREQUENCIES, FLAG_FREQ_BITMAPS);
				printf("\tBitmap files (specified using \"%s\") will be ignored.\n", FLAG_FREQ_BITMAPS);
			}
            inputs = generate_input_frequencies_from_sh(argc, argv, &num_inputnodes, tick_ms);
            //    nodeinputseries_t *inputs = read_input_behavior(FILE_NUM_INPUTNODES_DEFAULT, FILE_INPUT_NODES_X_INDICES_DEFAULT,
            //                                                    FILE_INPUT_NODES_Y_INDICES_DEFAULT, FILE_INPUTNODES_PATHS,
            //                                                    FILE_INPUT_NUMBER_OF_ELEMENTS_DEFAULT);
		} else if (contains_flag(argc, argv, FLAG_FREQ_BITMAPS) && contains_flag(argc, argv, FLAG_BITMAP_DURATION)) {
			printf("Parsing frequency input from bitmap image files.\n");
			if (contains_flag(argc, argv, FLAG_FREQUENCIES)) {
				printf("WARNING: \"%s\" and \"%s\" were set at the same time. This is not supported.\n", FLAG_FREQ_BITMAPS, FLAG_FREQUENCIES);
				printf("\tCommand-line specified input nodes (using \"%s\") will be ignored.\n", FLAG_FREQUENCIES);
			}
			inputs = generate_input_frequencies_from_sh_bitmap(argc, argv, &num_inputnodes, tick_ms);
		} else {
            printf("No input about frequencies of nodes found. Using default values.\n");
            inputs = generate_input_frequencies_default(&num_inputnodes, tick_ms);
		}
	}
    simulate(tick_ms, num_ticks, number_nodes_x, number_nodes_y, nodegrid,
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
