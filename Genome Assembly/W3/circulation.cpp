#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <limits.h>
#define fora(i, f, t) for(int i=f; i<t; i++)

using namespace std;

struct Edge {
        int from, to, cost;
};

//this class implement the Circulation problem
class Circulation {
private:
    /* List of all - forward and backward - edges */
    vector<Edge> edges;
    vector<vector<int>> init_flow;  // in flow and out flow for every vertex
    vector<vector<size_t> > graph;  //hold the graph in adj list
    vector<int> lower_edges;        //lower bound costs of edges
    int numvertexes;                // number of vertexes
    int numedges;                   // number of edges
    int lowerBoundSum;              // sum of all lower bounds
    int maxflow;                    // max flow

public:
    //read edges of the network
    void read_data(){
        int n;
        cin >> n >> numedges;
        numvertexes = n + 2;
        lowerBoundSum = 0;

        graph = vector<vector<size_t> >(numvertexes);
        init_flow = vector<vector<int>>(numvertexes+1, vector<int>(2, 0));

        fora (i, 0, numedges) {
            int u, v, low_bound,capacity;
            cin >> u >> v >> low_bound >> capacity;

            // add forward and backward edge to the network
            add_edge(u, v, capacity - low_bound);

            // increase in flow of v and increase outflow of u
            init_flow[u][1] += low_bound;
            init_flow[v][0] += low_bound;
            lowerBoundSum += low_bound;

            lower_edges.push_back(low_bound);
        }
    }

    void solve(){
        //add start and target vertexes to the network
        add_source_sink();

        //calculate the max flow from the source to the sink
        maxFlow();
    }

    void print(){
        if(maxflow == lowerBoundSum){
            cout<<"YES"<<endl;
            fora(i, 0, numedges)
                cout<<lower_edges[i] + edges[2*i + 1].cost<<endl;
        }else cout<<"NO"<<endl;
    }

    void add_edge(int from, int to, int cost) {
      ///forward edges stored in even indexes and backward edges stored in odd indexes in the edges container
        Edge forward_edge = {from, to, cost};
        Edge backward_edge = {to, from, 0};
        graph[from].push_back(edges.size());
        edges.push_back(forward_edge);
        graph[to].push_back(edges.size());
        edges.push_back(backward_edge);
    }

    void update_flow(size_t id, int flow) {
        edges[id].cost -= flow;         //forward edge
        edges[id ^ 1].cost += flow;     //backward edge
    }

    /// add start vertex and end vertex
    void add_source_sink(){
        fora(i, 1, numvertexes-1){
            // add forward and backward edge between source to i vertex
            add_edge(0, i, init_flow[i][0]);
            // add forward and backward edge between i vertex and target
            add_edge(i, numvertexes-1, init_flow[i][1]);
        }
    }

    ///find augmenting path using dfs
    void bfs(vector<vector<int>>& parents){
        set<int> visited = {0};   //not visit a node twice times
        queue<int> nodes;       //visit graph level by level
        nodes.push(0);

        while(!nodes.empty()){
            int curNod = nodes.front();
            nodes.pop();

            //search for Cf>=0 in edges from current node to other nodes
            for(const auto& edgeid : graph[curNod]){
                const Edge edge= edges[edgeid];
                if(edge.cost > 0 && visited.find(edge.to) == visited.end()){
                    nodes.push(edge.to);
                    visited.insert(edge.to);

                    parents[edge.to][0] = curNod;       //parent
                    parents[edge.to][1] = edgeid;       //edge id

                    //if the next node is the sink node then we find an augmenting path
                    if(edge.to == numvertexes-1) return;
                }
            }
        }
    }


    ///find augmenting path from 0 to v-1
    int findAp(vector<vector<int>>& parents){

        //find the shortest path form the source to the sink
        bfs(parents);

        //find the critical path in the augmenting path
        int minres = INT_MAX;
        int curnode = numvertexes-1;

        //if there is augmenting path find the critical edge of this path
        while(parents[curnode][0] != - 1){
            const Edge edge = edges[parents[curnode][1]];
            minres = min(minres, edge.cost);
            curnode = parents[curnode][0];
        }
        return minres;   //return the capacity of critical edge
    }

    ///find the max flow in network from s to t using Edmond-Karp algorithm
    void maxFlow(){
        vector<vector<int>> parents(numvertexes, vector<int>(2, -1));     //edges of augmenting path
        int mr;                  //min residual edge in the augmenting path
        maxflow = 0;             //total max flow of the network

        while((mr=findAp(parents)) != INT_MAX){
            //update residual of edges of augmenting path
            int curnode = numvertexes - 1;
            while(parents[curnode][0] != - 1){
                const Edge edge = edges[parents[curnode][1]];
                update_flow(parents[curnode][1], mr);
                curnode = parents[curnode][0];
            }

            //add to the cumulative flow
            maxflow += mr;

            //clear the path
            fora(i, 0, parents.size())
                parents[i][0] = parents[i][1] = -1;
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    Circulation circulation;
    circulation.read_data();
    circulation.solve();
    circulation.print();
    return 0;
}
