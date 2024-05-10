#include <iostream>
#include <string>

using namespace std;

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

    string pedido;
    int qtd_bytes;
    int id;

    string line;
    getline(cin, line);
    cout << endl;
    while (getline(cin, line)) {
        pedido = line[0];

        line.erase(0, line.find(" ")+1);

        if (!pedido.compare("A")) {
            qtd_bytes = stoi(line.substr(0, line.find(" ")));
            line.erase(0, line.find(" ")+1);
        }

        id = stoi(line);

        cout << "pedido = " << pedido << endl;
        if (!pedido.compare("A")) cout << "qtd bytes = " << qtd_bytes << endl;
        cout << "id = " << id << endl;
        cout << endl;
    }


    return 0;
}
