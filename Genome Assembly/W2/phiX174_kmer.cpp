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
#define KMER 10
#define DATASIZE 5396
using namespace std;
typedef vector<vector<int>> Graph;

///                            *** Eulerian cycle ****                   ///
///                                ---------------                       ///
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

///                                    *** De Bruijn Graph **
///                                        --------------

//add edge for kmer_composition
void add_edge(Graph& graph, const string& kmer, vector<string>& k_1_mers){
    static int k_1 = KMER - 1;
    static int vertex = 0;
    static map<string, int> labels;

    string L_kmer = kmer.substr(0, k_1);
    string R_kmer = kmer.substr(1, k_1);
    int L_label, R_label;

    if(labels.find(L_kmer) != labels.end()) L_label = labels[L_kmer];
    else { graph.push_back(vector<int>()); L_label = labels[L_kmer] = vertex++; k_1_mers.push_back(L_kmer); }

    if(labels.find(R_kmer) != labels.end()) R_label = labels[R_kmer];
    else { graph.push_back(vector<int>()); R_label = labels[R_kmer] = vertex++;   k_1_mers.push_back(R_kmer); }

    //add edge from left k-1 mer to right k-1 mer
    graph[L_label].push_back(R_label);
}

//build de bruijn graph
Graph de_bruijn_graph(vector<string>& k_1_mers){
    //read kmers
    string kmer;
    Graph graph;
    fora(i, 0, DATASIZE){
        cin >> kmer;
        add_edge(graph, kmer, k_1_mers);
    }
    return graph;
}

//cut one of common suffix and prefix of the genome
int findMatchedLength(const string& str, int n){
    fora(i, n/2, n){
        int k = i;

        for(int j=0; k < n; k++, j++)
            if(str[k] != str[j]) break;

        if(k == n)
            return n - i;
    }
    return 0;
}

//print the path
void get_genome_from_kmers(const vector<string>& k_1_mers, vector<int>& path, string& result){
    stringstream universal_string;
    universal_string << k_1_mers[path[0]];

    fora(i, 1, path.size())
        universal_string << k_1_mers[path[i]][KMER - 2];

    result = universal_string.str();
    int n = result.size();

    int overlap = findMatchedLength(result, n);
    result = result.substr(0, n - overlap);
}


int main(int argc, char** argv){
    //make de_bruijn graph
    vector<string> k_1_mers;
    Graph graph = de_bruijn_graph(k_1_mers);

    //build the Eulerian path
    vector<int> path;
    eulerian_path(graph, path);

    //print the path
    string k_universal_string;
    get_genome_from_kmers(k_1_mers, path, k_universal_string);
    cout<<k_universal_string<<endl;

    return 0;
}
