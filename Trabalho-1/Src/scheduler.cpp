#include "scheduler.h"

// Construtor
Scheduler::Scheduler(vector<Process*> process_list) {
    for (auto p : process_list) {
        processID_vector.push_back(p->getId());
    }
}

// chama a funcado de escalonamento conforme o algoritmo ja escolhido
void Scheduler::schedule(vector<Process*> process_list_) {
    process_list = process_list_;

    switch (algorithm) {
        case 1:
            sort(&Scheduler::compareByPriority);
            break;
        case 2:
            sort(&Scheduler::compareByAbsDeadline);
            break;
        default:
            printf("Error: algaritmo invalido! \n");
            exit(0);
    }
}

// Define o algoritmo a ser executado
void Scheduler::defineAlgorithm(int a) {
    algorithm = a;
}

// Retorna o processo em ativo
int Scheduler::getActiveProcess() {
    return processID_vector[0];
}

// Ordena o vetor "processID_vector" de acordo com a funcao de comparacao passada
// (bobble sort)
void Scheduler::sortVector(bool (Scheduler::*func)(int, int)) {
    if (not processID_vector.size()) return;
    bool swapped;
    for (size_t i = 0; i < processID_vector.size()-1; i++) {
        swapped = false;
        for (size_t j = 0; j < processID_vector.size()-i-1; j++) {
            if ((this->*func)(j, j+1)) {
                swap(processID_vector[j], processID_vector[j+1]);
                swapped = true;
            }
        }
        if (not swapped)
            break;
    }
}

// Compara os deadlines absolutos
// Para passar a funcao "Scheduler::sort"
bool Scheduler::compareByAbsDeadline(int a, int b) {
    return process_list[processID_vector[a]]->getAbsDeadline() > process_list[processID_vector[b]]->getAbsDeadline();
}

// Compara prioridade
// Para passar a funcao "Scheduler::sort"
bool Scheduler::compareByPriority(int a, int b) {
    return process_list[processID_vector[a]]->getPriority() < process_list[processID_vector[b]]->getPriority();
}

// Escalona
void Scheduler::sort(bool (Scheduler::*func)(int, int)) {
    vector<int> temp = {}; // vetor para os que estao prontos ou executando
    
    for (auto p : process_list) {
        if ((p->getStatus() != "terminated") && (p->getStatus() != "created"))
            temp.push_back(p->getId());
    }

    processID_vector = temp;

    sortVector(func);
}


