# This script makes runtime measurements of the program for different model parameters

import subprocess
import re
import csv

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


# Prepares and runs one specific measurement run
def execute_function(makeparameters, makevalues, runparameters, runvalues):
    # get makeparameters
    string = "DFLAGS="
    for i in range(len(makeparameters)):
        string = string + str(makeparameters[i])+ "="+ str(makevalues[i])+ " "
    print(string)
    # compile
    makecommand = ["make", string]
    for i in range(len(runparameters)):
        string = string + str(runparameters[i])+ "="+ str(runvalues[i])+ " "
    # get runparameters
    runcommand = "./brainsimulation"

    
    # execute one run
    return execute_command(makecommand, runcommand)

# Organizes and Executes the measurements
def take_measurements(paramnames_dict, output_stub):
    # for all parameters
    for param, values in param_dict.items():
        times = []
        for val in values:
            # for each measurement value
            times.append(execute_function([param], [val]))
        print_csv(pathname=output_stub+param+".csv", values=values, measurements=times)
    return

if __name__ == "__main__":
    # Defines the parameter grid to be measured
    param_dict = {
                "-DTHREADFACTOR": [0.25,0.5,0.75,1,2,4,6,8,10],
                "-DMULTITHREADING": [0,1]
                 }
    # The output directory
    output_stub = "./analyze/measurements/measurements"
    take_measurements(param_dict, output_stub)
