#include "doubly_linked_list.h"
#include <unistd.h>
#include <iostream>


template<typename T>
DoublyLinkedList<T>::DoublyLinkedList() {
    head = nullptr;
    tail = nullptr;

    size_ = 0;
}

template<typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    clear();
}

template<typename T>
void DoublyLinkedList<T>::clear() {

    while (!empty()) {
        pop_front();
    }
}

template<typename T>
void DoublyLinkedList<T>::push_back(const T& data) {
    Node* new_tail = new Node(data, tail, nullptr);
    if (!empty()) tail->next(new_tail);
    if (empty()) head = new_tail;
    tail = new_tail;
    size_++;
}

template<typename T>
void DoublyLinkedList<T>::push_front(const T& data) {
    Node* new_head = new Node(data, nullptr, head);
    if (!empty()) head->prev(new_head);
    if (empty()) tail = new_head;
    head = new_head;
    size_++;
}

template<typename T>
void DoublyLinkedList<T>::insert(const T& data, std::size_t index) {
    if (index < 0 || index >= size_) throw std::out_of_range("out of range");
    if (empty()) {
        push_front(data);
        return;
    }

    if (index == 0) {
        push_front(data);
        return;
    }

    if (index == size_ - 1) {
        push_back(data);
        return;
    }

    Node* new_node = new Node(data);
    Node* p;
    if (index < size_/2) {  // 1º metade
        p = head;
        for (std::size_t i = 0; i < index; i++) {
            p = p->next();
        }
    } else {  // 2º metade
        p = tail;
        for (std::size_t i = size_ - 1; i > index; i--) {
            p = p->prev();
        }
    }
    
    new_node->next(p); // ponteiro apontando pra lugares duvidosos 
    new_node->prev(p->prev());

    p->prev()->next(new_node);
    p->prev(new_node);

    size_++;

    return;
}

template<typename T>
void DoublyLinkedList<T>::insert_sorted(const T& data) {
	Node* p = head;
	size_t i = 0;
	if (static_cast<int>(size_) > 0) {
		if (static_cast<int>(size_) == 1) {
			if (p->data() > data) {
				this->push_front(data);
			} else {
				this->push_back(data);
			}
			return;
		}
		while (p->next() != nullptr) {
			if (p->data() > data) {
				this->insert(data, i);
				return;
			}
			i++;
			p = p->next();
		}
		this->push_back(data);
	} else {
		this->push_front(data);
	}
}

template<typename T>
T DoublyLinkedList<T>::pop(std::size_t index) {
    if (index < 0 || index >= size_) throw std::out_of_range("out of range");
    if (empty()) throw std::out_of_range("lista vazia");

    if (index == 0) {
        return pop_front();
    }

    if (index == size_-1) {
        return pop_back();
    }

    Node* popped;
    if (index < size()/2) {  // 1º metade
        popped = head;
        for (std::size_t i = 0; i < index; i++) {
            popped = popped->next();
        }
    } else {  // 2º metade
        popped = tail;
        for (std::size_t i = size() - 1; i > index; i--) {
            popped = popped->prev();
        }
    }

    T popped_value = popped->data();

    Node* tmp = popped->next();

    popped->next()->prev(popped->prev());
    popped->prev()->next(tmp);

    delete popped;
    size_--;

    return popped_value;
}
                           
template<typename T>
T DoublyLinkedList<T>::pop_back() {
    if (empty()) throw std::out_of_range("lista vazia");

    Node* popped = tail;
    tail = tail->prev();

    if (tail) {
        tail->next(nullptr);
    } else {
        head = nullptr;
    }

    T popped_value = popped->data();

    delete popped;
    size_--;

    return popped_value;
}

template<typename T>
T DoublyLinkedList<T>::pop_front() {
    if (empty()) throw std::out_of_range("lista vazia");

    Node* popped = head;
    head = head->next();

    if (head) {
        head->prev(nullptr);
    } else {
        tail = nullptr;
    }

    T popped_value = popped->data();

    delete popped;
    size_--;

    return popped_value;
}

template<typename T>
void DoublyLinkedList<T>::remove(const T& data) {
    if (empty()) throw std::out_of_range("lista vazia");

    Node* p = head;
    for (std::size_t i = 0; i < size_; i++) {
        if (p->data() == data) {
            pop(i);
            return;
        }
        p = p->next();
    }
}

template<typename T>
bool DoublyLinkedList<T>::empty() const{
    return (size_ == 0);
}

template<typename T>
bool DoublyLinkedList<T>::contains(const T& data) const {
    Node* p = head;
    for (std::size_t i = 0; i < size_; i++) {
        if (p->data() == data) return true;
        p = p->next();
    }
    return false;
}

template<typename T>
T& DoublyLinkedList<T>::at(std::size_t index) {
    if (index < 0 || index >= size_) throw std::out_of_range("out of range");

    Node *p;
    if (index < size()/2) {  // 1º metade
        p = head;
        for (std::size_t i = 0; i < index; i++) {
            p = p->next();
        }
    } else {  // 2º metade
        p = tail;
        for (std::size_t i = size() - 1; i > index; i--) {
            p = p->prev();
        }
    }
    return p->data();
}

template<typename T>
const T& DoublyLinkedList<T>::at(std::size_t index) const {
    if (index < 0 || index >= size_) throw std::out_of_range("out of range");

    Node *p;
    if (index < size()/2) {  // 1º metade
        p = head;
        for (std::size_t i = 0; i < index; i++) {
            p = p->next();
        }
    } else {  // 2º metade
        p = tail;
        for (std::size_t i = size() - 1; i > index; i--) {
            p = p->prev();
        }
    }
    return p->data();
}

template<typename T>
std::size_t DoublyLinkedList<T>::find(const T& data) const {
    Node* p = head;
    for (std::size_t i = 0; i < size_; i++) {
        if (p->data() == data) return i;
        p = p->next();
    }
    return size();
}

template<typename T>
std::size_t DoublyLinkedList<T>::size() const {
    return size_;
}

template<typename T>
void DoublyLinkedList<T>::check() const {
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

template class DoublyLinkedList<std::string>;
template class DoublyLinkedList<int>;
template class DoublyLinkedList<element>;

