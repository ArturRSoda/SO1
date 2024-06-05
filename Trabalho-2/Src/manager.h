#ifndef MngH
#define MngH

class Manager {
public:
    Manager();
    ~Manager();

    virtual int alloc(int size, int alloc_alg) = 0;
    virtual void del(int seg_start, int seg_size) {};

    virtual int firstFit(int size) = 0;
    virtual int nextFit(int size) = 0;
    virtual int searchSeg(int  seg_ptr, int size) = 0;
    virtual int getQtyAllocatedBlocks() = 0;
    
    virtual void printState() {};

    int block_size;
};

#endif
