#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

typedef struct Vertex {
    int weight;
    int childsOptSum;
    vector <int> children;

    Vertex(): weight(0), childsOptSum(0)
    {}
} Vertex;

typedef vector<Vertex> Graph;
typedef vector<int> Sum;

Graph ReadTree() {
    int vertices_count;
    cin >> vertices_count;

    Graph tree(vertices_count);

    for (int i = 0; i < vertices_count; ++i)
        cin >> tree[i].weight;

    for (int i = 1; i < vertices_count; ++i) {
        int from, to;
        cin >> from >> to;
        tree[from - 1].children.push_back(to - 1);
        tree[to - 1].children.push_back(from - 1);
    }

    return tree;
}

int grandchildrenSum(Graph& tree, int vertex, int parent){
    int sum = 0;
    for(int child : tree[vertex].children)
        if(child != parent)
            sum += tree[child].childsOptSum;
    return sum;
}

int dfs(Graph &tree, int vertex, int parent) {
    if(tree[vertex].children.size() == 1 && tree[vertex].children[0] == parent){ //leaf node
        return tree[vertex].weight;
    }

    for (int child : tree[vertex].children)
        if (child != parent)
            tree[vertex].childsOptSum += dfs(tree, child, vertex);

    int m0 = tree[vertex].childsOptSum; //exclude the node from the max fun
    int m1 = grandchildrenSum(tree, vertex, parent) + tree[vertex].weight; // include the vertex

    return tree[vertex].weight = max(m0, m1);

}

int MaxWeightIndependentTreeSubset(Graph &tree) {
    size_t size = tree.size();
    if (size == 0)
        return 0;
    return dfs(tree, 0, -1);
}

int main() {
    // Here begins the solution
    Graph tree = ReadTree();
    int weight = MaxWeightIndependentTreeSubset(tree);
    cout << weight << endl;
    return 0;
}
