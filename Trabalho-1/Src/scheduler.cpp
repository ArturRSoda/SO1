#include "scheduler.h"

// Construtor
Scheduler::Scheduler(vector<Process*> process_list) {
    for (auto p : process_list) {
        processID_vector.push_back(p->id);
    }
}

// chama a funcado de escalonamento conforme o algoritmo ja escolhido
void Scheduler::schedule(vector<Process*> process_list_) {
    process_list = process_list_;

    switch (algorithm) {
        case 1:
            return FCFS();
            break;
        case 2:
            return staticPrioritySchedule();
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
void Scheduler::sort(bool (Scheduler::*func)(int, int)) {
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
    return process_list[processID_vector[a]]->start_date > process_list[processID_vector[b]]->start_date;
}

// Compara prioridade
// Para passar a funcao "Scheduler::sort"
bool Scheduler::compareByPriority(int a, int b) {
    return process_list[processID_vector[a]]->priority < process_list[processID_vector[b]]->priority;
}

// Escalona por "first come, first served"
void Scheduler::FCFS() {
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
            if (process->status == "terminated") {
                processID_vector.erase(processID_vector.begin() + i);
                popped = true;
                break;
            }
            // 3.2. Tira do vertor caso estiver em "created",
            //      e Adiciona nos vetores dos processos que ainda estao em "created"
            else if (process->status == "created") {
                statusCreatedProcess.push_back(process->id);
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
    sort(&Scheduler::compareByStartDate);

    // 6. Concatena os processos que ainda estao em "created" no final de "processID_vector"
    processID_vector.insert(processID_vector.end(), statusCreatedProcess.begin(), statusCreatedProcess.end());
}


// Escalona por prioridade estatica
// Mesmos passos a passos de SCFS
// A diferenca eh que para a funcao "Scheduler::sort" eh passado a funcao "Scheduler::compareByPriority"
void Scheduler::staticPrioritySchedule() {
    Process* process;    
    vector<int> statusCreatedProcess = {};
    bool popped;
    while (1) {
        popped = false;
        for (size_t i = 0; i < processID_vector.size(); i++) {
            process = process_list[processID_vector[i]];

            if (process->status == "terminated") {
                processID_vector.erase(processID_vector.begin() + i);
                popped = true;
                break;
            }
            else if (process->status == "created") {
                statusCreatedProcess.push_back(process->id);
                processID_vector.erase(processID_vector.begin() + i);
                popped = true;
                break;
            }
        }
        if (not popped)
            break;
    }
    sort(&Scheduler::compareByPriority);
    processID_vector.insert(processID_vector.end(), statusCreatedProcess.begin(), statusCreatedProcess.end());
}

