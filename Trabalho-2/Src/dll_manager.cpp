#include "dll_manager.h"

// Construtor
DllManager::DllManager(int qty_block, int block_size_) {
    last_allocation_start = 0;
    block_size = block_size_;

    element e = {0, qty_block, 0};
    mem_list_dll.push_back(e);
}

 
// Metodo para alocar de acordo com o algoritmo de alocacao
// Se alocar com sucesso, retorna bloco de inicio do segmento alocado
// Se nao alocar, retorna -1
int DllManager::alloc(int size, int alloc_alg) {
    int seg_start;
    element old_seg, allocated_seg, free_seg;

    // Busca bloco de inicio do segmento livre a ser alocado
    // Caso nao ache segmento com tamanho o suficiente, nao aloca
    seg_start = (alloc_alg == 1) ? firstFit(size) : nextFit(size);
    if (seg_start == -1) return seg_start;

    // Marca bloco de inicio do ultimo bloco alocado
    last_allocation_start = seg_start;

    // Busca e remove seguimento livre encontrado
    for (size_t i = 0; i < mem_list_dll.size(); i++) {
        old_seg = mem_list_dll.at(i);
        if (old_seg.start == seg_start) break;
    }
    mem_list_dll.remove(old_seg);

    // Aloca seguimento do pedido feito
    allocated_seg = {seg_start, size, 1};
    mem_list_dll.insert_sorted(allocated_seg);

    // Aloca o restante do seguimento livre removido, se houver
    if (old_seg.size - allocated_seg.size) {
        free_seg = {seg_start+size, old_seg.size-allocated_seg.size, 0};
        mem_list_dll.insert_sorted(free_seg);
    }

    return seg_start;
}

// Desaloca seguimento
void DllManager::del(int seg_start, int seg_size) {
    element cur_seg, back_seg, front_seg, free_seg;

    for (size_t i = 0; i < mem_list_dll.size(); i++) {
        cur_seg = mem_list_dll.at(i);

        // Busca seguimento para desalocar
        if (seg_start == cur_seg.start) {
            // Guarda seguimentos vizinhos, se houverem
            if (i != 0) back_seg = mem_list_dll.at(i-1);
            if (i != (mem_list_dll.size()-1)) front_seg = mem_list_dll.at(i+1);

            // Define inicio, tamanho e status do
            //     do seguimento livre a ser alocado no lugar
            //     unindo com os segmentos vizinhos, se existirem e estiverem livres
            free_seg.start = ((i != 0) && (back_seg.status == 0)) ? back_seg.start : cur_seg.start;

            free_seg.size = seg_size;
            free_seg.size += ((i != 0) && (back_seg.status == 0)) ? back_seg.size : 0;
            free_seg.size += ((i != (mem_list_dll.size()-1)) && (front_seg.status == 0)) ? front_seg.size : 0;

            free_seg.status = 0;

            // Desaloca seguimento do pedido feito
            mem_list_dll.remove(cur_seg);

            // Une deleta vizinhos unidos com o segmento livre
            if ((i != 0) && (back_seg.status == 0)) mem_list_dll.remove(back_seg);
            if ((i != (mem_list_dll.size()-1)) && (front_seg.status == 0)) mem_list_dll.remove(front_seg);

            // Insere na lista o segmento livre
            mem_list_dll.insert_sorted(free_seg);

            return;
        }
    }
}


// Retorna bloco de inicio do primeiro segmento livre que caiba o pedido
//     a partir do inicio do da lista
int DllManager::firstFit(int size) {
    // Busca segmento a partir do inicio do lista
    return searchSeg(0, size);
}


// Retorna bloco de inicio do primeiro segmento livre que caiba o pedido
//     a partir do inicio do ultimo segmento alocado
int DllManager::nextFit(int size) {
    int seg_ptr;
    // Busca segmento a partir do ultimo segmento alocado
    for (size_t i = 0; i < mem_list_dll.size(); i++) {
        if (mem_list_dll.at(i).start == last_allocation_start) {
            seg_ptr = i;
            break;
        }
    }
    return searchSeg(seg_ptr, size);
}


// Procura primeiro seguimento que caiba o tamanho do pedido, apartir de seguimento dado no parametro
// Se nao achar, retorna -1
int DllManager::searchSeg(int seg_ptr, int size) {
    element elem;
    int seg_start;
    bool flag;

    // Array para controle de seguimentos ja visitados
    bool visited_seg[mem_list_dll.size()];
    for (size_t i = 0; i < mem_list_dll.size(); i++)
        visited_seg[i] = false;

    seg_start = -1;
    flag = true;
    // Loop enquanto ainda tiver segmentos a serem visitados
    while (true) {
        // Verifica se ha segmentos para ser visitados
        for (auto seg: visited_seg) {
            if (seg == false) flag = false;
        }
        if (flag) break;

        // Se ponteiro passar do ultimo segmento, volta a apontar ao primeiro segmento
        if (seg_ptr >= static_cast<int>(mem_list_dll.size()))
            seg_ptr = 0;

        // Indica como segmento visitado
        visited_seg[seg_ptr] = true;

        // Verifica se segmento apontado, possui tamanho desejado e se esta livre
        elem = mem_list_dll.at(seg_ptr);
        if ((elem.size >= size) and (elem.status == 0)) {
            seg_start = elem.start;
            break;
        }

        // Aponta para o proximo segmento
        seg_ptr++;
    }

    return seg_start;
}


// Retorna quantidade de blocos alocados
int DllManager::getQtyAllocatedBlocks() {
    int qty_blocks = 0;
    for (size_t i = 0; i < mem_list_dll.size(); i++) {
        if (mem_list_dll.at(i).status == 1)
            qty_blocks += mem_list_dll.at(i).size;
    }
    return qty_blocks;
}


// Imprime situacao atual do gerenciador do bitset
void DllManager::printState() {
    element elem;
    for (size_t i = 0; i < mem_list_dll.size(); i++) {
        elem = mem_list_dll.at(i);
        std::cout << elem.size*block_size << " " << elem.status << std::endl;
    }
}
