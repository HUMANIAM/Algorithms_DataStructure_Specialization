#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <set>
using namespace std;

/** Description: this problem is about we have n stocks and k points of every stock we need to draw the minimum number of charts
    ----------   without break this constraint which there is no any two charts that touch or intersect each other

    solution: If we think carefully in this problem we can find that there are n stocks kdata so the maximum charts is n charts
    --------   yet, we can combine two charts if they don't break the constraint and they don't break the constraint if all points
              of the first chart < all points of the second chart. the maximum joins lead to the minimum edges. in other words we
              need to find the minimum number of paths that pass through all charts where every chart belongs to only one path

    steps of solution:
    -----------------
        1- build DAG of the charts where every chart represent a vertex if all points of chart i are less than the corresponding
            points of chart j then we can join i to j so the total charts will reduce by 1

        2- building the flow network where every chart is divided into two vertex Vin and Vout and if i Vin can be join to j Vout
            add an edge between them

        3- maximum bipartite matching represent the total number of joins charts then the minimum charts will be
            total charts minus the maximum number of joins
*/

class StockCharts {
 public:
  void Solve() {
    ReadData();
    cout<<MinCharts();
  }

 private:
  int num_stocks, num_points;
  vector<vector<int>> stock_data;
  vector<map<int, bool> > graph;

  //read stocks data
  void ReadData() {
    cin >> num_stocks >> num_points;

    for (int i = 0; i < num_stocks; ++i){
      stock_data.push_back(vector<int>(num_points));
      for (int j = 0; j < num_points; ++j)
        cin >> stock_data[i][j];
    }
  }

  //create flow network for the bipartite matching graph
 void createFlowNetwork(){
    int N = 2 * num_stocks + 2;        //every chart has in joint and out joint where out joint can be assigned to only one in

    for(int i=0; i<N; i++)
        graph.push_back(map<int, bool>());

    //make edges from source to Charts out true and false from Charts out to the source
    for(int i=1; i<=num_stocks; i++){
        graph[0][i] = true;
        graph[i][0] = false;
    }

    //make edges form Charts in to sink with true cost and from sink to charts in with false flow
    for(int i=num_stocks+1; i<N-1; i++){
        graph[N-1][i] = false;
        graph[i][N-1] = true;
    }

    // if chart i kPoints < chart j kPoints and edge
    for(int i=0; i<num_stocks; i++)
        //compare current chart with all forward charts add edge
        //if the current chart can be joined with another chart
        for(int j=0; j<num_stocks; j++)
            if(compare(stock_data[i], stock_data[j])){
                graph[i + 1][j + num_stocks + 1] = true;
                graph[j + num_stocks + 1][i + 1] = false;
            }
  }

  //compare points of two charts
  bool compare(const vector<int>& stock1, const vector<int>& stock2) {
    for (int i = 0; i < stock1.size(); ++i)
      if (stock1[i] >= stock2[i])
        return false;
    return true;
  }

  //find the shortest augmenting path from source to the sink
  bool findAp(vector<int>& parents, int sink){
      //find the shortest augmenting path using bfs
    set<int> visited;
    visited.insert(0);

    queue<int> nodes;
    nodes.push(0);

    while(!nodes.empty()){
        int curnode = nodes.front();
        nodes.pop();

        //find edges that has residual and their other nodes not visited before
        for(const auto& edge : graph[curnode])
            if(edge.second && visited.find(edge.first) == visited.end()){
                visited.insert(edge.first);
                parents[edge.first] = curnode;
                nodes.push(edge.first);

                if(edge.first == sink) return true;
            }
    }

    return false;
  }

  //find the minimum charts to fit drawing the stocks data
  int MinCharts() {

    int maxJoins = 0;
    // create flow network of charts
    createFlowNetwork();

    //search for the max flow in the flow network. Note that the critical path in any augmenting path will be 1
    int sink = 2 * num_stocks + 1;
    vector<int> parents(sink+1, -1);

    while(findAp(parents, sink)){
        int curnode = sink;
        maxJoins++;

        while(parents[curnode] != -1){
            //update flow and residual edges of the flow network
            graph[parents[curnode]][curnode] = false;
            graph[curnode][parents[curnode]] = true;

            curnode = parents[curnode];
        }
    }
    return num_stocks - maxJoins;
 }

};

int main() {
  std::ios_base::sync_with_stdio(false);
  StockCharts stock_charts;
  stock_charts.Solve();
  return 0;
}
