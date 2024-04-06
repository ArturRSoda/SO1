# ifndef processorH
# define processorH

#include "fileReader.h"
#include "process.h"

class INE5412 {
 public:
    INE5412(vector<parameters*> pams);

    ~INE5412();

    vector<Process*> processos;

 private:
    void lerParametros(vector<parameters*> pams);
    int id_count = 0;
    int processo_ativo;
};

#endif
