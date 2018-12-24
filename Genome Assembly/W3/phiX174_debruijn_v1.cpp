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
#include <exception>
#include <stdexcept>
#include <float.h>

#define KMER 20
#define READSIZE 100
#define DATASET 1618
#define fora(i, f, t) for(int i = f; i<t; i++)
using namespace std;

struct BubblePath{
    int kmerSum;
    vector<int> path;
};

typedef map<int, map<int, vector<BubblePath>>> PATHS;
typedef vector<vector<set<int>>> Graph;     //every node hold its parents and also its children
typedef vector<vector<int> > Matrix;


///                                    *** De Bruijn Graph **
///                                        --------------
class DebruijnGraph{
private:

    ///add edge for kmer_composition
    void add_edge(Graph& graph, const string& kmer, vector<string>& k_1_mers, vector<map<int, int>>& kmer_count){
        static int k_1 = KMER - 1;
        static int vertex = 0;
        static map<string, int> labels;

        string L_kmer = kmer.substr(0, k_1);
        string R_kmer = kmer.substr(1, k_1);
        int L_label, R_label;

        //add edge from l_kmer to the r_kmer
        if(labels.find(L_kmer) != labels.end()) L_label = labels[L_kmer];
        else {
            graph.push_back(vector<set<int>>(2, set<int>()));
            kmer_count.push_back(map<int, int>());
            L_label = labels[L_kmer] = vertex++;
            k_1_mers.push_back(L_kmer);
        }

        if(labels.find(R_kmer) != labels.end()) R_label = labels[R_kmer];
        else {
            graph.push_back(vector<set<int>>(2, set<int>()));
            kmer_count.push_back(map<int, int>());
            R_label = labels[R_kmer] = vertex++;
            k_1_mers.push_back(R_kmer);
        }

        //add R_label as child for L_label
        graph[L_label][0].insert(R_label);

        //add L_label as parent for R_label
        graph[R_label][1].insert(L_label);

        //increment occurrence of this kmer
        if(kmer_count[L_label].find(R_label) == kmer_count[L_label].end()) kmer_count[L_label][R_label] = 1;
        else kmer_count[L_label][R_label]++;
    }
public:
    ///build de bruijn graph
    Graph de_bruijn_graph(vector<string>& k_1_labels, vector<map<int, int>>& kmer_coun){
        Graph graph;
        string read;

//        fora(i, 0, 10 ){ cin >> read;
       while( cin >> read)
        fora(j, 0, READSIZE - KMER + 1)
           add_edge(graph, read.substr(j, KMER), k_1_labels, kmer_coun);//}

        return graph;
    }
};

    ///                                             TipsRemoval
///                                             ---------------
class TipsRemoval{
private:
    Graph& graph;
    Matrix in_out;              // in and out degree of every vertex in the graph
    set<int> out_tips;          // vertexes with zero out degree
    set<int> in_tips;           // vertexes with zero in degree
    int nodes;                 //  number of vertexes

public:
    ///one argument constructor
    TipsRemoval(Graph& g): graph(g){
        nodes = graph.size();
        in_out = Matrix(nodes, vector<int>(2, 0));
    }

    ///find number of bubbles in the de bruijn graph
    void remove_tips(){
        //find candidate out and in vertexes
        find_candidates();

        //handle vertexes with in degree zero
        bfs_in_tips();

        //handle vertexes with out degree zero
        bfs_out_tips();
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
            if(in_out[i][0] == 0 && in_out[i][1] != 0) out_tips.insert(i);      //no children but has parents
            if(in_out[i][1] == 0 && in_out[i][0] != 0) in_tips.insert(i);       //no parents but has children
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
            int sz = graph[curtip][1].size();
            fora(i, 0, sz){ //parents loose one child
                int parent = *(graph[curtip][1].begin());
                graph[curtip][1].erase(parent);  //remove edge between current tip and current parent
                graph[parent][0].erase(curtip);

                //if the child became in_tip handle it
                if(graph[parent][0].size() == 0) out_tips.insert(parent);
            }
        }
    }
};


///                                             Bubble_Detection
///                                             ---------------
class BubbleDetection{
private:
    Graph& graph;
    Matrix in_out;
    int nodes;
    set<int> in, out;
    PATHS paths;
    int T;
    vector<map<int, int> >kmer_count;
    vector<string>& k_1_mers;

public:
    ///two argument constructor
    BubbleDetection(Graph& g, vector<map<int, int>> kmc, vector<string>& k_1_m)
            : graph(g), kmer_count(kmc), k_1_mers(k_1_m){
        nodes = graph.size();
        T = KMER;
        in_out = Matrix(nodes, vector<int>(2, 0));
    }

    ///find number of bubbles in the de bruijn graph
    void handle_bubbles(){
        //find candidate out and in vertexes
        find_candidates();

        //find bubbles between every pair v, w where v from out vertexes and w from in vertexes
        for(const auto& root : out){
            set<int> visited = {root};
            dfs(root, root, -1, visited, 0);
        }

        //remove bubble paths
        remove_bubbles();

    }

    ///remove bubbles
    void remove_bubbles(){
        for(const auto& w_pair : paths){ //this is all out paths from w vertex to all vertex with in degree > 1
            //all paths
            const map<int, vector<BubblePath>>& all_w_vs_paths = w_pair.second;
            int src = w_pair.first;

            //paths between w and every v with in degree > 1
            for(const auto& w_v_paths_pair : all_w_vs_paths){
                const vector<BubblePath>& w_v_paths = w_v_paths_pair.second;
                int des = w_v_paths_pair.first;

                //if two paths between v and w are disjoint delete the one with low average coverage kmers
                vector<bool> deleted_paths (w_v_paths.size(), false);
                fora(k, 0, w_v_paths.size()){
                    int i = k;
                    while(i < deleted_paths.size() && deleted_paths[i]) i++;       //choose path undeleted

                   //compare it with all other paths to find the bubble ones
                    fora(w, i+1, w_v_paths.size()){
                        int j = w;
                        while(j<deleted_paths.size() && deleted_paths[j]) j++;     //choose path undeleted

                        //if they disjoint they make bubble
                        if( disjoint_paths(w_v_paths[i].path, w_v_paths[j].path, src, des) ){
                            int p = min_avg_coverage_kmer(w_v_paths[i], w_v_paths[j], i, j);

                            //delete the path with low aveg coverage kmer
                            delete_path(w_v_paths[p].path);

                            //if it is the first path don't continue and break
                            deleted_paths[p] = true;
                            if(p == i) break;
                        }
                    }
                }
            }
        }
    }

    ///find the path with max average coverage kmer
    int min_avg_coverage_kmer(const BubblePath& path1, const BubblePath& path2, int i, int j){
        double avg_cov_kmer1 = ((double) path1.kmerSum) / (path1.path.size()-1);
        double avg_cov_kmer2 = ((double) path2.kmerSum) / (path2.path.size()-1);

        if(avg_cov_kmer1 < avg_cov_kmer2 ) return i;
        else return j;
    }

    /// delete path from the graph
    void delete_path(const vector<int>& path){
        fora(v, 0, path.size()-1){
            graph[ path[v] ][0].erase(path[v+1]);  // remove next node from children of current node
            graph[ path[v+1] ][1].erase(path[v]);  // remove current node from parents of the next node
        }
    }

    ///check if two paths are disjoint or not
    bool disjoint_paths(const vector<int>& vpath1, const vector<int>& vpath2, int src, int des){
        set<int> spath = set<int>(vpath2.begin(), vpath2.end());
        for(const auto& v : vpath1)
            if(v != src && v != des)
                if(spath.find(v) != spath.end()) return false;
        return true;
    }

    ///vertexes that may have bubbles
    void find_candidates(){
        //find out and in degree for every vertex in the graph
        fora(i, 0, nodes){
            in_out[i][0] = graph[i][0].size();         //out degree
            in_out[i][1] = graph[i][1].size();         //in degree
        }

        //find vertexes with out degree > 1 and vertexes with in degree > 1
        fora(i, 0, nodes){
            if(in_out[i][0] > 1) out.insert(i);
            if(in_out[i][1] > 1) in.insert(i);
        }
    }

    /// dfs with depth at most t and no overlapping
    void dfs(int root, int curnode, int parent, set<int>& visited, int tkmer_sum){
        //read kmer from the edge between current and previous vertexes
        int nkmer_sum;
        if(parent != -1)
            nkmer_sum = tkmer_sum + kmer_count[parent][curnode];

        //this is short and non_overlapping path
        if(curnode != root && in.find(curnode) != in.end()){
            BubblePath bpath;
            bpath.kmerSum = nkmer_sum;
            bpath.path = vector<int>(visited.begin(), visited.end());
            paths[root][curnode].push_back(bpath);
        }

        // don't branch with depth > T
        if(visited.size() > T) return;

        //make new paths from current node to all of its children
        for(const auto& v : graph[curnode][0])
            if(visited.find(v) == visited.end()){
                set<int> nvisited = visited;
                nvisited.insert(v);

                // cumulate occurrences of kmers in the path
                if(parent != -1)
                  dfs(root, v, curnode, nvisited, nkmer_sum);
                else dfs(root, v, curnode, nvisited, 0);
            }
    }
};


///                            *** Eulerian cycle ****                   ///
///                                ---------------                       ///
class EulerianCycle{
public:

    ///Eulerian path passes all edges and every edge exactly once
    void eulerian_cycle(Graph& graph, vector<int>& cycle){

        int n = graph.size();
        map<int, int> optionalStarts;               //vertexes still has unvisited edges
        vector<int> last_ones(1, 0);                //choose the last vertex to keep vertexes in the same order
        vector<int> new_cycle;

        int root = find_valid_start_node(graph);
        cycle.push_back(root);                         //start from 0 node
        optionalStarts[root] = root;                    //the first start cycle

        int k = 0;
        while(!optionalStarts.empty()){
            //find the last node still has unvisited edges. pick anyone of them
            int start = find_start_vertex(optionalStarts, last_ones);
            int position = optionalStarts[start];                       //it is position in the main cycle

            //find sub circle
            int next_node, cur_node;
            cur_node = start;
            int pos = position;

            do{
                next_node = *(graph[cur_node][0].begin());  //get it
                graph[cur_node][0].erase(next_node);        //pop it
                new_cycle.push_back(next_node);

                //if still has unvisited edges keep it to visit again
                if(graph[cur_node][0].size()) {optionalStarts[cur_node] = pos; last_ones.push_back(cur_node);}
                else                             optionalStarts.erase(cur_node);

                pos++;                  //position of the next node in the main cycle
                cur_node = next_node;
            }while(cur_node != start);

            //append the cycle behind the start nod
            cycle.insert(cycle.begin() + position + 1, new_cycle.begin(), new_cycle.end());
            new_cycle.clear();
        }
    }

    /// find new start vertex
    int find_start_vertex(const map<int, int>& opts, vector<int>& last_ones){
        while(!last_ones.empty()){
            int start = last_ones.back();
            last_ones.pop_back();

            if(opts.find(start) != opts.end())
                return start;
        }
    }

    ///find valid start node from the original graph
    int find_valid_start_node(const Graph& graph){
        fora(i, 0, graph.size())
            if(graph[i][0].size() != 0) return i;
    }
};

///                                         read_genome_from_eulerian_cycle
///                                         *******************************
class Genome_DebruijnGraph{
private:

    ///cut one of common suffix and prefix of the genome
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
public:

    ///print the path
    string get_genome_from_kmers(const vector<string>& k_1_mers, vector<int>& path){
        stringstream universal_string;
        universal_string << k_1_mers[path[0]];

        fora(i, 1, path.size())
            universal_string << k_1_mers[path[i]][KMER - 2];

        string result = universal_string.str();
        int n = result.size();

        int overlap = findMatchedLength(result, n);
        return result.substr(0, n - overlap);
    }
};
/*void print(Graph& graph, vector<string>& k_1_mers){
fora(i, 0, graph.size()){
        cout<<k_1_mers[i]<<" > ";
        for(const auto& v : graph[i][0]) cout<<k_1_mers[v]<<" ";
        cout<<endl;
    }
}*/

int main(int argc, char** argv){
    vector<string> k_1_mers;
    vector<map<int, int>> kmer_count;

    ///make de_bruijn graph
    DebruijnGraph dbGraph;
    Graph graph = dbGraph.de_bruijn_graph(k_1_mers, kmer_count);

    ///remove tips
    TipsRemoval tr1(graph) ;
    tr1.remove_tips();

    ///handle bubbles
   BubbleDetection bd(graph, kmer_count, k_1_mers);
   bd.handle_bubbles();

    ///remove tips
   TipsRemoval tr2(graph) ;
  tr2.remove_tips();

  ///Eulerian cycle
   vector<int> euler_cyc;
   EulerianCycle ec;
   ec.eulerian_cycle(graph, euler_cyc);

   ///read the genome from eulerian cycle
   Genome_DebruijnGraph gdbg;
   cout<<gdbg.get_genome_from_kmers(k_1_mers, euler_cyc)<<endl;

    return 0;
}
