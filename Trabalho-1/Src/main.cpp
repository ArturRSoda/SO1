#include "processor.h"

int main() {
    Processor INE5412 = Processor("Src/Entradas/entrada.txt");
    //Processor INE5412 = Processor("Src/Entradas/entrada_simples.txt");
    INE5412.run();
}
