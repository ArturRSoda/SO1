#ifndef processH
#define processH

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>
#include <random>

#include "fileReader.h"

using namespace std;

struct datas {
    int creation_date;
    int start_date;
    int duration;
    int period;
    int old_deadline;
    int deadline;
    int priority;
    int current_executed_time;
    int total_executed_time;
    int wait_time;
    int abs_deadline;
};

class Process {
 public:
    Process(int id_, int start_date_, int duration_, int period_, int deadline_, int prioridade_);
    void generateContext();
    datas* getDatas();

    int getId();
    int getCreationDate();
    int getStartDate();
    int getEndDate();
    int getPeriod();
    int getOldDeadline();
    int getDeadline();
    int getAbsDeadline();
    int getDuration();
    int getPriority();
    string getStatus();
    int getCurrentExecutedTime();
    int getTotalExecutedTime();
    int getWaitTime();
    uint64_t getSP();
    uint64_t getPC();
    uint64_t getST();
    vector<uint64_t> getREG();

    void setStartDate(int v);
    void setEndDate(int v);
    void setDeadLine(int v);
    void setOldDeadLine(int v);
    void setAbsDeadLine(int v);
    void setStatus(string v);
    void setCurrentExecutedTime(int v);
    void setTotalExecutedTime(int v);
    void setWaitTime(int v);
    void setSP(uint64_t v);
    void setPC(uint64_t v);
    void setST(uint64_t v);
    void setREG(vector<uint64_t> v);

 private:
    int id;
    int creation_date;
    int start_date;
    int end_date;
    int period;
    int old_deadline;
    int deadline;
    int abs_deadline;
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
