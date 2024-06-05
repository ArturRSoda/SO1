#ifndef BitmapManagerH
#define BitmapManagerH

#include <iostream>
#include "manager.h"
#include "bitset.h"

class BitmapManager: public Manager {
public:
    BitmapManager(int qty_block, int block_size_);
    ~BitmapManager();

    int alloc(int size, int alloc_alg) override;
    void del(int seg_start, int seg_size) override;

    int firstFit(int size) override;
    int nextFit(int size) override;
    int searchSeg(int seg_ptr, int size) override;
    int getQtyAllocatedBlocks() override;

    void printState() override;

private:
    Bitset* mem_list_bit;
    int last_allocation_start;
};

#endif
