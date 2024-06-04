#include "fileReader.h"

FileReader::FileReader() {
    p = new parameters;
}

FileReader::~FileReader() {
    delete p;
}

void FileReader::readFile() {
    cin >> p->manager;
    cin >> p->mem_size;
    cin >> p->block_size;
    cin >> p->alloc_alg;

    string line;
    getline(cin, line);
    int i = 0;
    int qtd_requests = 0;
    while (getline(cin, line)) {
        qtd_requests++;
        p->requests[i++] = line;
    }

    p->qtd_requests = qtd_requests;
}

parameters* FileReader::getParameters() {
    return p;
}

