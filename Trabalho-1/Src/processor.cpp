#include "processor.h"

Processor::Processor(string file_name) {
    FileReader fr = FileReader(file_name);
    vector<parameters*> pams = fr.readFile();
    loadProcess(pams);

    scheduler = new Scheduler(process_list);
    context = vector<int>(9);
    context_table = vector<vector<int>>(process_list.size(), vector<int>(9));
}

Processor::~Processor() {
    for (auto p : process_list)
        delete p;
    delete scheduler;
}

void Processor::loadProcess(vector<parameters*> pams) {
    for (auto p : pams) {
        Process* process = new Process(id_count++, p->data, p->duracao,
                                       p->prioridade);
        process_list.push_back(process);
    }
}

void Processor::changeContext() {
    context_table[old_process] = context;
    context = context_table[active_process];
    old_process = active_process;
}

void Processor::printStatus() {
    cout << "print" << '\n';
}

void Processor::run() {
    chrono::duration<double> dif;
    time = chrono::system_clock::now();
    while (true) {
        dif = chrono::system_clock::now() - time;
        time = chrono::system_clock::now();
        if (dif >= (chrono::duration<double>)1) {
            process_list[active_process]->current_executed_time++;

            active_process = scheduler->schedule(process_list);
            if (old_process != active_process) {
                Process* process = process_list[old_process];
                process->total_executed_time += process->current_executed_time;
                process->current_executed_time = 0;
                changeContext();
            }
            printStatus();
        }
        bool flag = true;
        for (auto p : process_list)
            if (p->status != "terminated") flag = false;
        if (flag) break;
    }
}
