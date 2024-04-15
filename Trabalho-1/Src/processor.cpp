#include "processor.h"
#include "fileReader.h"
#include <ctime>
#include <ostream>

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
//           lista de numero de instancias dos processos
//           lista de deadlines perdidos
void Processor::loadProcess(vector<parameters*> pams) {
    for (auto p : pams) {
        Process* process = new Process(id_count++, p->data, p->duracao,
                                       p->periodo, p->deadline, p->prioridade);
        process_list.push_back(process);
        instances.push_back(0);
        deadline_loss.push_back(0);
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

// Printa os contextos que estao no processador
void Processor::printContext() {
    cout << "SP=" << SP << " PC=" << PC << " ST=" << ST << " ";
    for (size_t i = 0; i < REG.size(); i++)
        cout << "R" << i << "=" << REG[i];
    cout << endl;
}

// Atualiza os atributos do processo em ativo
void Processor::updateActiveProcess(int active_process) {
    process_list[active_process]->setStatus("running");
    int temp;
    temp = process_list[active_process]->getCurrentExecutedTime();
    process_list[active_process]->setCurrentExecutedTime(temp+1);
    temp = process_list[active_process]->getTotalExecutedTime();
    process_list[active_process]->setTotalExecutedTime(temp+1);
}

// Confere o start date
// Ou seja, confere se o processo pode passar de "created" para "ready"
void Processor::checkStartDate() {
    for (auto p : process_list) 
        if (time_counter == p->getStartDate()) {
            p->setStatus("ready");
    }
}

// confere se ja alcancou o tempo de computacao
void Processor::checkComputionTime() {
    for (auto p : process_list) {
        if (p->getStatus() != "terminated")
            if (p->getCurrentExecutedTime() == p->getDuration()) {
                p->setCurrentExecutedTime(0);
                p->setStatus("terminated");
                p->setEndDate(time_counter);
                vector<int> new_turn_around_time = p->getTurnAroundTime();
                new_turn_around_time.push_back(abs(time_counter - p->getStartDate()));
                p->setTurnAroundTime(new_turn_around_time);
                instances[p->getId()]++;
            }
    }
}

// Confere o Deadline de todos
void Processor::checkDeadline() {
    for (auto p : process_list) {
        // 1. Executa se for deadline
        if (time_counter == p->getDeadline()) {
            int id = p->getId();

            // 2. Verifica se foi deadline perdido
            if (p->getStatus() != "terminated") {
                deadline_loss[id]++;
                instances[id]++;
                vector<int> new_turn_around_time = p->getTurnAroundTime();
                new_turn_around_time.push_back(abs(time_counter - p->getStartDate()));
                p->setTurnAroundTime(new_turn_around_time);
            }

            // 3. Cria o processo novamente
            //    equanto nao alcancar o numero maximo de instancias
            if (instances[id] < max_instances) {
                // 3.1. Cria Nova instancia com as mesmas caracteristicas 
                Process* pr = process_list[id];
                process_list.erase(process_list.begin()+id);

                Process* new_process = new Process(id, time_counter, pr->getDuration(),
                                                   pr->getPeriod(), pr->getDeadline(), pr->getPriority());
                new_process->setOldDeadLine(pr->getDeadline());
                new_process->setTurnAroundTime(pr->getTurnAroundTime());
                new_process->setTotalExecutedTime(pr->getTotalExecutedTime());
                new_process->setCurrentExecutedTime(0);
                new_process->setWaitTime(pr->getWaitTime());
                new_process->setStatus("ready");

                delete pr;

                if (static_cast<size_t>(id) < process_list.size())
                    process_list.insert(process_list.begin()+id, new_process);
                else
                    process_list.push_back(new_process);
            }
        }
    }
}

// Atualiza o deadline absoluto de todos
void Processor::updateAbsDeadlines() {
    for (auto p : process_list)
        p->setAbsDeadLine(abs(p->getDeadline() - time_counter));
}

// Imprime os status na execucao
void Processor::printStatus() {
    printf("%7d-%-2d ", time_counter, time_counter+1);
    for (auto p : process_list) {
        if (p->getStatus() == "ready") cout << "-- ";
        else if (p->getStatus() == "running") cout << "## ";
        else cout << "   ";
    }

    cout << "| ";

    for (auto p : process_list) {
        if (time_counter == p->getOldDeadline())
            cout << "P" << p->getId()+1;
        else
            cout << "  ";
    }

    cout << "  | ";

    for (auto i : process_list) {
        if (instances[i->getId()] <= max_instances)
            printf("%-2d ", instances[i->getId()]);
        else
            printf("%-2d ", max_instances);
    }
    cout << endl;

}

void Processor::printTimes() { 
    cout << " -=-=-=-=--=-=-=-=-=- Times -=-=-=-=-=-=-=-=-=-=-" << endl;
    cout << "    -> Turn Around Time:" << endl;
    for (auto p : process_list) {
        cout << "         P" << p->getId()+1 << " = ";
        cout << "[";
        for (auto t : p->getTurnAroundTime())
             cout << t << ",";
        cout << "]" << endl;
    }

    cout << endl;

    cout << "    -> Tempo medio de espera:" << endl;
    for (auto p : process_list) {
        cout << "         P" << p->getId()+1 << " = " << p->getWaitTime() << endl;
    }

    cout << endl;
    cout << "    -> Numero total de trocas de contexto" << endl;
    cout << "         " << preemption_counter << endl;

    cout << endl;
    cout << "    -> Numero de Deadlines perdidos para cada Processo:" << endl;
    for (auto p : process_list) {
        cout << "         P" << p->getId()+1 << " = " << deadline_loss[p->getId()] << endl;
    }
    cout << " -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-" << endl;
}

// Escolha de qual algorismo executar
void Processor::chooseAlgorithm() {
    int option;
    cout << endl;
    cout << " -=-=- Escolha o Algoritmo de Escalonamento -=-=-" << endl;
    cout << "      1 - Rate Monotonic (RM)" << endl;
    cout << "      2 - Earliest Deadline First (EDF)" << endl;
    cout << " -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-" << endl;
    cout << "R: ";
    cin >> option;
    cout << endl;

    if ((option < 1) || (option > 2)) {
        cout << "Error: Opcao incorreta" << endl;
        exit(0);
    }

    scheduler->defineAlgorithm(option);
}

// Escolhe o numero de criacoes de cada processo
void Processor::chooseNumInstances() {
    cout << " -=-=- Quantas vezes o processo deve terminar sua execucao? -=-=-" << endl;
    cout << "R: ";
    cin >> max_instances;
}

// Execucao
void Processor::run() {
    // 0. Escolhe o numero maximo de instancias
    chooseNumInstances();
    // 1. Escolhe o algorismo
    chooseAlgorithm();

    // 2. Imprime o cabecario
    cout << " -=-=-=-=-=-=-=-=-=- Execucao -=-=-=-=-=-=-=-=-=-" << endl;
    cout << "                                     INSTANCIAS" << endl;
    cout << "     TEMPO ";
    for (size_t i = 0; i < process_list.size(); i++)
        cout << 'P' << i+1 << ' ';
    cout << "| ";
    cout << "DEADLINES ";
    cout << "| ";
    for (size_t i = 0; i < process_list.size(); i++)
        cout << 'P' << i+1 << ' ';
    cout << endl;
    
    // 3. Comeca o looping de execucao
    //    So para quando todos os processos terminarem de executar a quantidade desejada
    while (1) {
        // 5. Confere se deu 1 sec
        if (difftime(time(0), time_val) >= 1) {
            // 6. Cofere o startdate, tempo de execucao, e a deadline
            //    E atualiz todos os deadlines absolutos
            checkStartDate();
            checkComputionTime();
            checkDeadline();
            updateAbsDeadlines();

            // 7. Caso todos os precessos terminarem a execucao na quantidade desejada
            //       -> Para a execucao
            bool flag = true;
            for (auto v : instances) {
                if (v < max_instances) {
                    flag = false;
                }
            }
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
