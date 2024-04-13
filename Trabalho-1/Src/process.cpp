#include "process.h"

Process::Process(int id_, int start_date_, int duration_, int period_, int deadline_, int priority_) {
    id = id_;
    creation_date = start_date_;
    start_date = start_date_;
    duration = duration_;
    period = period_;
    old_deadline = deadline_;
    abs_deadline = abs(deadline_ - start_date_);
    deadline = deadline_;
    priority = priority_;
    status = "created";
    current_executed_time = 0;
    total_executed_time = 0;
    wait_time = 0;
    creation_date = start_date;
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

int Process::getId() {
    return id;
}

int Process::getCreationDate() {
    return creation_date;
}

int Process::getStartDate() {
    return start_date;
}

int Process::getEndDate() {
    return period;
}

int Process::getPeriod() {
    return period;
}

int Process::getOldDeadline() {
    return old_deadline;
}

int Process::getDeadline() {
    return deadline;
}

int Process::getAbsDeadline() {
    return abs_deadline;
}

int Process::getDuration() {
    return duration; 
}

int Process::getPriority() {
    return priority;
}

string Process::getStatus() {
    return status;
}

int Process::getCurrentExecutedTime() {
    return current_executed_time;
}

int Process::getTotalExecutedTime() {
    return total_executed_time;
}

int Process::getWaitTime() {
    return wait_time;
}

uint64_t Process::getSP() {
    return SP;
}

uint64_t Process::getPC() {
    return PC;
}

uint64_t Process::getST() {
    return ST;
}

vector<uint64_t> Process::getREG() {
    return REG;
}

void Process::setStartDate(int v) {
    start_date = v;
}

void Process::setEndDate(int v) {
    end_date = v;
}

void Process::setOldDeadLine(int v) {
    old_deadline = v;
}

void Process::setDeadLine(int v) {
    deadline = v;
}

void Process::setAbsDeadLine(int v) {
    abs_deadline = v;
}

void Process::setStatus(string v) {
    status = v;
}

void Process::setCurrentExecutedTime(int v) {
    current_executed_time = v;
}

void Process::setTotalExecutedTime(int v) {
    total_executed_time = v;
}

void Process::setWaitTime(int v) {
    wait_time = v;
}

void Process::setSP(uint64_t v) {
    SP = v;
}

void Process::setPC(uint64_t v) {
    PC = v;
}

void Process::setST(uint64_t v) {
    ST = v;
}

void Process::setREG(vector<uint64_t> v) {
    REG = v;
}

