#pragma once

#include <iostream>
#include <string.h>

using namespace std;

int testSucceed = 0;
int testFail = 0;
int tests = 0;

enum TextColorCode {
    BLACK = 30,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    WHITE = 37,
};

string getColourText(const string& in, const TextColorCode colour){
    return "\033[1;" + to_string(colour) + "m" +  in  + "\033[0m";
}

void newtest(string test){
    cout << "Test: "<< test << " --- " ;
    tests++;
}

void testresult(bool res){
    if(res){
        testSucceed++;
        cout << getColourText("Test Passes", TextColorCode::GREEN) << endl;
    }
    else {
        testFail++;
        cout << getColourText("Test Failed!", TextColorCode::RED) << endl;
    }
}


void printstats(){
    cout << testSucceed << " test passed, " << testFail << " tests failed, Total: " << tests << endl; 
}





