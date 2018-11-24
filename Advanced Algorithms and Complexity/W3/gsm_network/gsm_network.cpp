#include <ios>
#include <iostream>
#include <vector>

using namespace std;

/**
In this problem I will transfer the 3-color problem into sat problem by build the 3-cnf formula
under these constraints nodes numbered from 1 to n
1- if u, v are adjacent then u and v must colored with different colors
2- every vertex must colored with only one color
I will use the numbering schema for literals related to
vertex constraint : 3*i+7*j
edge constraint : 3*i+7*j+11
*/
const int K = 3;
struct Edge {
    int from;
    int to;
};

struct ConvertGSMNetworkProblemToSat {
    int numVertices;
    int numEdges;
    vector<Edge> edges;

    ConvertGSMNetworkProblemToSat(int n, int m) :
        numVertices(n),
        edges(m),
        numEdges(m)
    {  }

    void add_edgeConstraints(vector<vector<int> >& clauses){
        //add constraints to make sure no 2 vertexes have the same color
        int nclauses = numVertices;

        for(int i=0; i<numEdges; i++){
            int from = edges[i].from - 1;
            int to = edges[i].to -1 ;
            for(int j=0; j<K; j++){
                clauses[nclauses].push_back(-clauses[from][j]);
                clauses[nclauses].push_back(-clauses[to][j]);
                nclauses++;
            }
        }
    }

    void add_vertexConstraints(vector<vector<int> >& clauses){
        for(int i=1; i<=numVertices; i++){
            //vertex constraint vertex must have only one color
            for(int j=1; j<=K; j++)
                clauses[i-1].push_back(3*i + 7*j);
        }
    }

    void printEquisatisfiableSatFormula() {
        vector<vector<int> > clauses(numVertices + 3*numEdges);
        add_vertexConstraints(clauses);
        add_edgeConstraints(clauses);

        cout<<clauses.size()<<" "<<3*numVertices+7*3<<endl;
        for(int i=0; i<clauses.size(); i++){
            for(int j=0; j<clauses[i].size(); j++)
                cout<<clauses[i][j]<<" ";
            cout<<"0\n";
        }
    }
};

int main() {
    ios::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;
    ConvertGSMNetworkProblemToSat converter(n, m);
    for (int i = 0; i < m; ++i) {
        cin >> converter.edges[i].from >> converter.edges[i].to;
    }

    converter.printEquisatisfiableSatFormula();

    return 0;
}
