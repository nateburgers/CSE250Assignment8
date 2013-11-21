#ifndef LIST_H
#define LIST_H

#include "object.h"
#include "application.h"
#include "sum.h"
#include <assert.h>
#include <iostream>
#include <exception>

enum ListType {
    ListTypeNil,
    ListTypeCons
};

template<typename T>
class List : public Sum<ListType>
{
protected:
    List(ListType type) : Sum(type) {}
public:
    typedef List<const List<T>*> Matrix;

    const virtual T first() const {
        throw new std::exception();
    }
    const virtual List<T> *rest() const {
        throw new std::exception();
    }

    class Nil : public List<T> {
    public:
        Nil() : List(ListTypeNil) {}
    };

    class Cons : public List<T> {
    private:
        const T _first;
        const List<T> *_rest;
    public:
        Cons(const T first, const List<T> *rest) : List(ListTypeCons), _first(first), _rest(rest) {
//            sharedApplication()->retain(first);
//            sharedApplication()->retain(rest);
        }

        const virtual T first() const {
            return _first;
        }

        const virtual List<T> *rest() const {
            return _rest;
        }
    };
};

#endif // LIST_H
