#include "process.h"

Process::Process(int id_, int start_date_, int duration_, int priority_) {
    id = id_;
    start_date = start_date_;
    deadline = 10000000; // TODO: Mudar dps
    duration = duration_;
    priority = priority_;
    status = "created";
    current_executed_time = 0;
    total_executed_time = 0;
    wait_time = 0;
    generateContext();
}

void Process::generateContext() {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> dis;

    SP = dis(gen);
    PC = dis(gen);
    ST = dis(gen);
    for (int i = 0; i < 6; i++)
        REG.push_back(dis(gen));
}

