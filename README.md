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

Available function modificators:
These factors can be customized during compilation to configure the behavior of each node during the simulation.
* `D_NEIGHBORFACTOR`:  Ratio of how much the direct neighbors influence the energy state of any node. This is a factor multiplied with the direct neighbor-energy. Usually a number in (0,1], however numbers > 1 are possible. Default = **1**.
* `ID_NEIGHBORFACTOR`:  Ratio of how much the indirect neighbors influence the energy state of any node. This is a factor multiplied with the indirect neighbor-energy. Usually a number in (0,1], however numbers > 1 are possible. Default = **1**.
* `ENERGY_FACTOR`:  Ratio of how much the historical energy state negatively influences the current energy slope. This is a factor multiplied with the historical energy. Usually a number in (0,1], however numbers > 1 are possible. Default = **1**.
* `ENERGY_WEIGHT`:  Ratio of how much the historical energy state influences the current energy state. This is a factor multiplied with the historical energy. Usually a number in (0,1], however numbers > 1 are possible. Default = **1**.


## How to run

1. Create a `./testoutput` directory in the directory from which you execute the brainsimulation. Results will be written to this directory.
2. (Optional) Configure inputs bitmap image files
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
* `--freqs FREQUENCIES` Frequencies of nodes generating energy using sin-frequencies. One or multiple integer parameters. Can not be used together with `--freqbitmaps` and its associated flags.
* `--freqx FREQUENCY_NODES_X_INDICES`: X indices of nodes generating energy using frequencies. One or multiple integer parameters. Must have the same number of parameters as `--freqs`.
* `--freqy STARTING_ENERGY_Y_INDICES`: Y indices of nodes generating energy using frequencies. One or multiple integer parameters. Must have the same number of parameters as `--freqs`.
* `--freqbitmaps FILENAMES`: File paths of bitmap images to be used for specifying sin-frequencies. Bitmaps must be uncompressed 24-bit bitmaps (MS-Paint default). When using multiple bitmaps, they must all have the same dimensions. Can not be used together with `--freqs` and its associated flags. One or multiple parameters.
* `--minbitmapfreq MIN_FREQUENCY`: The minimum frequency to generate, mapped to the minimum non-0 bitmap color (1). Single integer parameter.
* `--maxbitmapfreq MAX_FREQUENCY`: The maximum frequency to generate, mapped to the maximum bitmap color (765). Single integer parameter.
* `--bitmapduration DURATION_TICKS`: The generation duration (in ticks) for a bitmap's signal (analogous to a frame's duration in a movie). Single integer parameter.

**Example:**  

`brainsimulation -x 200 -y 200 --ticks 5000 --xobs 50 51 --yobs 50 51 --startlevels 10 11 --startx 10 11 --starty 10 11 --freqs 3 5 --freqx 25 26 --freqy 25 26`

**Example using bitmaps:**

`brainsimulation -x 200 -y 200 --ticks 3000 --xobs 50 51 --yobs 50 51 --freqbitmaps testinput/input0.bmp testinput/input1.bmp testinput/input2.bmp --minbitmapfreq 10 --maxbitmapfreq 40 --bitmapduration 1000`

### Using Bitmap Images as Inputs

You can choose to use bitmap images as inputs and pass them using the `--freqbitmaps` paramter. Doing this will generate input timeseries for all non-zero (non-black) pixels from the specified 24-bit bitmap images. Each pixel will be directly mapped to the corresponding node in the simulation (using its x and y coordinates, with 0,0 at the top left). Bitmaps must have the same dimensions and must be encoded using uncompressed, 24-bit bitmap files, which is the MS_Paint default. Bitmap color values are summed (R+G+B) and translated to frequencies. The minimum non-0 color (1) is mapped to the value provided using `--minbitmapfreq` and the maximum color (765) is mapped to the value provided using `--maxbitmapfreq`. Other frequencies are determined using linear interpolation. A black (value 0) pixel will always be interpreted as a pixel that is currently not generating a signal.

You can specify multiple images. Each image is shown for the duration specified using `--bitmapduration` (in ticks). Once its duration is up, the next image is used for generation (analogous to a frame in a movie). No further signals are generated once the last image has finished generating.

## Developing

Check out our code documentation at https://descartesresearch.github.io/BrainSimulation/
