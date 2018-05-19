#include <iostream>
#include <limits>
#include <vector>
#include <queue>
#include <set>
#include <stdlib.h>

using namespace std;

long long maxcost = numeric_limits<long long>::max();

struct Node{
    int parent;
    long long cost;
    Node():parent(-1), cost(maxcost){}
};

set<int> visited, updatedNodes;

void updateEdges(vector<vector<int> > &adj, vector<vector<int> > &cost, int s, vector<Node> &distance){
    visited.insert(s);
    for(size_t i=0; i<adj[s].size(); i++){
        int w = adj[s][i];

        if(distance[w].cost > (distance[s].cost+cost[s][i])){
            distance[w].cost = distance[s].cost + cost[s][i];
            distance[w].parent = s;
            updatedNodes.insert(w);
        }
        if(visited.find(w) == visited.end())
            updateEdges(adj, cost, w, distance);
    }
}

void dfs(int w, vector<vector<int> >& adj, set<int>& nonshortest, set<int>& explored){
   explored.insert(w);
   nonshortest.insert(w);
   for(size_t i=0; i<adj[w].size(); i++){
    if(explored.find(adj[w][i]) == explored.end())
        dfs(adj[w][i], adj, nonshortest, explored);
   }
}

void lastIteration(vector<vector<int> > &adj, vector<vector<int> > &cost, int s, vector<Node> &distance, set<int>& noshortest){
    visited.insert(s);
    for(size_t i=0; i<adj[s].size(); i++){
        int w = adj[s][i];

        if((distance[w].cost > (distance[s].cost+cost[s][i]))&& (noshortest.find(w) == noshortest.end()) ){
            set<int> explored;
            dfs(w, adj, noshortest, explored);
        }
        if(visited.find(w) == visited.end() && noshortest.find(w) == noshortest.end())
            lastIteration(adj, cost, w, distance, noshortest);

    }
}


void shortest_paths(vector<vector<int> > &adj, vector<vector<int> > &cost, int s, vector<Node> &distance, vector<int> &shortest) {
  // initialize nodes costs
  for(size_t i =0; i<adj.size(); i++)
    distance[i] = Node();
  distance[s].cost = 0;
  updatedNodes.insert(s);

  updateEdges(adj, cost, s, distance);
  int k = visited.size();
  if(updatedNodes.size() == 1)
    return;

  while(k-->2){
    visited.clear();
    updatedNodes.clear();
    updateEdges(adj, cost, s, distance);
    if(updatedNodes.size() == 0)
        break;
  }

  //explore the nodes has - infinity
  visited.clear();
  set<int> noshortest;
  lastIteration(adj, cost, s, distance, noshortest);

  for(set<int>::iterator it=noshortest.begin(); it != noshortest.end(); it++){
    shortest[*it] = 0;
  }

}

int main() {
  int n, m, s;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  vector<vector<int> > cost(n, vector<int>());
  for (int i = 0; i < m; i++) {
    int x, y, w;
    std::cin >> x >> y >> w;
    adj[x - 1].push_back(y - 1);
    cost[x - 1].push_back(w);
  }
  std::cin >> s;
  s--;
  vector<Node> distance(n);
  vector<int> shortest(n, 1);
  shortest_paths(adj, cost, s, distance, shortest);
  for (int i = 0; i < n; i++) {
    if (distance[i].cost == maxcost) {
      std::cout << "*\n";
    } else if (!shortest[i]) {
      std::cout << "-\n";
    } else {
      std::cout << distance[i].cost << "\n";
    }
  }
}
