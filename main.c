#include "brainsimulation.h"

int main(int argc, char *argv[])
{
    int tick_ms = 1;
    int num_ticks = 1000;
    int num_obervationnodes = 1;
    t_nodetimeseries obervationnodes[num_obervationnodes];
    obervationnodes[0].x_index = 20;
    obervationnodes[1].y_index = 20;
    int number_nodes_x = 200;
    int number_nodes_y = 50;
    t_nodeval ** nodegrid = alloc_2d(number_nodes_x,number_nodes_y);
    return simulate(tick_ms,num_ticks,number_nodes_x,number_nodes_y,nodegrid,
                    num_obervationnodes,obervationnodes);
}