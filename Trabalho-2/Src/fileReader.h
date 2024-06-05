#ifndef fileReaderH
#define fileReaderH

#define REQUEST_SIZE 1000

#include <iostream>
#include <string>
#include "doubly_linked_list.h"

using namespace std;

struct parameters {
    int manager;
    int mem_size;
    int block_size;
    int alloc_alg;
    int qty_requests;
    string requests[REQUEST_SIZE];
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
