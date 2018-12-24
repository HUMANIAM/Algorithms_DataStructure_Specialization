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
#include <math.h>
#define fora(i, f, t) for(int i = f; i<t; i++)
using namespace std;

typedef vector<set<int>> Graph;
typedef map<int, map<int, vector<set<int>>>> PATHS;
typedef vector<vector<int> > Matrix;
int KMER, T;

///                                    *** De Bruijn Graph **
///                                        --------------
class DebruijnGraph{
public:
    Graph create(){
        return de_bruijn_graph();
    }
private:
    //add edge for kmer_composition
    void add_edge(Graph& graph, const string& kmer){
        static int k_1 = KMER - 1;
        static int vertex = 0;
        static map<string, int> labels;

        string L_kmer = kmer.substr(0, k_1);
        string R_kmer = kmer.substr(1, k_1);
        int L_label, R_label;

        if(labels.find(L_kmer) != labels.end()) L_label = labels[L_kmer];
        else { graph.push_back(set<int>()); L_label = labels[L_kmer] = vertex++;}

        if(labels.find(R_kmer) != labels.end()) R_label = labels[R_kmer];
        else { graph.push_back(set<int>()); R_label = labels[R_kmer] = vertex++;}

        //add edge from left k-1 mer to right k-1 mer
        graph[L_label].insert(R_label);
    }

    //build de bruijn graph
    Graph de_bruijn_graph(){
        Graph graph;
        string read;
        cin >> KMER >> T;

        while(cin >> read)
            fora(i, 0, read.size() - KMER + 1)
               add_edge(graph, read.substr(i, KMER));
        
        return graph;
    }

};

///                                             Bubble_Detection
///                                             ---------------
class BubbleDetection{
private:
    const Graph graph;
    Matrix in_out;
    int nodes;
    set<int> in, out;
    PATHS paths;

public:
    //one argument constructor
    BubbleDetection(const Graph g): graph(g){
        nodes = graph.size();
        in_out = Matrix(nodes, vector<int>(2, 0));
    }

    //find number of bubbles in the de bruijn graph
    int bubbles(){
        //find candidate out and in vertexes
        find_candidates();

        //find bubbles between every pair v, w where v from out vertexes and w from in vertexes
        for(const auto& root : out){
            set<int> visited = {root};
            dfs(root, root, visited);
        }

        //count bubbles between nodes
        return count_bubbles();
    }

    //count bubbles
    int count_bubbles(){
        int bubbles = 0;
        for(const auto& w_pair : paths){ //this is all out paths from w vertex to all vertex with in degree > 1
            //all paths
            const map<int, vector<set<int>>> all_w_vs_paths = w_pair.second;

            //paths between w and every v with in degree > 1
            for(const auto& w_v_paths_pair : all_w_vs_paths){
                const vector<set<int> > w_v_paths = w_v_paths_pair.second;

                //if two paths between v and w are disjoint increase bubbles
                fora(i, 0, w_v_paths.size())
                    fora(j, i+1, w_v_paths.size())
                        disjoint_paths(w_v_paths[i], w_v_paths[j]) ? bubbles++ : false;
            }
        }
        return bubbles;
    }

    //check if two paths are disjoint or not
    bool disjoint_paths(const set<int>& path1, const set<int>& path2){
        for(const auto& v : path1)
            if(path2.find(v) != path2.end()) return false;
        return true;
    }

    //vertexes that may have bubbles
    void find_candidates(){
        //find out and in degree for every vertex in the graph
        fora(i, 0, nodes){
            in_out[i][1] = graph[i].size();         //out degree
            for(const auto& v : graph[i])           //in degree of children nodes
                in_out[v][0]++;
        }

        //find vertexes with out degree > 1 and vertexes with in degree > 1
        fora(i, 0, nodes){
            if(in_out[i][0] > 1) in.insert(i);
            if(in_out[i][1] > 1) out.insert(i);
        }
    }

    // dfs with depth at most t and no overlapping
    void dfs(int root, int curnode, set<int>& visited){
        if(curnode != root && in.find(curnode) != in.end()){ //this is short and non_overlapping path
            set<int> npath = visited;
            npath.erase(curnode);
            npath.erase(root);
            paths[root][curnode].push_back(npath);
        }

        // don't branch with depth > T
        if(visited.size() > T) return;

        //make new paths from current node to all of its children
        for(const auto& v : graph[curnode])
            if(visited.find(v) == visited.end()){
                set<int> nvisited = visited;
                nvisited.insert(v);
                dfs(root, v, nvisited);
            }
    }
};

int main(int argc, char** argv){
    //make de_bruijn graph
    DebruijnGraph dbGraph;
    Graph graph = dbGraph.create();

    //bubble detection
    BubbleDetection bd (graph);
    cout<<bd.bubbles()<<endl;

    return 0;
}
