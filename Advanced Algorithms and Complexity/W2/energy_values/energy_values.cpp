#include <cmath>
#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace std;

// this code is really readable I love it. but right efficient code is more important
const double EPS = 1e-6;
const int PRECISION = 20;

typedef vector<double> Row;
typedef vector<Row> GuassianMatrix;
typedef vector<bool> BoolV;


GuassianMatrix ReadEquation() {
    int sz;
    cin >> sz;

    GuassianMatrix guassmatrix(sz, Row(sz+1));
    for (int row = 0; row < sz; ++row)
        for (int column = 0; column <= sz; ++column)
            cin>>guassmatrix[row][column];

    return guassmatrix;
}

int SelectPivotElement(const GuassianMatrix &a, BoolV &used_rows, int col) {
   int row = 0;
    while (used_rows[row] || a[row][col] == 0) row++;
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

void SolveEquation(GuassianMatrix& guassMatrix) {
    int sz = guassMatrix.size();

    BoolV used_rows(sz, false);
    for (int col = 0; col < sz; ++col) {
        int row = SelectPivotElement(guassMatrix, used_rows, col);
        SwapLines(guassMatrix, used_rows, row, col);
        row = col;
        ProcessPivotElement(guassMatrix, col);
        used_rows[row] = true;
    }

}

void PrintColumn(const GuassianMatrix &guassmatrix) {
    int sz = guassmatrix.size();
    cout.precision(PRECISION);
    for (int row = 0; row < sz; ++row)
        cout << guassmatrix[row][sz] << endl;
}

int main() {
    GuassianMatrix guassmatrix = ReadEquation();
    SolveEquation(guassmatrix);
    PrintColumn(guassmatrix);
    return 0;
}
