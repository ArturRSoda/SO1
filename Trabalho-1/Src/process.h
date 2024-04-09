#ifndef processH
#define processH

#include <chrono>
#include <string>
#include <vector>
#include <random>

using namespace std;

class Process {
 public:
    Process(int id_, int data_, int duracao_, int prioridade_);

    int getId();

    void generateContext();

 //private:
    int id;
    int start_date;
    int end_date;
    int deadline;
    int duration;
    int priority;
    string status;
    int current_executed_time;
    int total_executed_time;
    int wait_time;

    uint64_t SP;
    uint64_t PC;
    uint64_t ST;
    vector<uint64_t> REG;
};

#endif
