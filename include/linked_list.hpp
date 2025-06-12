#pragma once
#include <stdexcept>
#include "errors.hpp"


template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(T data) : data(data), next(nullptr) {}
        Node(T data, Node* next) : data(data), next(next) {}
    };

    Node* head;
    Node* tail;
    int size;

public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    LinkedList(T* items, int count) : head(nullptr), tail(nullptr), size(0) {
        if (count < 0) throw Errors::InvalidSize();
        if (count == 0) return;
        
        head = new Node(items[0]);
        Node* current = head;
        size = count;
        
        for (int i = 1; i < count; i++) {
            current->next = new Node(items[i]);
            current = current->next;
        }
        tail = current;
    }

    LinkedList(const LinkedList<T>& other) : head(nullptr), tail(nullptr), size(0) {
        if (!other.head) return;
        
        head = new Node(other.head->data);
        Node* current = head;
        Node* otherCurrent = other.head->next;
        size = other.size;
        
        while (otherCurrent) {
            current->next = new Node(otherCurrent->data);
            current = current->next;
            otherCurrent = otherCurrent->next;
        }
        tail = current;
    }

    ~LinkedList() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    T GetFirst() const {
        if (!head) throw Errors::EmptyList();
        return head->data;
    }

    T GetLast() const {
        if (!tail) throw Errors::EmptyList();
        return tail->data;
    }

    T Get(int index) const {
        if (!head) throw Errors::EmptyList();
        if (index < 0 || index >= size) throw Errors::IndexOutOfRange();
        
        Node* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }

    LinkedList<T>* GetSubList(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex >= size || startIndex > endIndex)
            throw Errors::InvalidRange();
            
        LinkedList<T>* sublist = new LinkedList<T>();
        Node* current = head;
        
        for (int i = 0; i < startIndex; i++) {
            current = current->next;
        }
        
        for (int i = startIndex; i <= endIndex; i++) {
            sublist->Append(current->data);
            current = current->next;
        }
        
        return sublist;
    }

    int GetLength() const { return size; }

    void Append(T item) {
        Node* newNode = new Node(item);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    void Prepend(T item) {
        head = new Node(item, head);
        if (!tail) tail = head;
        size++;
    }

    void InsertAt(T item, int index) {
        if (index < 0 || index > size) throw Errors::IndexOutOfRange();
        if (index == 0) {
            Prepend(item);
            return;
        }
        if (index == size) {
            Append(item);
            return;
        }
        
        Node* current = head;
        for (int i = 0; i < index - 1; i++) {
            current = current->next;
        }
        
        current->next = new Node(item, current->next);
        size++;
    }

    void Remove(int index) {
        if (size == 0) throw Errors::EmptyList();
        if (index < 0 || index >= size) throw Errors::IndexOutOfRange();
        
        if (index == 0) {
            Node* temp = head;
            head = head->next;
            delete temp;
            if (!head) tail = nullptr;
        } else {
            Node* current = head;
            for (int i = 0; i < index - 1; i++) {
                current = current->next;
            }
            
            Node* temp = current->next;
            current->next = temp->next;
            if (temp == tail) tail = current;
            delete temp;
        }
        size--;
    }

    LinkedList<T>* Concat(const LinkedList<T>* list) const {
        if (!list) throw Errors::NullList();
        
        LinkedList<T>* result = new LinkedList<T>(*this);
        if (!list->head) return result;
        
        Node* current = list->head;
        while (current) {
            result->Append(current->data);
            current = current->next;
        }
        
        return result;
    }
};
