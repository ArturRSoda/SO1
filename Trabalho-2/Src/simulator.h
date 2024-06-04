#ifndef simulatorH
#define simulatorH

#include <bits/stdc++.h>
#include "fileReader.h"
#include "bitset.h"

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

    void allocDll(int size, int id);
    void delDll(int id);
    int firstFitDll(int size);
    int nextFitDll(int size);

    void allocBit(int size, int id);
    void delBit(int id);
    int firstFitBit(int size);
    int nextFitBit(int size);

    void printDll();
    void printStart();
    void printState();

    size_t next_fit_pointer;
    int last_allocation_start;

    int manager;
    int alloc_alg;
    int mem_size; // bytes
    int block_size; // bytes
    int qtd_requests;
    string requests[REQUEST_SIZE];

    Bitset* mem_list_bit;
    DoublyLinkedList<element> mem_list_dll;
    int* start;
    int* size;
};
#endif
