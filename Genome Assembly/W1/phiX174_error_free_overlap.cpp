#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <vector>
#include <set>
using namespace std;

#define K_MER 12
#define MIN_OVERLAP 1
#define READSIZE 100
#define DATASIZE 1618
#define fora(i, f, t) for(int i = f; i < t; i++)

// find the max overlapping between 2 reads with a minimum overlap k
int findMatchedLength(const string& read1, const string& read2){
    fora(i, 0, READSIZE){
        int k = i;

        for(int j=0; k < READSIZE; k++, j++)
            if(read1[k] != read2[j]) break;

        if(k == READSIZE)
            return READSIZE - i;
    }
    return 0;
}

//find max overlap graph where every node represent a read and every node has only one out edge represent the max
// overlap between it and another node
void overLap_graph(const vector<string>& reads, int n, string& genome){
    set<int> visited;
    visited.insert(0); // start with read0
    int curRead = 0;
    genome = reads[0];

    while(visited.size() != n){
        int molap = 0, othread = -1;

        fora(i, 0, n){
            if(visited.find(i) == visited.end()){
                // max overlap between read1 and read2
                int ovl = findMatchedLength(reads[curRead], reads[i]);

                if(ovl > molap){
                    molap = ovl;     //overlapping length
                    othread = i;         //other read
                }
            }
        }

        // concatenate the current next read with the genome sequence after remove the overlap from it.
        genome += reads[othread].substr(molap, READSIZE - molap);
        curRead = othread;
        visited.insert(curRead);  // start from this node now
    }

    //cut one of overlapping between last and first reads of the genome
    genome = genome.substr(0, genome.size() - findMatchedLength(reads[curRead], reads[0]));

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
    string genome;
    overLap_graph(reads, reads.size(), genome);
    cout<<genome<<endl;

    return 0;
}
