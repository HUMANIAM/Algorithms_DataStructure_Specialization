#include <ios>
#include <iostream>
#include <vector>
#include <set>

using namespace std;

/**
In this integer linear programming problem we encode it into sat problem by using 3cnf formula
we have n variables and m constraints. any variable i in constraint j must hold the same value either 0 or 1
constraints
1- any assignment of variables must hold of the LHS less than or equal the RHS
*/
struct Position{
    int coef, var;
    Position(int c, int v): coef(c), var(v){}
};

typedef vector<vector<int> > Matrix;
typedef vector<vector<Position>> CoEFFIENTS;

struct ConvertBudgetProblemToSat {
    private:
        int m, n, nclauses;
        vector<int> b_values;
        CoEFFIENTS coeffients;
        Matrix clauses;
        int noSolution;


    public:
        int mapint(int i){
            if(i==0) return 1;
            if(i==1) return -1;
        }
        bool handleOneVars(int con){
            int breakAssignments = 0;
            int coef = coeffients[con][0].coef;
            int var = coeffients[con][0].var;

            for(int boolv = 0; boolv<2; boolv++)
              if(boolv* coef > b_values[con]){
                clauses.push_back(vector<int>());
                clauses[nclauses++].push_back(mapint(boolv)*var);
                breakAssignments++;
              }

            if(breakAssignments == 2) return true;
            return false;
        }

        bool handleTwoVars(int con){
            int breakAssignments = 0;
            int coeffs[2] = {coeffients[con][0].coef, coeffients[con][1].coef};
            int vars[2] =   {coeffients[con][0].var, coeffients[con][1].var};

            for(int i=0; i<2; i++)
                for(int j=0; j<2; j++){
                    int val = i*coeffs[0] + j*coeffs[1];
                    if(val > b_values[con]){
                        clauses.push_back(vector<int>());
                        clauses[nclauses].push_back(mapint(i)*vars[0]);
                        clauses[nclauses++].push_back(mapint(j)*vars[1]);
                        breakAssignments++;
                    }
                }
            if(breakAssignments == 4) return true;
            return false;
        }

        bool handleThereVars(int con){
            int breakAssignments = 0;
            int coeffs[3] = {coeffients[con][0].coef, coeffients[con][1].coef, coeffients[con][2].coef};
            int vars[3] =   {coeffients[con][0].var, coeffients[con][1].var, coeffients[con][2].var};

            for(int i=0; i<2; i++)
                for(int j=0; j<2; j++)
                    for(int k=0; k<2; k++){
                    int val = i*coeffs[0] + j*coeffs[1] + k*coeffs[2];
                    if(val > b_values[con]){
                        clauses.push_back(vector<int>());
                        clauses[nclauses].push_back(mapint(i)*vars[0]);
                        clauses[nclauses].push_back(mapint(j)*vars[1]);
                        clauses[nclauses++].push_back(mapint(k)*vars[2]);
                        breakAssignments++;
                    }
                }
            if(breakAssignments == 8) return true;
            return false;
        }

        int addConstraints(){
            nclauses = 0;
            noSolution = false;
            int sz;
            for(int i=0; i<m; i++){
                sz = coeffients[i].size();
                // if coefficients are zero and b is negative
                if(sz == 0 && b_values[i]<0){
                    noSolution = true;
                    break;

                }else if(sz == 1){
                    if((noSolution = handleOneVars(i)))
                        break;

                }else if(sz == 2){
                    if((noSolution=handleTwoVars(i)))
                        break;
                }else if(sz==3){
                    if((noSolution=handleThereVars(i)))
                        break;
                }
            }
            if(noSolution)
                return -1;
            return nclauses;
        }
        void printEquisatisfiableSatFormula(){
            int res = addConstraints();
            if(res == -1){ // no solution
                cout<<2<<" "<<1<<endl;
                cout<<1<<" "<<0<<endl;
                cout<<-1<<" "<<0<<endl;
            }else if(nclauses == 0){ //any permutations of variables will satisfy the constraints
                cout<<1<<" "<<1<<endl;
                cout<<1<<" "<<-1<<" "<<0<<endl;
            }else{
                cout<<clauses.size()<<" "<<n<<endl;
                for(int i=0; i<clauses.size(); i++){
                    for(int j=0; j<clauses[i].size(); j++)
                        cout<<clauses[i][j]<<" ";
                    cout<<0<<endl;
                }

            }
        }


        void readConstraints(){
            int b, co;
            cin>>m>>n;
            //initialize the vertexes
            for(int i=0; i<m; i++)
                coeffients.push_back(vector<Position>());

            for(int i=0; i<m; i++)
                for(int j=0; j<n; j++){
                    cin>>co;
                    if(co!=0)
                        coeffients[i].push_back(Position(co, j+1));
                }

            //read b vector
            for(int i=0; i<m; i++){
               cin>>b;
               b_values.push_back(b);
            }
        }
};

int main() {
    ios::sync_with_stdio(false);

    ConvertBudgetProblemToSat converter;
    converter.readConstraints();
    converter.printEquisatisfiableSatFormula();

    return 0;
}
