#!/bin/bash

if test -f ./test.t; then
    rm ./test.t
fi

g++ test_rolling_average.cpp -o test.t && ./test.t