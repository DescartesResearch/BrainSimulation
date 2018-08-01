#include "brainsetup.h"

#include "utils.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define FLAG_TICKS "--ticks"
#define FLAG_X_OBSERVATIONNODES "--xobs"
#define FLAG_Y_OBSERVATIONNODES "--yobs"

#define PI 3.14159265
// we are working at a millisecond scale, hence the 1000.
#define SCALE 1000.0

static unsigned int startsWithMinus(const char * str) {
	size_t lenstr = strlen(str);
	return lenstr < 1 ? 0 : strncmp("-", str, 1) == 0;
}

static unsigned int strEquals(const char * str0, const char * str1) {
	if (strlen(str0) != strlen(str1)) {
		return 0;
	}
	return strcmp(str0, str1) == 0;
}

static unsigned int parsePositionalArgs(const int argc, const char * argv[], const int flagPos, const char * readArgs[]) {
	unsigned int count = 0;
	for (int i = flagPos + 1; i < argc; i++) {
		if (startsWithMinus(argv[i])) {
			break;
		} else {
			readArgs[count] = argv[i];
		}
		count++;
	}
	return count;
}

static unsigned int parseArgs(const int argc, const char * argv[], const char * flag, const char * readArgs[]) {
	for (int i = 0; i < argc; i++) {
		if (strEquals(flag, argv[i])) {
			return parsePositionalArgs(argc, argv, i, readArgs);
		}
	}
	return 0;
}

static unsigned int parseIntArgs(const int argc, const char * argv[], const char * flag, int * readArgs) {
	const char ** readArgStrings = malloc(argc * sizeof(char *));
	unsigned int count = parseArgs(argc, argv, flag, readArgStrings);
	for (int i = 0; i < count; i++) {
		readArgs[i] = atoi(readArgStrings[i]);
	}
	free(readArgStrings);
	return count;
}

nodetimeseries_t *init_observation_timeseries_from_sh(const int argc, const char *argv[],
	int * num_observationnodes) {
	
	int * x_indices = malloc(argc * sizeof(int));
	int * y_indices = malloc(argc * sizeof(int));
	int * ticks = malloc(argc * sizeof(int));
	*num_observationnodes = parseIntArgs(argc, argv, FLAG_X_OBSERVATIONNODES, x_indices);
	int num_y_observationnodes = parseIntArgs(argc, argv, FLAG_Y_OBSERVATIONNODES, y_indices);
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
	int num_tickargs = parseIntArgs(argc, argv, FLAG_TICKS, ticks);
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
    printf("Generating %d Hz frequency at at a resolution of %f ms per tick.\n", hz, tick_ms);
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
