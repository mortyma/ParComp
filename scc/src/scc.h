#ifndef SCC_H
#define SCC_H

#include "graph.h"
#include <stack>

class SCC {
public:
    typedef std::vector<size_t> Component;
    typedef std::vector<Component*> Components;

    SCC(Graph* graph);
    ~SCC();

    Components get();

private:
    void scc(size_t node);

private:
    bool* visited;
    bool* on_stack;
    size_t* idx;
    size_t* low_link;
    std::stack<size_t> stack;
    Components components;
    Graph* g;
    size_t cnt = 0;


};

#endif /* SCC_H */
