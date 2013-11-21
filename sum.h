#ifndef SUM_H
#define SUM_H

#include "object.h"

template<typename A>
class Sum : public Object
{
private:
    const A _type;
public:
    Sum(A type) : _type(type) {}

    const A type() const {
        return _type;
    }
};

#endif // SUM_H
