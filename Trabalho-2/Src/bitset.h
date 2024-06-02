#ifndef bitsetH
#define bitsetH

#include <string>

using namespace std;

class Bitset {
public:
    Bitset(int size_);

    void fix(int pos);
    void unfix(int pos);
    void flip();

    bool check();
    bool checkOne();
    int countOne();

    string value();

private:
    int qtd_zero;
    int qtd_one;
    int size;

    string bitset;
    string fliped_bitset;
};

#endif
