#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <stdlib.h>

using namespace std;

set<int> visited;
vector<int> order;
void dfs(vector<vector<int> >& adj, int x) {

    visited.insert(x);
    for(size_t i=0; i<adj[x].size(); i++){
        int w = adj[x][i];
        if(visited.find(w) == visited.end())
            dfs(adj, w);
    }
    order.push_back(x);
}

void toposort(vector<vector<int> >& adj) {

  for(size_t i=0; i<adj.size(); i++){
    if(visited.find(i) == visited.end())
        dfs(adj, i);
  }
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  for (size_t i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    adj[x - 1].push_back(y - 1);
  }
  toposort(adj);

  for (int i = order.size()-1; i >=0 ; i--) {
    std::cout << order[i] + 1 << " ";
  }
}
