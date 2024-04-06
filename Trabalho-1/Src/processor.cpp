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
}

void Processor::changeStatus(int old_process, int active_process) {
    Process* old_pr = process_list[old_process];
    Process* pr = process_list[active_process];
    old_pr->total_executed_time += old_pr->current_executed_time;
    old_pr->current_executed_time = 0;
    old_pr->status = old_pr->total_executed_time >= old_pr->duration ? "terminated" : "ready";
    pr->status = pr->total_executed_time >= pr->duration ? "terminated" : "running";
}

void Processor::printStatus() {
    cout << time_counter << '-' << time_counter+1 << "  ";
    for (auto p : process_list) {
        if (p->status == "ready") cout << "--" << ' ';
        else if (p->status == "running") cout << "##" << ' ';
        else cout << "  ";
    }
    cout << '\n';
}

void Processor::run() {
    cout << "tempo" << ' ';
    for (size_t i = 0; i < process_list.size(); i++)
        cout << 'P' << i+1 << ' ';
    cout << '\n';
    
    double dif;
    time_val = time(0);
    active_process = scheduler->schedule(process_list);
    old_process = active_process;
    changeStatus(old_process, active_process);
    printStatus();
    while (true) {
        dif = difftime(time(0), time_val);
        time_val = time(0);
        if (dif >= 1) {
            process_list[active_process]->current_executed_time++;
            active_process = scheduler->schedule(process_list);
            changeStatus(old_process, active_process);
            changeContext();
            old_process = active_process;

            bool flag = true;
            for (auto p : process_list)
                if (p->status != "terminated") flag = false;
            if (flag) break;

            time_counter++;
            printStatus();
        }
    }
}
