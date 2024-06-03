#include "simulator.h"
#include "bitset.h"
#include "doubly_linked_list.h"
#include "fileReader.h"

Simulator::Simulator(parameters* parameters_) {
    parameters* p = parameters_;
    loadParameters(p);

    next_fit_pointer = 0;
    element e = {0, mem_size/block_size, 0};
    mem_list_dll.push_back(e);
    start = new int[p->requests.size()+1];
    mem_list_bit = new Bitset(mem_size/block_size);
}

Simulator::~Simulator() {
    delete start;
    delete mem_list_bit;
}

void Simulator::loadParameters(parameters* p) {
    manager = p->manager;
    alloc_alg = p->alloc_alg;
    mem_size = p->mem_size;
    block_size = p->block_size;
    requests = p->requests;
}

void Simulator::allocDll(int size, int id) {
    int cur_start;

    if (alloc_alg == 1) cur_start = firstFitDll(size);
    else                cur_start = nextFitDll(size);

    int cur;
    for (size_t i = 0; i < mem_list_dll.size(); i++) {
        if (mem_list_dll.at(i).start == cur_start) {
            cur = i;
            break;
        }
    }

    next_fit_pointer = cur;

    element old_seg = mem_list_dll.at(cur);
    mem_list_dll.remove(old_seg);

    start[id] = cur_start;
    element allocated_seg = {cur_start, size, 1};
    mem_list_dll.insert_sorted(allocated_seg);

    if (old_seg.size - allocated_seg.size) {
        element free_seg = {cur_start+size, old_seg.size-allocated_seg.size, 0};
        mem_list_dll.insert_sorted(free_seg);
    }
}

//todo
void Simulator::delDll(int id) {
    int cur_start = start[id];
    for (size_t i = 0; i < mem_list_dll.size(); i++) {
        if (cur_start == mem_list_dll.at(i).start) {
            mem_list_dll.pop(i);
            return;
        }
    }
}

int Simulator::firstFitDll(int size) {
    int cur;
    element elem;
    for (size_t i = 0; i < mem_list_dll.size(); i++) {
        elem = mem_list_dll.at(i);
        if ((elem.size >= size) and (elem.status == 0)) {
            cur = mem_list_dll.at(i).start;
            break;
        }
    }
    return cur;
}

//mesmo do de cima mas colocar atributo que salva onde estava procurando
//e comecar i = atributo ate o final+atributo para rodar a lista inteira
int Simulator::nextFitDll(int size) {
    int cur;
    element elem;
    while (true) {
        if (next_fit_pointer >= mem_list_dll.size())
            next_fit_pointer = 0;

        elem = mem_list_dll.at(next_fit_pointer);
        if ((elem.size >= size) and (elem.status == 0)) {
            cur = elem.start;
            break;
        }

        next_fit_pointer++;
    }
    return cur;
}

void Simulator::allocBit(int size, int id) {
}

void Simulator::delBit(int id) {
}

void Simulator::alloc(int size, int id) {
    if (manager == 1) allocBit(size, id);
    else              allocDll(size, id);
}

void Simulator::del(int id) {
    if (manager == 1) delBit(id);
    else              delDll(id);
}

void Simulator::run() {
    printDll();
    printStart();
    cout << endl;
    for (size_t i = 0; i < requests.size(); i++) {
        string request = requests.at(i);
        stringstream ss(request);

        string type;
        int size, id;

        ss >> type;
        if (type == "A") {
            ss >> size >> id;
            alloc(size, id);
        } else {
            ss >> id;
            del(id);
        }
        printDll();
        printStart();
        cout << endl;

    }
    printState();
}

void Simulator::printDll() {
    for (size_t i = 0; i < mem_list_dll.size(); i++) {
        cout << mem_list_dll.at(i).start << ",";
        cout << mem_list_dll.at(i).size << ",";
        cout << mem_list_dll.at(i).status << " -> ";
    }
    cout << endl;
}

void Simulator::printState() {
    if (manager == 1) {
        cout << mem_list_bit->value() << endl;
    } else {
        element elem;
        for (size_t i = 0; i < mem_list_dll.size(); i++) {
            elem = mem_list_dll.at(i);
            cout << elem.size*block_size << " " << elem.status << endl;
        }
    }
}

void Simulator::printStart() {
    for (size_t i = 0; i < requests.size()+1; i++) {
        cout << start[i] << ", ";
    }
    cout << endl;
}
