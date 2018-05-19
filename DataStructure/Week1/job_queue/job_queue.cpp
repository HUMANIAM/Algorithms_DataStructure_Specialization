#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::cin;
using std::cout;
using std::pair;
using std::endl;
using std::make_pair;

class JobQueue {
 private:
  int num_workers_;
  vector<int> jobs_;
  vector< pair<int, long long> > Threads;  //every item consist from index and next_free time

  vector<int> assigned_workers_;
  vector<long long> start_times_;


  void WriteResponse() const {
    for (int i = 0; i < jobs_.size(); ++i) {
      cout << assigned_workers_[i] << " " << start_times_[i] << "\n";
    }
  }

  void ReadData() {
    int m;
    cin >> num_workers_ >> m;
    jobs_.resize(m);
    for(int i = 0; i < m; ++i)
      cin >> jobs_[i];
  }
/**
    priority compose of (small index, less next free time)
    if the next free time of the child is less then sift it up
    else if they have the same next free time then sift child up if
    its index less than the parent
*/
  int MinNode(int node1, int node2){
    if(Threads[node1].second == Threads[node2].second){
        if(Threads[node1].first < Threads[node2].first)
            return node1;

        else
            return node2;
    }
    else if (Threads[node1].second < Threads[node2].second){
        return node1;
    }
    else
        return node2;
  }

  void SiftDown(int i){
    int minindex;
    int leftchild = i*2 + 1;
    int rightchild = leftchild + 1;
    int min1 = i, min2 = i;

    if(leftchild < num_workers_)
        min1 = MinNode(leftchild, i);
    if(rightchild < num_workers_)
        min2 = MinNode(rightchild, i);

    if(i != min1 && i != min2){
        minindex = MinNode(min1, min2);
        std::swap(Threads[minindex], Threads[i]);
        SiftDown(minindex);
    }
    else if(i != min1){
        minindex = min1;
        std::swap(Threads[minindex], Threads[i]);
        SiftDown(minindex);
    }
    else if(i != min2){
        minindex = min2;
        std::swap(Threads[minindex], Threads[i]);
        SiftDown(minindex);
    }
    else{}
  }

  void AssignJobs() {
    // TODO: replace this code with a faster algorithm.
    assigned_workers_.resize(jobs_.size());
    start_times_.resize(jobs_.size());
    Threads.resize(num_workers_);

    //initialize the Threads with their index
    for(int i =0; i<num_workers_; ++i){
        Threads[i].first = i;
        Threads[i].second = 0;
    }

    for (int i = 0; i < jobs_.size(); ++i) {
        assigned_workers_[i] = Threads[0].first;;
        start_times_[i] = Threads[0].second;
        Threads[0].second += jobs_[i];
        SiftDown(0);
    }
  }

 public:
  void Solve() {
    ReadData();
    AssignJobs();
    WriteResponse();
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  JobQueue job_queue;
  job_queue.Solve();
  return 0;
}
