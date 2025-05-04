#pragma once
#include "sequence.hpp"
#include "dynamic_array.hpp"
#include "errors.hpp"
#include <stdexcept>

template <typename T>
class ArraySequence : public ISequence<T> {
protected:
    DynamicArray<T>* array;

    ISequence<T>* CreateFromArray(DynamicArray<T>* arr) const {
        return new ArraySequence<T>(*arr);
    }

public:
    ArraySequence() : array(new DynamicArray<T>(0)) {}

    ArraySequence(T* items, int count) : array(new DynamicArray<T>(items, count)) {}

    ArraySequence(const ArraySequence<T>& other) : array(new DynamicArray<T>(*other.array)) {}

    explicit ArraySequence(const DynamicArray<T>& arr) : array(new DynamicArray<T>(arr)) {}

    ~ArraySequence() override {
        delete array;
    }

    T Front() const override {
        if (array->GetSize() == 0) throw Errors::EmptyContainer();
        return array->Get(0);
    }

    T Back() const override {
        if (array->GetSize() == 0) throw Errors::EmptyContainer();
        return array->Get(array->GetSize() - 1);
    }

    T At(int index) const override {
        return array->Get(index);
    }

    int Size() const override {
        return array->GetSize();
    }

    ISequence<T>* Slice(int start, int end) const override {
        DynamicArray<T>* sub = array->GetSubArray(start, end);
        auto* result = new ArraySequence<T>(*sub);
        delete sub;
        return result;
    }

    ISequence<T>* Combine(const ISequence<T>* other) const override {
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

    ISequence<T>* AddToEnd(T item) override {
        array->Resize(array->GetSize() + 1);
        array->Set(array->GetSize() - 1, item);
        return this;
    }

    ISequence<T>* AddToFront(T item) override {
        array->Resize(array->GetSize() + 1);
        for (int i = array->GetSize() - 1; i > 0; i--) {
            array->Set(i, array->Get(i - 1));
        }
        array->Set(0, item);
        return this;
    }

    ISequence<T>* Insert(T item, int index) override {
        if (index < 0 || index > array->GetSize()) throw Errors::IndexOutOfRange();
        array->Resize(array->GetSize() + 1);
        for (int i = array->GetSize() - 1; i > index; i--) {
            array->Set(i, array->Get(i - 1));
        }
        array->Set(index, item);
        return this;
    }

    ISequence<T>* Delete(int index) override {
        if (array->GetSize() == 0) throw Errors::EmptyContainer();
        array->Remove(index);
        return this;
    }

    ISequence<T>* GetReference() override {
        return this;
    }

    ISequence<T>* Copy() const override {
        return new ArraySequence<T>(*this);
    }
};

template <typename T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    using ArraySequence<T>::ArraySequence;

    ISequence<T>* Combine(const ISequence<T>* other) const override {
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

    ISequence<T>* AddToEnd(T item) override {
        auto* copy = new ImmutableArraySequence<T>(*this);
        copy->ArraySequence<T>::AddToEnd(item);
        return copy;
    }

    ISequence<T>* AddToFront(T item) override {
        auto* copy = new ImmutableArraySequence<T>(*this);
        copy->ArraySequence<T>::AddToFront(item);
        return copy;
    }

    ISequence<T>* Insert(T item, int index) override {
        auto* copy = new ImmutableArraySequence<T>(*this);
        copy->ArraySequence<T>::Insert(item, index);
        return copy;
    }

    ISequence<T>* Delete(int index) override {
        auto* copy = new ImmutableArraySequence<T>(*this);
        copy->ArraySequence<T>::Delete(index);
        return copy;
    }

    ISequence<T>* GetReference() override {
        return this->Copy();
    }

    ISequence<T>* Copy() const override {
        return new ImmutableArraySequence<T>(*this);
    }
};