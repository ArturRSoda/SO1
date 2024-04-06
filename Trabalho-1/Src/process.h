#ifndef processH
#define processH

class Process {
 public:
    Process(int id_, int data_, int duracao_, int prioridade_);

 //private:
    int id;
    int data;
    int duracao;
    int prioridade;
    int estado;
    int tempo_executado;
};

#endif
