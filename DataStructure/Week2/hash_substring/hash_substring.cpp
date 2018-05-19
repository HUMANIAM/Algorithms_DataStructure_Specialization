#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#define P 1000007
#define X 263

using namespace std;
using std::string;
using std::vector;
typedef unsigned long long ull;

struct Data {
    string pattern, text;
};

Data read_input() {
    Data data;
    std::cin >> data.pattern >> data.text;
    return data;
}

size_t PolyHash(const string& s){
    size_t hash = 0;
    //smart way to calculate the polynomial hashing value
    for (int i = static_cast<int> (s.size()) - 1; i >= 0; --i)
        hash = ((hash * X + s[i])) % P;
    if(hash<0)
        return hash += P;
    return hash;
}

std::vector<size_t> PreComputeHashes(const string& Text, int plength){
    int Tlength = Text.size();
    int y = 1;
    std::vector<size_t> hashes(Tlength - plength + 1);

    string substring = Text.substr(Tlength - plength, plength);
    hashes[hashes.size()-1] = PolyHash(substring);

    for(int i = 0; i< plength; ++i)
        y = (y * X) % P;
    //calculate the rest of hash values
    for(int i = hashes.size()-2; i>=0; --i){
        hashes[i] = (X*hashes[i+1] - y*Text[i+plength] + Text[i]) % P;
        hashes[i] < 0? hashes[i] += P : true;
    }
    return hashes;

}
void print_occurrences(const std::vector<int>& output) {
    for (size_t i = 0; i < output.size(); ++i)
        std::cout << output[i] << " ";
    std::cout << "\n";
}

bool AreEqual(const string& str, const string& subst, int from, int to){
    for(int i = from, j=0; i<to; i++, j++)
        if(str[i++] != subst[j++])
            return false;
    return true;
}

std::vector<int> get_occurrences(const Data& input) {
    const string& s = input.pattern, t = input.text;
    size_t hashpattern = PolyHash(s);      //hash value of the pattern
    std::vector<int> ans;
    //compute hashes previously to reduce the complexity time
    std::vector<size_t> Hashes = PreComputeHashes(t, s.size());
    //search for the matched patterns in the Text
    for(size_t i=0; i<Hashes.size(); ++i)
       if((Hashes[i] == hashpattern) && AreEqual(t, s, i, i+s.size()))
         ans.push_back(i);
    return ans;
}



int main() {
    std::ios_base::sync_with_stdio(false);
    print_occurrences(get_occurrences(read_input()));
    return 0;
}
