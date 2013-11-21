
#include "object.h"
#include <random>
#include "application.h"
#include "prefix.h"

Object::Object() :
    _uniqueIdentifier(generateUniqueIdentifier())
{}

const UniqueIdentifier Object::uniqueIdentifier() const
{
    return _uniqueIdentifier;
}

Object::~Object()
{

}

/*
 *Actions
 */
void Action::operator ()()
{
    (object.*callback)();
}
