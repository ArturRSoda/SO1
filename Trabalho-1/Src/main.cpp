#include <bits/stdc++.h>

using namespace std;


struct parameters {
    int data;
    int duracao;
    int prioridade;
};

class Process {
 public:
    Process(int id_, int data_, int duracao_, int prioridade_) {
        id = id_;
        data = data_;
        duracao = duracao_;
        prioridade = prioridade_;
    }

 //private:
    int id;
    int data;
    int duracao;
    int prioridade;
    int estado;
    int tempo_executado;
};

class Processor {
 public:
    Processor(vector<parameters*> pams) {
        lerParametros(pams);
    }

    ~Processor() {
        for (auto p : processos)
            delete p;
    }

    void iniciar() {

    }

    vector<Process*> processos;
 private:
    void lerParametros(vector<parameters*> pams) {
        for (auto p : pams) {
            Process* process = new Process(id_count++, p->data, p->duracao, p->prioridade);
            processos.push_back(process);
        }
    }
    int id_count = 0;
    int processo_ativo;
};

class File {
 public:
    File(string file_name) {
        file.open(file_name);
    }

    ~File() {
        for (auto v : pams)
            delete v;
    }

    vector<parameters*> readFile() {
        int a, b, c;
        while (file >> a >> b >> c) {
            parameters* p = new parameters;
            p->data = a;
            p->duracao = b;
            p->prioridade = c;
            pams.push_back(p);
        }
        return pams;
    }

    vector<parameters*> pams;
    fstream file;
};


int main() {
    File file = File("./Entradas/entrada.txt");
    vector<parameters*> pam = file.readFile();
    for (auto p : pam)
        cout << p->data << p->duracao << p->prioridade << '\n';

    Processor processor = Processor(pam);
    for (auto p : processor.processos)
        cout << p->id << p->data << p->duracao << p->prioridade << '\n';
}
