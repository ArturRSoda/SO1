#include "bitmap_manager.h"

// Contrutor
BitmapManager::BitmapManager(int qty_block, int block_size_) {
    mem_list_bit = new Bitset(qty_block);
    last_allocation_start = 0;
    block_size = block_size_;
}

// Destrutor
BitmapManager::~BitmapManager() {
    delete mem_list_bit;
}

// Metodo para alocar de acordo com o algoritmo de alocacao
// Se alocar com sucesso, retorna bloco de inicio do segmento alocado
// Se nao alocar, retorna -1
int BitmapManager::alloc(int size, int alloc_alg) {
    // Busca bloco de inicio do segmento livre a ser alocado
    // Caso nao ache segmento com tamanho o suficiente, nao aloca
    int seg_start = (alloc_alg == 1) ? firstFit(size) : nextFit(size);
    if (seg_start == -1) return seg_start;

    // Marca bloco de inicio do ultimo bloco alocado
    last_allocation_start = seg_start + size;

    // Aloca
    for (int i = 0; i < size; i++)
        mem_list_bit->fix(seg_start + i);

    return seg_start;
}


// Desaloca segmento
void BitmapManager::del(int seg_start, int seg_size) {
    for (int i = 0; i < seg_size; i++)
        mem_list_bit->unfix(seg_start+i);
}


// Retorna bloco de inicio do primeiro segmento livre que caiba o pedido
// A partir do inicio do bitset
int BitmapManager::firstFit(int size) {
    // Busca segmento a partir do inicio do bitset
    return searchSeg(0, size);
}

// Retorna bloco de inicio do primeiro segmento livre que caiba o pedido
// A partir do inicio do ultimo segmento alocado
int BitmapManager::nextFit(int size) {
    int block_ptr;
    // Busca segmento a partir do ultimo segmento alocado
    for (int i = 0; i < mem_list_bit->size(); i++) {
        if (i == last_allocation_start) {
            block_ptr = i;
            break;
        }
    }
    return searchSeg(block_ptr, size);
}

// Procura primeiro seguimento que caiba o tamanho do pedido, apartir de seguimento dado no parametro
int BitmapManager::searchSeg(int seg_ptr, int size) {
    int seg_start, seg_size, bitset_ptr;
    bool flag;

    Bitset visited_block = Bitset(mem_list_bit->size());

    seg_start = -1;
    flag = false;
    while (visited_block.get_qtd_zero()) {
        if (seg_ptr >= mem_list_bit->size())
            seg_ptr = 0;

        seg_size = 0;
        bitset_ptr = seg_ptr;
        visited_block.fix(seg_ptr); 

        while (mem_list_bit->value()[bitset_ptr] == '0') {
            seg_size++;

            if (seg_size >= size) {
                flag = true;
                break;
            }

            bitset_ptr++;
            if (bitset_ptr >= static_cast<int>(mem_list_bit->size()))
                break;

            visited_block.fix(bitset_ptr);
        }

        if (flag) {
            seg_start = seg_ptr;
            break;
        }
        seg_ptr += (seg_size) ? seg_size : 1;
    }
    return seg_start;
}


// Retorna quantidade de blocos alocados
int BitmapManager::getQtyAllocatedBlocks() {
    return mem_list_bit->get_qtd_one();
}


// Imprime situacao atual do gerenciador do bitset
void BitmapManager::printState() {
    cout << mem_list_bit->value() << endl;
}
