#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::cin;
using std::cout;
using std::swap;
using std::pair;
using std::make_pair;

class HeapBuilder {
 private:
  int SZ;
  vector<int> data_;
  vector< pair<int, int> > swaps_;

  void WriteResponse() const {
    cout << swaps_.size() << "\n";
    for (int i = 0; i < swaps_.size(); ++i) {
      cout << swaps_[i].first << " " << swaps_[i].second << "\n";
    }
  }

  void ReadData() {
    int n;
    cin >> n;
    SZ = n;
    data_.resize(n);
    for(int i = 0; i < n; ++i)
      cin >> data_[i];
  }

  void SiftDown(int i){
    int minindex = i;
    int leftchild = i*2 + 1;
    int rightchild = leftchild + 1;

    if((leftchild < SZ) && (data_[leftchild] < data_[minindex])){
        minindex = leftchild;
    }
    if((rightchild < SZ) && (data_[rightchild] < data_[minindex])){
        minindex = rightchild;
    }
    if(i != minindex){
        swap(data_[minindex], data_[i]);
        swaps_.push_back(make_pair(i, minindex));
        SiftDown(minindex);
    }
  }
  void GenerateSwaps() {
    swaps_.clear();
    // The following naive implementation just sorts
    // the given sequence using selection sort algorithm
    // and saves the resulting sequence of swaps.
    // This turns the given array into a heap,
    // but in the worst case gives a quadratic number of swaps.
    //
    // TODO: replace by a more efficient implementation
    for (int i = SZ/2; i > -1; --i)
        SiftDown(i);
  }

 public:
  void Solve() {
    ReadData();
    GenerateSwaps();
    WriteResponse();
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  HeapBuilder heap_builder;
  heap_builder.Solve();
  return 0;
}
