#ifndef OBJECT_H
#define OBJECT_H

#include "prefix.h"

class Object
{
private:
    const UniqueIdentifier _uniqueIdentifier;
public:
    Object();
    const UniqueIdentifier uniqueIdentifier() const;

    ~Object();
};

typedef Object &ObjectReference;
typedef void (Object::*Callback)(void);

struct Action {
    ObjectReference object;
    const Callback callback;
    Action(ObjectReference object, const Callback callback) :
        object(object),
        callback(callback) {}
    void operator()();
};

#endif // OBJECT_H
