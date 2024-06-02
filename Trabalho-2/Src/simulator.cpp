#include "simulator.h"

Simulator::Simulator(string file_name) {
    FileReader fr = FileReader(file_name);
    parameters* p = fr.readFile();
    loadParameters(p);

    element e = {0, (mem_size+1)/block_size, 0};
    mem_list_dll.push_back(e);
    mem_list_bit = new Bitset((mem_size+1)/block_size);
}

Simulator::~Simulator() {
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
    if (alloc_alg == 1) cur_start = firstFitDll();
    else                cur_start = nextFitDll();
    start[id] = cur_start;
    mem_list_dll.insert_sorted({cur_start, size, 1});
    int cur;
    for (int i = 0; i < mem_list_dll.size(); i++) {
        if (mem_list_dll.at(i).start == cur_start)
            cur = i;
    }
    auto cur_seg = mem_list_dll.at(cur);
    auto next_seg = mem_list_dll.at(cur+1);
    mem_list_dll.remove(next_seg);
    // next - cur
    if (next_seg.size-cur_seg.size) {
        element e = {cur_start+size, next_seg.size-cur_seg.size, 0};
        mem_list_dll.insert(e, cur+1);
    }
}

//todo
void Simulator::delDll(int id) {
    int cur_start = start[id];
    auto cur = mem_list_dll.at(cur_start);
    mem_list_dll.remove(cur);
}

int Simulator::firstFitDll(int size) {
    int cur = -1;
    for (int i = 0; i < mem_list_dll.size(); i++) {
        if (mem_list_dll.at(i).size >= size)
            cur = mem_list_dll.at(i).start;
    }
}

//mesmo do de cima mas colocar atributo que salva onde estava procurando
//e comecar i = atributo ate o final+atributo para rodar a lista inteira
int Simulator::nextFitDll(int size) {
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
    }
    // print etc
    //
}
