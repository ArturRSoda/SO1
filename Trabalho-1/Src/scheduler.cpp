#include "scheduler.h"

Scheduler::Scheduler(vector<Process*> process_list) {
    for (auto p : process_list) {
        process_queue.push(p->id);
    }
}

int Scheduler::schedule(vector<Process*> process_list) {
    int active_id = process_queue.front();
    process_queue.pop();
    Process* active_process = process_list[active_id];
    if (active_process->total_executed_time < active_process->duration)
        process_queue.push(active_id);
    return active_id;
}
