#ifndef fileReaderH
#define fileReaderH

#include <iostream>
#include <string>
#include "doubly_linked_list.h"

using namespace std;

struct parameters {
    int manager;
    int mem_size;
    int block_size;
    int alloc_alg;
    int qtd_requests;
    string requests[1000];
};

class FileReader {
 public:
    FileReader();
    ~FileReader();

    void readFile();

    parameters* getParameters();

 private:
    parameters* p;
};

#endif
