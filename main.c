#include "brainsimulation.h"
#include <stdio.h>
#include <stdlib.h>


// Forward declaration.
t_nodeval ** alloc_2d(int m, int n);

int main(int argc, char *argv[])
{
    int tickms = 1;
    int num_ticks = 1000;
    int num_obervationnodes = 1;
    t_nodetimeseries obervationnodes[num_obervationnodes];
    obervationnodes[0].x_index = 20;
    obervationnodes[1].y_index = 20;
    int number_nodes_x = 200;
    int number_nodes_y = 50;
    t_nodeval ** nodegrid = alloc_2d(number_nodes_x,number_nodes_y);
    return simulate(tickms,num_ticks,number_nodes_x,number_nodes_y,nodegrid,
                    num_obervationnodes,obervationnodes);
}

t_nodeval ** alloc_2d(int m, int n)
{
    t_nodeval ** arr = malloc(m * sizeof(*arr));
    int i;
    for (i = 0; i < m; ++i)
    {
        arr[i] = malloc(n * sizeof(t_nodeval));
    }
    return arr;
}