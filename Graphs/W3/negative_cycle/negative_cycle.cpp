#include <iostream>
#include <vector>
using namespace std;

int maxcost = 100000000, n, m;

struct Node{
    int node, cost;
    Node(int n, int c=maxcost):node(n), cost(c){}
};

int negative_cycle(vector<vector<int> > &adj, vector<vector<int> > &cost) {
  int changes;
  int k=n;
  vector<int> costToReach(n, maxcost);
  costToReach[0] = 0;

  while(k-->0){
    changes = 0;
    for(int i=0; i<n; i++){
        for(int j=0; j<adj[i].size(); j++){
            int w = adj[i][j];
            if((costToReach[i]+cost[i][j]) < costToReach[w]){
                changes++;
                costToReach[w] = costToReach[i] + cost[i][j];
            }
        }
    }
  }
  if(changes>0) return 1;

  return 0;
}

int main() {

  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  vector<vector<int> > cost(n, vector<int>());
  for (int i = 0; i < m; i++) {
    int x, y, w;
    std::cin >> x >> y >> w;
    adj[x - 1].push_back(y - 1);
    cost[x - 1].push_back(w);
  }
  std::cout << negative_cycle(adj, cost);
}
