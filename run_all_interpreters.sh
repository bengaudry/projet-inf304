#!/bin/sh

make clean
clear

for i in 0 1 2 3 4 5 6 7 8 9;
do
    echo
    ./run_tests.sh $i
    if [ $? -ne 0 ]; then
        echo "\033[0;0;41m => INTERPRETE $i FAILED \033[00m"
    else 
        echo "\033[0;0;42m => INTERPRETE $i PASSED \033[00m"
    fi
    echo
done
