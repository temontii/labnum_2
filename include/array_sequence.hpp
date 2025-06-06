#pragma once
#include "sequence.hpp"
#include "dynamic_array.hpp"
#include "errors.hpp"
#include <stdexcept>

template <typename T>
class ArraySequence : public ISequence<T> {
protected:
    DynamicArray<T>* array;
    int capacity;
    ISequence<T>* CreateFromArray(DynamicArray<T>* arr) const;

public:
    ArraySequence();
    ArraySequence(T* items, int count);
    ArraySequence(const ArraySequence<T>& other);
    explicit ArraySequence(const DynamicArray<T>& arr);
    ~ArraySequence() override;

    T Front() const override;
    T Back() const override;
    T At(int index) const override;
    int Size() const override;
    ISequence<T>* Slice(int start, int end) const override;
    ISequence<T>* Combine(const ISequence<T>* other) const override;
    ISequence<T>* AddToEnd(T item) override;
    ISequence<T>* AddToFront(T item) override;
    ISequence<T>* Insert(T item, int index) override;
    ISequence<T>* Delete(int index) override;
    ISequence<T>* GetReference() override;
    ISequence<T>* Copy() const override;

protected:
    void EnsureCapacity(int newCapacity);
};

template <typename T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    using ArraySequence<T>::ArraySequence;
    ISequence<T>* Combine(const ISequence<T>* other) const override;
    ISequence<T>* AddToEnd(T item) override;
    ISequence<T>* AddToFront(T item) override;
    ISequence<T>* Insert(T item, int index) override;
    ISequence<T>* Delete(int index) override;
    ISequence<T>* GetReference() override;
    ISequence<T>* Copy() const override;
};

template <typename T>
ISequence<T>* ArraySequence<T>::CreateFromArray(DynamicArray<T>* arr) const {
    return new ArraySequence<T>(*arr);
}

template <typename T>
void ArraySequence<T>::EnsureCapacity(int newCapacity) {
    if (newCapacity <= capacity) return;
    capacity = newCapacity < 2 * capacity ? 2 * capacity : newCapacity;
    DynamicArray<T>* newArray = new DynamicArray<T>(capacity);
    for (int i = 0; i < array->GetSize(); i++) {
        newArray->Set(i, array->Get(i));
    }
    delete array;
    array = newArray;
}

template <typename T>
ArraySequence<T>::ArraySequence() : array(new DynamicArray<T>(0)), capacity(1) {}

template <typename T>
ArraySequence<T>::ArraySequence(T* items, int count) : array(new DynamicArray<T>(items, count)), capacity(count) {}

template <typename T>
ArraySequence<T>::ArraySequence(const ArraySequence<T>& other) : array(new DynamicArray<T>(*other.array)), capacity(other.capacity) {}

template <typename T>
ArraySequence<T>::ArraySequence(const DynamicArray<T>& arr) : array(new DynamicArray<T>(arr)), capacity(arr.GetSize()) {}

template <typename T>
ArraySequence<T>::~ArraySequence() {
    delete array;
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
    return array->Get(index);
}

template <typename T>
int ArraySequence<T>::Size() const {
    return array->GetSize();
}

template <typename T>
ISequence<T>* ArraySequence<T>::Slice(int start, int end) const {
    DynamicArray<T>* sub = array->GetSubArray(start, end);
    auto* result = new ArraySequence<T>(*sub);
    delete sub;
    return result;
}

template <typename T>
ISequence<T>* ArraySequence<T>::Combine(const ISequence<T>* other) const {
    const auto* otherArray = dynamic_cast<const ArraySequence<T>*>(other);
    if (!otherArray) throw Errors::TypeMismatch();
    int total = array->GetSize() + otherArray->array->GetSize();
    DynamicArray<T>* result = new DynamicArray<T>(total);
    for (int i = 0; i < array->GetSize(); i++) {
        result->Set(i, array->Get(i));
    }
    for (int j = 0; j < otherArray->array->GetSize(); j++) {
        result->Set(j + array->GetSize(), otherArray->array->Get(j));
    }
    return CreateFromArray(result);
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
    for (int i = array->GetSize() - 1; i >= 1; i--) { 
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
    for (int i = array->GetSize() - 1; i >= index + 1; i--) { 
        array->Set(i, array->Get(i - 1));
    }
    array->Set(index, item);
    return this;
}

template <typename T>
ISequence<T>* ArraySequence<T>::Delete(int index) {
    if (array->GetSize() == 0) throw Errors::EmptyContainer();
    array->Remove(index);
    return this;
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
ISequence<T>* ImmutableArraySequence<T>::Combine(const ISequence<T>* other) const {
    const auto* otherArray = dynamic_cast<const ImmutableArraySequence<T>*>(other);
    if (!otherArray) throw Errors::TypeMismatch();
    int total = this->Size() + otherArray->Size();
    DynamicArray<T> combined(total);
    for (int i = 0; i < this->Size(); ++i) {
        combined.Set(i, this->At(i));
    }
    for (int j = 0; j < otherArray->Size(); ++j) {
        combined.Set(j + this->Size(), otherArray->At(j));
    }
    return new ImmutableArraySequence<T>(combined);
}

template <typename T>
ISequence<T>* ImmutableArraySequence<T>::AddToEnd(T item) {
    auto* copy = new ImmutableArraySequence<T>(*this);
    copy->ArraySequence<T>::AddToEnd(item);
    return copy;
}

template <typename T>
ISequence<T>* ImmutableArraySequence<T>::AddToFront(T item) {
    auto* copy = new ImmutableArraySequence<T>(*this);
    copy->ArraySequence<T>::AddToFront(item);
    return copy;
}

template <typename T>
ISequence<T>* ImmutableArraySequence<T>::Insert(T item, int index) {
    auto* copy = new ImmutableArraySequence<T>(*this);
    copy->ArraySequence<T>::Insert(item, index);
    return copy;
}

template <typename T>
ISequence<T>* ImmutableArraySequence<T>::Delete(int index) {
    auto* copy = new ImmutableArraySequence<T>(*this);
    copy->ArraySequence<T>::Delete(index);
    return copy;
}

template <typename T>
ISequence<T>* ImmutableArraySequence<T>::GetReference() {
    return this->Copy();
}

template <typename T>
ISequence<T>* ImmutableArraySequence<T>::Copy() const {
    return new ImmutableArraySequence<T>(*this);
}
