#include "simulator.h"

// Carrega parametros, inicia variaveis e gerenciadores de memoria
Simulator::Simulator() {
    FileReader fr = FileReader();
    fr.readFile();
    parameters* p = fr.getParameters();
    loadParameters(p);

    bytes_allocated = 0;
    bytes_deleted = 0;
    qty_allocation = 0;
    qty_deletion = 0;

    start = new int[REQUEST_SIZE];
    size = new int[REQUEST_SIZE];

    int qty_block = mem_size/block_size;
    qty_block += (mem_size % block_size == 0) ? 0 : 1;

    if (memory_manager == 1) manager = new BitmapManager(qty_block, block_size);
    else                     manager = new DllManager(qty_block, block_size);
}

// Desconstrutor
Simulator::~Simulator() {
    delete size;
    delete start;
}

// Carrega parametros
void Simulator::loadParameters(parameters* p) {
    memory_manager = p->manager;
    alloc_alg = p->alloc_alg;
    mem_size = p->mem_size;
    block_size = p->block_size;
    qty_requests = p->qty_requests;

    for (int i = 0; i < qty_requests; i++) {
        requests[i] = p->requests[i];
    }
}


void Simulator::alloc(int size_, int id) {
    int seg_start = manager->alloc(size_, alloc_alg);

    if (seg_start != -1) {
        bytes_allocated += size_;
        qty_allocation += 1;
        size[id] = size_;
        start[id] = seg_start;
    }
}

void Simulator::del(int id) {
    bytes_deleted += size[id];
    qty_deletion += 1;
    manager->del(start[id], size[id]);
}

void Simulator::run() {
    cout << endl;
    for (int i = 0; i < qty_requests; i++) {
        string request = requests[i];
        stringstream ss(request);

        string type;
        int size_, id, qty_block;

        ss >> type;
        if (type == "A") {
            ss >> size_ >> id;
            qty_block = size_/block_size;
            qty_block += (size_ % block_size == 0) ? 0 : 1;
            alloc(qty_block, id);

        } else {
            ss >> id;
            del(id);
        }
        cout << " -> " << type << " " << (type == "A" ? to_string(size_)+" " : "") << id << endl;
        manager->printState();
        cout << endl;
    }

    cout << endl;
    cout << "-=-=-=-=-=-=-=-=-=-=-=- Impressoes finais -=-=-=-=-=-=-=-=-=-=-=- " << endl;
    cout << endl;

    cout << "Estado Final do Gerenciador de Memoria:" << endl;
    manager->printState();
    cout << endl;
    printFinalState();
    cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- " << endl;
}

void  Simulator::printFinalState() {
    cout << "Quantidade de bytes em uso (ocupados):" << endl;
    cout << " -> " << manager->getQtyAllocatedBlocks()*block_size << " Bytes." << endl << endl;

    cout << "Quantidade de bytes alocados:" << endl;
    cout << " -> " << bytes_allocated << " Bytes." << endl << endl;

    cout << "Quantidade de bytes desalocado:" << endl;
    cout << " -> " << bytes_deleted << " Bytes." << endl << endl;

    cout << "Numero de alocacoes:" << endl;
    cout << " -> " << qty_allocation << " Alocacoes." << endl << endl;

    cout << "Numero de desalocacoes:" << endl;
    cout << " -> " << qty_deletion << " Desalocacoes." << endl;

}

void Simulator::printStart() {
    for (int i = 0; i < qty_requests; i++) {
        cout << start[i] << ", ";
    }
    cout << endl;
}
