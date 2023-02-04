#!/bin/bash

# COMMAND LINE ARGUMENTS CHECK #
if [[ $# -ne 2 && $# -ne 4 ]]; then
    echo "Usage: ./test_them_all -i <input_folder> [-o <benchmarks_output_folder> | optional]"
    exit
fi

OUTPUT_FOLDER=""

while :; do
    case $1 in
        -i) 
            INPUT_FOLDER=$2
            shift
            ;;
        -o)       
            OUTPUT_FOLDER=$2
            shift
            ;;
        *)                  # Default case: No more options, so break out of the loop.
            break
    esac
    shift
done

# BUILD THE PROJECT #
cd ../build
cmake .. && make

built_successfully=$(echo $?)

if [[ $built_successfully -ne 0 ]]; then
    echo "Did not built successfully."
    exit
fi

if [[ $OUTPUT_FOLDER != "-1" ]]; then
    rm -rf $OUTPUT_FOLDER
    mkdir $OUTPUT_FOLDER

    rm -rf ${OUTPUT_FOLDER}/incremental
    mkdir ${OUTPUT_FOLDER}/incremental

    rm -rf ${OUTPUT_FOLDER}/convex_hull
    mkdir ${OUTPUT_FOLDER}/convex_hull

    rm -rf ${OUTPUT_FOLDER}/onion
    mkdir ${OUTPUT_FOLDER}/onion
fi

# incremental
for initialization in 1a 1b 2a 2b
do
    for edge_selection in 1 2 3
    do
        output_file_piece=""
        if [[ $OUTPUT_FOLDER != "" ]]; then
            rm -rf ${OUTPUT_FOLDER}/incremental/${initialization}
            mkdir ${OUTPUT_FOLDER}/incremental/${initialization}
            output_file=${OUTPUT_FOLDER}/incremental/${initialization}/incr_${initialization}_${edge_selection}.txt
            output_file_piece="-o ${output_file} "
        fi
        ../scripts/run_test_cases.sh -i $INPUT_FOLDER $output_file_piece-algorithm incremental -initialization $initialization -edge_selection $edge_selection 
    done
done

# convex hull
for edge_selection in 1 2 3
do
    output_file_piece=""
    if [[ $OUTPUT_FOLDER != "" ]]; then
        output_file=${OUTPUT_FOLDER}/convex_hull/ch_${edge_selection}.txt
        output_file_piece=-o $output_file
    fi
    ../scripts/run_test_cases.sh -i $INPUT_FOLDER -o $output_file_piece-algorithm convex_hull -edge_selection $edge_selection
done

# onion
for onion_initialization in 1 2 3 4 5
do
    output_file_piece=""
    if [[ $OUTPUT_FOLDER != "" ]]; then
        output_file=${OUTPUT_FOLDER}/onion/onion_${onion_initialization}.txt
        output_file_piece=-o $output_file
    fi
    ../scripts/run_test_cases.sh -i $INPUT_FOLDER $output_file_piece-algorithm onion -onion_initialization $onion_initialization
done