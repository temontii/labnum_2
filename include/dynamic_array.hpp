#pragma once
#include <stdexcept>
#include "errors.hpp"

template <typename T>
class DynamicArray {
private:
    T* data;
    int size;
    int capacity;

    void EnsureCapacity(int newCapacity) {
        if (newCapacity > capacity) {
            T* newData = new T[newCapacity];
            for (int i = 0; i < size; i++) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
            capacity = newCapacity;
        }
    }

public:
    DynamicArray(T* items, int count) {
        if (count < 0) throw Errors::InvalidSize();
        size = count;
        capacity = count;
        data = new T[size];
        for (int i = 0; i < size; i++) {
            data[i] = items[i];
        }
    }

    DynamicArray(int size) {
        if (size < 0) throw Errors::InvalidSize();
        this->size = size;
        this->capacity = size;
        data = new T[size];
    }

    DynamicArray(const DynamicArray<T>& other) {
        size = other.size;
        capacity = other.capacity;
        data = new T[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    ~DynamicArray() {
        delete[] data;
    }

    T Get(int index) const {
        if (index < 0 || index >= size) throw Errors::IndexOutOfRange();
        return data[index];
    }

    int GetSize() const { return size; }
    int GetCapacity() const { return capacity; }

    void Set(int index, T value) {
        if (index < 0 || index >= size) throw Errors::IndexOutOfRange();
        data[index] = value;
    }

    void Resize(int newSize) {
        if (newSize < 0) throw Errors::InvalidSize();
        if (newSize > capacity) {
            EnsureCapacity(newSize);
        }
        size = newSize;
    }

    DynamicArray<T>* GetSubArray(int startIndex, int endIndex) const {
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

    void Remove(int index) {
        if (index < 0 || index >= size) throw Errors::IndexOutOfRange();
        for (int i = index; i < size - 1; i++) {
            data[i] = data[i + 1];
        }
        size--;
    }

    T& operator[](int index) {
        if (index < 0 || index >= size) throw Errors::IndexOutOfRange();
        return data[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= size) throw Errors::IndexOutOfRange();
        return data[index];
    }
};

template<typename T>
bool operator==(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) return false;
    for (int i = 0; i < lhs.GetSize(); ++i) {
        if (lhs[i] != rhs[i]) return false;
    }
    return true;
}