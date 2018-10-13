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
void suffixArray(const char *text){
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

    //print the suffix array
    for (int i = 0; i < n; i++){
      cout<<suffixes[i].index<<" ";
    }
}


// Driver program to test functions above
int main()
{
    string text;
    cin>>text;
    const char* str = text.c_str();

    suffixArray(str);
    return 0;
}
