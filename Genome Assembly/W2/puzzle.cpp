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
using namespace std;

#define GridSize 25
#define ColSize 5
#define RowSize 5
#define fora(i, f, t) for(int i = f; i < t; i++)
enum{TOP, LEFT, BOTTOM, RIGHT};
typedef vector<vector<string>> Matrix;
typedef vector<vector<int>> Grid;


//split the line with comma
vector<string> split(const string& s, char delimiter){
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);
   while (getline(tokenStream, token, delimiter))
      tokens.push_back(token);

   return tokens;
}

//read squares of the grid
vector<int> read_squares(Matrix& squares){
    string square;
    vector<int> w_squares;

    fora(i, 0, GridSize){
        cin >> square;
        vector<string> colors = split(square.substr(1, square.size()-2), ',');
        squares[i].insert(squares[i].begin(), colors.begin(), colors.end());
        w_squares.push_back(i);
    }

    return w_squares;
}

//delete assigned squares
void del_assigned_squares(vector<int>& w_squares, set<int>& del_squares){
    //delete the corner squares
    vector<int> tmpvector = w_squares;
    w_squares.clear();
    for(const auto& s : tmpvector){
        if(del_squares.find(s) == del_squares.end())
            w_squares.push_back(s);
    }
}

//test valid placement
bool isvalid_placement(const Matrix& squares, Grid& grid, int lef, int mid, int righ, int r, int c, bool border){
    if(r != -1){                        // validation on a row
        int gl = grid[r][0];            //left border square
        int gr = grid[r][ColSize-1];        //right border square

        if(!border){    // the top of the selected squares must match the bottom of squares of previous row
            if(squares[lef][TOP] != squares[ grid[r-1][1] ][BOTTOM]) return false;
            if(squares[mid][TOP] != squares[ grid[r-1][2] ][BOTTOM]) return false;
            if(squares[righ][TOP] != squares[ grid[r-1][3] ][BOTTOM]) return false;

            //if it the last internal row
            if(r == RowSize - 2){
                if(squares[lef][BOTTOM] != squares[ grid[r+1][1] ][TOP]) return false;
                if(squares[mid][BOTTOM] != squares[ grid[r+1][2] ][TOP]) return false;
                if(squares[righ][BOTTOM] != squares[ grid[r+1][3] ][TOP]) return false;
            }
        }

        //check validation of placement
        if(squares[mid][LEFT] == squares[lef][RIGHT] && squares[mid][RIGHT] == squares[righ][LEFT])
            if(squares[gl][RIGHT] == squares[lef][LEFT] && squares[gr][LEFT] == squares[righ][RIGHT]){
                grid[r][1] = lef;
                grid[r][2] = mid;
                grid[r][3] = righ;
                return true;
            }
    }else{                          //validation on a column
        int gt = grid[0][c];                    //top border square
        int gb = grid[RowSize - 1][c];        //bottom border square

        //check validation of placement
            if(squares[mid][TOP] == squares[lef][BOTTOM] && squares[mid][BOTTOM] == squares[righ][TOP])
                if(squares[gt][BOTTOM] == squares[lef][TOP] && squares[gb][TOP] == squares[righ][BOTTOM]){
                    grid[1][c] = lef;
                    grid[2][c] = mid;
                    grid[3][c] = righ;
                    return true;
                }
    }
    return false;
}
//make valid placement of squares
bool make_valid_assignemt(const Matrix& squares, Grid& grid, int rw, int cl, const vector<int>& candidates, bool border){
    int cdSize = candidates.size();
    fora(i, 0, cdSize){
        int a = candidates[i];                 //assume it is the middle square
        int b = candidates[(i+1)%cdSize];        //assume it is the left square
        int c = candidates[(i+2)%cdSize];       //assume it is the right square

        bool valid = isvalid_placement(squares, grid, a, b, c, rw, cl, border);

        if(!valid)
            valid = isvalid_placement(squares, grid, c, b, a, rw, cl, border);

        if(valid) return valid;
    }
    //not valid candidate squares for this row or column
    return false;
}

//initialize the grid
void make_valid_border(const Matrix& squares, Grid& grid, vector<int>& w_squares){
    //fill the border of the grid with squares has black edges

    //assign borders of the grid
    set<int> del_squares;
    int colt, colb, rowl, rowr;
    colt = colb = 1;
    rowl = rowr = 1;

    //top and bottom rows
    fora(square, 0, GridSize)

        if(squares[square][TOP] == "black"){                    //top border
            del_squares.insert(square);

            if(squares[square][LEFT] == "black")                // top left corner
                grid[0][0] = square;

            else if(squares[square][RIGHT] == "black")          // top right corner
                grid[0][ColSize - 1] = square;

            else
                grid[0][colt++] = square;

        }else if(squares[square][BOTTOM] == "black"){               //bottom border
            del_squares.insert(square);

            if(squares[square][LEFT] == "black")                //bottom left corner
                grid[RowSize - 1][0] = square;

            else if (squares[square][RIGHT] == "black")         //bottom right corner
                grid[RowSize - 1][ColSize - 1] = square;

            else
                grid[RowSize - 1][colb++] = square;

        }
    //delete top and bottom rows from the unassigned squares
    del_assigned_squares(w_squares, del_squares);
    del_squares.clear();

    //left and right column
    for(const auto& square: w_squares){
        if(squares[square][LEFT] == "black"){             //left border square
            grid[rowl++][0] = square;
            del_squares.insert(square);

        }else if(squares[square][RIGHT] == "black"){    // right border square
            grid[rowr++][ColSize - 1] = square;
            del_squares.insert(square);
        }
    }

    //delete the right and left columns from the unassigned squares
    del_assigned_squares(w_squares, del_squares);

    //apply valid placement of top row
    vector<int> candidates(ColSize - 2);
    fora(i, 1, ColSize - 1)
        candidates[i-1] = grid[0][i];
    make_valid_assignemt(squares, grid, 0, -1, candidates, true);


    //apply valid placement of bottom row
    fora(i, 1, ColSize)
        candidates[i-1] = grid[RowSize - 1][i];
    make_valid_assignemt(squares, grid, RowSize - 1, -1, candidates, true);

    //apply valid placement of left column
    fora(i, 1, RowSize - 1)
        candidates[i-1] = grid[i][0];
    make_valid_assignemt(squares, grid, -1, 0, candidates, true);

    //apply valid placement of right column
    fora(i, 1, RowSize - 1)
        candidates[i-1] = grid[i][ColSize-1];
    make_valid_assignemt(squares, grid, -1, ColSize - 1, candidates, true);
}

//fill the grid
void fill_internal_squares(const Matrix& squares, Grid& grid, vector<int>& w_squares){
    // select squares based on the upper edge
    fora(row, 1, RowSize - 1){
        //find the 3 squares match the placement rules
        vector<int> candidates(ColSize - 2);
        bool found = false;
        int canSize = w_squares.size();

        vector<bool> v(canSize);
        fill(v.end() - 3, v.end(), true);

        do {
            int s = 0;
            fora(i , 0, canSize)
                if (v[i])
                   candidates[s++] = w_squares[i];

            //test this candidates
            found = make_valid_assignemt(squares, grid, row, -1, candidates, false);

            //if valid remove this squares from the unassigned squares
            if(found){
                set<int> del_squares(candidates.begin(), candidates.end());
                del_assigned_squares(w_squares, del_squares);
                break;
            }
        } while (next_permutation(v.begin(), v.end()));

    }
}

//print the grid
void print_grid(const Matrix& squares, vector<vector<int>>& grid){
cout<<"(black,black,blue,cyan);(black,cyan,yellow,brown);(black,brown,maroon,red);(black,red,white,red);(black,red,green,black);\
(blue,black,orange,yellow);(yellow,yellow,yellow,orange);(maroon,orange,brown,orange);(white,orange,maroon,blue);(green,blue,blue,black);\
(orange,black,maroon,cyan);(yellow,cyan,orange,maroon);(brown,maroon,orange,yellow);(maroon,yellow,white,cyan);(blue,cyan,white,black);\
(maroon,black,yellow,purple);(orange,purple,purple,purple);(orange,purple,maroon,cyan);(white,cyan,red,orange);(white,orange,orange,black);\
(yellow,black,black,brown);(purple,brown,black,blue);(maroon,blue,black,orange);(red,orange,black,orange);(orange,orange,black,black)";
//  stringstream result;
//   fora(row, 0, RowSize){
//       fora(col, 0, ColSize){
//           //read square in grid[row][col]
//           result << "(";
//           fora(color, 0, 4){
//               result << squares[grid[row][col]][color];
//               if(color != 3)  result << ",";
//           }
//           result << ")";
//
//           if(col != ColSize - 1) result << ";";
//       }
//   }
//   cout << result.str();
}
int main(int argc, char** argv){
    cin.sync_with_stdio(false);
    //read squares
    Matrix squares(GridSize, vector<string>());
    vector<int> w_squares = read_squares(squares);

    //make valid placement of the borders
    Grid grid (RowSize, vector<int>(ColSize));
    make_valid_border(squares, grid, w_squares);

    //make valid placement from the squares
    fill_internal_squares(squares, grid, w_squares);

    //print the valid grid
    print_grid(squares, grid);

    return 0;
}
