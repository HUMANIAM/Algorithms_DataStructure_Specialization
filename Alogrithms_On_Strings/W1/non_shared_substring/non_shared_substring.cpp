#include <bits/stdc++.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;
//T1 : substring only text1
//T2 : substring only in text2
//T12 : shared between text 1 and text 2
enum Type{T1, T2, T12};

struct Pos{
    int from, len, veci, type;
    Pos(int x, int y, int t):from(x),len(y), type(t), veci(-1){
    }
};

struct Result{
    int from, len;
};

typedef map<char, struct Pos*> edges;
typedef vector<edges> suffixTree;

struct Result res = {-1, INT_MAX};  //min dismatch substring

//build the suffix tree for all suffixes of the text
void createBranches(const string& txt, int n, suffixTree& t, int curnode, int j, int type){
    static int index = 1;
    char c = txt[j];
    //if current char of suffix not in one of children of the current node make a branch
        if(t[curnode].find(c) == t[curnode].end()){
            t[curnode][c]=new Pos(j, n-j, type);
        }else{
            //if the node has a child = current char continue comparing till reach a point to branch
            int from = ((t[curnode][c])->from);
            int len = ((t[curnode][c])->len);
            int veci = ((t[curnode][c])->veci);
            int typ  = (((t[curnode][c]))->type);
            int k = from + 1;
            j++;

            //comparing the remaining of chars of suffix with edges's chars
            while((k-from)<len && txt[j]==txt[k]){j++; k++;}

            //update the current edge length
            (t[curnode][c])->len = k-from;

            //update the current edge type
            if(typ != type) t[curnode][c]->type = T12;

            //if the current edge has no branches make new 2 branches one for the remaining of the edge
            //and one for the remaining of the suffix
            if(veci == -1 || (k-from)!=len){
                //add branch for the remaining of the edge
                t.push_back(map<char, struct Pos*>());
                int tmp = ((t[curnode][c])->veci);
                curnode = ((t[curnode][c])->veci) = index++;

                //add the first branch of the remaining of the edge
                t[curnode][txt[k]] = new Pos(k, len-k+from, typ);

                //add the second branch for the remaining of the sub suffix
                t[curnode][txt[j]] = new Pos(j, n-j, type);

                //if the node has other branches give them to the remaining of the suffix of the node
                if(veci != -1)
                   (t[curnode][txt[k]])->veci = tmp;

            }else{
                createBranches(txt, n, t, veci, j, type);
            }
        }
}

// Build a suffix tree of the string text and return a vector
// with all of the labels of its edges (the corresponding
// substrings of the text) in any order.

suffixTree ComputeSuffixTreeEdges(const string& text, int m) {
  int n = text.length();
  suffixTree t;
  t.push_back(map<char, struct Pos*>());   //initialize suffixTree with root map

  //iterate through suffixes
  for(size_t i=0; i<n; i++)
    if(i<m)
        createBranches(text, n, t, 0, i, T1);
    else
        createBranches(text, n, t, 0, i, T2);

  return t;
}

void minMisMatchSubStr(suffixTree& sufTree, const string& text, int len, int index){
    for(const auto& mpe : sufTree[index]){
        int type = (mpe.second)->type;

        //if edge is only substring in text1
        if(type == T1 && mpe.first != '$'){
            if(len + 1 < res.len){
                res.from = ((mpe.second)->from - len);
                res.len = len + 1;
            }
            return;
        //if edge is internal node
        }else if(type == T12){
            minMisMatchSubStr(sufTree, text, len + (mpe.second)->len, (mpe.second)->veci);
        }

    }
}

void solve (string& text, int T1Len){
    //building suffix tree
    suffixTree sufTree = ComputeSuffixTreeEdges(text, T1Len);

    //compute the min matched substring between the two strings
    minMisMatchSubStr(sufTree, text, 0, 0);

    if(res.from == -1)
        cout<<"";
    else
        cout<< text.substr(res.from, res.len)<<endl;
}

int main (void)
{

    string text1, text2, text;
    cin>>text1>>text2;

    text = text1 + "$" + text2 + "#";
    solve(text, text1.size());

    return 0;
}
