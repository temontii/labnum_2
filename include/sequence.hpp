#pragma once

#include <stdexcept>
#include <typeinfo>
#include "errors.hpp"

template <typename T>
class ISequence {
public:
    virtual ~ISequence() = default;

    virtual T Front() const = 0;
    virtual T Back() const = 0;
    virtual T At(int position) const = 0;
    virtual int Size() const = 0;

    virtual ISequence<T>* Slice(int start_pos, int end_pos) const = 0;
    virtual ISequence<T>* Combine(const ISequence<T>* other) const = 0;

    virtual ISequence<T>* AddToEnd(T element) = 0;
    virtual ISequence<T>* AddToFront(T element) = 0;
    virtual ISequence<T>* Insert(T element, int position) = 0;
    virtual ISequence<T>* Delete(int position) = 0;

    virtual ISequence<T>* GetReference() = 0;
    virtual ISequence<T>* Copy() const = 0;
};

template<typename T>
ISequence<T>* operator+(const ISequence<T>& first, const ISequence<T>& second) {
    if (typeid(first) != typeid(second)) {
        throw Errors::TypeMismatch("Cannot concatenate sequences of different types");
    }
    return first.Combine(&second);
}

template<typename T>
bool operator==(const ISequence<T>& first, const ISequence<T>& second) {
    if (first.Size() != second.Size()) return false;
    
    for (int i = 0; i < first.Size(); ++i) {
        if (first.At(i) != second.At(i)) {
            return false;
        }
    }
    return true;
}

template<typename T>
bool operator!=(const ISequence<T>& first, const ISequence<T>& second) {
    return !(first == second);
}
