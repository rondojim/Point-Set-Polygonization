#!/bin/bash

# COMMAND LINE ARGUMENTS CHECK #
if [[ $# -ne 6 && $# -ne 8 && $# -ne 10 ]]; then
    echo "Usage: ./run_test_cases -i <test_cases_folder> 
                    -algorithm <incremental/convex_hull/onion> 
                    -initialization <1a/1b/2a/2b | only in incremental>
                    -edge_selection <1 to 3 | except onion>
                    -onion_initialization <1 to 5 | only in onion>"
    exit
fi

OUT_FILE=""

while :; do
    case $1 in
        -i) 
            TESTS_FOLDER=$2
            shift
            ;;
        -o)       
            OUT_FILE=$2
            shift
            ;;
        -algorithm)
            ALGORITHM=$2
            shift
            ;;
        -initialization)         
            INITIALIZATION=$2
            shift
            ;;
        -edge_selection)
            EDGE_SELECTION=$2
            shift
            ;;
        -onion_initialization)    
            ONION_INITIALIZATION=$2
            shift
            ;;
        *)               # Default case: No more options, so break out of the loop.
            break
    esac
    shift
done

if [[ ALGORITHM == "incremental" && (INITIALIZATION == "" || EDGE_SELECTION == "") ]]; then
    echo "Incremental algorithm needs to specify initialization and edge selection policy!"
    exit
elif [[ ALGORITHM == "convex_hull" && EDGE_SELECTION == "" ]]; then
    echo "Based on convex hull algorithm needs to specify edge selection policy!"
    exit
elif [[ ALGORITHM == "onion" && ONION_INITIALIZATION == "" ]]; then
    echo "Onion algorithm needs to specify onion initialization policy!"
    exit
fi

function ProgressBar {
	let _progress=(${1}*100/${2}*100)/100
	let _done=(${_progress}*4)/10
	let _left=40-$_done
	_done=$(printf "%${_done}s")
	_left=$(printf "%${_left}s")
    printf "\rProgress : [${_done// /#}${_left// /-}] ${_progress}%%\n"
}

case $ALGORITHM in

  incremental)
    algo_cmd="-o output -algorithm $ALGORITHM -initialization $INITIALIZATION -edge_selection $EDGE_SELECTION"
    ;;

  convex_hull)
    algo_cmd="-o output -algorithm $ALGORITHM -edge_selection $EDGE_SELECTION"
    ;;

  onion)
    algo_cmd="-o output -algorithm $ALGORITHM -onion_initialization $ONION_INITIALIZATION"
    ;;

  *)
    echo "Invalid algorithm name!"
    exit
    ;;
esac

count=1
num_of_tests=$(ls $TESTS_FOLDER | wc -l)
echo "No of test to check: $num_of_tests"
all_passed=1

TIMEFORMAT=%R

for input_file in $TESTS_FOLDER/*
do
    if [[ $input_file == *"uniform"* ]]
    then
        idx=8
    else
        idx=7
    fi
    # extract number of points from input_file 
    point_num=$(sed -n '1p' $input_file | tr -d '(,)'| awk '{ print $'"$idx"' }')

    # extract area of convex hull from input file
    ch_area=$(sed -n '2p' $input_file | tr -d '",{,}' | awk '{ print $5 }')
    
    user_time="$(time ( ../build/bin/to_polygon -t 1 -i $input_file $algo_cmd ) 2>&1 1>/dev/null )"

    res_point_num=$(sed -n '1p' ../build/results.txt | awk '{ print $1 }')
    is_simple=$(sed -n '1p' ../build/results.txt | awk '{ print $2 }')
    res_ch_area=$(sed -n '1p' ../build/results.txt | awk '{ print $3 }')

    test_case_passed=1
    if [[ $point_num != $res_point_num ]]
    then
        printf "$input_file  --Different number of points!\n"
        printf "Num of points found: $res_point_num \t True points num $point_num\n"
        test_case_passed=0
    fi

    if [[ $is_simple != 'Y' ]]
    then
        printf "$input_file  --Polygon is not simple!\n"
        test_case_passed=0
    fi

    if [[ $ch_area != $res_ch_area ]]
    then
        printf "$input_file  --Different convex hull area!\n"
        printf "Area found: $res_ch_area \t True Area $ch_area\n"
        test_case_passed=0
    fi

    if [[ $test_case_passed -ne 1 ]] 
    then
        echo "$(tput setaf 1)ATTENTION!$ $(tput sgr0)TEST '$input_file' FAILED!"
        all_passed=0
        exit
    else
        echo "$(tput setaf 2)OK$(tput sgr0) TEST CASE '$input_file' PASSED"
        if [[ $OUT_FILE != "" ]]; then
            echo $point_num $user_time >> $OUT_FILE
        fi
    fi

    # sleep 0.2
	ProgressBar ${count} ${num_of_tests}
    count=$(( $count + 1 ))
done

if [[ $all_passed -eq 1 ]]
then
    echo "All test cases passed!"
else 
    echo "Some test case(s) failed"
fi

# Delete the results file used for testing 
rm -f ../build/results.txt