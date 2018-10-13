#include<string>
#include<string.h>
#include <iostream>
#include <stdlib.h>
using namespace std;
// Structure to store data of a suffix
typedef struct {
    int index;
    const char *suffix;
} Suffix;

// sort rotations lexicographically
int compare (const void *r1, const void *r2){
    return strcmp(((Suffix *) r1)->suffix, ((Suffix *)r2)->suffix);
}

// in this function we build the suffix array and from we get the last col in the suffixes
string BWT(const char *text){
    string result = "";
    int n = strlen(text);   //number of suffixes
    Suffix suffixes[n];    //holds suffixes

    // create suffixes
    for(int i = 0; i < n; i++){
        suffixes[i].index = i;
        suffixes[i].suffix = text + i;
    }

    // Sorts suffixes lexicographically
    qsort(suffixes, n, sizeof(Suffix), compare);

    //get the last character from suffixes in suffix array
    for (int i = 0; i < n; i++){
        char c = text[(suffixes[i].index + n - 1)%n];
        result.append(1, c);
    }
    return result;
}


// Driver program to test functions above
int main()
{
    string text;
    cin>>text;
    const char* str = text.c_str();

    cout<<BWT(str)<<endl;
    return 0;
}
