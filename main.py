#!/usr/bin/env python3
import getopt
import os
from statistics import stdev
from sys import argv


def usage():
    print("Name")
    print("\tCalculate the standard deviation for a file")

    print("Flags")
    print("-h")
    print("\tPrint help")
    print("-r <rounds>")
    print("\tSpecify the number of rounds - required")
    print("-f <filename>")
    print("\tFilename - required")

    print("Usage")
    print("\tpython3 main.py -r [number] -f [filename]")


def calculate_std_read(file_name: str, n_rounds: int):
    try:
        file = open(file_name, "r")
    except IOError:
        print(f"Error when opening the file '{file_name}'")
        exit(1)
    else:
        with file:
            content = file.readlines()

    new_file = []
    for i in range(1, len(content), n_rounds + 4):
        test = content[i - 1:i + n_rounds + 2]
        times = [float(x.split(":")[1].rstrip()) for x in content[i:i + n_rounds]]
        test.append(f"deviation:{'%.4f' % round(stdev(times), 4)}\n\n")
        new_file.append(test)

    try:
        file = open(file_name, "w")
    except IOError:
        print(f"Error when opening the file '{file_name}'")
        exit(1)
    else:
        with file:
            for item in new_file:
                for x in item:
                    file.write(x)

    os.system(f"sed -i '$d' {file_name}")


def calculate_std_mmap(file_name: str, n_rounds: int):
    try:
        file = open(file_name, "r+")
    except IOError:
        print(f"Error when opening the file '{file_name}'")
        exit(1)
    else:
        with file:
            times = [float(next(file).split(":")[1].rstrip()) for _ in range(n_rounds)]
            file.write(f"deviation:{'%.4f' % round(stdev(times), 4)}\n")


if __name__ == '__main__':
    opts = []

    try:
        opts, _ = getopt.getopt(argv[1:], "hr:f:", ["help", "rounds=", "file="])
    except getopt.GetoptError:
        usage()
        exit(1)

    rounds = 10
    filename = ""

    if opts:
        for opt, arg in opts:
            if opt in ("-h", "--help"):
                usage()
                exit()
            elif opt in ("-r", "--rounds"):
                try:
                    rounds = int(arg)
                except ValueError:
                    print("-r argument has to be a number")
                    exit(1)
            elif opt in ("-f", "--file"):
                filename = arg
            else:
                usage()
                exit(1)
    else:
        usage()
        exit(1)

    if rounds < 1:
        print("-r argument has to be greater than 1")
        exit(1)

    calculate_std_mmap(filename, rounds) if filename[0:4] == "mmap" else calculate_std_read(filename, rounds)
