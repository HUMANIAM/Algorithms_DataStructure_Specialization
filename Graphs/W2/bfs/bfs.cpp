#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <stdlib.h>
using namespace std;

struct Vertex{
size_t v, d;
Vertex(size_t x, size_t y): v(x), d(y)
    {}
};

set<size_t> discovered;
queue<Vertex> dis_proc;

int distance(vector<vector<int> > &adj, size_t s, size_t t) {
  discovered.insert(s);

  while(!dis_proc.empty()){
    //process the end front node in the queue
    Vertex vv = dis_proc.front();
    dis_proc.pop();

    if(vv.v == t) return vv.d;
    else{
        //discover the sub nodes
        for(size_t i = 0; i<adj[vv.v].size(); i++){
            size_t w = adj[vv.v][i];
            if(discovered.find(w) == discovered.end()){
                dis_proc.push(Vertex(w, vv.d + 1));
                discovered.insert(w);
            }
        }
    }
  }
  return -1;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  for (int i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    adj[x - 1].push_back(y - 1);
    adj[y - 1].push_back(x - 1);
  }
  size_t s, t;
  std::cin >> s >> t;
  s--, t--;
  dis_proc.push(Vertex(s, 0));
  std::cout << distance(adj, s, t);
}
