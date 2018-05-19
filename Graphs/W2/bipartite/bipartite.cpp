#include <iostream>
#include <vector>
#include <queue>
#include <map>

using namespace std;

map<size_t, bool> discovered;
queue<size_t> dis_proc;

int bipartite(vector<vector<int> > &adj) {
  discovered[0] = true;
  dis_proc.push(0);

  while(!dis_proc.empty()){
    //process the end front node in the queue
    size_t vv = dis_proc.front();
    dis_proc.pop();

    //explore the sub nodes
    bool x = !discovered[vv];
    for(size_t i = 0; i<adj[vv].size(); i++){
        size_t w = adj[vv][i];
        if(discovered.find(w) == discovered.end()){
            dis_proc.push(w);
            discovered[w] = x;
        }else{
            if(discovered[vv] == discovered[w])
                return 0;
        }
    }
  }
  return 1;
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
  std::cout << bipartite(adj);
}
