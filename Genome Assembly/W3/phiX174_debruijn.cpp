#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <vector>
#include <set>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define K_MER 12
#define MIN_OVERLAP 1
#define READSIZE 100
#define DATASIZE 1618
#define fora(i, f, t) for(int i = f; i < t; i++)


struct Edge{
    int read1, read2, overlap, mismat[2];
    Edge(int r1, int r2 = -1, int o = 0) :read1(r1), read2(r2), overlap(o)
    {
       mismat[0] =  mismat[1] = -1;
    }
};

typedef vector<Edge*> Edges;
typedef vector<vector<int> > Matrix;

// find the max overlapping between 2 reads with a minimum overlap k
int findMatchedLength(const string& read1, const string& read2, int* mismat){
    fora(i, 0, READSIZE){
        int k = i;
        int max_error = 2;
        int error = 0;
        mismat[0] = mismat[1] = -1;

        for(int j=0; k < READSIZE; k++, j++){
            if(read1[k] != read2[j]){
                if(error < 2)
                    mismat[error] = k;
                error++;
            }

            if(error > max_error) // error tolerance is just two character
                break;
        }

        if(k == READSIZE)
            return READSIZE - i;
    }
    return -1;
}

//find max overlap graph where every node represent a read and every node has only one out edge represent the max
// overlap between it and another node
Edges overLap_graph(const vector<string>& reads, int n){
    Edges edges;                        // edges of the overlap graph
    set<int> visited = {0};             // every vertex visited only once (Hamiltonian path condition)
    int curRead = 0;                    // working read
    int last;                           //last edge
    bool finalEdge = false;             //if the edge between the first and last reads are connected we are done

    while(visited.size() != n){
        last = edges.size();
        edges.push_back(new Edge(curRead));

        fora(i, 0, n){
            //search for the read has max overlapping with the current read and not visited before
            if(visited.find(i) == visited.end()){
                //compute the overlapping between current read and i read
                int mismat[2] = {-1};
                int ovl = findMatchedLength(reads[curRead], reads[i], mismat);

                if(ovl > edges[last]->overlap){
                    edges[last]->overlap = ovl;             //overlapping length
                    edges[last]->read2 = i;                  //other read
                    edges[last]->mismat[0] = mismat[0];   //mismatch position between the two reads
                    edges[last]->mismat[1] = mismat[1];
                }
            }
        }

        curRead = edges[last]->read2;
        visited.insert(curRead);  // start from this node now

        //if it is the last read check overlapping between it and the first read where genome is circular string
        if(visited.size() == n && !finalEdge){
            visited.erase(0);
            finalEdge = true;
        }
    }
    return edges;
}

// fix the mismatch in overlapping by the voting between 4 reads
void reviseTheGenome(string& genome, const Edges& edges, vector<string>& reads, int n){
    // pass through all overlapping sections and fix errors if there with voting between 4 adjacent reads
    int edgesSize = edges.size();

    for(int i=0; i<edgesSize; i++){
        fora(j, 0, 2){
            int mis_in_read1 = edges[i]->mismat[j];         //position of mis character in read1

            if(mis_in_read1 != -1){ //if there is mismatch
                const Edge* edge1 = edges[i];
                const Edge* edge2 = edges[(i+1) % edgesSize];
                const Edge* edge3 = edges[(i+2) % edgesSize];

                //overlaps between the 4 adjacent reads
                int read1_ovl = READSIZE - edge1->overlap;  //overlap between read1 and read2
                int read2_ovl = READSIZE - edge2->overlap;  //overlap between read2 and read3
                int read3_ovl = READSIZE - edge3->overlap;  //overlap between read3 and read4

                //position of mis match character in every read of the 3 other reads
                int mis_in_read2 = mis_in_read1 - read1_ovl;    // index of mis match char in read2
                int mis_in_read3 = mis_in_read2 - read2_ovl;    // index of mis match char in read3
                int mis_in_read4 = mis_in_read3 - read3_ovl;    // index of mis match char in read4

                //if read3 cover the mis match character and that character is correct in read3
                if(mis_in_read3 >= 0){
                    int mis_char1 = edge2->mismat[0] - read2_ovl;           // mis char between read2 and read3
                    int mis_char2 = edge2->mismat[1] - read2_ovl;           // mis char between read2 and read3
                    char correctChar = reads[edge1->read1][mis_in_read1];   // assume char in read1 is correct

                    //check the selected char is correct from read3
                    if(mis_char1 != mis_in_read3 && mis_char2 != mis_in_read3)
                        correctChar = reads[edge2->read2][mis_in_read3];
                    else if(mis_in_read4 >= 0)
                        correctChar = reads[edge3->read2][mis_in_read4];

                    reads[edge1->read1][mis_in_read1] = reads[edge1->read2][mis_in_read2] = correctChar;
                }
            }
        }
    }
}

//build the genome from the Hamiltonian path
void build_genome(string& genome, const vector<string>& reads, int n, const Edges& edges){
    // concatenate the current next read with the genome sequence after remove the overlap from it.
    int edgesSize = edges.size();
    genome = reads[0];

    //glue genome sequence
    fora(i, 0, edgesSize-1)
        genome += reads[edges[i]->read2].substr(edges[i]->overlap, READSIZE - edges[i]->overlap);

    //cut one of overlapping between last and first reads of the genome
    genome = genome.substr(0, genome.size() - edges[edgesSize-1] -> overlap);
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

    string genome;
    int readsSize = reads.size();

    //create edges for Hamiltonian path
    Edges edges = overLap_graph(reads, readsSize);

    //revise the genome to fix error
    reviseTheGenome(genome, edges, reads, readsSize);

    //build the genome from the greedy Hamiltonian path
    build_genome(genome, reads, readsSize, edges);

    //print the genome
    cout<<genome<<endl;

    return 0;
}
