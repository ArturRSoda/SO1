#ifndef processH
#define processH

#include <chrono>
#include <string>

using namespace std;

class Process {
 public:
    Process(int id_, int data_, int duracao_, int prioridade_);

    int getId();

 //private:
    int id;
    int start_date;
    int end_date;
    int duration;
    int priority;
    string status;
    int current_executed_time;
    int total_executed_time;
    int wait_time;
};

#endif
