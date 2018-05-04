#include "brainsimulation.h"
#include <stdio.h>
#include <stdlib.h>

struct observations {
    int num_obervationnodes;
    t_nodetimeseries * obervationnodes;
};

void init_state(int number_nodes_x, int number_nodes_y, t_nodeval ** nodes){
    // init the (one-time) state here
    nodes[20][20] = 304;
    nodes[40][40] = 12;
    nodes[50][50] = 3;
    nodes[100][100] = 1000;
}

struct observations * init_observationnodes(int num_ticks){
    struct observations * nodes = malloc (sizeof(struct observations));
    nodes->num_obervationnodes = 4;
    nodes->obervationnodes = malloc (nodes->num_obervationnodes * sizeof(t_nodetimeseries));
    nodes->obervationnodes[0].x_index = 20;
    nodes->obervationnodes[0].y_index = 20;
    nodes->obervationnodes[0].timeseries = malloc (num_ticks*sizeof(t_nodeval));
    nodes->obervationnodes[0].timeseries_ticks = num_ticks;
    nodes->obervationnodes[1].x_index = 21;
    nodes->obervationnodes[1].y_index = 20;
    nodes->obervationnodes[1].timeseries = malloc (num_ticks*sizeof(t_nodeval));
    nodes->obervationnodes[1].timeseries_ticks = num_ticks;
    nodes->obervationnodes[1].timeseries_ticks = num_ticks;
    nodes->obervationnodes[2].x_index = 22;
    nodes->obervationnodes[2].y_index = 20;
    nodes->obervationnodes[2].timeseries = malloc (num_ticks*sizeof(t_nodeval));
    nodes->obervationnodes[2].timeseries_ticks = num_ticks;
    nodes->obervationnodes[3].x_index = 100;
    nodes->obervationnodes[3].y_index = 100;
    nodes->obervationnodes[3].timeseries = malloc (num_ticks*sizeof(t_nodeval));
    nodes->obervationnodes[3].timeseries_ticks = num_ticks;
    return nodes;
}

int main(int argc, char *argv[])
{
    int tick_ms = 1;
    int num_ticks = 1000;
    struct observations * nodes = init_observationnodes(num_ticks);
    int number_nodes_x = 1200;
    int number_nodes_y = 1200;
    t_nodeval ** nodegrid = alloc_2d(number_nodes_x,number_nodes_y);
    init_state(number_nodes_x, number_nodes_y, nodegrid);
    simulate(tick_ms,num_ticks,number_nodes_x,number_nodes_y,nodegrid,
                    nodes->num_obervationnodes,nodes->obervationnodes);
    printf("Output:\n");
    for (int j = 0; j < nodes->num_obervationnodes; ++j) {
        printf("    Node %d: (%d|%d):\n", j, nodes->obervationnodes[j].x_index,  nodes->obervationnodes[j].y_index);
        int i;
        for (i = 0; i < num_ticks; ++i) {
            printf("    Time: %d, Energy-Value: %f.\n", i,  nodes->obervationnodes[j].timeseries[i]);
        }
    }
}

