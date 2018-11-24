#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

const int INF = 1e9;
typedef vector<vector<int> > Matrix;
typedef struct Cell{
    int cost, parent;
    Cell():cost(INF), parent(-1)
    {}
}Cell;
typedef vector<vector<Cell> > DpTable;

Matrix read_data() {
    int vertex_count, edge_count;
    cin >> vertex_count >> edge_count;
    Matrix graph(vertex_count, vector<int>(vertex_count, INF));

    //weight from every vertex to itself is equal 0
    for(int i=0; i<vertex_count; i++)
        graph[i][i] = 0;

    for (int i = 0; i < edge_count; ++i) {
        int from, to, weight;
        cin >> from >> to >> weight;
        --from, --to;
        graph[from][to] = graph[to][from] = weight;
    }
    return graph;
}

vector<int> getMembers(int s){
    vector<int> setMembers(1, 0);
    int j=0, i;
    while((i=pow(2, ++j)) < s){
        if((i & s) > 0)
            setMembers.push_back(j);
    }

    return setMembers;
}

int findPath(const DpTable& dptable, const Matrix& graph, vector<int>& path){
    int r = dptable.size() - 1;
    int shortestOne = INF;
    int c = -1;
    for(int j=0; j<dptable[r].size(); j++)
        if(dptable[r][j].cost + graph[0][j] < shortestOne){
            shortestOne = dptable[r][j].cost + graph[0][j] ;
            c = j;
        }

    if(c==-1) return -1;

    int sset = r*2 + 1;
    //build the path
    do{
        path.push_back(c);
        sset = sset ^ (1<<c);
        c = dptable[r][c].parent;
        r = (sset-1)/2;
    }while(sset);

    return shortestOne;
}

pair<int, vector<int> > optimal_path(const Matrix& graph) {
    size_t n = graph.size();
    const int UpperBound = pow(2, n);
     DpTable dpTable(UpperBound/2, vector<Cell>(n, Cell()));

    //fill the dpTable
    dpTable[0][0].cost = 0;
    for(int s=3; s<UpperBound; s+=2){
        vector<int> setMembers = getMembers(s);
        int sindex = (s-1)/2;
        int setSize = setMembers.size();

        for(int i=0; i<setSize; i++){
            int iMember = setMembers[i];
            if(iMember != 0){
                int setj = ((s ^ (1 << iMember)) - 1) / 2;     //the set without i to get the best path from s-{i} to i
                for(int j=0; j<setSize; j++){
                    int jMember = setMembers[j];
                    if(jMember != iMember){
                        int candCost = dpTable[setj][jMember].cost + graph[jMember][iMember];
                        if( candCost < dpTable[sindex][iMember].cost){
                            dpTable[sindex][iMember].cost = candCost;
                            dpTable[sindex][iMember].parent = jMember;
                        }
                    }
                }
            }
        }
    }

    // get the path if found
    vector<int> best_path;
    int best_ans = findPath(dpTable, graph, best_path);

    return make_pair(best_ans, best_path);
}

void print_answer(const pair<int, vector<int> >& answer) {
    cout << answer.first << "\n";
    if (answer.first == -1)
        return;
    const vector<int>& path = answer.second;
    for (int i = path.size()-1; i >= 0; i--)
        cout << path[i] + 1 << " ";
    cout << "\n";
}

int main() {
    print_answer(optimal_path(read_data()));
    return 0;
}
