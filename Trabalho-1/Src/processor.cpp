#include "processor.h"
#include <ctime>

// Construtor
Processor::Processor(string file_name) {
    FileReader fr = FileReader(file_name);
    vector<parameters*> pams = fr.readFile();
    loadProcess(pams);

    scheduler = new Scheduler(process_list);
    context = vector<int>(9);
    context_table = vector<vector<int>>(process_list.size(), vector<int>(9));
}

// Destrutor
Processor::~Processor() {
    for (auto p : process_list)
        delete p;
    delete scheduler;
}

// Carrega a lista de processos
void Processor::loadProcess(vector<parameters*> pams) {
    for (auto p : pams) {
        Process* process = new Process(id_count++, p->data, p->duracao,
                                       p->prioridade);
        process_list.push_back(process);
    }
}

// Simula mudanca de contexto
void Processor::changeContext() {
    context_table[old_process] = context;
    context = context_table[active_process];
}

// Atualiza os atributos do processo em ativo
void Processor::updateActiveProcess(int active_process) {
    process_list[active_process]->status = "running";
    process_list[active_process]->current_executed_time++;
    process_list[active_process]->total_executed_time++;
}

// Confere o start date
// Ou seja, confere se o processo pode passar de "created" para "ready"
void Processor::checkStartDate() {
    for (auto p : process_list) 
        if (time_counter == p->start_date)
            p->status = "ready";
}

// Confere se o processo ja terminou a execucao
// alcancou o dead-line ou terminou o tempo de computacao
void Processor::checkTerminated() {
    for (auto p : process_list) {
        if ((time_counter > p->end_date) || (p->total_executed_time >= p->duration)) {
            p->status = "terminated";
        }
    }
}

// Imprime os status na execucao
void Processor::printStatus() {
    printf("%7d-%-2d ", time_counter, time_counter+1);
    for (auto p : process_list) {
        if (p->status == "ready") cout << "-- ";
        else if (p->status == "running") cout << "## ";
        else cout << "   ";
    }
    cout << '\n';
}

// Escolha de qual algorismo executar
void Processor::chooseAlgorithm() {
    int option;
    cout << endl;
    cout << " -=-=- Escolha o Algoritmo de Escalonamento -=-=-" << endl;
    cout << "      1 - First Come, First Served (FCFS)" << endl;
    cout << "      2 - Escalonamento por prioridade estatica" << endl;
    cout << " -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-" << endl;
    cout << "R: ";
    cin >> option;
    cout << endl;

    if ((option < 1) || (option > 3)) {
        cout << "Error: Opcao incorreta" << endl;
        exit(0);
    }

    scheduler->defineAlgorithm(option);
}

// Execucao
void Processor::run() {
    // 1. Escolhe o algorismo
    chooseAlgorithm();

    // 2. Imprime o cabecario
    cout << " -=-=-=-=-=-=-=-=-=- Execucao -=-=-=-=-=-=-=-=-=-" << endl;
    cout << "     TEMPO ";
    for (size_t i = 0; i < process_list.size(); i++)
        cout << 'P' << i+1 << ' ';
    cout << '\n';
    
    // 3. Comeca o looping de execucao
    //    So para quando todos processos estiverem em "terminated"
    while (1) {
        // 5. Confere se deu 1 sec
        if (difftime(time(0), time_val) >= 1) {
            // 6. Cofere o startdate e se terminou a execucao
            checkStartDate();
            checkTerminated();

            // 7. Caso todos os processos estiverem em "terminated" 
            //       -> Para a execucao
            bool flag = true;
            for (auto p : process_list)
                if (p->status != "terminated") flag = false;
            if (flag) break;

            // 8. Escalona
            old_process = scheduler->getActiveProcess();
            scheduler->schedule(process_list);
            active_process = scheduler->getActiveProcess();

            // 9. Atualiza os atributos do processo em ativo
            updateActiveProcess(active_process);

            // 10. Caso houver a troca de processo em ativa
            //        -> Atualiza os atributos do processo antigo
            if (old_process != active_process) {
                process_list[old_process]->current_executed_time = 0;
                if (not (process_list[old_process]->status == "terminated"))
                    process_list[old_process]->status = "ready";
            }

            // 11. Simula a mudanca de contexto
            changeContext();
            // 12. Imprime os Status
            printStatus();

            // 13. Atualiza os indicadores de tempo
            time_counter++;
            time_val = time(0);
        }
    }
    cout << " -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
}
