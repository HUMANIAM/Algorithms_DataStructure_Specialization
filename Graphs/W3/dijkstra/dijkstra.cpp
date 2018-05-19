#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <limits>
#include <algorithm>

using namespace std;

int maxcost = 100000009, m, n;
struct Vertex{
 int cost, parent;

 Vertex(int c=maxcost):cost(c), parent(-1)
 {}
};

struct Node{
int node, cost;
Node(int n, int c=maxcost):node(n), cost(c)
{}
bool operator < (const Node& rhs)const
 {
     return cost>rhs.cost;
 }
};

struct Nd{
    int cost, parent;
    bool known;
    Nd(int c=maxcost):cost(c), known(false), parent(-1){}
};

priority_queue<Node> exploredNodes;
map<int, Vertex> nodesTable;

int getMin(Nd arr[]){
    int minv = numeric_limits<int>::max();
    int index = -1;
    for(int i=0; i<n; i++)
        if(arr[i].cost <minv && !arr[i].known){
            index = i;
            minv = arr[i].cost;
    }
    if(index != -1)
        arr[index].known = true;

    if(minv == maxcost)
        index = -1;
    return index;
}

int distance(vector<vector<int> > &adj, vector<vector<int> > &costs, int s, int t) {
  //exploredNodes.push(Node(s, 0));
  Nd arr[n] = {Nd()};
  arr[s].cost=0;

  //start exploring and edge relaxation to get the shortest path from s to t
  while(true){
    int minindex = getMin(arr);      //get the min cost
    if(minindex == -1) break;

    if(t == minindex) return arr[minindex].cost;

    for(size_t i=0; i<adj[minindex].size(); i++){
        int u = adj[minindex][i];
        int newcost = arr[minindex].cost + costs[minindex][i];

        //edge relaxation
        if(newcost < arr[u].cost){
            arr[u].cost = newcost;      //change priority
            arr[u].parent = minindex;
        }
    }
  }
  return -1;
}

int main() {
  std::cin >> n >> m;

  vector<vector<int> > adj(n, vector<int>());
  vector<vector<int> > costs(n, vector<int>());

  for (int i = 0; i < m; i++) {
    int x, y, w;
    std::cin >> x >> y >> w;
    adj[x - 1].push_back(y - 1);
    costs[x - 1].push_back(w);
  }
  int s, t;
  std::cin >> s >> t;
  s--, t--;

  std::cout << distance(adj, costs, s, t);
}

