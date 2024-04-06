#include "processor.h"

INE5412::INE5412(vector<parameters*> pams) {
    lerParametros(pams);
}

INE5412::~INE5412() {
    for (auto p : processos)
    delete p;
}

void INE5412::lerParametros(vector<parameters*> pams) {
    for (auto p : pams) {
        Process* process = new Process(id_count++, p->data, p->duracao, p->prioridade);
        processos.push_back(process);
    }
}


