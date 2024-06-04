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
    bytes_allocated = 0;
    bytes_deleted = 0;
    qtd_allocation = 0;
    qtd_deletion = 0;

    int qtd_block = mem_size/block_size;
    qtd_block += (mem_size % block_size == 0) ? 0 : 1;

    element e = {0, qtd_block, 0};
    mem_list_dll.push_back(e);
    start = new int[REQUEST_SIZE];
    size = new int[REQUEST_SIZE];
    mem_list_bit = new Bitset(qtd_block);

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

void Simulator::allocDll(int size_, int id) {
    int cur_start;
    if (alloc_alg == 1) cur_start = firstFitDll(size_);
    else                cur_start = nextFitDll(size_);
    if (cur_start == -1) return;

    bytes_allocated += size_;
    qtd_allocation += 1;

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

    size[id] = size_;
    start[id] = cur_start;
    element allocated_seg = {cur_start, size_, 1};
    mem_list_dll.insert_sorted(allocated_seg);

    if (old_seg.size - allocated_seg.size) {
        element free_seg = {cur_start+size_, old_seg.size-allocated_seg.size, 0};
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
    int cur = -1;
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
    int cur = -1;
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
        if (next_fit_pointer >= mem_list_dll.size())
            break;

    }
    return cur;
}

void Simulator::allocBit(int size_, int id) {
    int cur_start;
    if (alloc_alg == 1) cur_start = firstFitBit(size_);
    else                cur_start = nextFitBit(size_);
    if (cur_start == -1) return;

    bytes_allocated += size_;
    qtd_allocation += 1;

    last_allocation_start = cur_start + size_;
    start[id] = cur_start;
    size[id] = size_;
    for (int i = 0; i < size_; i++) {
        mem_list_bit->fix(cur_start + i);
    }
}

void Simulator::delBit(int id) {
    int cur_start = start[id]; 
    int cur_size = size[id];

    for (int i = 0; i < cur_size; i++) {
        mem_list_bit->unfix(cur_start + i);
    }
}

int Simulator::firstFitBit(int size) {
    int cur_size;
    bool flag;
    int j;

    int cur_pos = -1;
    for (int i = 0; i < mem_list_bit->size(); i++) {
        flag = false;
        cur_size = 0;
        j = i;

        while (mem_list_bit->value()[j] == '0') {
            cur_size++;

            if (cur_size >= size) {
                flag = true;
                break;
            }

            j++;

            if (j >= static_cast<int>(mem_list_bit->value().size()))
                break;
        }

        if (flag) {
            cur_pos = i;
            break;
        }

    }
    return cur_pos;
}

int Simulator::nextFitBit(int size) {
    int cur_pos = -1;
    int cur_size;
    for (int i = 0; i < mem_list_bit->size(); i++) {
        if (i == last_allocation_start)
            next_fit_pointer = i;
    }

    int j;
    bool flag;
    while (true) {
        if (static_cast<int>(next_fit_pointer) >= mem_list_bit->size())
            next_fit_pointer = 0;

        flag = false;
        cur_size = 0;

        j = next_fit_pointer;
        while (mem_list_bit->value()[j] == '0') {
            cur_size++;

            if (cur_size >= size) {
                flag = true;
                break;
            }

            j++;
            if (j >= static_cast<int>(mem_list_bit->value().size()))
                break;
        }

        if (flag) {
            cur_pos = next_fit_pointer;
            break;
        }

        next_fit_pointer++;
        if (static_cast<int>(next_fit_pointer) >= mem_list_bit->size())
            break;
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
    cout << endl;
    for (int i = 0; i < qtd_requests; i++) {
        string request = requests[i];
        stringstream ss(request);

        string type;
        int size_, id, num_block;

        ss >> type;
        if (type == "A") {
            ss >> size_ >> id;

            num_block = size_/block_size;
            num_block += (size_ % block_size == 0) ? 0 : 1;

            alloc(num_block, id);
        } else {
            ss >> id;

            bytes_deleted += size[id];
            qtd_deletion += 1;

            del(id);
        }

    }
    printState();
    cout << endl;
    printFinalState();
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
    cout << "Estado final do gerenciador de memoria:" << endl;
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

void  Simulator::printFinalState() {
    cout << "Quantidade de bytes em uso (ocupados):" << endl;
    cout << " -> ";
    if (manager == 1) {
        cout << mem_list_bit->get_qtd_one()*block_size;
    } else {
        int qtd_blocks = 0;
        for (size_t i = 0; i < mem_list_dll.size(); i++) {
            if (mem_list_dll.at(i).status == 1)
                qtd_blocks += mem_list_dll.at(i).size;
        }
        cout << qtd_blocks;
    }
    cout << " Bytes." << endl << endl;

    cout << "Quantidade de bytes alocados:" << endl;
    cout << " -> " << bytes_allocated << " Bytes." << endl << endl;

    cout << "Quantidade de bytes desalocado:" << endl;
    cout << " -> " << bytes_deleted << " Bytes." << endl << endl;

    cout << "Numero de alocacoes:" << endl;
    cout << " -> " << qtd_allocation << " Alocacoes." << endl << endl;

    cout << "Numero de desalocacoes:" << endl;
    cout << " -> " << qtd_deletion << " Desalocacoes." << endl;

}

void Simulator::printStart() {
    for (int i = 0; i < qtd_requests; i++) {
        cout << start[i] << ", ";
    }
    cout << endl;
}
