#include <vector>
#include <iostream>
#include <functional>
#include <unordered_set>

template<typename T>
class Graph
{
public:
  void addVertex(const T&v);
  void addEdge(const T& v1, const T&v2, int w);
  size_t getNumEdges() const;
  size_t getNumVertices() const{ return head.size(); }; // inline function
  // set default function to `it`? what is `it`?
  std::vector<T> getNeighbors(T vertex) const;
  bool isConnected(T source, T destination) const;
  // strongly connected components or weakly connected components
  std::vector<T> bfs(int vertex);
  // int getNumConnectedComponents() const; // this is questionable as it is not clear if it means
  // size_t getNumSCC() const; // use this to replace getNumConnectedComponents
  std::vector<T> findShortestPath(T source, T destination) const;
  // vector<T> topologicalSort() const;

private:
    // Linked List
    class Node
    {
    public:
        T value;
        int weight{};
        Node* next{};
        Node(const T& v) : value(v), weight(0), next(nullptr) {}
        Node* findVertex(T v) {
            Node* temp= this->next;
            while (temp){
                if (temp->value == v){
                    return temp;
                }
                temp = temp->next;
            }
            return nullptr;
        }
    };
    // same-value vertices are not allowed
    std::unordered_map<int, Node*>head;
    size_t numEdges=0;
    void backtrack(T destination, std::unordered_map<T, T> parent, std::vector<T>& path) const;
    bool dfs(T source, T destination, std::unordered_set<T>&visited) const;

};

// struct Node { //// for part 2
//   int value;
//   Node* left;
//   Node* right;

//   Node(int value) {
//     this->value = value;
//     left = nullptr;
//     right = nullptr;
//   }
// };

