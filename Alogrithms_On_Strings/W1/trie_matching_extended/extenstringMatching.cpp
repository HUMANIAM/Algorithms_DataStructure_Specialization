#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;
int const Letters =    4;
int const NA      =   -1;

struct Node
{
    int next [Letters];

    Node ()
    {
        fill (next, next + Letters, NA);
    }

    bool isPattern () const
    {
        return (next[0] == -5 || next[1] == -5 || next[2] == -5 || next[3] == -5);
    }
};

int letterToIndex (char letter)
{
    switch (letter)
    {
        case 'A': return 0; break;
        case 'C': return 1; break;
        case 'G': return 2; break;
        case 'T': return 3; break;
        default: assert (false); return -1;
    }
}

//every node has index and map of childeren <value, position>
typedef map<char, int> edges;
typedef vector<edges> trie;

vector<Node> build_trie(const vector<string> & patterns) {
  int nex = 1;
  vector<Node> nodes;
  nodes.push_back(Node()); //root node

  //iterate through patterns
  for(size_t i=0; i<patterns.size(); i++){
    int curnode = 0;

    //iterate through pattern
    for(size_t j=0; j<patterns[i].size(); j++){
        int c = letterToIndex(patterns[i][j]);

        if(nodes[curnode].next[c] == NA){
            curnode = nodes[curnode].next[c] = nex++;
            nodes.push_back(Node());
        }else curnode = nodes[curnode].next[c];
    }
    nodes[curnode].next[0] = -5;
  }

  return nodes;
}

int prefixTrieMatching(const string& txt, vector<Node>& t, int k, size_t& n){
    int curnode = 0;

    for(int i=k; i<n; i++){
        int c = letterToIndex(txt[i]);

        if(t[curnode].isPattern())
            return k;                                       // it is a leaf node so text match a pattern in trie
        else if(t[curnode].next[c] != NA)
            curnode = t[curnode].next[c];                         //edge matches. move to the next edge
        else
            return -1;                                           //no matching found
    }

    if(t[curnode].isPattern()) return k;
    else                    return -1;

}


vector <int> trieMatching (const string& text, const vector <string>& patterns)
{
    vector <int> result;
    vector<Node> t = build_trie(patterns);
    size_t n = text.size();

    for(size_t i=0; i<n; i++)
        result.push_back(prefixTrieMatching(text, t, i, n));

    return result;
}

int main (void)
{
    string text;
    cin >> text;

    int n;
    cin >> n;

    vector <string> patterns (n);
    for (int i = 0; i < n; i++)
        cin >> patterns[i];

    vector <int> ans;
    ans = trieMatching (text, patterns);

    for (int i = 0; i < (int) ans.size (); i++)
        if(ans[i] != -1) cout << ans[i]<<" ";

    cout<<endl;

    return 0;
}
