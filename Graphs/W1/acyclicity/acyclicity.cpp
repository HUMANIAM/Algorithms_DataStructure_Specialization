#include <iostream>
#include <vector>
#include <set>

using namespace std;

set<size_t> visited;
set<size_t> parents;

int explore(vector<vector<int> > &adj, set<size_t>& parents, size_t v){
    visited.insert(v);

    for(size_t i =0; i<adj[v].size(); i++){
        int w = adj[v][i];

        if(visited.find(w) == visited.end()){
          parents.insert(w);
          if(explore(adj, parents, w) == 1)
            return 1;
        }
        else if(parents.find(w) != parents.end()){
            return 1;
        }
        parents.erase(w);
    }
    return 0;
}

int acyclic(vector<vector<int> > &adj) {

    for(size_t i=0; i<adj.size(); i++){
        if(visited.find(i) == visited.end()){
            parents.insert(i);
            if(explore(adj, parents, i) == 1)
                return 1;
        }
        parents.clear();
    }
  return 0;
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
  std::cout << acyclic(adj);
}
