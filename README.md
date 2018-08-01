# BrainSimulation

Simulation of the human brain.

## How to build

**Linux**
1. Clone the repository.
2. From the working directory root run

    `$ make`

3. The makefile supports the `install` target (optional).

**Windows**
1. Clone the repository.
2. Open *win32\win32.sln* in Visual Studio.
3. Build using Visual Studio

### Customizing the build

You can customize the build by passing Preprocessor options. Using make, pass them to the *DFLAGS* variable. Use GCC's regular -D switch.

**Example**, sets the *THREADFACTOR* directive to *0.5* using GCCs -D switch:

 `$ make DFLAGS="-DTHREADFACTOR=0.5"`

Available switches:
* `THREADFACTOR`: Factor to multiple the logical corecount with in order to determine the number of threads. May be a floating point number (*0.5* is a common option). Default = **1**.
* `MULTITHREADING`: Set to 0 to turn multithreading off and only use a single thread. Default = **1**.

## How to run

1. Create a `./testoutput` directory in the directory from which you execute the brainsimulation. Results will be written to this directory.
2. Configure inputs files (WIP, currently disabled by default)
3. Execute the simulation in one of the following ways:
  * Linux, running from the build directory:

    `$ ./brainsimulation -x X_NODES -y Y_NODES --ticks SIMULATION_TICKS [OPTIONAL PARAMETERS]`
    
  * Linux, after compiling using *make install*:
    
    `$ brainsimulation -x X_NODES -y Y_NODES --ticks SIMULATION_TICKS [OPTIONAL PARAMETERS]`
    
  * Windows:

    `> .\brainsimulation -x X_NODES -y Y_NODES --ticks SIMULATION_TICKS [OPTIONAL PARAMETERS]`

### Command Line Parameters
Command line parameters are also shown when starting the brainsimulation with `brainsimulation --help`.

Generally, run with: `brainsimulation -x X_NODES -y Y_NODES --ticks SIMULATION_TICKS [OPTIONAL PARAMETERS]`

Mandatory parameters:
* `-x X_NODES`: Size of the simulated node grid on the X axis. Single integer parameter.
* `-y Y_NODES`: Size of the simulated node grid on the Y axis. Single integer parameter.
* `--ticks SIMULATION_TICKS`: Number of time ticks to simulate. Single integer parameter.

Recommended parameters (technically optional, but doesn't really make sense not to use them):
* `--xobs OBS_X_INDICES`: X indices of the observed nodes. One or multiple integer parameters.
* `--yobs OBS_Y_INDICES`: Y indices of the observed nodes. One or multiple integer parameters. Must have the same number of parameters as `--xobs`.

Optional parameters:
* `--startlevels STARTING_ENERGY_LEVELS`: Initial energy levels of nodes with starting energy. One or multiple floating point parameters.
* `--startx STARTING_ENERGY_X_INDICES`: X indices of nodes with starting energy. One or multiple integer parameters. Must have the same number of parameters as `--startlevels`.
* `--starty STARTING_ENERGY_Y_INDICES`: Y indices of nodes with starting energy. One or multiple integer parameters. Must have the same number of parameters as `--startlevels`.
* `--freqs FREQUENCIES` Frequencies of nodes generating energy using sin-frequencies. One or multiple integer parameters.
* `--freqx FREQUENCY_NODES_X_INDICES`: X indices of nodes generating energy using frequencies. One or multiple integer parameters. Must have the same number of parameters as `--freqs`.
* `--freqy STARTING_ENERGY_Y_INDICES`: Y indices of nodes generating energy using frequencies. One or multiple integer parameters. Must have the same number of parameters as `--freqs`.

**Example:**  
`brainsimulation -x 200 -y 200 --ticks 5000 --xobs 50 51 --yobs 50 51 --startlevels 10 11 --startx 10 11 --starty 10 11 --freqs 3 5 --freqx 25 26 --freqy 25 26`

## Developing

Check out our code documentation at https://descartesresearch.github.io/BrainSimulation/
