#include <bits/stdc++.h>

#include "fileReader.h"
#include "processor.h"
#include "process.h"

using namespace std;

int main() {
    FileReader file = FileReader("./Src/Entradas/entrada.txt");
    vector<parameters*> pam = file.readFile();
    for (auto p : pam)
        cout << p->data << p->duracao << p->prioridade << '\n';

    INE5412 processor = INE5412(pam);
    for (auto p : processor.processos)
        cout << p->id << p->data << p->duracao << p->prioridade << '\n';
}
