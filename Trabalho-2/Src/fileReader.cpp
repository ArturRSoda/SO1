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
    while (getline(cin, line)) {
        p->requests.push_back(line);
    }
}

parameters* FileReader::getParameters() {
    return p;
}

