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

void exploreUnshortestNodes(int s, set<int>& noshortest, vector<vector<int> >& adj){
    visited.insert(s);
    for(size_t i=0; i<adj[s].size(); i++){
        noshortest.insert(adj[s][i]);
        if(visited.find(adj[s][i]) == visited.end())
            exploreUnshortestNodes(adj[s][i], noshortest, adj);
    }
}

void findCycle(int s, vector<Node> dis, set<int>& noshortest, vector<vector<int> >& adj){
    visited.clear();
    visited.insert(s);
    int y = dis[s].parent;
    while(visited.find(y) != visited.end()){
        noshortest.insert(y);
        visited.insert(y);
        y = dis[y].parent;
    }
    visited.clear();
    exploreUnshortestNodes(y, noshortest, adj);
}

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

  while(k-->1){
    visited.clear();
    updatedNodes.clear();
    updateEdges(adj, cost, s, distance);
    if(updatedNodes.size() == 0)
        break;
  }

  //search for nodes that are reachable through negative cycle weight
  set<int>::iterator it;
  set<int> noshortest;

  for(it=updatedNodes.begin(); it != updatedNodes.end(); it++){
    if(noshortest.find(*it) == noshortest.end()){
        noshortest.insert(*it);
        findCycle(*it, distance, noshortest, adj);
    }
  }

  for(it=noshortest.begin(); it != noshortest.end(); it++){
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
