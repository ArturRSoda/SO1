#include "fileReader.h"

FileReader::FileReader(string file_name) {
    file.open(file_name);
}

FileReader::~FileReader() {
    delete p;
}

parameters* FileReader::readFile() {
    parameters* pi = new parameters;
    file >> pi->manager;
    file >> pi->mem_size;
    file >> pi->block_size;
    file >> pi->alloc_alg;
    file.get();

    string line;
    DoublyLinkedList<string> requests;
    while (getline(file, line)) {
        requests.push_back(line);
    }
    pi->requests = requests;
    p = pi;
    return p;
}

