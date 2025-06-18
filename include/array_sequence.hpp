#ifndef ARRAY_SEQUENCE_HPP
#define ARRAY_SEQUENCE_HPP

#include "dynamic_array.hpp"
#include "sequence.hpp"
#include <algorithm>

template <typename T>
class ArraySequence : public ISequence<T> {
protected:
    DynamicArray<T>* array;
    int capacity;
    void EnsureCapacity(int newCapacity);

public:
    ArraySequence();
    ArraySequence(int size);
    ArraySequence(T* items, int size);
    ArraySequence(const ArraySequence<T>& other);
    ~ArraySequence() override;

    ArraySequence<T>& operator=(const ArraySequence<T>& other);

    void Reserve(int newCapacity);

    T Front() const override;
    T Back() const override;
    T At(int index) const override;
    ISequence<T>* AddToEnd(T item) override;
    ISequence<T>* AddToFront(T item) override;
    ISequence<T>* Insert(T item, int index) override;
    ISequence<T>* Delete(int index) override;
    ISequence<T>* Slice(int start, int end) const override;
    ISequence<T>* Combine(const ISequence<T>* other) const override;
    int Size() const override;
    ISequence<T>* GetReference() override;
    ISequence<T>* Copy() const override;

    int Capacity() const;
};

template <typename T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    ImmutableArraySequence(const ArraySequence<T>& seq) : ArraySequence<T>(seq) {}
    ISequence<T>* AddToEnd(T item) override;
    ISequence<T>* AddToFront(T item) override;
    ISequence<T>* Insert(T item, int index) override;
    ISequence<T>* Delete(int index) override;
    ISequence<T>* GetReference() override;
    ISequence<T>* Copy() const override;
};

template <typename T>
ArraySequence<T>::ArraySequence() : array(new DynamicArray<T>(0)), capacity(1) {}

template <typename T>
ArraySequence<T>::ArraySequence(int size) : array(new DynamicArray<T>(size)), capacity(size) {
    if (size < 0) throw Errors::InvalidSize();
}

template <typename T>
ArraySequence<T>::ArraySequence(T* items, int size) : array(new DynamicArray<T>(items, size)), capacity(size) {
    if (size < 0) throw Errors::InvalidSize();
}

template <typename T>
ArraySequence<T>::ArraySequence(const ArraySequence<T>& other) 
    : array(new DynamicArray<T>(*other.array)), capacity(other.capacity) {}

template <typename T>
ArraySequence<T>::~ArraySequence() {
    delete array;
}

template <typename T>
ArraySequence<T>& ArraySequence<T>::operator=(const ArraySequence<T>& other) {
    if (this != &other) {
        DynamicArray<T>* newArray = nullptr;
        try {
            newArray = new DynamicArray<T>(*other.array);
            delete array;
            array = newArray;
            capacity = other.capacity;
        } catch (...) {
            delete newArray;
            throw;
        }
    }
    return *this;
}

template <typename T>
void ArraySequence<T>::EnsureCapacity(int newCapacity) {
    if (newCapacity <= capacity) return;
    capacity = std::max(newCapacity, capacity * 2);
    DynamicArray<T>* newArray = nullptr;
    try {
        newArray = new DynamicArray<T>(capacity);
        for (int i = 0; i < array->GetSize(); i++) {
            newArray->Set(i, array->Get(i));
        }
        newArray->Resize(array->GetSize());
        delete array;
        array = newArray;
    } catch (...) {
        delete newArray;
        throw;
    }
}

template <typename T>
void ArraySequence<T>::Reserve(int newCapacity) {
    if (newCapacity < 0) throw Errors::InvalidSize();
    EnsureCapacity(newCapacity);
}

template <typename T>
T ArraySequence<T>::Front() const {
    if (array->GetSize() == 0) throw Errors::EmptyContainer();
    return array->Get(0);
}

template <typename T>
T ArraySequence<T>::Back() const {
    if (array->GetSize() == 0) throw Errors::EmptyContainer();
    return array->Get(array->GetSize() - 1);
}

template <typename T>
T ArraySequence<T>::At(int index) const {
    if (index < 0 || index >= array->GetSize()) throw Errors::IndexOutOfRange();
    return array->Get(index);
}

template <typename T>
ISequence<T>* ArraySequence<T>::AddToEnd(T item) {
    EnsureCapacity(array->GetSize() + 1);
    array->Resize(array->GetSize() + 1);
    array->Set(array->GetSize() - 1, item);
    return this;
}

template <typename T>
ISequence<T>* ArraySequence<T>::AddToFront(T item) {
    EnsureCapacity(array->GetSize() + 1);
    array->Resize(array->GetSize() + 1);
    for (int i = array->GetSize() - 1; i > 0; i--) {
        array->Set(i, array->Get(i - 1));
    }
    array->Set(0, item);
    return this;
}

template <typename T>
ISequence<T>* ArraySequence<T>::Insert(T item, int index) {
    if (index < 0 || index > array->GetSize()) throw Errors::IndexOutOfRange();
    EnsureCapacity(array->GetSize() + 1);
    array->Resize(array->GetSize() + 1);
    for (int i = array->GetSize() - 1; i > index; i--) {
        array->Set(i, array->Get(i - 1));
    }
    array->Set(index, item);
    return this;
}

template <typename T>
ISequence<T>* ArraySequence<T>::Delete(int index) {
    if (index < 0 || index >= array->GetSize()) throw Errors::IndexOutOfRange();
    for (int i = index; i < array->GetSize() - 1; i++) {
        array->Set(i, array->Get(i + 1));
    }
    array->Resize(array->GetSize() - 1);
    return this;
}

template <typename T>
ISequence<T>* ArraySequence<T>::Slice(int start, int end) const {
    if (start < 0 || end > array->GetSize() || start > end) throw Errors::IndexOutOfRange();
    ArraySequence<T>* result = new ArraySequence<T>();
    result->EnsureCapacity(end - start);
    result->array->Resize(end - start);
    for (int i = start; i < end; i++) {
        result->array->Set(i - start, array->Get(i));
    }
    return result;
}

template <typename T>
ISequence<T>* ArraySequence<T>::Combine(const ISequence<T>* other) const {
    ArraySequence<T>* result = new ArraySequence<T>(*this);
    result->EnsureCapacity(array->GetSize() + other->Size());
    for (int i = 0; i < other->Size(); i++) {
        result->AddToEnd(other->At(i));
    }
    return result;
}

template <typename T>
int ArraySequence<T>::Size() const {
    return array->GetSize();
}

template <typename T>
ISequence<T>* ArraySequence<T>::GetReference() {
    return this;
}

template <typename T>
ISequence<T>* ArraySequence<T>::Copy() const {
    return new ArraySequence<T>(*this);
}

template <typename T>
int ArraySequence<T>::Capacity() const {
    return capacity;
}

template <typename T>
ISequence<T>* ImmutableArraySequence<T>::AddToEnd(T item) {
    ArraySequence<T>* copy = new ArraySequence<T>(*this);
    copy->AddToEnd(item);
    return new ImmutableArraySequence<T>(*copy);
}

template <typename T>
ISequence<T>* ImmutableArraySequence<T>::AddToFront(T item) {
    ArraySequence<T>* copy = new ArraySequence<T>(*this);
    copy->AddToFront(item);
    return new ImmutableArraySequence<T>(*copy);
}

template <typename T>
ISequence<T>* ImmutableArraySequence<T>::Insert(T item, int index) {
    ArraySequence<T>* copy = new ArraySequence<T>(*this);
    copy->Insert(item, index);
    return new ImmutableArraySequence<T>(*copy);
}

template <typename T>
ISequence<T>* ImmutableArraySequence<T>::Delete(int index) {
    ArraySequence<T>* copy = new ArraySequence<T>(*this);
    copy->Delete(index);
    return new ImmutableArraySequence<T>(*copy);
}

template <typename T>
ISequence<T>* ImmutableArraySequence<T>::GetReference() {
    return new ImmutableArraySequence<T>(*this);
}

template <typename T>
ISequence<T>* ImmutableArraySequence<T>::Copy() const {
    return new ImmutableArraySequence<T>(*this);
}

#endif
