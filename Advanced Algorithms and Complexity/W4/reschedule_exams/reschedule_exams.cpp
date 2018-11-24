#include <bits/stdc++.h>
using namespace std;
#define NIL -1
#define COLORS 3
/**
In this problem I practice a very powerful technique to solve many problems. it is the (Reduction) technique
this problem is about 3 recoloring graph where every node already has a color and we want to assign to every vertex
a new color and avoid 2 adjacent colors have the same color
*how we reduce it
1- we have 3 constraints
    * every color must recolored with new color
    * every node must assigned to only one of the two other colors
    * every 2 adjacent nodes must have different colors

2- 2sat is a special case of cnf formula and can be solved in liner time using implication graph and SCCs to find the
    solution O(V + M)

This problem is really a beautiful problem where you use previous knowledge to solve new problems (reduction)
*/
struct Edge {
    int from;
    int to;
};
struct MapNode{
    int coloring;
    char color;
    MapNode(char co): color(co), coloring(-1)
    {}
};
typedef vector<vector<int>> Matrix;
typedef vector<Edge> EDGES;

//********************************************* convert the 3-recoloring problem to 2-sat problem *************
//                                              -------------------------------------------------
struct ConvertThreeColorToSat {
    int n, m;
    int literals;
    ConvertThreeColorToSat(int n, int m){
        this->n = n;
        this->m = m;
        this->literals = n * COLORS;
    }

    void add_edgeConstraints(Matrix& clauses, const vector<int>& colors, const EDGES& edges, const Matrix& labelTable){
        //add constraints to make sure no 2 vertexes have the same color
        int nclauses = n * COLORS;

        for(int i=0; i<m; i++)
            for(int color=0; color<COLORS; color++){
                clauses[nclauses].push_back(literals + labelTable[edges[i].from - 1][color]);
                clauses[nclauses].push_back(literals + labelTable[edges[i].to - 1][color]);
                nclauses++;
            }
    }

    void add_vertexConstraints(Matrix& clauses, const vector<int>& colors, const Matrix& labelTable){
        int nclauses = 0;
        for(int node=0; node<n; node++){
            // each vertex must change to new color
            int color = colors[node];
            clauses[nclauses].push_back(literals + labelTable[node][color]);
            clauses[nclauses++].push_back(literals + labelTable[node][color]);

            //this node must take one of the two other colors
            int color1 = (color + 1) % COLORS;
            int color2 = (color + 2) % COLORS;

            clauses[nclauses].push_back(labelTable[node][color1]);
            clauses[nclauses++].push_back(labelTable[node][color2]);

            // only one of them must be true
            clauses[nclauses].push_back(literals + labelTable[node][color1]);
            clauses[nclauses++].push_back(literals + labelTable[node][color2]);
        }
    }

    Matrix convert2sat(const vector<int>& colors, const EDGES& edges, const Matrix& labelTable) {
        Matrix clauses(COLORS * (m + n));
        add_vertexConstraints(clauses, colors, labelTable);
        add_edgeConstraints(clauses, colors, edges, labelTable);
        return clauses;
    }
};
//************************************* 2-sat solver ******************************
//                                      ------------
class CurcuitDesign
{
    public:
    vector<set<int> > adj, sccs;
    int vertixNum;
    int literals;
    int ncluases;
    map<int, MapNode*> mapVector;
    Matrix labelTable;

    CurcuitDesign(int literals, int nclauses, map<int, MapNode*>& mapVector, const Matrix& labelTable){
        this->literals = literals;
        this->ncluases = nclauses;
        this->mapVector = mapVector;
        this->labelTable = labelTable;

        //initialize the implication graph
        vertixNum = 2*literals;
        for(int i=0; i<vertixNum; i++)
            adj.push_back(set<int>());
    }

     int curcuitDesignIsSatisfied(const Matrix& clauses){
        createImplicationGraph(clauses);
        SCC();

        // if there is a scc contains a literal and its negation the formula can't be satisfied
        if(contradictionAssignment()){
            return -1;
        }

        //find a possible assignment of literals that satisfies the formula
        assignSolution();
        return 1;
     }

     void assignSolution(){
        for(const auto& scc : sccs)
            for(const auto& v : scc){
                int node = v%literals;
                if(mapVector[node]->coloring == -1)
                    if(v >= literals)
                        mapVector[node]->coloring = 0;
                    else
                        mapVector[node]->coloring = 1;
            }
     }

     bool contradictionAssignment(){
        for(const auto& scc : sccs){
            set<int> check;
            for(const auto& s : scc){
                if(s >= literals && check.find(s%literals) != check.end())
                    return true;
                if(s<literals && check.find(s+literals) != check.end())
                    return true;
                check.insert(s);
            }
        }
        return false;
     }

    void SCC(){    // prints strongly connected components
        int *disc = new int[vertixNum];
        int *low = new int[vertixNum];
        bool *stackMember = new bool[vertixNum];
        stack<int> st;

        // Initialize disc and low, and stackMember arrays
        fill(disc, disc+vertixNum, NIL);
        fill(low, low+vertixNum, NIL);
        fill(stackMember, stackMember+vertixNum, false);

        // find strong connected component of vertex i
        for (int i = 0; i < vertixNum; i++)
            if (disc[i] == NIL)
                SCCUtil(i, disc, low, st, stackMember);
    }

    void createImplicationGraph(const Matrix& clauses){
        // read clauses
        for(int i=0; i<ncluases; i++){
            int var, vvar, nvar, nvvar;
            var = clauses[i][0];
            vvar = clauses[i][1];
            nvar = (var + literals) % vertixNum;
            nvvar = (vvar + literals) % vertixNum;

            adj[nvar].insert(vvar);     //var1 implies var2
            adj[nvvar].insert(var);     //var2 implies var1
        }
    }

    void SCCUtil(int u, int disc[], int low[], stack<int>& st, bool stackMember[]){
    static int time = 0;    //order of discovery

    disc[u] = low[u] = ++time;  //at the first visiting every vertex considered a scc
    st.push(u);
    stackMember[u] = true;

    // Go through all vertices adjacent to this
    for(const auto& v : adj[u]){
        if (disc[v] == -1){
            SCCUtil(v, disc, low, st, stackMember);

            // Check if the subtree rooted with 'v' has a
            // connection to one of the ancestors of 'u'
            low[u]  = min(low[u], low[v]);
        }

        //if v is discovered before then (u, v) may be a connection edge between 2 scc's or
        //u belongs to a strong connected component rooted at v
        else if (stackMember[v] == true)
            low[u]  = min(low[u], disc[v]);
    }

    // head node found, pop the stack and print an SCC
    int w = 0;
    if (low[u] == disc[u]){
        sccs.push_back(set<int>());
        int index = sccs.size()-1;
        while (true)
        {
            w = (int) st.top();
            sccs[index].insert(w);
            stackMember[w] = false;
            st.pop();
            if(w==u) //this is the root of the SCC
                break;
        }
    }
}

};

//################################## 3-recoloring class ####################
class ThreeRecoloring{
public:
    int n, m;
    string colors;

    void solve(){
        cin >> n >> m;
        cin >> colors;

        EDGES edges(m);
        //read edges of the graph
        for (int i = 0; i < m; i++)
            cin >> edges[i].from >> edges[i].to;

        //map colors from char to int
        vector<int> icolors;
        convetcolorsToInt(n, icolors, colors);

         //make table of labels for [vertex][color]
        Matrix labelTable(n);
        map<int, MapNode*> mapVector;
        createLabelTable(labelTable, mapVector, n);

        //reduce the problem from 3-recoloring problem to 2-sat problem
        ConvertThreeColorToSat converter(n, m);
        Matrix clauses = converter.convert2sat(icolors, edges, labelTable);

        //solve the problem using 2-sat solver
        int literals = COLORS * n;
        int nclauses = clauses.size();

        CurcuitDesign curcuitdes(literals, nclauses, mapVector, labelTable);
        int thereisAsolution = curcuitdes.curcuitDesignIsSatisfied(clauses);

        if(thereisAsolution == 1)
            //print the solution
            printSolution(mapVector);
        else
            cout<<"Impossible"<<endl;
    }

    void convetcolorsToInt(int n, vector<int>& icolors, const string& colors){
        for(int i=0; i<n; i++){
            if(colors[i] == 'R')
                icolors.push_back(0);
            else if(colors[i] == 'G')
                icolors.push_back(1);
            else
                icolors.push_back(2);
        }
    }

    void createLabelTable(Matrix& labelTable, map<int, MapNode*>& mapVector, int n){
        int label = 0;
        char arrColor[3] = {'R', 'G', 'B'};
        for(int node=0; node<n; node++)
            for(int color=0; color<COLORS; color++){
                labelTable[node].push_back(label);
                mapVector[label++] = new MapNode(arrColor[color]);
            }
    }

    void printSolution(map<int, MapNode*>& mapvector){
        int label = 0;

        for(int j=0; j<n; j++){
            int i = 0;
            while(i++<COLORS){
                if(mapvector[label]->coloring == 1){
                    cout<<mapvector[label]->color;
                    break;
                }
                label++;
            }
            label += COLORS - i + 1;
        }
        cout<<endl;
    }

};

int main() {
    ThreeRecoloring threerecolorgraph;
    threerecolorgraph.solve();
}

