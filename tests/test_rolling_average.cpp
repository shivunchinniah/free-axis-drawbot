#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include "../RollingAverage.h"

using namespace std;

int testSucceed = 0;
int testFail = 0;
int tests = 0;

void newtest(string test){
    cout << "Test: "<< test << " --- " ;
    tests++;
}

void testresult(bool res){
    if(res){
        testSucceed++;
        cout << "Test Passes" << endl;
    }
    else {
        testFail++;
        cout << "Test Failed!" << endl;
    }
}

void printstats(){
    cout << testSucceed << " test passed, " << testFail << " tests failed, Total: " << tests << endl; 
}


class Test{
    public:
    RollingAverage<unsigned long>* testbuffer;

    Test(int size){
        testbuffer = new RollingAverage<unsigned long>(size);
    }

    ~Test(){
        delete testbuffer;
    }
};

int main(){

    Test x(12);

    newtest("Zero initially");
    bool zero_begin = x.testbuffer->avg() == 0;
    testresult(zero_begin);


    newtest("Add one element");
    x.testbuffer->push(12);
    testresult(x.testbuffer->avg() == 1);

    newtest("Fill buffer with same value");
    for(int i =0; i<12; i++){
        x.testbuffer->push(123);
    }
    testresult(x.testbuffer->avg() == 123);

    unsigned long expected = (123 * 11) / 12;
    newtest("Push a zero, and get aproximate average " + to_string(expected) );
    x.testbuffer->push(0);
    testresult(x.testbuffer->avg() == expected);

    expected = (unsigned long) 8  / (unsigned long) 12;
    newtest("Test fraction result in 0, e.g.: " + to_string(expected));
    for(int i =0; i<12; i++){
        x.testbuffer->push(0);
    }
    x.testbuffer->push(8);
    testresult(x.testbuffer->avg() == expected);



    printstats();

    return 0;
}