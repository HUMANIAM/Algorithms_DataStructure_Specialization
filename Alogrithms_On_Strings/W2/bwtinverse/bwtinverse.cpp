#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

typedef struct {
    char symbol;
    int index;
}Symbol;

typedef map<char, vector<Symbol> > FirstCol;
typedef map<char, vector<int> > SymCount;


void CountOcurrence(const string& bwt, SymCount& symbol_count){
  int n = bwt.size();
  symbol_count['A'] = symbol_count['$'] = symbol_count['T'] = vector<int>(2, 0);
  symbol_count['G'] = symbol_count['C'] = vector<int>(2, 0);

  for(int i=0; i<n; i++){
    symbol_count[bwt[i]][1]++;
  }
}

void buildFirstCol(const string& bwt, FirstCol & firstcol, SymCount& symbol_count){
    int j=0;
    for(const auto& symbol : symbol_count){
    char c = symbol.first;                      // symbol
    int occur = (symbol.second)[1];             //number of occurrence in the bwt

    if(occur == 0) continue;                    //does not occur

    // add entry of the current symbol in the first column
    firstcol[c] = vector<Symbol>();

    //add the ith symbols in the last column that the current symbol points to
    while(occur--){
        Symbol symb;
        char lc = bwt[j++];
        symb.symbol = lc;
        symb.index = symbol_count[lc][0]++;

        //c in first column points to lc with specific rank in the last column
        firstcol[c].push_back(symb);
    }
  }
}

string getOriginal(const string& bwt, FirstCol& firstcol){
//using the first-last property in bwt we can create the original text
    string text = "";
   char curSymb = firstcol['$'][0].symbol;
   int number = firstcol['$'][0].index;

   while(curSymb != '$'){
    text.append(1, curSymb);

    //update curSymbol
    char tc = firstcol[curSymb][number].symbol;
    number = firstcol[curSymb][number].index;
    curSymb = tc;
   }

  reverse(text.begin(), text.end());
  return text + "$";
}

string InverseBWT(const string& bwt) {

  SymCount symbol_count;
  CountOcurrence(bwt, symbol_count);        //count how many every symbol occurs?

  //build the first column in form (key:symbol : value[symbols that refers to and their positions in the first column])
  FirstCol firstcol;
  buildFirstCol(bwt, firstcol, symbol_count);

  return getOriginal(bwt, firstcol);
}

int main() {

  string bwt;
  cin >> bwt;
  cout << InverseBWT(bwt) << endl;
  return 0;
}
