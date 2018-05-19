#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>
#include <set>

using namespace std;
int maxcost = numeric_limits<int>::max();

struct Node{
 int index, x, y, p, known;
 double c;
 Node(int i, int xx, int yy):index(i), x(xx), y(yy), p(-1), c(maxcost), known(false)
 {}
};
vector<Node> nodes;

int getMin(){
    int mini = -1;
    double minv = numeric_limits<double>::max();
    for(size_t i=0; i<nodes.size(); i++){
        if(!nodes[i].known && nodes[i].c < minv){
            mini = i;
            minv = nodes[i].c;
        }
    }
    return mini;
}

double minimum_distance() {
  double result = 0.;
  nodes[0].c=0;
  int in;
  while((in=getMin()) != -1){
    nodes[in].known = true;
    result += nodes[in].c;
    for(size_t i=0; i<nodes.size(); i++){
        if(!nodes[i].known){
            double costedge = sqrt(pow((nodes[in].x - nodes[i].x), 2) + pow((nodes[in].y - nodes[i].y), 2));
            if(costedge<nodes[i].c){
                nodes[i].c = costedge;
                nodes[i].p = in;
            }
        }
    }
  }
  return result;
}

int main() {
  size_t n, x, y;
  std::cin >> n;
  for (size_t i = 0; i < n; i++) {
    std::cin >> x>> y;
    nodes.push_back(Node(i, x, y));
  }
  std::cout << std::setprecision(10) << minimum_distance() << std::endl;
}
