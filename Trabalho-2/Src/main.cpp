#include <iostream>

#include "fileReader.h"
#include "simulator.h"

using namespace std;

int main() {
    FileReader fr = FileReader();
    fr.readFile();
    parameters* p = fr.getParameters();

    Simulator sm = Simulator(p);
    sm.run();

}

