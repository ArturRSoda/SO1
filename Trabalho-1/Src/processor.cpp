#include "processor.h"
#include <ctime>

// Construtor
Processor::Processor(string file_name) {
    FileReader fr = FileReader(file_name);
    vector<parameters*> pams = fr.readFile();
    loadProcess(pams);

    scheduler = new Scheduler(process_list);
    SP = 0;
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
                                       p->periodo, p->deadline, p->prioridade);
        process_list.push_back(process);
    }
}

// Simula mudanca de contexto
void Processor::changeContext(int old_process, int active_process) {
    // Se nao for a primeira iteracao
    if (SP) {
        // Carraga contexto
        process_list[old_process]->setSP(SP);
        process_list[old_process]->setPC(PC);
        process_list[old_process]->setST(ST);
        process_list[old_process]->setREG(REG);
    }

    // Salva o contexto no processador
    SP = process_list[active_process]->getSP();
    PC = process_list[active_process]->getPC();
    ST = process_list[active_process]->getST();
    REG = process_list[active_process]->getREG();
}

void Processor::printContext() {
    cout << "SP=" << SP << " PC=" << PC << " ST=" << ST << " ";
    for (size_t i = 0; i < REG.size(); i++)
        cout << "R" << i << "=" << REG[i];
    cout << endl;
}

// Atualiza os atributos do processo em ativo
void Processor::updateActiveProcess(int active_process) {
    int temp;
    process_list[active_process]->setStatus("running");
    temp = process_list[active_process]->getCurrentExecutedTime();
    process_list[active_process]->setCurrentExecutedTime(temp+1);
    temp = process_list[active_process]->getTotalExecutedTime();
    process_list[active_process]->setTotalExecutedTime(temp+1);
}

// Confere o start date
// Ou seja, confere se o processo pode passar de "created" para "ready"
void Processor::checkStartDate() {
    for (auto p : process_list) 
        if (time_counter == p->getStartDate())
            p->setStatus("ready");
}

// Confere se o processo ja terminou a execucao
// alcancou o dead-line ou terminou o tempo de computacao
void Processor::checkTerminated() {
    for (auto p : process_list) {
        if (p->getStatus() != "terminated")
            if ((time_counter > p->getDeadline()) || (p->getTotalExecutedTime() >= p->getDuration())) {
                p->setStatus("terminated");
                p->setEndDate(time_counter);
            }
    }
}

// Imprime os status na execucao
void Processor::printStatus() {
    printf("%7d-%-2d ", time_counter, time_counter+1);
    for (auto p : process_list) {
        if (p->getStatus() == "ready") cout << "-- ";
        else if (p->getStatus() == "running") cout << "## ";
        else cout << "   ";
    }
    cout << '\n';
}

void Processor::printTimes() { 
    cout << " -=-=-=-=--=-=-=-=-=- Times -=-=-=-=-=-=-=-=-=-=-" << endl;
    cout << "    -> Turn Around Time:" << endl;
    for (auto p : process_list) {
        cout << "         P" << p->getId() << " = " << (p->getEndDate() - p->getStartDate()) << endl;
    }

    cout << endl;

    cout << "    -> Tempo medio de espera:" << endl;
    for (auto p : process_list) {
        cout << "         P" << p->getId() << " = " << p->getWaitTime() << endl;
    }

    cout << endl;
    cout << "    -> Numero total de trocas de contexto" << endl;
    cout << "         " << preemption_counter << endl;
    cout << " -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-" << endl;
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
                if (p->getStatus() != "terminated") flag = false;
            if (flag) break;

            // 8. Escalona
            old_process = scheduler->getActiveProcess();
            scheduler->schedule(process_list);
            active_process = scheduler->getActiveProcess();

            // 9. Atualiza os atributos do processo em ativo
            updateActiveProcess(active_process);

            int temp;
            for (auto p : process_list) {
                if (p->getStatus() == "ready") {
                    temp = p->getWaitTime();
                    p->setWaitTime(temp+1);
                }
            }

            // 10. Caso houver a troca de processo em ativa
            //        -> Atualiza os atributos do processo antigo
            if (old_process != active_process) {
                preemption_counter++;
                process_list[old_process]->setCurrentExecutedTime(0);
                if (not (process_list[old_process]->getStatus() == "terminated"))
                    process_list[old_process]->setStatus("ready");
            }

            // 11. Simula a mudanca de contexto
            changeContext(old_process, active_process);

    
            //printContext(); Para textar o gerenciamento de contexto

            // 12. Imprime os Status
            printStatus();

            // 13. Atualiza os indicadores de tempo
            time_counter++;
            time_val = time(0);
        }
    }
    cout << " -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << endl;
    printTimes();
}
