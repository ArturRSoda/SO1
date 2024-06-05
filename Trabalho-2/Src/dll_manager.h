#ifndef DllManagerH
#define DllManagerH

#include <iostream>
#include "manager.h"
#include "doubly_linked_list.h"

class DllManager: public Manager {
public:
    DllManager(int qty_block, int block_size_);

    int alloc(int size, int alloc_alg) override;
    void del(int seg_start, int seg_size) override;

    int firstFit(int size) override;
    int nextFit(int size) override;
    int searchSeg(int seg_ptr, int size) override;
    int getQtyAllocatedBlocks() override;

    void printState() override;

private:
    int last_allocation_start;
    DoublyLinkedList<element> mem_list_dll;
};

#endif
