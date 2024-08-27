#include "gtest/gtest.h"
#include "../include/graph.h"


TEST(APHW5TEST, test1)
{
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
    EXPECT_EQ(g1.getNumEdges(), 5);
    EXPECT_EQ(g1.getNeighbors(a).size(), 2);
    EXPECT_EQ(g1.findShortestPath(a, b).size(), 2);
    EXPECT_EQ(g1.findShortestPath(a, d), ad_sp);

}
// ARCHIVE: 發現這是有向圖
TEST(APHW5TEST, test2)
{
    // https://www.youtube.com/watch?v=pVfj6mxhdMw
    // Initialize the graph

    /*

    a --> b-----> c
    |    /|     />
    |   / |   /
    |  /  |  /
    ˇ </  ˇ /
    d---> e

    */
    Graph<char> g2;
    char a{'a'}, b{'b'}, c{'c'}, d{'d'}, e{'e'};
    char x{'x'};
    g2.addVertex(a);
    g2.addVertex(b);
    g2.addVertex(c);
    g2.addVertex(d);
    g2.addVertex(e);
    g2.addVertex(x);
    // auto f{[](const int a, const int b){return a == b;}};
    g2.addEdge(a, b, 6.1);
    g2.addEdge(a, d, 1);
    g2.addEdge(b, c, 5);
    g2.addEdge(b, d, 2);
    g2.addEdge(d, e, 1.3);
    g2.addEdge(e, c, 5);
    g2.addEdge(b, e, 2);

    EXPECT_EQ(g2.getNumEdges(), 7);
    std::vector<char> neighbors_a{'b', 'd'}; // b, d
    std::vector<char> neighbors_b{'c', 'd', 'e'}; // c, d, e
    std::vector<char> neighbors_e{'c'}; // c

    EXPECT_EQ(g2.getNeighbors(a), neighbors_a);
    EXPECT_EQ(g2.getNeighbors(b), neighbors_b);
    EXPECT_EQ(g2.getNeighbors(e), neighbors_e);
    EXPECT_EQ(g2.getNeighbors(x), std::vector<char>{});

    // EXPECT_EQ(g2.getNumSCC(), 2);

    // ShortestPath: a-c, the longest path on the graph
    std::vector<char> ac_sp{'a', 'd', 'e', 'c'}; // a-d-e-c
    EXPECT_EQ(g2.findShortestPath(a, c), ac_sp);
    // a)-b-d)-c-e)
    // BFS
    std::vector<char> bfs_result_a{'a', 'b', 'd', 'c', 'e'};
    EXPECT_EQ(g2.bfs(a), bfs_result_a);
    // b)-c-d-e)
    std::vector<char> bfs_result_b{'b', 'c', 'd', 'e'};
    EXPECT_EQ(g2.bfs(b), bfs_result_b);

    // BFS: dangling node
    std::vector<char> bfs_dangling_x{'x'};
    EXPECT_EQ(g2.bfs(x), bfs_dangling_x);
    std::vector<char> bfs_dangling_c{'c'};
    EXPECT_EQ(g2.bfs(c), bfs_dangling_c);

    // ShortestPath: non-existent sp
    std::vector<char> empty_sp{};
    EXPECT_EQ(g2.findShortestPath(a, x), empty_sp);
    EXPECT_EQ(g2.findShortestPath(x, a), empty_sp);
}


// TEST(APHW5TEST, test3)
// {

//     Node* root = nullptr;

//     root = insert(root, 10);
//     root = insert(root, 5);
//     root = insert(root, 15);
//     root = insert(root, 2);
//     root = insert(root, 7);
//     root = insert(root, 12);
//     root = insert(root, 17);

//     printInOrder(root);
//     cout << endl;

//     Node* node = find(root, 10);
//     EXPECT_EQ(node->value, 10);

//     node = find(root, 11);
//     EXPECT_EQ(node, nullptr);

//     deleteNode(root, 10);
//     Node* node = find(root, 10);
//     EXPECT_EQ(node->value, nullptr);

//     printInOrder(root);
// }