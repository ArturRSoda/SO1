#include "fileReader.h"

FileReader::FileReader(string file_name) {
    file.open(file_name);
}

FileReader::~FileReader() {
    for (auto v : pams)
    delete v;
}

vector<parameters*> FileReader::readFile() {
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

