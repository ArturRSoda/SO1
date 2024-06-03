#ifndef simulatorH
#define simulatorH

#include <bits/stdc++.h>
#include "fileReader.h"
#include "bitset.h"

using namespace std;

/*
struct element {
    int start;
    int size;
    int status; // 0 == free ; 1 == occupied

    bool operator>=(const element& other) const {
        return (this->start >= other.start);
    }
};
*/

class Simulator {
 public:
    Simulator(parameters* parameters_);
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

    int manager;
    int alloc_alg;
    int mem_size; // bytes
    int block_size; // bytes
    DoublyLinkedList<string> requests;

    Bitset* mem_list_bit;
    DoublyLinkedList<element> mem_list_dll;
    int* start;
};
#endif
