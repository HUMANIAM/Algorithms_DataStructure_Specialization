#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

void InitializeOrder(const string& text, vector<int>& order){
    int n = text.size();
    map<char, int> charCount;
    charCount['$'] = charCount['A'] = charCount['C'] = 0;
    charCount['G'] = charCount['T'] = 0;

    //count how many every char occurs in the text
    for(int i=0; i<n; i++)
      charCount[text[i]]++;

    //accumulate the chars counts
    int acc = 0;
    for(auto& Char : charCount)
     acc = (Char.second) += acc;

    //sort the text in stable order where in the sorted array Ai must come before Ai+1
    for(int i=n-1; i>=0; i--)
        order[--charCount[text[i]]] = i;
}

int InitializeClass(const string& text, const vector<int>& order, vector<int>& classes){
    classes[order[0]] = 0;
    int n = text.size();

    //update classes
    for(int i=1; i<n; i++){
        classes[order[i]] = classes[order[i-1]];
        if(text[order[i]] != text[order[i-1]])
            classes[order[i]]++;
    }

    return classes[order[n-1]];  //the new number of equivelance classes
}

void updateOrder(int L, int nc, vector<int>& order, const vector<int>& classes){
    //count class
    int n = order.size();
    int classcount[nc+1] = {0};

    for(int i=0; i<n; i++)
      classcount[classes[i]]++;

    //accumulate class counts
    int acc=0;
    for(int i=0; i<=nc; i++)
      acc = classcount[i] += acc;

    //update the order array
    vector<int> newOrder(n);
    for(int i=n-1; i>=0; i--){
        int start = (order[i] - L + n) % n; //rotate anti clock with to get doubled cyclic shift order
        newOrder[--classcount[classes[start]]] = start;
    }

    //copy the new order into the older order
    for(int i=0; i<n;i++)
        order[i] = newOrder[i];
}

int updateClass(int L, const vector<int>& order, vector<int>& classes){
    int n = order.size();
    vector<int> newClasses(n);
    newClasses[order[0]] = 0;

    for(int i=1; i<n; i++){
        int cur = order[i], pre = order[i-1];
        int curmid = (cur+L)%n , premid = (pre+L)%n;

        newClasses[cur] = newClasses[pre];
        if(classes[cur] != classes[pre] || classes[curmid] != classes[premid])
          newClasses[cur]++;
    }

    //copy new class into the old class
    for(int i=0; i<n; i++)
        classes[i] = newClasses[i];

    return newClasses[order[n-1]];
}

vector<int> BuildSuffixArray(const string& text) {
  int n = text.size();
  int L = 1, nc;
  vector<int> order(n);
  vector<int> classes(n);

  //initialize the order array of single shift L=1
  InitializeOrder(text, order);     // O(|T| + |ALPHA|)

  //initialize the class of single shift L=1
  nc = InitializeClass(text, order, classes);   // O(|T|)

  // go through doubled cyclic shift order from 1 to L<n
  while(L<n){                               //O(|T|log(|T|))
    updateOrder(L, nc, order, classes);
    nc = updateClass(L, order, classes);
    L *= 2;
  }
  return order;
}

int main() {
  string text;
  cin >> text;
  vector<int> suffix_array = BuildSuffixArray(text);
  for (int i = 0; i < suffix_array.size(); ++i) {
    cout << suffix_array[i] << ' ';
  }
  cout << endl;
  return 0;
}
