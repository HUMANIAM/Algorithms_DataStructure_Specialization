#include <ios>
#include <iostream>
#include <vector>
#include <set>

using namespace std;

/**
In this problem I will transfer the clean-apartment problem into sat problem by build the 3-cnf formula
we can solve this problem by Hamiltonian path where we need to find the path that visits all vertexes only once
so we map Hamiltonian problem to sat problem
constraints
1- every vertex is visited only once in all the path
2- every position in Hamiltonian path has only one vertex of the graph
3- any 2 vertexes in graph are not adjacent so they must also not adjacent in the path
 using numeric schema 3*position + 7*vertex
*/
typedef vector<vector<int> > Clauses;
typedef vector<set<int>> Edges;

struct ConvertCleanApartmentProblemToSat {
    int numVertices;
    int numEdges;
    Edges edges;

    void add_adjacConstraints(Clauses& clauses){
        // any two vertexes in the graph are not adjacent they must also adj in the Hamiltonian path
        int nclauses = clauses.size();
        for(int v=1; v<=numVertices; v++)
            for(int vv=1; vv<=numVertices; vv++)
                if(v!=vv && edges[v].find(vv) == edges[v].end())
                    for(int p=1; p<numVertices; p++){
                      clauses.push_back(vector<int>());
                      int var1 = (v - 1)*100 + (p - 1) + 1;
                      int var2 = (vv - 1)*100 + (p) + 1;
                      clauses[nclauses].push_back(-var1);
                      clauses[nclauses++].push_back(-var2);
                    }
    }

    void exactOne(Clauses& clauses, int from){
        int nclauses = clauses.size();

        for(int i=from; i<2*numVertices; i++)
            for(int j=0; j<numVertices; j++)
                for(int k=j+1; k<numVertices; k++){
                    clauses.push_back(vector<int>());
                    clauses[nclauses].push_back(-clauses[i][j]);
                    clauses[nclauses].push_back(-clauses[i][k]);
                    nclauses++;
                }
    }

   void addConstraints(Clauses& clauses){
    int nclauses = 0;
    //add position constraints
    for(int p=1; p<=numVertices; p++){
        clauses.push_back(vector<int>());
        for(int v=1; v<=numVertices; v++)
            clauses[nclauses].push_back((v - 1)*100 + (p - 1) + 1);
        nclauses++;
    }

    //add vertex constraints
    for(int v=0; v<numVertices; v++){
        clauses.push_back(vector<int>());
        for(int p=0; p<numVertices; p++)
            clauses[nclauses].push_back(clauses[p][v]);
        nclauses++;
    }

    //every vertex occupy only once in the path
    exactOne(clauses, numVertices);

    //if two vertexes in the graph are disadjacent they must not also in the path
    add_adjacConstraints(clauses);

   }

    void printEquisatisfiableSatFormula() {
        Clauses clauses;
        addConstraints(clauses);

        cout<<clauses.size()<<" "<<(numVertices - 1)*100 + (numVertices - 1) + 1<<endl;
        for(int i=0; i<clauses.size(); i++){
            for(int j=0; j<clauses[i].size(); j++)
                cout<<clauses[i][j]<<" ";
            cout<<"0"<<endl;
        }
    }

    void readEdges(){
        cin>>numVertices>>numEdges;
        //initialize the vertexes
        for(int i=0; i<=numVertices; i++)
            edges.push_back(set<int>());

        for(int i=0; i<numEdges; i++){
            int f, t;
            cin>>f>>t;
            edges[f].insert(t);
            edges[t].insert(f);
        }
    }
};

int main() {
    ios::sync_with_stdio(false);

    ConvertCleanApartmentProblemToSat converter;
    converter.readEdges();
    converter.printEquisatisfiableSatFormula();

    return 0;
}
