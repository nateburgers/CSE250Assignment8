#ifndef PAIR_H
#define PAIR_H

#include "object.h"

template<typename A, typename B>
class Pair : public Object
{
private:
    const A _first;
    const B _second;
public:
    Pair(A first, B second) : _first(first), _second(second) {}

    const A first() const {
        return _first;
    }

    const B second() const {
        return _second;
    }
};

#endif // PAIR_H
