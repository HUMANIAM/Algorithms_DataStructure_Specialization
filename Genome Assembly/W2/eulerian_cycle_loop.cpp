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

int find_start_vertex(const map<int, int>& opts, vector<int>& last_ones){
    while(!last_ones.empty()){
        int start = last_ones.back();
        last_ones.pop_back();

        if(opts.find(start) != opts.end())
            return start;
    }
}
//Eulerian path passes all edges and every edge exactly once
void eulerian_path(Graph& graph, vector<int>& path){
    int n = graph.size();
    map<int, int> optionalStarts;               //vertexes still has unvisited edges
    vector<int> last_ones(1, 0);                //choose the last vertex to keep vertexes in the same order
    vector<int> new_cycle;

    path.push_back(0);      //start from 0 node
    optionalStarts[0] = 0;  //the first start cycle

    while(!optionalStarts.empty()){
        //find the last node still has unvisited edges. pick anyone of them
        int start = find_start_vertex(optionalStarts, last_ones);
        int position = optionalStarts[start];

        //find sub circle
        int next_node, cur_node;
        cur_node = start;
        int pos = position;

        do{
            next_node = graph[cur_node].back();
            new_cycle.push_back(next_node);
            graph[cur_node].pop_back();

            //if still has unvisited edges keep it to visit again
            if(graph[cur_node].size()) {optionalStarts[cur_node] = pos; last_ones.push_back(cur_node);}
            else                        optionalStarts.erase(cur_node);

            pos++;
            cur_node = next_node;

        }while(cur_node != start);

        //append the cycle behind the start nod
        path.insert(path.begin() + position + 1, new_cycle.begin(), new_cycle.end());
        new_cycle.clear();
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
