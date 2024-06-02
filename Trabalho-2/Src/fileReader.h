#ifndef fileReaderH
#define fileReaderH

#include <bits/stdc++.h>
#include "doubly_linked_list.h"

using namespace std;

struct parameters {
    int manager;
    int mem_size;
    int block_size;
    int alloc_alg;
    DoublyLinkedList<string> requests;    
};

class FileReader {
 public:
    FileReader(string file_name);
    ~FileReader();

    parameters* readFile();

 private:
    parameters* p;
    fstream file;
};

#endif
