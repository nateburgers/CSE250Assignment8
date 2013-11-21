#ifndef APPLICATION_H
#define APPLICATION_H

#include <set>
#include <map>
#include <unordered_map>
#include <tuple>
#include "prefix.h"
#include "object.h"

class Application : Object {
private:
    std::set<Reference> *_automaticReleasePool;
    std::map<Reference, UInt> *_retainCountsByObject;
public:
    Application();
    void run();

    void retain(Reference);
    void release(Reference);
    void autorelease(Reference);
};

Application *sharedApplication();

#endif // APPLICATION_H
