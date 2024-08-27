
#include <iostream>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <limits>
#include <queue>
#include "graph.h"

template<typename T>
void Graph<T>::addVertex(const T&v){
    Node* node= new Node(v);
    head[v] = node;
}
template<typename T>
void Graph<T>::addEdge(const T& v1, const T&v2, int w){
    if (head.find(v1) == head.end()){
        std::cerr << "Vertex " << v1 << " not found" << std::endl;
        return;
    }

    // make a new node for v2
    // try to find if node2 is already node1's neighbor
    Node* n2 = head.at(v1)->findVertex(v2);
    // case 1: v2 is in the list
    if (n2){
        // if v2 is already in the list, update the weight
        n2->weight = w;
        return;
    }
    // case 2: v2 is not in the list, make a new node
    // no known conversion for argument 1 from ‘const int’ to ‘const Graph<int>::Node&’
    n2 = new Node(v2);
    n2->weight = w;
    // we add it to the front of the list to avoid traversing the list
    n2->next = head[v1]->next;
    head[v1]->next = n2;
    numEdges++;
}

template<typename T>
size_t Graph<T>::getNumEdges() const{
    return numEdges;
}


template<typename T>
std::vector<T> Graph<T>::getNeighbors(T vertex) const{
    Node* p_node = head.at(vertex); // if not found, throw an exception
    Node* nei = p_node->next;
    std::vector<T> neighbors;
    while (nei){
        neighbors.push_back(nei->value);
        nei = nei->next;
    }
    // sort the neighbors from small to large, to ensure the order
    std::sort(neighbors.begin(), neighbors.end());
    return neighbors;
}


template<typename T>
bool Graph<T>::isConnected(T source, T destination) const{
    std::unordered_set<T> visited;
    return dfs(source, destination, visited);
}


template<typename T>
bool Graph<T>::dfs(T source, T destination, std::unordered_set<T>&visited) const{
    if (source == destination){
        return true;
    }
    visited.insert(source);
    // if this lookup does not intend to insert, use at() instead of []
    // furthermore, declare `const` function prevents []
    Node* temp = head.at(source)->next;
    while (temp){
        if (visited.find(temp->value) == visited.end()){
            if (dfs(temp->value, destination, visited)){
                return true;
            }
        }
        temp = temp->next;
    }
    return false;
}


template<typename T>
std::vector<T> Graph<T>::bfs(int vertex){
    std::vector<T> path;
    std::queue<T> q;
    std::unordered_set<T> visited;
    q.push(vertex);
    visited.insert(vertex);

    while (!q.empty()){
        T current = q.front();
        q.pop();
        path.push_back(current);
        // traverse the neighbors of `curr_node`

        std::vector<T> neighbors = getNeighbors(current);
        for (T n:neighbors){
            if (visited.find(n) == visited.end()){
                q.push(n);
                visited.insert(n);
            }
        }
    }
    return path;
}



template<typename T>
std::vector<T> Graph<T>::findShortestPath(T source, T destination) const{
    // find src_node first
    if (head.find(source) == head.end()){
        std::cerr << "Vertex " << source << " not found" << std::endl;
        return {};
    }

    // find SP: (unweighted) BFS, (weighted) Dijkstra
    // 1. initialization
    auto comp = [](const Node* n1, const Node* n2){
        return n1->weight > n2->weight;
    };
    std::priority_queue<Node*, std::vector<Node*>, decltype(comp)> pq(comp);
    std::unordered_set<T> visited;
    std::unordered_map<T, size_t> distance; // std::numeric_limits<T>::max()>
    std::unordered_map<T, T> parent;
    for (auto& [key, n]: head){
        parent[n->value] = n->value;
        distance[n->value] = std::numeric_limits<size_t>::max(); // weight is `int` type
    }

    // 2. prepare for the first node
    pq.push(head.at(source));
    distance[source] = 0;
    // 3. main loop of Dijkstra
    while (!pq.empty()){
        Node* _curr = pq.top(); pq.pop();
        Node* curr = head.at(_curr->value);
        T min_ = curr->value;
        // std::cout << "min_=" << min_ << std::endl;
        if (visited.find(min_) != visited.end()) {continue;}
        visited.insert(min_);
        Node* temp = curr->next;
        // traverse the neighbors of `curr_node`
        while (temp){
            if (visited.find(temp->value) == visited.end() &&
            ((distance[curr->value] + temp->weight) < distance[temp->value])){
                // note that we only record one shortest path in this way
                distance[temp->value] = distance[curr->value] + temp->weight;
                parent[temp->value] = curr->value;
                pq.push(temp);

            }
            temp = temp->next;
        }
    }// print parent

    // backtrack the parent vector
    std::vector<T> path;
    backtrack(destination, parent, path);
    std::reverse(path.begin(), path.end());
    if (path[0] != source){
        return {};
    }
    return path;
}
template<typename T>
void Graph<T>::backtrack(T destination, std::unordered_map<T, T> parent, std::vector<T>& path) const{
    while (destination != parent[destination]){
        path.push_back(destination);
        destination = parent[destination];
    }
    path.push_back(destination);

}




// explicit instantiation
template class Graph<int>;
template class Graph<char>;
template class Graph<float>;
template class Graph<double>;
