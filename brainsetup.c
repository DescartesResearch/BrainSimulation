#include "brainsetup.h"

#include "utils.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define PI 3.14159265
// we are working at a millisecond scale, hence the 1000.
#define SCALE 1000.0

static unsigned int starts_with_minus(const char * str) {
	size_t lenstr = strlen(str);
	return lenstr < 1 ? 0 : strncmp("-", str, 1) == 0;
}

static unsigned int str_equals(const char * str0, const char * str1) {
	if (strlen(str0) != strlen(str1)) {
		return 0;
	}
	return strcmp(str0, str1) == 0;
}

static unsigned int parse_positional_args(const int argc, const char * argv[], const int flagPos, const char * readArgs[]) {
	unsigned int count = 0;
	for (int i = flagPos + 1; i < argc; i++) {
		if (starts_with_minus(argv[i])) {
			break;
		} else {
			readArgs[count] = argv[i];
		}
		count++;
	}
	return count;
}

static unsigned int parse_args(const int argc, const char * argv[], const char * flag, const char * readArgs[]) {
	for (int i = 0; i < argc; i++) {
		if (str_equals(flag, argv[i])) {
			return parse_positional_args(argc, argv, i, readArgs);
		}
	}
	return 0;
}

static unsigned int parse_int_args(const int argc, const char * argv[], const char * flag, int * readArgs) {
	const char ** readArgStrings = malloc(argc * sizeof(char *));
	unsigned int count = parse_args(argc, argv, flag, readArgStrings);
	for (int i = 0; i < count; i++) {
		readArgs[i] = atoi(readArgStrings[i]);
	}
	free(readArgStrings);
	return count;
}

static unsigned int parse_nodeval_args(const int argc, const char * argv[], const char * flag, nodeval_t * readArgs) {
	const char ** readArgStrings = malloc(argc * sizeof(char *));
	unsigned int count = parse_args(argc, argv, flag, readArgStrings);
	for (int i = 0; i < count; i++) {
		readArgs[i] = atof(readArgStrings[i]);
	}
	free(readArgStrings);
	return count;
}

static int min_val(int a, int b, int c) {
	int min = a;
	if (b < min) {
		min = b;
	}
	if (c < min) {
		min = c;
	}
	return min;
}

unsigned int contains_flag(const int argc, const char * argv[], const char * flag) {
	for (int i = 0; i < argc; i++) {
		if (str_equals(flag, argv[i])) {
			return 1;
		}
	}
	return 0;
}

int parse_int_arg(const int argc, const char * argv[], const char * flag) {
	const char ** readArgStrings = malloc(argc * sizeof(char *));
	unsigned int count = parse_args(argc, argv, flag, readArgStrings);
	if (count != 1) {
		printf("No argument with single integer parameter found for: ");
		printf("%s", flag);
		printf("\n");
		return 0;
	}
	int result = atoi(readArgStrings[0]);
	free(readArgStrings);
	return result;
}

nodetimeseries_t *init_observation_timeseries_from_sh(const int argc, const char *argv[],
	int * num_observationnodes) {
	
	int * x_indices = malloc(argc * sizeof(int));
	int * y_indices = malloc(argc * sizeof(int));
	int * ticks = malloc(argc * sizeof(int));
	*num_observationnodes = parse_int_args(argc, argv, FLAG_X_OBSERVATIONNODES, x_indices);
	int num_y_observationnodes = parse_int_args(argc, argv, FLAG_Y_OBSERVATIONNODES, y_indices);
	if (num_y_observationnodes < *num_observationnodes) {
		*num_observationnodes = num_y_observationnodes;
	}
	if (*num_observationnodes < 1) {
		printf("No nodes specified for observation. Use ");
		printf(FLAG_X_OBSERVATIONNODES);
		printf(" and ");
		printf(FLAG_Y_OBSERVATIONNODES);
		printf(" to specify the x and y coordinates to observe.\n");
		return NULL;
	}
	int num_tickargs = parse_int_args(argc, argv, FLAG_TICKS, ticks);
	if (num_tickargs != 1) {
		printf("Must specify the number of ticks to simulate (exactly one argument). Use ");
		printf(FLAG_TICKS);
		printf(" to specify the ticks.\n");
		return NULL;
	}
	int num_timeseries_elements = ticks[0];
	nodetimeseries_t *series = init_observation_timeseries(*num_observationnodes,
		x_indices, y_indices, num_timeseries_elements);
	free(x_indices);
	free(y_indices);
	free(ticks);
	return series;
}

nodetimeseries_t *init_observation_timeseries_default(int * num_observationnodes, int num_ticks) {
    *num_observationnodes = 4;
    int observation_x_indices_default[] = {20, 21, 22, 100};
    int observation_y_indices_default[] = {20, 20, 20, 100};
    return init_observation_timeseries(*num_observationnodes, observation_x_indices_default,
                                       observation_y_indices_default, num_ticks);
}

nodetimeseries_t *init_observation_timeseries(const int num_observationnodes,
                                              const int *x_indices, const int *y_indices,
                                              const int num_timeseries_elements) {
	nodetimeseries_t *series = malloc(num_observationnodes * sizeof(nodetimeseries_t));
	for (int i = 0; i < num_observationnodes; i++) {
        series[i].x_index = x_indices[i];
        series[i].y_index = y_indices[i];
        series[i].timeseries = malloc(num_timeseries_elements * sizeof(nodeval_t));
        series[i].timeseries_ticks = num_timeseries_elements;
    }
    return series;
}

void init_start_time_state_from_sh(const int argc, const char * argv[],
	const int number_nodes_x, const int number_nodes_y, nodeval_t **nodes) {
	nodeval_t * start_levels = malloc(argc * sizeof(nodeval_t));
	int *start_nodes_x = malloc(argc * sizeof(int));
	int *start_nodes_y = malloc(argc * sizeof(int));
	unsigned int startnodecount =
		parse_nodeval_args(argc, argv, FLAG_START_LEVELS, start_levels);
	unsigned int startnodecount_x =
		parse_int_args(argc, argv, FLAG_START_NODES_X, start_nodes_x);
	unsigned int startnodecount_y =
		parse_int_args(argc, argv, FLAG_START_NODES_Y, start_nodes_y);
	startnodecount = min_val(startnodecount, startnodecount_x, startnodecount_y);
	init_start_time_state(number_nodes_x, number_nodes_y, nodes,
		startnodecount, start_levels,
		start_nodes_x, start_nodes_y);
	free(start_levels);
	free(start_nodes_x);
	free(start_nodes_y);
}

nodeval_t **init_nodegrid_default(int *number_nodes_x, int *number_nodes_y){
	*number_nodes_x = 200;
	*number_nodes_y = 200;
	nodeval_t **nodegrid = alloc_2d(*number_nodes_x, *number_nodes_y);


	int start_nodes_x_indices_default[] = {20, 40, 50, 100};
	int start_nodes_y_indices_default[] = {20, 40, 50, 100};
	int num_start_nodes_default = 4;
	nodeval_t start_nodes_levels_default[] = {304, 12, 3, 100};

	init_start_time_state(*number_nodes_x, *number_nodes_y, nodegrid,
						  num_start_nodes_default, start_nodes_levels_default,
						  start_nodes_x_indices_default, start_nodes_y_indices_default);

	return nodegrid;
}

void init_start_time_state(const int number_nodes_x, const int number_nodes_y, nodeval_t **nodes,
                           const int num_start_levels, const nodeval_t *start_levels, const int *start_nodes_x,
                           const int *start_nodes_y) {
    init_zeros_2d(nodes, number_nodes_x, number_nodes_y);

    //initialize with start levels
    for (int i = 0; i < num_start_levels; i++) {
        nodes[start_nodes_x[i]][start_nodes_y[i]] = start_levels[i];
    }
}

nodeinputseries_t *read_input_behavior(const int number_of_inputnodes, const int *x_indices, const int *y_indices,
                                       const char **inputnodefilenames, const int *number_of_elements) {
    nodeinputseries_t *series = malloc(number_of_inputnodes * sizeof(nodeinputseries_t));
    int i;
    for (i = 0; i < number_of_inputnodes; ++i) {
        series[i].x_index = x_indices[i];
        series[i].y_index = y_indices[i];
        series[i].timeseries = malloc(number_of_elements[i] * sizeof(nodeval_t));
        series[i].timeseries_ticks = number_of_elements[i];
        parse_file(series[i], inputnodefilenames[i]);
    }
    return series;
}

nodeinputseries_t *generate_input_frequencies_from_sh(const int argc, const char * argv[], int *num_inputnodes, double tick_ms) {
	int *frequencies = malloc(argc * sizeof(int));
	int *x_indices = malloc(argc * sizeof(int));
	int *y_indices = malloc(argc * sizeof(int));
	*num_inputnodes = parse_int_args(argc, argv, FLAG_FREQUENCIES, frequencies);
	int num_inputnodes_x = parse_int_args(argc, argv, FLAG_FREQ_NODES_X, x_indices);
	int num_inputnodes_y = parse_int_args(argc, argv, FLAG_FREQ_NODES_Y, y_indices);
	*num_inputnodes = min_val(*num_inputnodes, num_inputnodes_x, num_inputnodes_y);
	nodeinputseries_t *series = generate_input_frequencies(*num_inputnodes, x_indices, y_indices, frequencies, tick_ms);
	free(frequencies);
	free(x_indices);
	free(y_indices);
	return series;
}

nodeinputseries_t *generate_input_frequencies_default(int *num_inputnodes, double tick_ms){
	*num_inputnodes = 40;
	int input_nodes_x_indices_default[] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
										   29, 30, 31, 32, 33, 34,
										   35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49};
	int input_nodes_y_indices_default[] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
										   29, 30, 31, 32, 33, 34,
										   35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49};
	int input_frquencies_default[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73,
									  79, 83, 89, 97, 101,
									  103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173};
	return generate_input_frequencies(*num_inputnodes,input_nodes_x_indices_default, input_nodes_y_indices_default,
									  input_frquencies_default, tick_ms);
}

nodeinputseries_t *generate_input_frequencies(const int number_of_inputnodes, const int *x_indices,
                                              const int *y_indices, const int *frequencies, double tick_ms) {
    nodeinputseries_t *series = malloc(number_of_inputnodes * sizeof(nodeinputseries_t));
    int i;
    for (i = 0; i < number_of_inputnodes; ++i) {
        series[i].x_index = x_indices[i];
        series[i].y_index = y_indices[i];
        series[i].timeseries = generate_sin_frequency(frequencies[i], tick_ms);
        series[i].timeseries_ticks = calculate_period_length(frequencies[i], tick_ms);
    }
    return series;
}


double *generate_sin_time_series(int hz, double tick_ms, int number_of_samples) {
    double *series = malloc(number_of_samples * sizeof(double));
    int i;
    for (i = 0; i < number_of_samples; ++i) {
        double arg = PI * hz * 2 * tick_ms * ((double) i) / (SCALE);
        series[i] = sin(arg);
    }
    return series;
}

double *generate_sin_frequency(int hz, double tick_ms) {
    int samples = calculate_period_length(hz, tick_ms);
    printf("Generating %d Hz frequency at at a resolution of %f ms per tick. ", hz, tick_ms);
    printf("Detected period of %d samples.\n", samples);
    if (samples <= 2) {
        printf("----------------------------------");
        printf("WARNING: Frequency of %d Hz can not be realized at a resolution of %f ms per tick. Increase tick "
               "granularity or reduce frequency.\n", hz, tick_ms);
        printf("----------------------------------");
    }
    return generate_sin_time_series(hz, tick_ms, samples);
}

int calculate_period_length(int hz, double tick_ms) {
    double period = SCALE / (hz * tick_ms);
    return period;
}
