#include "../Var.h"
#include "basic_testing.hpp"
#include <iostream>


using namespace std;


int main(){


    newtest("Create a test 'Var' Object of a float");
    Var<float> test_float(2.2f, "test_float");
    testresult(test_float.get() == 2.2f);

    newtest("Var index is 1 after adding an instance");
    testresult(Var<float>::getInstancesCount() == 1);

    newtest("Var at index 1 has appropriate name");
    testresult(Var<float>::getInstancesName(0) == "test_float");


    newtest("Create a test var in a different type");
    Var<int> test_int(42, "test_int");
    testresult(test_int.get() == 42);

    newtest("Add another instance");
    Var<float> test_float2(2.4f, "test_float2");
    testresult(test_float2.get() == 2.4f && Var<float>::getInstancesCount() == 2 && Var<float>::getInstancesName(1) == "test_float2");


    newtest("Change value of an instance");
    test_float.set(69.68f);
    testresult(test_float.get() == 69.68f);

    printstats();

    return 0;
}