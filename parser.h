#ifndef PARSER_H
#define PARSER_H

#include "object.h"
#include "list.h"
#include "pair.h"

template<typename Output, typename Input>
class Parser : public Object
{
    typedef Pair<Output, List<Input>*> Result;
    typedef List<Result *> ResultList;
public:
    const virtual ResultList *operator()(const List<Input> *input) const =0;

    class Unit : Parser {
    private:
        const Input _value;
    public:
        Unit(Input value) : _value(value) {}
        const virtual ResultList *operator()(const List<Input> *input) const {
            return new const typename ResultList::Cons(new const Result(_value, input), new const typename ResultList::Nil());
        }
    };

    class Zero : Parser {
    public:
        const virtual ResultList *operator()(const List<Input> *input) const {
            return new typename ResultList::Nil();
        }
    };

    class Char : Parser {
    public:
        const virtual ResultList *operator()(const List<Input> *input) const {
            if (input->first() != ' ') {
                return Unit(input->first())(input->rest());
            } else {
                return Zero()();
            }
        }
    };

    class Then : Parser {
    public:
        const virtual ResultList *operator()(const List<Input> *input) const {

        }
    };
};

#endif // PARSER_H
