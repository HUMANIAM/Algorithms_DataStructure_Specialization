#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <limits.h>
#include <vector>
#include <set>
using namespace std;

#define K_MER 12
#define MIN_OVERLAP 1
#define READSIZE 100
#define DATASIZE 400
#define fora(i, f, t) for(int i = f; i < t; i++)

//find max overlap graph where every node represent a read and every node has only one out edge represent the max
// overlap between it and another node
int overLap_graph(const vector<string>& reads, int n){
    int minovlap = INT_MAX;

    fora(i, 0, n)
     fora(j, 0, n){
        if(i != j)
            fora(w, 0, READSIZE){
                int k = w;

                for(int f=0; k < READSIZE; k++, f++)
                    if(reads[i][k] != reads[j][f]) break;

                if(k == READSIZE)   minovlap = min(minovlap, READSIZE - w);
            }
     }

    return minovlap;
}

int main(int argc, char** argv){
    vector<string> reads;
    set<string> reads_unique;
    string read;

    //read reads
    fora(i, 0, DATASIZE){
        cin >> read;
        //reads.push_back(read);
        reads_unique.insert(read);
    }

    // insert unique reads into the vector
    for(auto r : reads_unique)
        reads.push_back(r);

    //find max overlap graph
    cout<<overLap_graph(reads, reads.size())<<endl;

    return 0;
}
