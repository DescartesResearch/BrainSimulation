# This script makes runtime measurements of the program for different model parameters

import subprocess
import re
import csv
import math
import os

# Prints the results to the given csv.
def print_csv(pathname, values, measurements):
    with open(pathname, "w+", newline='') as csvfile:
        csvwriter = csv.writer(csvfile, delimiter=",")
        for i, value in enumerate(values):
            csvwriter.writerow([value, measurements[i]])
    return

# Executes one specific measurement runs
def execute_command(makecommand, runcommand):
    # remove previous compilation unit
    subprocess.run(["rm", "./brainsimulation"], check=False)
    print(subprocess.run(makecommand))
    
    # execute simulation
    result = subprocess.run(runcommand, check=True, stdout=subprocess.PIPE)
    if result.stderr:
        print(result.stderr)
    # extract time measurements from log
    time = re.search("Total time = .* seconds",str(result.stdout)).group()
    # remove rest of line and convert to float
    time = float(time.split("= ")[1].split(" seconds")[0])
    return time

# Parses a given runtime parameter into the commands actually executed for the simulation
def parse_runcommand(parameter, value):
    if parameter == "gridsize":
        root = int(math.sqrt(float(value)))
        return ["-x",str(root),"-y",str(root)]
    elif parameter == "ticks":
        return ["--ticks",str(value)]
    elif parameter == "obsnodes":
        obslist = ["--xobs"]
        for i in range(value):
            obslist.append(get_node_x(i))
        obslist.append("--yobs")
        for i in range(value):
            obslist.append(get_node_y(i))
        return obslist
    elif parameter == "inputnodes":
        inputlist = ["--freqs"]
        for i in range(value):
            inputlist.append(get_node_freq(i))
        inputlist.append("--freqx")
        for i in range(value):
            inputlist.append(get_node_x(i))
        inputlist.append("--freqy")
        for i in range(value):
            inputlist.append(get_node_y(i))
        return inputlist
    elif parameter == "startnodes":
        startlist = ["--startlevels"]
        for i in range(value):
            # we just use the frequency values here, as it does not matter
            startlist.append(get_node_freq(i))
        startlist.append("--startx")
        for i in range(value):
            startlist.append(get_node_x(i))
        startlist.append("--starty")
        for i in range(value):
            startlist.append(get_node_y(i))
        return startlist
    else:
        print("Parameter "+str(parameter)+" is not known.")
        return []

# Get a x coordinate for the given index.
def get_node_x(index):
    return str(index)

# Get a y coordinate for the given index.
def get_node_y(index):
    return str(index)

# Get a frequency value for the given index.
def get_node_freq(index):
    primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173]
    if index > len(primes):
        print("No more frequencies available.")
        return 1
    return str(primes[index])

# Prepares and runs one specific measurement run
def execute_function(makeparameters, makevalues, runparameters, runvalues):
    # get makeparameters
    makestring = "DFLAGS="
    for i in range(len(makeparameters)):
        makestring = makestring + str(makeparameters[i])+ "="+ str(makevalues[i])+ " "
    makecommand = ["make", makestring]
    if len(makeparameters) == 0:
        makecommand = "make"
    print(makecommand)

    # get runparameters
    runcommand = ["./brainsimulation"]
    for i in range(len(runparameters)):
        for arg in parse_runcommand(runparameters[i], runvalues[i]):
            runcommand.append(arg)
    print(runcommand)
    
    # execute one run
    return execute_command(makecommand, runcommand)

# Organizes and Executes the measurements
def take_measurements(param_comp_dict, param_run_dict, output_dir):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    # for compile parameters
    for param, values in param_comp_dict.items():
        times = []
        for val in values:
            # for each measurement value
            times.append(execute_function([param], [val], [], []))
        print_csv(pathname=output_dir+"/measurements-"+param+".csv", values=values, measurements=times)
    
    # for runtime all parameters
    for param, values in param_run_dict.items():
        times = []
        for val in values:
            # for each measurement value
            times.append(execute_function([],[], [param], [val]))
        print_csv(pathname=output_dir+"/measurements-"+param+".csv", values=values, measurements=times)
    return

# Main entry point
if __name__ == "__main__":
    # Defines the parameter grid to be measured
    param_comp_dict = {
                "-DTHREADFACTOR": [0.1, 0.25, 0.5, 0.75, 1, 2, 3, 4, 8, 16, 32, 64, 128],
                "-DMULTITHREADING": [0,1]
                 }
    param_run_dict = {
        "gridsize": [15000, 20000, 25000, 30000, 35000, 40000, 45000, 50000, 60000, 70000, 80000, 90000, 100000, 200000, 300000, 400000, 500000, 1000000, 2000000, 3000000, 4000000, 5000000],
        "ticks": [50, 100, 200, 300, 400, 500, 1000, 2000, 3000, 4000, 5000, 10000, 20000, 30000, 40000, 50000, 100000, 200000, 300000, 400000, 500000, 1000000],
        "obsnodes" : [1,2,3,4,5,6,7,8,9,10],
        "inputnodes" : [1,2,3,4,5,6,7,8,9,10],
        "startnodes" : [1,2,3,4,5,6,7,8,9,10]
        }
    # The output directory
    output_dir = "./analyze/measurements"
    take_measurements(param_comp_dict, param_run_dict, output_dir)
