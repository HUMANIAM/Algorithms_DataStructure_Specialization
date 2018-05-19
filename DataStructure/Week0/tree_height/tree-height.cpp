#include <algorithm>
#include <iostream>
#include <vector>
#include <algorithm>
#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
#endif

class Node;

class Node {
public:
    int key;
    Node *parent;
    std::vector<Node *> children;

    Node() {
      this->parent = NULL;
    }

    void setParent(Node *theParent) {
      parent = theParent;
      parent->children.push_back(this);
    }
};

int TreeHeight(Node * node, int myheight){
  int maxheight = myheight;

  for(int child = 0; child < node->children.size(); child++){
    maxheight = std::max(maxheight, TreeHeight(node->children[child], myheight+1));
  }
  return maxheight;

}
int main_with_large_stack_space() {
  std::ios_base::sync_with_stdio(0);
  int n;
  Node * root;
  std::cin >> n;

  std::vector<Node> nodes(n);
  for (int child_index = 0; child_index < n; child_index++) {
    int parent_index;
    std::cin >> parent_index;
    if (parent_index >= 0)
      nodes[child_index].setParent(&nodes[parent_index]);
    else
      root = &nodes[child_index];
    nodes[child_index].key = child_index;
  }

  // Replace this code with a faster implementation
  int maxHeight;
  if(n == 0){
    maxHeight =  0;
  }
  else{
    maxHeight = TreeHeight(root, 1);
  }

  std::cout << maxHeight << std::endl;
  return 0;
}

int main (int argc, char **argv)
{
#if defined(__unix__) || defined(__APPLE__)
  // Allow larger stack space
  const rlim_t kStackSize = 16 * 1024 * 1024;   // min stack size = 16 MB
  struct rlimit rl;
  int result;

  result = getrlimit(RLIMIT_STACK, &rl);
  if (result == 0)
  {
      if (rl.rlim_cur < kStackSize)
      {
          rl.rlim_cur = kStackSize;
          result = setrlimit(RLIMIT_STACK, &rl);
          if (result != 0)
          {
              std::cerr << "setrlimit returned result = " << result << std::endl;
          }
      }
  }

#endif
  return main_with_large_stack_space();
}
