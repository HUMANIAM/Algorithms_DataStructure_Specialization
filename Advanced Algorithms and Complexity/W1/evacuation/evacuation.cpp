#include <vector>
#include <string>
#include <string.h>
#include <set>
#include <map>
#include <queue>
#include <algorithm>
#include <iostream>
#include <limits.h>
#include <time.h>
#include <stdlib.h>
using namespace std;
/** implement the max flow in the flow network
    * using ford-fulkerson and dfs this algorithm results in runtime failure test where its runtime is O(|E||p|)
    	so if the edge cost is so large it will results in so big run time 

    * using Edomn-Karp algorithm. where instead of choosing an arbitrary augmenting path form source to sink
    	we choose the shortest augmenting path (with less edges) this results in less runtime in general
    	O(|E|^2|V|)
*/

typedef vector<map<int, int> > Graph;

//using with Ford_Fulkerson algorithms where we chose the ap arbitrary
int dfs(Graph& graph, set<int>& visited, vector<int>& ap, int vertex=0, int res=INT_MAX){

    //search for edge with residual > 0
    visited.insert(vertex);
    for(const auto& edges : graph[vertex])
        //not visited and has a residual edge
        if(visited.find(edges.first) == visited.end() && edges.second > 0){
            int minres;
            if((minres=dfs(graph, visited, ap, edges.first, edges.second)) != INT_MAX){
                ap.push_back(edges.first);
                return min(res, minres);
            }
        }


    //if the current node is the sink then we found augmenting path
    return (vertex==graph.size()-1)? res : INT_MAX;
}


void bfs(Graph& graph, vector<int>& parents, int sink){
    set<int> visited;   //not visit a node twice times
    visited.insert(0);

    queue<int> nodes;       //visit graph level by level
    nodes.push(0);

    while(!nodes.empty()){
        int curNod = nodes.front();
        nodes.pop();

        //search for Cf>=0 in edges from current node to other nodes
        for(const auto& edge : graph[curNod]){
            if(edge.second > 0 && visited.find(edge.first) == visited.end()){
                nodes.push(edge.first);
                visited.insert(edge.first);

                parents[edge.first] = curNod;
                //if the next node is the sink node then we find an augmenting path
                if(edge.first == sink) return;
            }
        }
    }

}


//find augmenting path from 0 to v-1
int findAp(Graph& graph, vector<int>& parents, int v){

    //find the shortest path form the source to the sink
    bfs(graph, parents, v-1);

    //find the critical path in the augmenting path
    int minres = INT_MAX;
    int curnode = v - 1;

    while(parents[curnode] != -1){
        minres = min(minres, graph[parents[curnode]][curnode]);
        curnode = parents[curnode];
    }
    return minres;   //search for the augmenting path from the s to t
}

//find the max flow in network from s to t using Edmond-Karp algorithm
int maxFlow(Graph& graph, int v){
    vector<int> parents(v, -1);     //residual edges of augmenting path
    int mr;             //min residual edge in the augmenting path
    int maxflow = 0;    //total max flow of the network

    while((mr=findAp(graph, parents, v)) != INT_MAX){
            
        //update residual of edges of augmenting path
        int curnode = v - 1;
        while(parents[curnode] != -1){
            graph[parents[curnode]][curnode] -= mr;         //decrement residual from left to right
            graph[curnode][parents[curnode]] += mr;         //increment residual from right to left
            curnode = parents[curnode];                     //go backward to update residuals
        }
        maxflow += mr;                                      //increment the maximum flow from the source to the sink
        fill(parents.begin(), parents.end(), -1);           //clear parents for searching for new augmenting paths
    }

    return maxflow;
}

int main()
{

    int n, e;
    cin>>n>>e;

    //create the flow network
    Graph graph(n);

    //initialize the graph
    for(int i=0; i<n; i++)
        graph[i] = map<int, int>();

    for(int i=0; i<e; i++){ // get edges
        int v1, v2, c;
        cin>>v1>>v2>>c;

        //create edge between v1 and v2 with cost c. initially residual = maxcost
        graph[v1-1][v2-1] += c;      //residual between v1 and v2 = c

        //update residual between v2 and v1
        if(graph[v2-1].find(v1-1) == graph[v2-1].end())
            graph[v2-1][v1-1] = 0;
    }

    // print the max flow from s to t (1 to N) in the flow network
    cout<<maxFlow(graph, n)<<endl;

    return 0;
}
