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
#define KMER 15
#define READSIZE 100
#define DATASET 1618
#define fora(i, f, t) for(int i = f; i<t; i++)
using namespace std;

typedef vector<vector<set<int>>> Graph;     //every node hold its parents and also its children
typedef vector<vector<int> > Matrix;


///                                    *** De Bruijn Graph **
///                                        --------------
class DebruijnGraph{
public:
    ///create the graph from kmers
    Graph create(){
        return de_bruijn_graph();
    }
private:
    ///add edge for kmer_composition
    void add_edge(Graph& graph, const string& kmer){
        static int k_1 = KMER - 1;
        static int vertex = 0;
        static map<string, int> labels;

        string L_kmer = kmer.substr(0, k_1);
        string R_kmer = kmer.substr(1, k_1);
        int L_label, R_label;

        if(labels.find(L_kmer) != labels.end()) L_label = labels[L_kmer];
        else { graph.push_back(vector<set<int>>(2, set<int>()));  L_label = labels[L_kmer] = vertex++;}

        if(labels.find(R_kmer) != labels.end()) R_label = labels[R_kmer];
        else { graph.push_back(vector<set<int>>(2, set<int>())); R_label = labels[R_kmer] = vertex++;}

        //add R_label as child for L_label
        graph[L_label][0].insert(R_label);

        //add L_label as parent for R_label
        graph[R_label][1].insert(L_label);
    }

    ///build de bruijn graph
    Graph de_bruijn_graph(){
        Graph graph;
        string read;

           while( cin >> read)
            fora(j, 0, READSIZE - KMER + 1)
               add_edge(graph, read.substr(j, KMER));
               
        return graph;
    }

};

///                                             TipsRemoval
///                                             ---------------
class TipsRemoval{
private:
    Graph graph;                // graph hold every node with its parents and children
    Matrix in_out;              // in and out degree of every vertex in the graph
    set<int> out_tips;          // vertexes with zero out degree
    set<int> in_tips;           // vertexes with zero in degree
    int nodes;                 //  number of vertexes
    int tips;                  //  number of tips

public:
    ///one argument constructor
    TipsRemoval(Graph g): graph(g){
        nodes = graph.size();
        tips = 0;
        in_out = Matrix(nodes, vector<int>(2, 0));
    }

    ///find number of bubbles in the de bruijn graph
    int tips_number(){
        //find candidate out and in vertexes
        find_candidates();

        //handle vertexes with in degree zero
        bfs_in_tips();

        //handle vertexes with out degree zero
        bfs_out_tips();

        return tips;
    }

    ///vertexes that may have bubbles
    void find_candidates(){
        //find out and in degree for every vertex in the graph
        fora(i, 0, nodes){
            in_out[i][0] = graph[i][0].size();         //out degree
            in_out[i][1] = graph[i][1].size();         //in degree
        }

        //find vertexes with out_degree 0 and also those have in_degree 0
        fora(i, 0, nodes){
            if(in_out[i][0] == 0) out_tips.insert(i);      //no children
            if(in_out[i][1] == 0) in_tips.insert(i);     //no parents
        }
    }

    ///remove in tips and new tips from removal
    void bfs_in_tips(){
        while(! in_tips.empty()){
            int curtip = *(in_tips.begin());
            in_tips.erase(curtip);

            //decrement in_degree of all children of current in_tip vertex
            for(const auto& child : graph[curtip][0]){
                graph[curtip][0].erase(child);  // remove the edge between tip and the child
                graph[child][1].erase(curtip);
                tips++;
                //if the child became in_tip handle it
                if(graph[child][1].size() == 0 && out_tips.find(child) == out_tips.end())
                    in_tips.insert(child);
            }
        }
    }

    ///remove out tips and new tips from removal
    void bfs_out_tips(){
         while(! out_tips.empty()){
            int curtip = *(out_tips.begin());
            out_tips.erase(curtip);

            //decrement in_degree of all children of current in_tip vertex
            for(const auto& parent : graph[curtip][1]){ //parents loose one child
                graph[curtip][1].erase(parent);  //remove edge between current tip and current parent
                graph[parent][0].erase(curtip);
                tips++;

                //if the child became in_tip handle it
                if(graph[parent][0].size() == 0) out_tips.insert(parent);
            }
        }
    }
};

int main(int argc, char** argv){
    //make de_bruijn graph
    DebruijnGraph dbGraph;
    Graph graph = dbGraph.create();

    //bubble detection
    TipsRemoval tr (graph);
    cout<<tr.tips_number()<<endl;

    return 0;
}
