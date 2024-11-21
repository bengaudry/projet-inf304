#!/bin/sh

make clean
clear

for i in 0 1 2 3 4 5 6 7 8 9;
do
    cp interpretes_tp7/interprete$i.c .
    echo
    ./run_tests.sh $i
    if [ $? -ne 0 ]; then
        echo "\033[0;0;41m => INTERPRETE $i FAILED \033[00m"
    else 
        echo "\033[0;0;42m => INTERPRETE $i PASSED \033[00m"
    fi
    echo
    rm ./interprete$i.c
done
make clean
