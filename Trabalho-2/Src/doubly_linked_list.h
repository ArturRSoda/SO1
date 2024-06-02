#ifndef dllH
#define dllH

#include <cstdint>
#include <stdexcept>

template<typename T>
class DoublyLinkedList {
 public:
    DoublyLinkedList();
    ~DoublyLinkedList();
    void clear();

    void push_back(const T& data);  // insere no fim
    void push_front(const T& data);  // insere no início
    void insert(const T& data, std::size_t index);  // insere na indexição
    void insert_sorted(const T& data);  // insere em ordem

    T pop(std::size_t index);  // retira da indexição
    T pop_back();  // retira do fim
    T pop_front();  // retira do início
    void remove(const T& data);  // retira específico

    bool empty() const;  // lista vazia
    bool contains(const T& data) const;  // contém

    T& at(std::size_t index);  // acesso a um elemento (checando limites)
    const T& at(std::size_t index) const;  // getter constante a um elemento

    std::size_t find(const T& data) const;  // indexição de um dado
    std::size_t size() const;  // tamanho

    //tirar depois
    void check() const {
        if (size_ == 0) {
            if (head || tail) {
                printf("size = 0, head e tail invalidos\n");
                fflush(stdout);
            }
        } else {
            if (!head || !tail) {
                printf("size != 0, head e tail invalidos\n");
                fflush(stdout);
            }
            Node* prev = 0;
            Node* curr = head;
            std::size_t i = 0;
            while (curr) {
                if (curr->prev() != prev) {
                    printf("lista invalida\n");
                    fflush(stdout);
                }

                i++;
                prev = curr;
                curr = curr->next();
            }
            if (i != size_) {
                printf("i = size invalido\n");
                fflush(stdout);
            }
        }
    }

 private:
    class Node {
     public:
        Node(const T& data):
            data_{data}
        {}
        Node(const T& data, Node* next):
            data_{data},
            next_{next}
        {}
        Node(const T& data, Node* prev, Node* next):
            data_{data},
            prev_{prev},
            next_{next}
        {}

        T& data() {
            return data_;
        }
        const T& data() const {
            return data_;
        }

        Node* prev() {
            return prev_;
        }
        const Node* prev() const {
            return prev_;
        }

        void prev(Node* node) { // setter
            prev_ = node;
        }

        Node* next() {
            return next_;
        }
        const Node* next() const {
            return next_;
        }

        void next(Node* node) { // setter
            next_ = node;
        }

     private:
        T data_;
        Node* prev_{nullptr};
        Node* next_{nullptr};
    };

    Node* head{nullptr};  // primeiro da lista
    Node* tail{nullptr};  // ultimo da lista
    std::size_t size_{0};
};
#endif
