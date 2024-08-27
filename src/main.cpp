#include "../include/graph.h"
#include "gtest/gtest.h"

int main(int argc, char *argv[])
{
    if (false) // make false to run unit tests
    {
        // debug section
        Graph<int> g1;
        int a{4};
        int b{5};
        int c{6}, d{10};
        g1.addVertex(a);
        g1.addVertex(b);
        g1.addVertex(c);
        g1.addVertex(d);
        // auto f{[](const int a, const int b){return a == b;}};
        g1.addEdge(a, b, 1);
        g1.addEdge(b, c, 1);
        g1.addEdge(a, c, 5);
        g1.addEdge(b, a, 5); // 有向圖
        g1.addEdge(c, d, 2);

        std::vector<int> ad_sp={4, 5, 6, 10}; // a->b->c->d
        g1.findShortestPath(a, d);


    }
    else {
        ::testing::InitGoogleTest(&argc, argv);
        std::cout << RUN_ALL_TESTS() << std::endl;
    }
    return 0;
}
