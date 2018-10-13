#include <bits/stdc++.h> 
#include <string>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

typedef map<char, int> edges;
typedef vector<edges> trie;

trie build_trie(vector<string> & patterns) {
  int index = 1;
  trie t;
  t.push_back(map<char, int>());   //initialize trie with root map

  //iterate through patterns
  for(size_t i=0; i<patterns.size(); i++){
    int curnode = 0;

    //iterate through pattern
    for(size_t j=0; j<patterns[i].size(); j++){
        char c = patterns[i][j];

        if(t[curnode].find(c) == t[curnode].end() ){
            t[curnode][c] = index;    //put new edge
            curnode = index++;
            t.push_back(map<char, int>());

        }else curnode = t[curnode][c];
    }
  }

  return t;
}

int main() {
  size_t n;
  std::cin >> n;
  vector<string> patterns;
  for (size_t i = 0; i < n; i++) {
    string s;
    std::cin >> s;
    patterns.push_back(s);
  }

  trie t = build_trie(patterns);
  for (size_t i = 0; i < t.size(); ++i) {
    for (const auto & j : t[i]) {
      std::cout << i << "->" << j.second << ":" << j.first << "\n";
    }
  }

  return 0;
}