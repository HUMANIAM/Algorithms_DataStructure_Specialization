#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdio>
#include <set>
#include <float.h>
using namespace std;

/**this program solves the diet problem using linear programming. some factors needed to put in mind
    1- we have constraints Ax <= b A may be n*m dimensions m variables and n+m constrains
        m for non-negativity of variables and n for the constrains

    2- we can solve the problem by find all intersection points and test all the constrains if it satisfies them
        then it is a candidate solution. continue till find the feasible region gives the maximum solution
*/
const double EPS = 1e-3;
const int PRECISION = 20;

typedef vector<double> Row;
typedef vector<Row> GuassianMatrix;
typedef vector<bool> BoolV;
typedef vector<set<int> > SubSets;
enum {NSOL, SOL, INF} ;
int n, m;

int SelectPivotElement(const GuassianMatrix &a, BoolV &used_rows, int col) {
   int row = 0;
    while (row<m && (used_rows[row] || a[row][col] == 0)) row++;
    if(row==m) return -1;
    return row;
}

void SwapLines(GuassianMatrix &a, vector <bool> &used_rows, int r, int c) {
    swap(a[r], a[c]);
    swap(used_rows[r], used_rows[c]);
}

void ProcessPivotElement(GuassianMatrix &a, int pivot) {
    double A = a[pivot][pivot];
    int sz = a.size();
    //scale the pivot row
    for(int column=0; column<=sz; column++)
        a[pivot][column] /= A;

    //make the rest values of column zeros
    for(int row = 0; row<sz; row++)
        if(row != pivot && a[row][pivot] != 0){
            double val = a[row][pivot];
            //update row
            for(int colum=0; colum<=sz; colum++)
                a[row][colum] -= a[pivot][colum]*val;
        }
}

int SolveEquation(GuassianMatrix& guassMatrix) {
    int sz = guassMatrix.size();
    BoolV used_rows(sz, false);
    for (int col = 0; col < sz; ++col) {
        int row;
        if((row = SelectPivotElement(guassMatrix, used_rows, col)) == -1)
            return NSOL;
        SwapLines(guassMatrix, used_rows, row, col);
        row = col;
        ProcessPivotElement(guassMatrix, col);
        used_rows[row] = true;
    }
    return 1;
}

bool verifyConstraints(GuassianMatrix& guassMat, GuassianMatrix& subguassMat){
    //check all candidate solution for x are non-negative
    for(int i=0; i<m; i++)
        if(subguassMat[i][m]<0)
            return false;

    //check the remaining constraints <= b
    for(int i=m; i<=n+m; i++){
        double constrainVal = 0.0;
        for(int j=0; j<m; j++)
            constrainVal += guassMat[i][j]*subguassMat[j][m];

        if((constrainVal - EPS) >= guassMat[i][m] )
            return false;
    }
    return true;
}

bool isTheOptimal(GuassianMatrix& guassMat, GuassianMatrix& subGuassMat, Row& optSol, double& optimum, int& type){
    double subOptmal = 0.0;
    for(int i=0; i<m; i++)
        subOptmal += guassMat[n+m+1][i]*subGuassMat[i][m];

    if((subOptmal > optimum) || (subOptmal == optimum && type==INF)){
        optimum = subOptmal;
        for(int i=0; i<m; i++)
            optSol[i] = subGuassMat[i][m];
        return true;
    }
    return false;
}

pair<int, vector<double>> solve_diet_problem(GuassianMatrix guassMarix, SubSets& subsets) {
    int type = NSOL;
    double optimum = -DBL_MAX;       // It is a maximization problem
    Row optimalsolution(m, -1);
    int setSize = subsets.size();

    //test every subset of equations to get a vertex make the optimum value
    for(int s=0; s<setSize; s++){
        GuassianMatrix subGuassMat;
        for(const auto& row : subsets[s])
            subGuassMat.push_back(guassMarix[row]);

        //check if this solution satisfies the constraints
        if(SolveEquation(subGuassMat) == 1){ //there is a solution bounded or infinite
            int nsol = SOL;
             if(subsets[s].find(n+m) != subsets[s].end()) //it is unbounded solution
                nsol = INF;

           if(type==NSOL || !(type==INF && nsol==INF))
               if(verifyConstraints(guassMarix, subGuassMat))
                    if(isTheOptimal(guassMarix, subGuassMat, optimalsolution, optimum, type))
                        type = nsol;
        }
    }

    return {type, optimalsolution};
}

//subsets of n equations
void powerSets(SubSets& subsets, int row=0){
    static SubSets tsubsets;
    static int maxRow = n+m;

    if(row > maxRow){
        tsubsets.push_back(set<int>());
        return;
    }
    powerSets(subsets, row+1);

    //add current row to other sets
    int sz = tsubsets.size();
    for(int i=0; i<sz; i++){
        set<int> s = tsubsets[i];
        s.insert(row);
        tsubsets.push_back(s);

        //use only subsets with m equations
        if(s.size() == m)
            subsets.push_back(s);
    }
}

GuassianMatrix ReadEquation() {
    cin >> n >> m;
    GuassianMatrix guassmatrix(n+m+2, Row(m+1, 0));

    //set main constraints (non-negative constrains)
    for(int i=0; i<m; i++)
        guassmatrix[i][i] = 1;

    //get equations
    for(int i=m; i<n+m; i++)
        for(int j=0; j<m; j++)
            cin>>guassmatrix[i][j];

    //get b vector
    for(int i=m; i<n+m; i++)
        cin>>guassmatrix[i][m];

    //add infinity constrain
    for(int i=0; i<m; i++)
        guassmatrix[n+m][i] = 1;
    guassmatrix[n+m][m] = pow(10, 9);

    //get c vector
    for(int i=0; i<m; i++)
        cin>>guassmatrix[n+m+1][i];
    return guassmatrix;
}

int main(){
  GuassianMatrix guassmatrix = ReadEquation();
  SubSets subsets;
  powerSets(subsets);

  pair<int, vector<double>> ans = solve_diet_problem(guassmatrix, subsets);
  switch (ans.first) {
    case NSOL:
      printf("No solution\n");
      break;
    case SOL:
      printf("Bounded solution\n");
      for (int i = 0; i < m; i++) {
        cout.precision(20);
        cout<<ans.second[i]<< " \n"[i + 1 == m];
      }
      break;
    case INF:
      printf("Infinity\n");
      break;
  }
  return 0;
}
