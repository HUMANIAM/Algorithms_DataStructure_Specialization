#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

typedef map<char, vector<int> > Matrix;
typedef map<char, vector<int> > SymCount;

SymCount symbol_count;      //how many every character occurs in the text
                            // where it starts in the first column
void CountOcurrence(const string& bwt){
  int n = bwt.size();
  symbol_count['A'] = symbol_count['$'] = symbol_count['T'] = vector<int>(2, 0);
  symbol_count['G'] = symbol_count['C'] = vector<int>(2, 0);

  for(int i=0; i<n; i++){
    symbol_count[bwt[i]][1]++;
  }
}


// Preprocess the Burrows-Wheeler Transform bwt of some text
// and compute as a result:
//   * starts - for each character C in bwt, starts[C] is the first position
//       of this character in the sorted array of
//       all characters of the text.
//   * occ_count_before - for each character C in bwt and each position P in bwt,
//       occ_count_before[C][P] is the number of occurrences of character C in bwt
//       from position 0 to position P inclusive.
void PreprocessBWT(const string& bwt, Matrix & occ_count_before) {
    CountOcurrence(bwt);
  int n = bwt.size();
  //where every char in the first column starts
  int st = 0;
  for(auto& el : symbol_count){
    (el.second)[0] = st;
    st += (el.second)[1];

    occ_count_before[el.first] = vector<int>(1,0);
  }

  //build occ_count_before from the last column
  for(int i=0; i<n; i++){

  for(auto& el : occ_count_before){
    (el.second).push_back((el.second)[i]);
  }
  occ_count_before[bwt[i]][i+1]++;
 }
}

// Compute the number of occurrences of string pattern in the text
// given only Burrows-Wheeler Transform bwt of the text and additional
// information we get from the preprocessing stage - starts and occ_counts_before.
int CountOccurrencesP(const string& pattern, const string& bwt, Matrix& occ_count_before) {
    int m = pattern.size()-1;
    int n = bwt.size();
    int top=0, bottom=n-1;

    while(top<=bottom){
        char c = pattern[m];
        top = symbol_count[c][0] + occ_count_before[c][top];
        bottom = symbol_count[c][0] + occ_count_before[c][bottom+1] - 1;
        if(--m<0) break;
    }
  return bottom - top + 1;
}


int main() {
  string bwt;
  cin >> bwt;
  int npattern;
  cin >> npattern;

  // Occurrence counts for each character and each position in bwt,
  Matrix occ_count_before;

  // Preprocess the BWT once to get starts and occ_count_before. it takes O(|text|^2)
  // For each pattern, we will then use these precomputed values and
  // spend only O(|pattern|) to find all occurrences of the pattern
  // in the text instead of O(|pattern| + |text|).
  PreprocessBWT(bwt, occ_count_before);
  while(npattern--){
    string pattern;
    cin >> pattern;
    cout<<CountOccurrencesP(pattern, bwt, occ_count_before)<<" ";
  }
  cout<<endl;
  return 0;
}
