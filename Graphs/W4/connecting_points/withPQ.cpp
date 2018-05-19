#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>
#include <set>
#include <stdlib.h>

using namespace std;
int maxcost = numeric_limits<int>::max();
int n;

struct Node{
 int index, x, y, p, known;
 double c;
 Node(int i, int xx, int yy):index(i), x(xx), y(yy), p(-1), c(maxcost), known(false)
 {}
};
vector<Node> nodes;

class PriorityQueue{
private:
    vector<size_t> pos;
public:
    PriorityQueue(){
        for(int i=0; i<=n; i++){
            pos.push_back(i);
            cout<<nodes[pos[i]].index<<"   "<<i<<endl;
        }
        n++;
    }

    size_t findMin(){
        if(n==0) return 0;

        pos[nodes[1].index] = n;
        pos[nodes[n].index] = 1;
        Node x = nodes[1];
        nodes[1] = nodes[n];
        nodes[n--] = x;
        percolateDown(1);
        return 1;
    }

    void percolateDown(int hole){
        int child;
        Node node = nodes[hole];
        for(; hole*2<=n; hole=child){
            if(child != n && nodes[child+1].c < nodes[child].c)
                child++;
            if(nodes[child].c < nodes[hole].c){
                nodes[hole] = nodes[child];
                pos[nodes[child].index] = hole;
            }else break;
        }
        nodes[hole] = node;
        pos[nodes[hole].index] = hole;
    }

  void decreasePriority(int x, int cost){
    int p = pos[x];
    nodes[p].c = cost;
    Node tmp = nodes[p];
    for(; p>1 && nodes[p].c<nodes[p/2].c; p/=2){
        nodes[p] = nodes[p/2];
        pos[nodes[p/2].index] = p;
    }
    nodes[p] = tmp;
    pos[nodes[p].index] = p;
  }

  bool inPriorityQueue(int x){
    if(pos[x] > n) return false;
    return true;
  }
};

double minimum_distance() {
  PriorityQueue qu = PriorityQueue();
  double result = 0.;
  nodes[0].c=0;
  int in;
  while((in=qu.findMin()) != 0){
    Node node = nodes[in];
    cout<<"min "<<in<<endl;
    result += nodes[in].c;
    cout<<"n "<<n<<endl;
    for(size_t i=1; i<=n; i++){
            cout<<nodes[i].index<<endl;
//            double costedge = sqrt(pow((nodes[in].x - nodes[i].x), 2) + pow((nodes[in].y - nodes[i].y), 2));
//            if(costedge<nodes[i].c){
//                    cout<<"change priority of "<<nodes[i].index<<endl;
//                nodes[i].c = costedge;
//                nodes[i].p = in;
//                qu.decreasePriority(i, costedge);
//            }
        }
    }
  return result;
}

int main() {
  size_t x, y;
  std::cin >> n;
  nodes.push_back(Node(0, -1, -1));
  for (size_t i = 1; i <= n; i++) {
    std::cin >> x>> y;
    nodes.push_back(Node(i, x, y));
  }
  std::cout << std::setprecision(10) << minimum_distance() << std::endl;
}
