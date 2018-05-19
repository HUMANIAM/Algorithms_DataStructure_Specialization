#include <algorithm>
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <stdlib.h>

using namespace std;
std::set<size_t> visited;
list<size_t> postorder;
list<size_t>::iterator it;

void dfs(vector<vector<int> >& adj, size_t v, bool po=false){
  visited.insert(v);
  for (size_t i = 0; i < adj[v].size(); ++i)
  {
    size_t w = adj[v][i];
    if (visited.find(w) == visited.end())
     dfs(adj, w, po);
  }
  if(po)
    postorder.push_front(v);
}

int number_of_strongly_connected_components(vector<vector<int> >& adj, vector<vector<int> >& radj) {
  int result = 0;
  for (size_t i = 0; i < adj.size(); ++i)
  {
    if(visited.find(i) == visited.end())
      dfs(adj, i, true);
  }
  visited.clear();
  for (it = postorder.begin(); it != postorder.end(); it++)
  {
    if(visited.find(*it) == visited.end()){
      result++;
      dfs(radj, *it);
    }
  }
  return result;
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  vector<vector<int> > radj(n, vector<int>());
  for (size_t i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    adj[x - 1].push_back(y - 1);
    radj[y - 1].push_back(x - 1);
  }
  std::cout << number_of_strongly_connected_components(adj, radj);
}
