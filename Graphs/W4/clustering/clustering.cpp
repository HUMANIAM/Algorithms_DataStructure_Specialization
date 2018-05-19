#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>
#include <cmath>
#include <set>
#include <map>
#include <limits>
#include <stdlib.h>
#include <queue>

using namespace std;

//define user types
struct Node{
int x, y, parent;
Node(int x, int y):x(x), y(y), parent(-1){}
};

struct EdgeCost{
     int n1, n2;
     double c;
     EdgeCost(int x, int y, double cost):n1(x), n2(y), c(cost){}
     bool operator > (const EdgeCost& rhs)const{
        return c>rhs.c;
     }
};

class DisJointSet{
private:
   vector<Node> nodes;
public:
    void add(int x, int y){
        nodes.push_back(Node(x, y));
    }

    int findparent(int i){
        if(nodes[i].parent < 0) return i;
        else return findparent(nodes[i].parent);
    }

    void unionSets(int root1, int root2){
        if(nodes[root1].parent < nodes[root2].parent){
           nodes[root2].parent = root1;
        }else{
            if(nodes[root1].parent == nodes[root2].parent)
                nodes[root2].parent--;
            nodes[root1].parent = root2;
        }
    }

    Node& operator [] (int i){
        return nodes[i];
    }
};

//define global variables
priority_queue<EdgeCost, vector<EdgeCost>, greater<EdgeCost>> priQuOfCosts;
DisJointSet nodes = DisJointSet();
int formedSets;

//define used functions to get d

void createCostTable(size_t n, vector<vector<double> >& costTable){
    for(size_t i=0; i<n-1; i++){
       for(size_t j=i+1; j<n; j++){
            double dis = sqrt(pow((nodes[i].x - nodes[j].x), 2) + pow((nodes[i].y - nodes[j].y), 2));
            costTable[i][j] = costTable[j][i] = dis;
            priQuOfCosts.push(EdgeCost(i, j, dis));
       }
    }
}

void min2Vec(vector<int>& vec1, vector<int>& vec2, double& mind, vector<vector<double> >& costTable){
    for(size_t i=0; i<vec1.size(); i++)
        for(size_t j=0; j<vec2.size(); j++){
            int x = vec1[i], y = vec2[j];
            if(costTable[x][y] < mind) {
                mind = costTable[x][y];
            }
        }
}


double minDistance(map<int, vector<int> >& groups, vector<vector<double> >& costTable){
    map<int, vector<int> >::iterator it;
    map<int, vector<int> >::iterator jt;
    double mind = numeric_limits<double>::max();

    for(it = groups.begin(); it != groups.end(); ){
        vector<int>& rootgroup = it->second;

        for(jt = ++it; jt != groups.end(); jt++){
            vector<int>& subgroup = jt->second;
            min2Vec(rootgroup, subgroup, mind, costTable);
        }
    }
    return mind;
}

double findMinDistance(vector<vector<double> >& costTable){
    map<int, vector<int> > groups;
    size_t n = costTable.size();

    for(size_t i=0; i<n; i++){
        int p = nodes.findparent(i);
        if(groups.find(p) == groups.end())
            groups[p] = vector<int>();
        groups[p].push_back(i);
    }
    return minDistance(groups, costTable);
}

double clustering(int k, vector<vector<double> >& costTable) {
    if(k==1)    return -1.;

    while(!priQuOfCosts.empty() && formedSets>k){
        EdgeCost edgcost = priQuOfCosts.top();
        priQuOfCosts.pop();

        //union these sets
        int root1 = nodes.findparent(edgcost.n1);
        int root2 = nodes.findparent(edgcost.n2);
        if(root1 != root2){
           nodes.unionSets(root1, root2);
           formedSets--;
        }

    }
    if(formedSets == k)
        return findMinDistance(costTable);
    else
        clustering(k, costTable);
}
int main() {
  size_t n;
  int k, x, y;
  std::cin >> n;
  for (size_t i = 0; i < n; i++) {
    cin>>x>>y;
    nodes.add(x, y);
  }
  std::cin >> k;
  formedSets = n;
  vector<vector<double>> costTable(n, vector<double>(n, 0));
  createCostTable(n, costTable);
  std::cout << std::setprecision(10) << clustering(k, costTable) << std::endl;
}
