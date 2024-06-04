#ifndef bitsetH
#define bitsetH

#include <cstdint>
#include <stdexcept>
#include <string>

using namespace std;

class Bitset {
public:
    Bitset(int size_);
    ~Bitset();

    void fix(int pos);
    void unfix(int pos);
    void flip();

    bool check();
    bool checkOne();
    int countOne();

    string value();
    int size();

private:
    int qtd_zero;
    int qtd_one;
    int size_;

    string bitset;
    string fliped_bitset;
};

#endif
