#include "fileReader.h"

FileReader::FileReader(string file_name) {
    file.open(file_name);
}

FileReader::~FileReader() {
    for (auto v : pams)
    delete v;
}

vector<parameters*> FileReader::readFile() {
    int a, b, c, d, e;
    while (file >> a >> b >> c >> d >> e) {
        parameters* p = new parameters;
        p->data = a;
        p->duracao = b;
        p->periodo = c;
        p->deadline = d;
        p->prioridade = e;
        pams.push_back(p);
    }
    return pams;
}

