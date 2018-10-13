#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

struct Pos{
    int from, len, veci;
    Pos(int x, int y):from(x),len(y), veci(-1){}
};

typedef map<char, struct Pos*> edges;
typedef vector<edges> trie;

//build the suffix trie for all suffixes of the text
void createBranches(const string& txt, int n, trie& t, int curnode, int j){
    static int index = 1;
    char c = txt[j];
    //if current char of suffix not in one of children of the current node make a branch
        if(t[curnode].find(c) == t[curnode].end()){
            t[curnode][c]=new Pos(j, n-j);
        }else{
            //if the node has a child = current cur continue diving till reach a point to branch
            int from = ((t[curnode][c])->from);
            int len = ((t[curnode][c])->len);
            int veci = ((t[curnode][c])->veci);
            int k = from + 1;
            j++;

            //comparing the remaining of suffixes
            while((k-from)<len && txt[j]==txt[k]){j++; k++;}

            //update the current edge length
            (t[curnode][c])->len = k-from;

            //if the current edge has no branches make new 2 branches one for the remaining of the edge
            //and one for the remaining of the suffix
            if(veci == -1 || (k-from)!=len){
                //add branch for the remaining of the edge
                t.push_back(map<char, struct Pos*>());
                int tmp = ((t[curnode][c])->veci);
                curnode = ((t[curnode][c])->veci) = index++;

                //add the first branch of the remaining of the edge
                t[curnode][txt[k]] = new Pos(k, len-k+from);

                //add the second branch for the remaining of the sub suffix
                t[curnode][txt[j]] = new Pos(j, n-j);

                //if the node has other branches give them to the remaining of the suffix of the node
                if(veci != -1)
                   (t[curnode][txt[k]])->veci = tmp;

            }else{
                createBranches(txt, n, t, veci, j);
            }
        }
}

// Build a suffix tree of the string text and return a vector
// with all of the labels of its edges (the corresponding
// substrings of the text) in any order.

trie ComputeSuffixTreeEdges(const string& text) {
  int n = text.length();
  trie t;
  t.push_back(map<char, struct Pos*>());   //initialize trie with root map

  //iterate through suffixes
  for(size_t i=0; i<n; i++)
        createBranches(text, n, t, 0, i);

   //print edges
  for (int i = 0; i < t.size(); ++i){
        for(const auto& mpe : t[i]){
            int from = (mpe.second)->from;
            int len = (mpe.second)->len;
            cout <<text.substr(from, len) << endl;
        }

  }
  return t;
}

int main() {
    string text;
    cin >> text;
    trie edges = ComputeSuffixTreeEdges(text);

  return 0;
}
