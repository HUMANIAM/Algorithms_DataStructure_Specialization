#include <iostream>
#include <string>
#include <vector>

using namespace std;

void buildLPS(const string& text, vector<int>& lps){
 int border = 0;
 int n = text.size();

 for(int i=1; i<n; i++){
    while(text[i] != text[border] && border>0)
        border = lps[border-1];

    if(text[i] == text[border])
        border++;
    else
        border = 0;
    lps[i] = border;
 }
}

// Find all occurrences of the pattern in the text and return a
// vector with all positions in the text (starting from 0) where
// the pattern starts in the text.
vector<int> find_pattern(const string& pattern, const string& text) {
  string ntext = pattern + "$" + text;
  int n = ntext.size();
  int m = pattern.size();
  vector<int> result;
  vector<int> lps(n, 0);   //longest common prefix without the last char suffix at i position

  //build the lps
  buildLPS(ntext, lps);

  //where are the positions the pattern matches the a substring of text
  //they are positions i in text with suffix  = pattern
  for(int i=0; i<lps.size(); i++)
    if(lps[i] == m)
        result.push_back(i-2*m);

  return result;
}

int main() {
  string pattern, text;
  cin >> pattern >> text;

  vector<int> result = find_pattern(pattern, text);
  for (int i = 0; i < result.size(); ++i) {
    cout<< result[i] <<" ";
  }
  cout<<endl;
  return 0;
}
