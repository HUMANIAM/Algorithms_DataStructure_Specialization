#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <memory>
using namespace std;

class MaxMatching {
 public:
  void Solve() {
    vector<vector<bool>> adj_matrix = ReadData();
    vector<int> matching = FindMatching(adj_matrix);
    WriteResponse(matching);
  }

 private:
  int flights;
  int crews;
  vector<map<int, bool> > graph;

  //read available matches between flights and crews
  vector<vector<bool>> ReadData() {
    cin >> flights >> crews;
    vector<vector<bool>> adj_matrix(flights, vector<bool>(crews));

    for (int i = 0; i < flights; ++i)
      for (int j = 0; j < crews; ++j) {
        int bit;
        cin >> bit;
        adj_matrix[i][j] = (bit == 1);
      }
    return adj_matrix;
  }

  // write the response
  void WriteResponse(const vector<int>& matching) {
    for (int i = 1; i < matching.size(); ++i) {
      if (i > 1)
        cout << " ";
      if (matching[i] == -1)
        cout << "-1";
      else
        cout << matching[i];
    }
    cout << "\n";
  }

  void createFlowNetwork(const vector<vector<bool>>& adj_matrix){
    int N = flights+crews+2;        //flights nodes + crews nodes + source + sink

    for(int i=0; i<N; i++)
        graph.push_back(map<int, bool>());

    //make edges from source to flight true and false from flights to the source
    for(int i=1; i<=flights; i++){
        graph[0][i] = true;
        graph[i][0] = false;
    }

    //make edges form crews to sink with true cost and from sink to crews with false flow
    for(int i=flights+1; i<N-1; i++){
        graph[N-1][i] = false;
        graph[i][N-1] = true;
    }

    //make edges between flights and crews with true cost
    for(int i=0; i<flights; i++)
        for(int j=0; j<crews; j++)
            if(adj_matrix[i][j]){
                graph[i+1][j+flights+1] = true;
                graph[j+flights+1][i+1] = false;
            }
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

  //find the maximum matching between flights and crews
  vector<int> FindMatching(const vector<vector<bool>>& adj_matrix) {
    vector<int> matches(flights+1, -1);

    //create the flow network form the adj matrix
    createFlowNetwork(adj_matrix);

    //search for the max flow in the flow network. Note that the critical path in any augmenting path will be 1
    int sink = flights + crews + 1;
    vector<int> parents(sink+1, -1);

    while(findAp(parents, sink)){
        int curnode = sink;
        int c ;                          //assigned craw
        int f;                            //assigned flight

        while(parents[curnode] != -1){
            //update flow and residual edges of the flow network
            graph[parents[curnode]][curnode] = false;
            graph[curnode][parents[curnode]] = true;

            //update matches
            if(parents[parents[curnode]] != -1 && ((curnode>=1 && curnode<=flights) || curnode == sink)){
               c = parents[curnode];
               f = parents[parents[curnode]];

               matches[f] = c - flights;   //assign flight to specific crew
            }

            curnode = parents[curnode];
        }
    }
    return matches;
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  MaxMatching max_matching;
  max_matching.Solve();
  return 0;
}
