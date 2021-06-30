#!/bin/bash
file_sizes=(512 2k 8k 32k 128k 512k 1M 8M 32M 256M 1G)
buffer_sizes=(512 1024 4096 8192 16384)

help() {
    echo "NAME"
    echo -e "\tRun specific ./main test\n"

    echo -e "SYNOPSIS"
    echo -e "\tbash script.sh <flags>"
    echo -e "\tchmod +x script.sh => ./script.sh <flags>"

    echo -e "\nFLAGS"
    echo "-h"
    echo -e "\tPrint help"

    echo "-c"
    echo -e "\tCreate test files"

    echo "-n [number]"
    echo -e "\tPerform specific 'number' of rounds; 'number' > 1"
    echo -e "\tDefault is 10"

    echo "-r"
    echo -e "\tRun 'read' test"

    echo "-m"
    echo -e "\tRun 'mmap' test"
}

create_files() {
    rm -rf test_files
    mkdir test_files

    cd test_files || exit

    for i in "${file_sizes[@]}"; do
        echo "Generating file_${i}.txt ..."
        base64 /dev/urandom | head -c "$i" >file_"$i".txt
        echo -e "Generated file_${i}.txt\n"
    done
    cd ..
}

run_read() {
    rm -rf read_results
    mkdir read_results

    make clean >>/dev/null
    make >>/dev/null
    n=$1

    echo 1 | sudo tee /proc/sys/vm/drop_caches >>/dev/null

    for i in "${file_sizes[@]}"; do
        echo "Starting with read_${i}.txt ..."
        touch read_results/read_"$i".txt
        for j in "${buffer_sizes[@]}"; do
            echo 1 | sudo tee /proc/sys/vm/drop_caches >>/dev/null
            total_time=0
            echo "buffer_size:${j}" >>read_results/read_"$i".txt
            for z in $(seq -f "%0${#n}g" 1 "$n"); do
                time_taken=$(./main -t -r "$j" test_files/file_"$i".txt | awk '{printf "%f", $1 * 1000}')
                total_time=$(echo "$total_time $time_taken" | awk '{printf "%f", $1 + $2}')
                echo "${z}:${time_taken}" >>read_results/read_"$i".txt
            done
            {
                echo "total_time:${total_time}"
                echo "average_time:$(echo "$total_time $n" | awk '{printf "%f", $1 / $2}')"
                printf "\n"
            } >>read_results/read_"$i".txt
        done
        echo -e "Done with read_${i}.txt\n"
        sed -i '$d' "read_results/read_${i}.txt"
    done
}

calculate_deviations() {
    chmod +x main.py
    echo "Calculating standard deviations ..."
    for i in "${file_sizes[@]}"; do
        ./main.py -r "$1" -f "${2}_results/${2}_${i}.txt"
    done
    echo "Done with calculating standard deviations"
}

run_mmap() {
    rm -rf mmap_results
    mkdir mmap_results

    make clean >>/dev/null
    make >>/dev/null
    n=$1

    echo 1 | sudo tee /proc/sys/vm/drop_caches >>/dev/null

    for i in "${file_sizes[@]}"; do
        echo "Starting with mmap_${i}.txt ..."
        total_time=0
        touch mmap_results/mmap_"$i".txt
        for j in $(seq -f "%0${#n}g" 1 "$n"); do
            time_taken=$(./main -tm test_files/file_"$i".txt | awk '{printf "%f", $1 * 1000}')
            total_time=$(echo "$total_time $time_taken" | awk '{printf "%f", $1 + $2}')
            echo "${j}:${time_taken}" >>mmap_results/mmap_"$i".txt
        done
        {
            echo "total_time:${total_time}"
            echo "average_time:$(echo "$total_time $n" | awk '{printf "%f", $1 / $2}')"
        } >>mmap_results/mmap_"$i".txt
        echo -e "Done with mmap_${i}.txt\n"
    done
}

if [[ -z "$*" ]]; then
    help
    exit 0
fi

c_flag=false
r_flag=false
m_flag=false
n_arg=10

while getopts "hcrmn:" opt; do
    case $opt in
    h)
        help
        exit 0
        ;;
    c)
        c_flag=true
        ;;
    r)
        r_flag=true
        ;;
    m)
        m_flag=true
        ;;
    n)
        n_arg="$OPTARG"
        if ! [[ "$n_arg" =~ ^[0-9]+$ ]]; then
            echo "Argument must be a number"
            exit 1
        fi

        if [ "$n_arg" -lt 1 ]; then
            echo "Argument must be greater than 0"
            exit 1
        fi
        ;;
    *)
        printf "\n"
        help
        exit 1
        ;;
    esac
done

if [[ $c_flag = true ]]; then
    create_files
fi

if [[ $r_flag = true ]]; then
    run_read "$n_arg"
    calculate_deviations "$n_arg" "read"
    make clean >>/dev/null
fi

if [[ $m_flag = true ]]; then
    run_mmap "$n_arg"
    calculate_deviations "$n_arg" "mmap"
    make clean >>/dev/null
fi
