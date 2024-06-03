#include "bitset.h"

Bitset::Bitset(int size_) {
    string temp(size_, '0');
    string fliped_temp(size_, '1');

    bitset = temp;
    fliped_bitset = temp;

    qtd_zero = size_;
    qtd_one = 0;
    size = size_;
}
 Bitset::~Bitset() {}

void Bitset::fix(int pos) {
    if (pos < 0 || pos >= size) throw std::out_of_range("out of range");
    if (bitset[pos] == '0') {
        qtd_one++;
        qtd_zero--;
    }

    bitset[pos] = '1';
    fliped_bitset[pos] = '0';
}


void Bitset::unfix(int pos) {
    if (pos < 0 || pos >= size) throw std::out_of_range("out of range");
    if (bitset[pos] == '1') {
        qtd_zero++;
        qtd_one--;
    }

    bitset[pos] = '0';
    fliped_bitset[pos] = '1';
}

void Bitset::flip() {
    string temp = bitset;
    bitset = fliped_bitset;
    fliped_bitset = temp;

    int t = qtd_one;
    qtd_one = qtd_zero;
    qtd_zero = t;
}

bool Bitset::check() {
    return (qtd_one == size);
}

bool Bitset::checkOne() {
    return (qtd_one > 0);
}

int Bitset::countOne() {
    return qtd_one;
}

string Bitset::value() {
    return bitset;
}
