#include "simulator.h"
#include "bitset.h"
#include "doubly_linked_list.h"
#include "fileReader.h"

Simulator::Simulator() {
    FileReader fr = FileReader();
    fr.readFile();
    parameters* p = fr.getParameters();
    loadParameters(p);

    next_fit_pointer = 0;
    last_allocation_start = 0;

    element e = {0, (mem_size+1)/block_size, 0};
    mem_list_dll.push_back(e);
    start = new int[REQUEST_SIZE];
    size = new int[REQUEST_SIZE];
    mem_list_bit = new Bitset((mem_size+1)/block_size);
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
    qtd_requests = p->qtd_requests;

    for (int i = 0; i < qtd_requests; i++) {
        requests[i] = p->requests[i];
    }
}

void Simulator::allocDll(int size, int id) {
    int cur_start;
    if (alloc_alg == 1) cur_start = firstFitDll(size);
    else                cur_start = nextFitDll(size);
    if (cur_start == -1) return;

    int cur;
    for (size_t i = 0; i < mem_list_dll.size(); i++) {
        if (mem_list_dll.at(i).start == cur_start) {
            cur = i;
            break;
        }
    }

    last_allocation_start = cur_start;

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

void Simulator::delDll(int id) {
    element cur_seg, back_seg, front_seg, free_seg;
    int cur_start = start[id];
    for (size_t i = 0; i < mem_list_dll.size(); i++) {
        cur_seg = mem_list_dll.at(i);
        if (cur_start == cur_seg.start) {
            if (i != 0) {
                back_seg = mem_list_dll.at(i-1);
            }

            if (i != (mem_list_dll.size()-1)) {
                front_seg = mem_list_dll.at(i+1);
            }

            free_seg.start = ((i != 0) && (back_seg.status == 0)) ? back_seg.start : cur_seg.start;

            free_seg.size = cur_seg.size;
            free_seg.size += ((i != 0) && (back_seg.status == 0)) ? back_seg.size : 0;
            free_seg.size += ((i != (mem_list_dll.size()-1)) && (front_seg.status == 0)) ? front_seg.size : 0;

            free_seg.status = 0;
             
            mem_list_dll.pop(i);
            if ((i != 0) && (back_seg.status == 0)) mem_list_dll.remove(back_seg);
            if ((i != (mem_list_dll.size()-1)) && (front_seg.status == 0)) mem_list_dll.remove(front_seg);

            mem_list_dll.insert_sorted(free_seg);

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

int Simulator::nextFitDll(int size) {
    int cur;
    element elem;

    for (size_t i = 0; i < mem_list_dll.size(); i++) {
        if (mem_list_dll.at(i).start == last_allocation_start)
            next_fit_pointer = i;
    }

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
    int cur_start;
    if (alloc_alg == 1) cur_start = firstFitBit(size);
    else                cur_start = nextFitBit(size);
    if (cur_start == -1) return;

    start[id] = cur_start;
    for (int i = 0; i < size-1; i++) {
        mem_list_bit->fix(cur_start + i);
    }
}

void Simulator::delBit(int id) {
    int cur_start = start[id]; 
    int cur_size = size[id];
    int i = 0;
    for (int i = 0; i < cur_size; i++) {
        mem_list_bit->unfix(cur_start + i);
    }
}

int Simulator::firstFitBit(int size) {
    int cur_pos = -1;
    int cur_size;
    for (size_t i = 0; i < mem_list_bit->size(); i++) {
        bool ok = false;
        cur_size = 0;
        while (mem_list_bit->value()[i] == '0') {
            cur_size++;
            if (cur_size == size) {
                ok = true;
                break;
            }
            i++;
            if (i >= mem_list_bit->value().size()) 
                break;
        }
        if (ok) {
            cur_pos = i;
            break;
        }
    }
    return cur_pos;
}

int Simulator::nextFitBit(int size) {
    int cur_pos = -1;
    int cur_size;
    for (size_t i = 0; i < mem_list_bit->size(); i++) {
        if (i == last_allocation_start)
            next_fit_pointer = i;
    }

    int i = 0;
    while (true) {
        if (next_fit_pointer >= mem_list_bit->size())
            next_fit_pointer = 0;

        bool ok = false;
        cur_size = 0;
        while (mem_list_bit->value()[i] == '0') {
            cur_size++;
            if (cur_size == size) {
                ok = true;
                break;
            }
            i++;
            if (i >= mem_list_bit->value().size()) 
                break;
        }
        if (ok) {
            cur_pos = i;
            break;
        }
        i++;
        next_fit_pointer++;
    }
    return cur_pos;
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
    cout << endl;
    for (int i = 0; i < qtd_requests; i++) {
        string request = requests[i];
        stringstream ss(request);

        string type;
        int size, id;

        ss >> type;
        if (type == "A") {
            ss >> size >> id;
            int num_block = (size+1)/block_size;
            alloc(num_block, id);
        } else {
            ss >> id;
            del(id);
        }
        printDll();
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
    for (int i = 0; i < qtd_requests; i++) {
        cout << start[i] << ", ";
    }
    cout << endl;
}
