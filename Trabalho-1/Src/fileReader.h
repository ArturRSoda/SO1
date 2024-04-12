#ifndef fileReaderH
#define fileReaderH

#include <bits/stdc++.h>

struct parameters {
    int data;
    int duracao;
    int periodo;
    int deadline;
    int prioridade;
};

using namespace std;

class FileReader {
 public:
    FileReader(string file_name);
    ~FileReader();

    vector<parameters*> readFile();

 private:
    vector<parameters*> pams;
    fstream file;
};


#endif
