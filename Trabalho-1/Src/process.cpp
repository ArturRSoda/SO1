#include "process.h"

Process::Process(int id_, int start_date_, int duration_, int priority_) {
    id = id_;
    start_date = start_date_;
    duration = duration_;
    priority = priority_;
    status = "created";
    current_executed_time = 0;
    total_executed_time = 0;
    wait_time = 0;
}

