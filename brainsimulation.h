//types

/**
 * Type that the nodes in the brainsimulation use to store their energy level.
 */
typedef double t_nodeval;

/**
 * Struct to store the results of a simulation for a single observed node.
 * x_index and y_index members must be set when passing it to a simulation.
 */
typedef struct 
{
	/**
	 * x index of the node.
	 */
	int x_index;
	/**
	 * y index of the node.
	 */
	int y_index;
	/**
	 * Series of observed node energy levels. One element per tick.
	 * Has #timeseries_ticks as length.
	 */
	t_nodeval * timeseries;
	/**
	 * Length of #timeseries.
	 */
	int timeseries_ticks;
} t_nodetimeseries;

//function declarations

/**
 * Simulates the brain.
 * @param tick_ms Milliseconds in between each simulation tick.
 * @param num_ticks The number of ticks to simulate.
 * This will also be the length of all timeseries contained in the returned
 * oberservationnodes.
 * @param number_nodes_x The number of nodes in the first dimension of nodes.
 * @param number_nodes_y The number of nodes in the second dimension of nodes.
 * @param nodes 2D array of nodes with their starting energy level.
 * @param num_obervationnodes The number of nodes to observe during simulation.
 * @param The nodes to observe during simulation. x_index and y_index members
 * must be set. All other members will be overwritten with the simulation
 * results. Must have observationnodes as length.
 * @return Return-codes.
 */
int simulate(int tick_ms,
	int num_ticks,
	int number_nodes_x,
	int number_nodes_y,
	t_nodeval ** nodes,
	int num_obervationnodes,
	t_nodetimeseries * oberservationnodes);
	