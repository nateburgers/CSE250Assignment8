#include "application.h"
#include <iterator>
#include <assert.h>

Application *sharedApplication()
{
    static Application *application;
    static bool onceToken = true;
    if (onceToken) {
        application = new Application();
    }
    onceToken = false;
    return application;
}

inline void retain(Reference reference)
{
    sharedApplication()->retain(reference);
}

inline void release(Reference reference)
{
    sharedApplication()->release(reference);
}

Application::Application()
{
    _automaticReleasePool = new std::set<Reference>();
    _retainCountsByObject = new std::map<Reference, UInt>();
}

void Application::retain(Reference reference)
{
    if (_retainCountsByObject->find(reference) == _retainCountsByObject->end()) {
        _retainCountsByObject->at(reference) = 1;
    } else {
        _retainCountsByObject->at(reference) ++;
    }
}

void Application::release(Reference reference)
{
    std::map<Reference, UInt>::iterator iteratorAtReference = _retainCountsByObject->find(reference);

    if(iteratorAtReference == _retainCountsByObject->end()) assert(false);

//    if (*iteratorAtReference < 1) {
//        assert(false);
//    } else if (retainCount == 1) {

//    } else {
//        (*_retainCountsByObject)[reference] = retainCount + 1;
//    }
}
