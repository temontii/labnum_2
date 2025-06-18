#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include "errors.hpp"
#include <algorithm>

template <typename T>
class DynamicArray {
protected:
    T* data;
    int size;

public:
    DynamicArray();
    DynamicArray(int size);
    DynamicArray(T* items, int size);
    DynamicArray(const DynamicArray<T>& other);
    ~DynamicArray();

    T& Get(int index);
    const T& Get(int index) const;
    void Set(int index, T value);
    int GetSize() const;
    void Resize(int newSize);
    void Remove(int index);
    DynamicArray<T>* GetSubArray(int start, int end) const;
};

template <typename T>
DynamicArray<T>::DynamicArray() : data(nullptr), size(0) {}

template <typename T>
DynamicArray<T>::DynamicArray(int size) : size(size) {
    if (size < 0) throw Errors::InvalidSize();
    data = new T[size];
    for (int i = 0; i < size; i++) {
        data[i] = T();
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(T* items, int size) : size(size) {
    if (size < 0) throw Errors::InvalidSize();
    data = new T[size];
    for (int i = 0; i < size; i++) {
        data[i] = items[i];
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other) : size(other.size) {
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
T& DynamicArray<T>::Get(int index) {
    if (index < 0 || index >= size) throw Errors::IndexOutOfRange();
    return data[index];
}

template <typename T>
const T& DynamicArray<T>::Get(int index) const {
    if (index < 0 || index >= size) throw Errors::IndexOutOfRange();
    return data[index];
}

template <typename T>
void DynamicArray<T>::Set(int index, T value) {
    if (index < 0 || index >= size) throw Errors::IndexOutOfRange();
    data[index] = std::move(value);
}

template <typename T>
int DynamicArray<T>::GetSize() const {
    return size;
}

template <typename T>
void DynamicArray<T>::Resize(int newSize) {
    if (newSize < 0) throw Errors::InvalidSize();
    T* newData = new T[newSize];
    int copySize = std::min(newSize, size);
    for (int i = 0; i < copySize; i++) {
        newData[i] = std::move(data[i]);
    }
    for (int i = copySize; i < newSize; i++) {
        newData[i] = T();
    }
    delete[] data;
    data = newData;
    size = newSize;
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
DynamicArray<T>* DynamicArray<T>::GetSubArray(int start, int end) const {
    if (start < 0 || end > size || start > end) throw Errors::IndexOutOfRange();
    DynamicArray<T>* result = new DynamicArray<T>(end - start);
    for (int i = start; i < end; i++) {
        result->Set(i - start, data[i]);
    }
    return result;
}

#endif
