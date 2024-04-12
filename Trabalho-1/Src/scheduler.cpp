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
            sort(&Scheduler::compareByStartDate);
            break;
        case 2:
            sort(&Scheduler::compareByPriority);
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
void Scheduler::sortVector(bool (Scheduler::*func)(int, int)) {
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

// Compara start date
// Para passar a funcao "Scheduler::sort"
bool Scheduler::compareByStartDate(int a, int b) {
    return process_list[processID_vector[a]]->getStartDate() > process_list[processID_vector[b]]->getStartDate();
}

// Compara prioridade
// Para passar a funcao "Scheduler::sort"
bool Scheduler::compareByPriority(int a, int b) {
    return process_list[processID_vector[a]]->getPriority() < process_list[processID_vector[b]]->getPriority();
}

// Escalona por "first come, first served"
void Scheduler::sort(bool (Scheduler::*func)(int, int)) {
    Process* process;                      // Processo a ser analizado
    vector<int> statusCreatedProcess = {}; // vetor para os processos que ainda nao ficaram em ready
    bool popped;                           // Para verificar se ainda ha algum processo em "terminated" ou "created"

    // 1. Looping que roda enquando houver processo em "terminated" ou "created"
    while (1) {
        popped = false;
        // 2. Passa por todos os processos referentes ao ID de "processID_vector"
        for (size_t i = 0; i < processID_vector.size(); i++) {
            process = process_list[processID_vector[i]];

            // 3.1. Tira do vetor caso estiver "terminated"
            if (process->getStatus() == "terminated") {
                processID_vector.erase(processID_vector.begin() + i);
                popped = true;
                break;
            }
            // 3.2. Tira do vertor caso estiver em "created",
            //      e Adiciona nos vetores dos processos que ainda estao em "created"
            else if (process->getStatus() == "created") {
                statusCreatedProcess.push_back(process->getId());
                processID_vector.erase(processID_vector.begin() + i);
                popped = true;
                break;
            }
        }
        // 4. Para o looping caso nao houver nenhum processo em "terminated" ou "created"
        if (not popped)
            break;
    }
    // 5. Ordena o "processID_vector" pelo start date,
    //    sem os processos que estao no estado "terminated" ou "created"   
    sortVector(func);

    // 6. Concatena os processos que ainda estao em "created" no final de "processID_vector"
    processID_vector.insert(processID_vector.end(), statusCreatedProcess.begin(), statusCreatedProcess.end());
}


