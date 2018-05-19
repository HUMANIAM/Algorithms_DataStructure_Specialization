#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

struct Node {
  int key;
  int left;
  int right;

  Node() : key(0), left(-1), right(-1) {}
  Node(int key_, int left_, int right_) : key(key_), left(left_), right(right_) {}
};

bool BSTCondition(int node, int parent, const vector<Node>& tree){
    int L, R, K;
    bool state = true;
    if(node == -1)
        return true;

    L = tree[node].left;
    R = tree[node].right;
    K = tree[node].key;
    /* check the condition of all nodes in the left are less than its parent
        and all nodes in the right are greater than their parent
    **/
    if(parent != -1)
       if(K > tree[parent].key && L != -1)
        state = tree[L].key >= tree[parent].key;
       else if(K < tree[parent].key && R != -1)
        state = tree[R].key < tree[parent].key;
        else{}

    /*check that every node its left child is less than it
      and its right child is greater than it
    **/
       if(L != -1 && R != -1)
         state = state && (K > tree[L].key ) && (K <= tree[R].key);
       else if(R != -1)
         state = state && (K <= tree[R].key);
       else if(L != -1)
        state = state && (K > tree[L].key);
       else
        return true;

       return state && BSTCondition(L, node, tree) && BSTCondition(R, node, tree);
}

bool IsBinarySearchTree(const vector<Node>& tree) {
  if(tree.size() == 0)
    return true;
  return BSTCondition(0, -1, tree);
}

int main() {
  int nodes;
  cin >> nodes;
  vector<Node> tree;
  for (int i = 0; i < nodes; ++i) {
    int key, left, right;
    cin >> key >> left >> right;
    tree.push_back(Node(key, left, right));
  }
  if (IsBinarySearchTree(tree)) {
    cout << "CORRECT" << endl;
  } else {
    cout << "INCORRECT" << endl;
  }
  return 0;
}
