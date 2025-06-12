#pragma once
#include <stdexcept>
#include "errors.hpp"

template <typename T>
class DynamicArray {
private:
    T* data;
    int size;

public:
    DynamicArray(int size);
    DynamicArray(const T* items, int count);
    DynamicArray(const DynamicArray<T>& other);
    ~DynamicArray();

    T Get(int index) const;
    int GetSize() const;
    void Set(int index, T value);
    void Resize(int newSize);
    DynamicArray<T>* GetSubArray(int startIndex, int endIndex) const;
    void Remove(int index);

    T& operator[](int index);
    const T& operator[](int index) const;
};

template<typename T>
bool operator==(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs);

template <typename T>
DynamicArray<T>::DynamicArray(int size) {
    if (size < 0) throw Errors::InvalidSize();
    this->size = size;
    data = new T[size];
}

template <typename T>
DynamicArray<T>::DynamicArray(const T* items, int count) { 
    if (count < 0) throw Errors::InvalidSize();
    size = count;
    data = new T[size];
    for (int i = 0; i < size; i++) {
        data[i] = items[i];
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other) {
    size = other.size;
    data = new T[size];
    for (int i = 0; i < size; i++) {
        data[i] = other.data[i];
    }
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
}

template <typename T>
T DynamicArray<T>::Get(int index) const {
    if (index < 0 || index >= size) throw Errors::IndexOutOfRange();
    return data[index];
}

template <typename T>
int DynamicArray<T>::GetSize() const {
    return size;
}

template <typename T>
void DynamicArray<T>::Set(int index, T value) {
    if (index < 0 || index >= size) throw Errors::IndexOutOfRange();
    data[index] = value;
}

template <typename T>
void DynamicArray<T>::Resize(int newSize) {
    if (newSize < 0) throw Errors::InvalidSize();
    T* newData = new T[newSize];
    int copySize = newSize < size ? newSize : size;
    for (int i = 0; i < copySize; i++) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    size = newSize;
}

template <typename T>
DynamicArray<T>* DynamicArray<T>::GetSubArray(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex >= size || startIndex > endIndex) {
        throw Errors::InvalidRange();
    }
    int subSize = endIndex - startIndex + 1;
    DynamicArray<T>* subArray = new DynamicArray<T>(subSize);
    for (int i = 0; i < subSize; i++) {
        subArray->Set(i, data[startIndex + i]);
    }
    return subArray;
}

template <typename T>
void DynamicArray<T>::Remove(int index) {
    if (index < 0 || index >= size) throw Errors::IndexOutOfRange();
    for (int i = index; i < size - 1; i++) {
        data[i] = data[i + 1];
    }
    size--;
}

template <typename T>
T& DynamicArray<T>::operator[](int index) {
    if (index < 0 || index >= size) throw Errors::IndexOutOfRange();
    return data[index];
}

template <typename T>
const T& DynamicArray<T>::operator[](int index) const {
    if (index < 0 || index >= size) throw Errors::IndexOutOfRange();
    return data[index];
}

template<typename T>
bool operator==(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) return false;
    for (int i = 0; i < lhs.GetSize(); ++i) {
        if (lhs[i] != rhs[i]) return false;
    }
    return true;
}
