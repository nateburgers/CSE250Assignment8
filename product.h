#ifndef PRODUCT_H
#define PRODUCT_H

#include "application.h"
#include "object.h"

template<typename A, typename B>
class Product : public Object
{
private:
    const A _firstValue;
    const B _secondValue;
public:
    Product(const A firstValue, const B secondValue) :
        Object(), _firstValue(firstValue), _secondValue(secondValue) {}

    const A firstValue() const;
    const B secondValue() const;
};

#endif // PRODUCT_H
