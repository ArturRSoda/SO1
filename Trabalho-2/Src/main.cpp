#include <iostream>
#include <string>

using namespace std;

struct pedidoStruct {
    string pedido;
    int bytes;
    int id;
};

string* split(string line, char sep) {
    static string arr[3];
    int k = 0;
    string temp = "";
        
    for (int i = 0; i < static_cast<int>(line.size()); i++) {
        cout << (line[i] != sep) << endl;
        if (line[i] != sep) {
            temp += line[i];
        } else {
            arr[k] = temp;
            k++;
            temp = "";
        }
    }

    return arr;
}

pedidoStruct* loadPedidos() {
    pedidoStruct* pedidos = new pedidoStruct[4];
    int i = 0;

    string line;
    getline(cin, line);
    cout << endl;
    while (getline(cin, line)) {
        pedidos[i].pedido = line[0];

        line.erase(0, line.find(" ")+1);

        if (!pedidos[i].pedido.compare("A")) {
            pedidos[i].bytes = stoi(line.substr(0, line.find(" ")));
            line.erase(0, line.find(" ")+1);
        }

        pedidos[i].id = stoi(line);

        i++;
    }

    return pedidos;
}


int main() {

    int gerenciamento_de_memoria;
    int qtd_memoria_bytes;
    int tam_bloco_aloc_bytes;
    int alg_aloc;

    cin >> gerenciamento_de_memoria;
    cin >> qtd_memoria_bytes;
    cin >> tam_bloco_aloc_bytes;
    cin >> alg_aloc;

    cout << "Gerenciamento de memoria.: " << gerenciamento_de_memoria << endl;
    cout << "Qtd de memoria em bytes..: " << qtd_memoria_bytes << endl;
    cout << "Tamanho de bloco minimo..: " << tam_bloco_aloc_bytes << endl;
    cout << "algoritmo de alocacao....: " << alg_aloc << endl;

    pedidoStruct* pedidos = loadPedidos();

    for (int i = 0; i < 4; i++) {
        cout << "pedido = " << pedidos[i].pedido << endl;
        if (!pedidos[i].pedido.compare("A")) cout << "qtd bytes = " << pedidos[i].bytes << endl;
        cout << "id = " << pedidos[i].id << endl;
        cout << endl;
    }

    return 0;
}
