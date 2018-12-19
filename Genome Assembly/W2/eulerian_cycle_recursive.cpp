#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <stdio.h>
#define fora(i, f, t) for(int i = f; i<t; i++)

using namespace std;
typedef vector<vector<int>> Graph;

//find connect cycle
void graph_walk(Graph& graph, map<int, int>& op_starts, int start,  vector<int>& cycle, int cur_v, int st){
    //next vertex
    int next_vertex = graph[cur_v].back();
    cycle.push_back(next_vertex);
    graph[cur_v].pop_back();
    
    //if the current vertex still has another edges we can start from it again
    if(graph[cur_v].size()) op_starts[cur_v] = st;
    else                   op_starts.erase(cur_v);

    //if the next vertex is the start vertex then we have a cycle
    if(next_vertex == start) return;

    //go to the next vertex
    graph_walk(graph, op_starts, start, cycle, next_vertex, st+1);
}

//Eulerian path passes all edges and every edge exactly once
void eulerian_path(Graph& graph, vector<int>& path){
    int n = graph.size();
    map<int, int> optionalStarts;       //when we stuck we take the last node has non visited edges in the cycle
    vector<int> new_cycle;

    int pos_cycle = 0;      // position of the next cycle
    path.push_back(0);      //start from 0 node

    graph_walk(graph, optionalStarts, 0, new_cycle, 0, pos_cycle);

    while(true){
        //append the cycle behind the start nod
        path.insert(path.begin() + pos_cycle + 1, new_cycle.begin(), new_cycle.end());
        new_cycle.clear();

        //if there are unvisited edges go and visit them
        if(!optionalStarts.empty()){
            //find the last node still has unvisited edges. pick anyone of them
            int start = (optionalStarts.begin()) -> first;
            pos_cycle = (optionalStarts.begin()) -> second;

            //start from this node and find another cycle
            graph_walk(graph, optionalStarts, start, new_cycle, start, pos_cycle);

        }else {
            break;      //we are done
        }
    }
}

//read the edges of the graph
Graph read_graph(Graph& balancing, int n, int m){
    Graph graph(n, vector<int>());

    fora(i, 0, m){
        int u, v;
        cin >> u >> v;
        graph[u-1].push_back(v-1);
        balancing[u-1][0]++;
        balancing[v-1][1]++;
    }
    return graph;
}

//print the path
void print_path(Graph& graph, vector<int>& path){
    //the first visited vertex and the last one must be the same vertex
    for(const auto& v : graph) if(v.size() != 0) {cout<<0<<endl; return;}

    cout<<1<<endl;
    fora(i, 0, path.size()-1) cout<<path[i]+1<<" "; cout<<endl;
}

//test balancing of the graph
bool is_balancing(const Graph& graph){
    for(const auto& v : graph) if(v[0] != v[1]) return false;
    return true;
}

int main(int argc, char** argv){
    //read edges
    int n, m;
    cin >> n >> m;
    Graph balancing (n, vector<int>(2));
    Graph graph = read_graph(balancing, n, m);

    //test balancing
    if(! is_balancing(balancing)){cout<<0<<endl; return 0;}

    //build the Eulerian path
    vector<int> path;
    eulerian_path(graph, path);

    //print the path
    print_path(graph, path);

    return 0;
}
