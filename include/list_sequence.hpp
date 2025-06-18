#pragma once
#include "sequence.hpp"
#include "linked_list.hpp"
#include "errors.hpp"
#include <stdexcept>

template <typename T>
class ListSequence : public ISequence<T> {
protected:
    LinkedList<T>* list;

    ISequence<T>* CreateFromList(LinkedList<T>* lst) const {
        return new ListSequence<T>(*lst);
    }

public:
    ListSequence() : list(new LinkedList<T>()) {}

    ListSequence(T* items, int count) : list(new LinkedList<T>(items, count)) {}

    ListSequence(const ListSequence<T>& other) : list(new LinkedList<T>(*other.list)) {}

    explicit ListSequence(const LinkedList<T>& lst) : list(new LinkedList<T>(lst)) {}

    ~ListSequence() override {
        delete list;
    }

    T Front() const override {
        return list->GetFirst();
    }

    T Back() const override {
        return list->GetLast();
    }

    T At(int index) const override {
        return list->Get(index);
    }

    int Size() const override {
        return list->GetLength();
    }

    ISequence<T>* Slice(int start, int end) const override {
        LinkedList<T>* sub = list->GetSubList(start, end);
        auto* result = new ListSequence<T>(*sub);
        delete sub;
        return result;
    }

    ISequence<T>* Combine(const ISequence<T>* other) const override {
        const auto* otherList = dynamic_cast<const ListSequence<T>*>(other);
        if (!otherList) throw Errors::TypeMismatch();
        
        LinkedList<T>* result = list->Concat(otherList->list);
        return CreateFromList(result);
    }

    ISequence<T>* AddToEnd(T item) override {
        list->Append(item);
        return this;
    }

    ISequence<T>* AddToFront(T item) override {
        list->Prepend(item);
        return this;
    }

    ISequence<T>* Insert(T item, int index) override {
        list->InsertAt(item, index);
        return this;
    }

    ISequence<T>* Delete(int index) override {
        if (list->GetLength() == 0) throw Errors::EmptyContainer();
        list->Remove(index);
        return this;
    }

    ISequence<T>* GetReference() override {
        return this;
    }

    ISequence<T>* Copy() const override {
        return new ListSequence<T>(*this);
    }
};

template <typename T>
class ImmutableListSequence : public ListSequence<T> {
public:
    using ListSequence<T>::ListSequence;

    ISequence<T>* Combine(const ISequence<T>* other) const override {
        const auto* otherList = dynamic_cast<const ImmutableListSequence<T>*>(other);
        if (!otherList) throw Errors::TypeMismatch();

        LinkedList<T> combined;
        for (int i = 0; i < this->Size(); ++i) {
            combined.Append(this->At(i));
        }
        for (int j = 0; j < otherList->Size(); ++j) {
            combined.Append(otherList->At(j));
        }

        return new ImmutableListSequence<T>(combined);
    }

    ISequence<T>* AddToEnd(T item) override {
        auto* copy = new ImmutableListSequence<T>(*this);
        copy->ListSequence<T>::AddToEnd(item);
        return copy;
    }

    ISequence<T>* AddToFront(T item) override {
        auto* copy = new ImmutableListSequence<T>(*this);
        copy->ListSequence<T>::AddToFront(item);
        return copy;
    }

    ISequence<T>* Insert(T item, int index) override {
        auto* copy = new ImmutableListSequence<T>(*this);
        copy->ListSequence<T>::Insert(item, index);
        return copy;
    }

    ISequence<T>* Delete(int index) override {
        auto* copy = new ImmutableListSequence<T>(*this);
        copy->ListSequence<T>::Delete(index);
        return copy;
    }

    ISequence<T>* GetReference() override {
        return this->Copy();
    }

    ISequence<T>* Copy() const override {
        return new ImmutableListSequence<T>(*this);
    }
};


