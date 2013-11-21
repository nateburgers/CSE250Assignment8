//
//  main.cpp
//  Assignment8
//
//  Created by Nathan Burgers on 11/13/13.
//  Copyright (c) 2013 Nathan Burgers. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <set>
#include <assert.h>
#include <algorithm>
#include <set>
#include <queue>
#include <tuple>

using namespace std;

typedef unsigned int uint;
typedef vector<vector<int>*> matrix;

void print_vector(const vector<int> &v)
{
    cout << "[" << v[0];
    for (size_t i=1; i<v.size(); i++) {
        cout << "," << v[i];
    }
    cout << "]" << endl;
}

void write_vector(const vector<int> &v, const string &fileName)
{
    ofstream fileStream;
    fileStream.open(fileName);
    fileStream << v[0];
    for (size_t i=1; i<v.size(); i++) {
        fileStream << " " << v[i];
    }
    fileStream.close();
}

vector<string> *parse(string &input, size_t idx, vector<string> *builder)
{
    if (idx == input.size()) return builder;
    if (input[idx] == ' ') {
        if (!(builder->back() == "")) {
            builder->push_back("");
        }
        return parse(input, idx+1, builder);
    } else {
        builder->back() += input[idx];
        return parse(input, idx+1, builder);
    }
}

vector<string> *parse(string &input)
{
    vector<string> *results = new vector<string>();
    results->push_back("");
    return parse(input, 0, results);
}

vector<int> *parseInts(string &input)
{
    vector<int> *result = new vector<int>();
    vector<string> *parsedInput = parse(input);
    for(size_t i=0; i<parsedInput->size(); i++) {
        result->push_back(stoi((*parsedInput)[i]));
    }
    return result;
}

vector<vector<int>*> *parseIntFile(const string &fileName)
{
    vector<vector<int>*> *result = new vector<vector<int>*>();
    ifstream file(fileName);
    string line;
    while(getline(file, line)) {
        result->push_back(parseInts(line));
    }
    return result;
}

bool intCompare(int a, int b) { return a < b; }
void sortMatrix(vector<vector<int>*> &matrix)
{
    for(size_t i=0; i<matrix.size(); i++){
        std::sort(matrix[i]->begin(), matrix[i]->end(), intCompare);
    }
}

/*
 * Fast Intersect
 * We create a product type that acts as the delayed argument and results
 * of indexing a vector (sorted,) whose comparisons operate on the result.
 * This way we avoid having to maintain parallel data structures in the fast_unify
 * algorithm.
 */
template<typename T>
class maybe {
public:
    const T value;
    const bool nothing;

    maybe () : nothing(true), value((T)0) {}
    maybe (const T &value) : nothing(false), value(value) {}

    maybe<T> operator|| (const maybe<T> &other) const {
        if (!nothing && !other.nothing) {
            return *this;
        } else if (!nothing && other.nothing) {
            return *this;
        } else if (nothing && !other.nothing) {
            return other;
        } else {
            return maybe<T>();
        }
    }
};
template<typename T>
maybe<T> nothing() { return maybe<T>(); }
template<typename T>
maybe<T> just(const T &value) { return maybe<T>(value); }

template<typename T>
struct delayed_index {
    const T value;
    const size_t idx;
    const vector<int> *values;

    delayed_index(const size_t idx, const vector<T> *values) :
        value((*values)[idx]), idx(idx), values(values) {
    }
    delayed_index(const vector<T> *values) :
        value((*values)[0]), idx(0), values(values) {
    }

    bool atEnd() const {
        return idx >= values->size() - 1;
    }
    delayed_index *next() const {
        return new delayed_index(idx+1, values);
    }
};

template<typename T>
struct compare_delayed_index {
    bool operator() (delayed_index<T> *a, delayed_index<T> *b) {
        return a->value >= b->value;
    }
};

/*
 * Hand-Rolling ugly lambdas
 */
enum comparison_result {
    comparison_result_less = -1,
    comparison_result_equal = 0,
    comparison_result_more = 1
};

template<typename T>
class comparator {
public:
    virtual comparison_result operator()(const T &value) const =0;
};

template<typename T>
class reverse_comparison : public comparator<T> {
private:
    const comparator<T> &sub_comparator;
public:
    reverse_comparison (const comparator<T> &sub_comparator) : sub_comparator(sub_comparator) {}
    virtual comparison_result operator()(const T &value) const {
        switch(sub_comparator(value)) {
        case comparison_result_less: return comparison_result_more;
        case comparison_result_equal: return comparison_result_equal;
        case comparison_result_more: return comparison_result_less;
        }
    }
};

class compare_integer : public comparator<int> {
private:
    const int operand;
public:
    compare_integer (const int &operand) : operand(operand) {}
    virtual comparison_result operator()(const int &value) const {
        return value == operand ? comparison_result_equal
                                : value < operand ? comparison_result_less
                                                  : comparison_result_more ;
    }
};

class greater_integer : public comparator<int> {
private:
    const int operand;
public:
    greater_integer (const int &operand) : operand(operand) {}
    virtual comparison_result operator()(const int &value) const {
        return value >= operand ? comparison_result_equal
                                : comparison_result_more ;
    }
};

/*
 * Divide and Conquer binary search
 */
template<typename T>
inline T middle(const vector<T> &vec) {
    return vec[vec.size() / 2];
}
template<typename T>
inline vector<T> *lower(const vector<T> &vec) {
    return new vector<T>(vec.begin(), vec.end() - vec.size() / 2);
}
template<typename T>
inline vector<T> *upper(const vector<T> &vec) {
    return new vector<T>(vec.begin() + vec.size() / 2, vec.end());
}

template<typename T>
maybe<delayed_index<T>*> binary_search(const comparator<T> &compare, const vector<T> &vec)
{
    const size_t middleIdx = vec.size() / 2;
    const comparison_result result = compare(vec[middleIdx]);

    if (vec.size() <= 1 && result != comparison_result_equal) return nothing<delayed_index<T>*>();

    switch (result) {
    case comparison_result_equal: return just<delayed_index<T>*>(new delayed_index<T>(middleIdx, &vec));
    case comparison_result_less: return binary_search(compare, upper<T>(vec));
    case comparison_result_more: return binary_search(compare, lower<T>(vec));
    default: return nothing<delayed_index<T>*>();
    }
}

template<typename T>
maybe<delayed_index<T>*> first_satisfying(const comparator<T> &compare, const vector<T> &vec)
{
    const size_t middleIdx = vec.size() / 2;
    const comparison_result result = compare(vec[middleIdx]);

    if (vec.size() <= 1 && result != comparison_result_equal) return nothing<delayed_index<T>*>();

    switch (result) {
    case comparison_result_equal: return first_satisfying<T>(compare, *lower(vec))
                || just<delayed_index<T>*>(new delayed_index<T>(middleIdx, &vec));
    case comparison_result_less: return first_satisfying<T>(compare, *upper(vec));
    case comparison_result_more: return first_satisfying<T>(compare, *lower(vec));
    }
}

bool contains(const vector<int> &m, int i)
{
    size_t middleIdx = m.size() / 2;
    if (m.size() <= 1 && m[middleIdx] != i) return false;

    if (m[middleIdx] == i) {
        return true;
    } else if (m[middleIdx] > i) {
        return contains(*lower<int>(m), i);
    } else if (m[middleIdx] < i) {
        return contains(*upper<int>(m), i);
    } else {
        return false;
    }
}

/*
 * General idea stolen from monad comprehensions:
 * comprehend :: m a -> (a -> m b) -> m b
 */
vector<int> *intersection(const vector<int> &a, const vector<int> &b)
{
    vector<int> *result = new vector<int>();
    for (size_t i=0; i<a.size(); i++) {
        if (contains(b, a[i])) {
            result->push_back(a[i]);
        }
    }
    return result;
}

/*
 * General idea stolen from folding:
 * fold :: [a] -> (a -> a -> a) -> a
 */
vector<int> *intersection(const matrix &vectors)
{
    vector<int> *result = vectors[0];
    for (size_t i=1; i<vectors.size(); i++) {
        result = intersection(*result, *vectors[i]);
    }
    return result;
}

set<int> *toSet(const vector<int> &v)
{
    return new set<int>(v.begin(), v.end());
}

vector<int> *toVector(const set<int> &s)
{
    return new vector<int>(s.begin(), s.end());
}

vector<set<int>*> *mapToSet(const vector<vector<int>*> &vectors)
{
    vector<set<int>*> *result = new vector<set<int>*>();
    for (size_t i=0; i<vectors.size(); i++) {
        result->push_back(toSet(*vectors[i]));
    }
    return result;
}

/*
 * Re-Using the concept of folding over a concatenative operator,
 * but cpp makes it hard to generalize our algorithms without
 * templated typedefs; which exist in cpp11 >.>
 */
set<int> *unify(const vector<int> &a, const vector<int> &b)
{
    set<int> *result = toSet(a);
    for (size_t i=0; i<b.size(); i++) {
        result->insert(b[i]);
    }
    return result;
}

set<int> *unify(const vector<set<int>*> &sets)
{
    set<int> *result = sets[0];
    for (size_t i=1; i<sets.size(); i++) {
        result->insert(sets[i]->begin(), sets[i]->end());
    }
    return result;
}

typedef priority_queue<
    delayed_index<int>*,
    vector<delayed_index<int>*>,
    compare_delayed_index<int>
    > index_queue;
index_queue *prioritize(const vector<vector<int>*> &sets)
{
    index_queue *queue = new index_queue();
    for (size_t i=0; i<sets.size(); i++) {
        queue->push(new delayed_index<int>(sets[i]));
    }
    return queue;
}

vector<int> *fast_unify(const vector<vector<int>*> &sets)
{
    vector<int> *result = new vector<int>();
    index_queue *queue = prioritize(sets);
    while (queue->size() > 0) {
        delayed_index<int> *top = queue->top();
        queue->pop();
        if (! top->atEnd()) queue->push(top->next());
        if (result->size() == 0 || result->back() != top->value) result->push_back(top->value);
    }
    return result;
}

/*
 * Fast Intersection
 */
vector<delayed_index<int>*> *to_delayed_index(const vector<vector<int>*> &sets)
{
    vector<delayed_index<int>*> *result = new vector<delayed_index<int>*>();
    for (size_t i=0; i<sets.size(); i++) {
        result->push_back(new delayed_index<int>(sets[i]));
    }
    return result;
}

vector<int> *fast_intersection(const vector<vector<int>*> &sets)
{
    vector<int> *result = new vector<int>();
    vector<delayed_index<int>*> *delayed_indices = to_delayed_index(sets);
    delayed_index<int> *current_index = (*delayed_indices)[0];
    while (delayed_indices->size() > 0) {
        for (size_t i=0; i<delayed_indices->size(); i++) {
            delayed_index<int> *compared_index = (*delayed_indices)[i];
            maybe<delayed_index<int>*> greater = first_satisfying<int>(greater_integer(current_index->value),
                                                                  *compared_index->values);
        }
    }
    return result;
}

/*
 * Command Map
 */

enum RETURN_STATUS {
    RETURN_SUCCESS,
    RETURN_FAILURE,
    RETURN_EXIT
};

typedef RETURN_STATUS (*command)(const string &);

struct named_command {
    string command_name;
    command c;
};

/*
 * Commands
 */
static vector<vector<int>*> *global_database;
RETURN_STATUS exit(const string &input) {
    return RETURN_EXIT;
}
RETURN_STATUS loaddb(const string &input) {
    global_database = parseIntFile(input);
    sortMatrix(*global_database);
    for (size_t i=0; i<global_database->size(); i++) {
        print_vector(*(*global_database)[i]);
    }
    return RETURN_SUCCESS;
}
RETURN_STATUS slowintersect(const string &input) {
    vector<int> *intersect = intersection(*global_database);
    print_vector(*intersect);
    if (input != "") write_vector(*intersect, input);
    return RETURN_SUCCESS;
}

RETURN_STATUS slowunion(const string &input) {
    vector<int> *union_vector = toVector(*unify(*mapToSet(*global_database)));
    print_vector(*union_vector);
    if (input != "") write_vector(*union_vector, input);
    return RETURN_SUCCESS;
}

RETURN_STATUS fastunion(const string &input) {
    vector<int> *union_vector = fast_unify(*global_database);
    print_vector(*union_vector);
    if (input != "") write_vector(*union_vector, input);
    return RETURN_SUCCESS;
}

int run() {
    map<string,command> command_map;
    command_map["exit"] = exit;
    command_map["loaddb"] = loaddb;
    command_map["slowintersect"] = slowintersect;
    command_map["slowunion"] = slowunion;
    command_map["fastunion"] = fastunion;

    RETURN_STATUS return_status = RETURN_SUCCESS;
    while (return_status != RETURN_EXIT) {
        cout << "> ";
        string command;
        getline(cin, command);
        const vector<string> *commands = parse(command);
        if (commands->size() < 2) {
            return_status = command_map[(*commands)[0]]("");
        } else {
            return_status = command_map[(*commands)[0]]((*commands)[1]);
        }
    }
    return EXIT_SUCCESS;
}

int main(int argc, const char * argv[])
{
    run();
    return 0;
}
