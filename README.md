# BrainSimulation

Simulation of the human brain.

## How to build

* Linux
    1. Clone the repository.
    2. From the working directory root run

    ```
    $ make
    ```
    
    3. The makefile supports the `install` target (optional).

* Windows
    1. Clone the repository.
    2. Open `win32\win32.sln` in Visual Studio.
    3. Build using Visual Studio

### Customizing the build

You can customize the build by passing Preprocessor options. Using make, pass them to the `DFLAGS` variable. Use GCC's regular -D switch.

**Example**, sets the `THREADFACTOR` directive to `0.5` using GCCs -D switch:

```
$ make DFLAGS="-DTHREADFACTOR=0.5"
```

Available switches:
* `THREADFACTOR`: Factor to multiple the logical corecount with in order to determine the number of threads. May be a floating point number (*0.5* is a common option). Default = **1**.
* `MULTITHREADING`: Set to 0 to turn multithreading off and only use a single thread. Default = **1**.

## How to run

1. Create a `./testoutput` directory in the directory from which you execute the brainsimulation. Results will be written to this directory.
2. Configure inputs (WIP, using hardcoded inputs atm for testing)
3. Execute the simulation in one of the following ways:
    * Linux, running from the build directory:
 
    ```
    $ ./brainsimulation
    ```
    
    * Linux, after compiling using *make install*:
    
    ```
    $ brainsimulation
    ```
    
    * Windows:
    
    ```
    > .\brainsimulation
    ```
