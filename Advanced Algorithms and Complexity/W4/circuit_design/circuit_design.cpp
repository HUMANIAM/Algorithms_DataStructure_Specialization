#include <bits/stdc++.h>
#define NIL -1
using namespace std;
typedef vector<set<int> > Matrix;

// A class that represents an directed graph
class CurcuitDesign
{
    public:
    Matrix adj, sccs;
    int vertixNum;
    int literals;

     void curcuitDesignIsSatisfied(){
        getFormula();
        SCC();
        // if there is a scc contains a literal and its negation the formula can't be satisfied
        if(contradictionAssignment()){
            cout<<"UNSATISFIABLE"<<endl;
            return;
        }

        //find a possible assignment of literals that satisfies the formula
        vector<int> solution(vertixNum, -1);
        assignSolution(solution);

        //print solution
        cout<<"SATISFIABLE"<<endl;
        for(int i=0; i<literals; i++){
            if(solution[i] == 1)
                cout<<i+1;
            else cout<<-(i+1);

            if(i == literals-1) cout<<endl;
            else cout<<" ";
        }
     }

     void assignSolution(vector<int>& solution){
        for(const auto& scc : sccs)
            for(const auto& v : scc){
                if(solution[v] == -1){
                    solution[v] = 1;
                    if(v<literals)
                        solution[v+literals] = 0;
                    else
                        solution[v%literals] = 0;
                }
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

        // Call the recursive helper function to find strongly
        // connected components in DFS tree with vertex 'i'
        for (int i = 0; i < vertixNum; i++)
            if (disc[i] == NIL)
                SCCUtil(i, disc, low, st, stackMember);
    }

    int mapInt(int v){
        if(v>0)
            return v-1;
        else
            return (abs(v)-1) + vertixNum/2;
    }

    void getFormula(){
        int v, c;
        cin>>v>>c;
        literals = v;
        vertixNum = 2*literals;
        for(int i=0; i<vertixNum; i++)
            adj.push_back(set<int>());

        // read clauses
        for(int i=0; i<c; i++){
            int var, vvar, nvar, nvvar;
            cin>>var>>vvar;
            nvar = mapInt(-var);
            nvvar = mapInt(-vvar);
            var = mapInt(var);
            vvar = mapInt(vvar);
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
            if(w==u)
                break;
        }
    }
}

};

// Driver program to test above function
int main()
{
    CurcuitDesign curcuitdes;
    curcuitdes.curcuitDesignIsSatisfied();

    return 0;
}
