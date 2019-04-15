import matplotlib.pyplot as plt
import numpy as np

mat_dims = []

def parse_input_line(line):
    tokens = line.strip().split(' ')
    if len(tokens) != 3:
        return

    mat_dims.append(float(tokens[0]))

def parse_input_file(in_file):
    with open(in_file) as file:
        for line in file:
            parse_input_line(line)
    

def parse_time_line(times, line):
    tokens = line.strip().split(' ')
    if len(tokens) != 3:
        return
    
    if tokens[0] == "<<<":
        return    

    timestr = tokens[2]
    timetok = timestr.split('=')
    time = timetok[1]
    times.append(float(time))

def parse_time_file(in_file):
    times = []

    with open(in_file) as file:
        for line in file:
            parse_time_line(times, line)

    return times
    

def main():
    parse_input_file("input")
    neopt = parse_time_file("./gcc/neopt_gcc_times.txt")
    blas = parse_time_file("./gcc/blas_gcc_times.txt")
    optm = parse_time_file("./gcc/opt_m_gcc_times.txt")
    optf = parse_time_file("./gcc/opt_f_gcc_times.txt")

    neopt_icc = parse_time_file("./icc/neopt_icc_times.txt")
    blas_icc = parse_time_file("./icc/blas_icc_times.txt")
    optm_icc = parse_time_file("./icc/opt_m_icc_times.txt")
    optf_icc = parse_time_file("./icc/opt_f_icc_times.txt")

    mat_dims.insert(0,0)
    neopt.insert(0,0)
    blas.insert(0,0)
    optm.insert(0,0)
    optf.insert(0,0)
    neopt_icc.insert(0,0)
    blas_icc.insert(0,0)
    optm_icc.insert(0,0)
    optf_icc.insert(0,0)

    plt.figure(1)
    plt.subplot(311)
    plt.ylabel("Time")
    plt.title("Plot of times with GCC")

    plt.plot(mat_dims, blas, label='blas')
    plt.plot(mat_dims, neopt, label='neopt')
    plt.plot(mat_dims, optm, label='optm')
    plt.plot(mat_dims, optf, label='optf')

    plt.axis([0, 1600, 0, 80])
    plt.legend()

    plt.subplot(312)
    plt.ylabel("Time")
    plt.title("Plot of times with ICC")

    plt.plot(mat_dims, blas_icc, label='blas')
    plt.plot(mat_dims, neopt_icc, label='neopt')
    plt.plot(mat_dims, optm_icc, label='optm')
    plt.plot(mat_dims, optf_icc, label='optf')

    plt.axis([0, 1600, 0, 80])
    plt.legend()

    plt.subplot(313)
    plt.ylabel("Time")
    plt.ylabel("Matrix Dimension")
    plt.title("Comparison GCC vs ICC")

    plt.plot(mat_dims, optf, label='opt_f_gcc')
    plt.plot(mat_dims, optf_icc, label='opt_f_icc')

    plt.axis([0, 1600, 0, 20])
    plt.legend()

    plt.show()

if __name__ == "__main__":
    main()