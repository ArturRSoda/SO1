#ifndef simulatorH
#define simulatorH

#include <sstream>

#include "fileReader.h"
#include "manager.h"
#include "dll_manager.h"
#include "bitmap_manager.h"

using namespace std;

class Simulator {
 public:
    Simulator();
    ~Simulator();

    void run();

 private:
    void loadParameters(parameters* p);

    void alloc(int size, int id);
    void del(int id);

    void printDll();
    void printStart();
    void printFinalState();

    int bytes_allocated;
    int bytes_deleted;
    int qty_allocation;
    int qty_deletion;

    int memory_manager;
    int alloc_alg;
    int mem_size; // bytes
    int block_size; // bytes
    int qty_requests;
    string requests[REQUEST_SIZE];

    int* start;
    int* size;

    Manager* manager;
};
#endif
