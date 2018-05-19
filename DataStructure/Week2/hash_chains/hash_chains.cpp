#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>

using std::string;
using std::vector;
using std::list;
using std::cin;
using std::cout;
using std::endl;

struct Query {
    string type, s;
    size_t ind;
};

class QueryProcessor {
    int bucket_count;
    // store all strings in one vector
    vector<list<string> > elems;
    size_t hash_func(const string& s) const {
        static const size_t multiplier = 263;
        static const size_t prime = 1000000007;
        unsigned long long hash = 0;
        //smart way to calculate the polynominal hashing value
        for (int i = static_cast<int> (s.size()) - 1; i >= 0; --i)
            hash = (hash * multiplier + s[i]) % prime;
        return hash % bucket_count;
    }

public:
    explicit QueryProcessor(int bucket_count): bucket_count(bucket_count) {
                        elems.resize(bucket_count);
    }

    Query readQuery() const {
        Query query;
        cin >> query.type;
        if (query.type != "check")
            cin >> query.s;
        else
            cin >> query.ind;
        return query;
    }

    void writeSearchResult(const Query& query) const {
        int hashvalue = hash_func(query.s);
        const list<string>& elements = elems[hashvalue];

        if(find(elements.begin(), elements.end(), query.s) != elements.end())
            std::cout<<"yes\n";
        else
            std::cout<<"no\n";
    }

    void CheckCommand(int hashvalue){
        if(hashvalue < bucket_count){
           if(elems[hashvalue].size() > 0){
             list<string>& elements = elems[hashvalue];
             list<string>::iterator it = elements.begin();

             while(it != elements.end()){
                cout<<(*it)<<" ";
                it++;
             }
             std::cout<<std::endl;
             }
             else
                std::cout<<std::endl;
             }
             else
                std::cout<<std::endl;
    }

    void AddCommand(const Query& querry){

        int hashvalue = hash_func(querry.s);
        list<string>& elements = elems[hashvalue];

        if(find(elements.begin(), elements.end(), querry.s) == elements.end())
            elements.push_front(querry.s);
    }

    void DeleteCommand(const Query& querry){
        int hashvalue = hash_func(querry.s);
        list<string>& elements = elems[hashvalue];
        list<string>::iterator it = find(elements.begin(), elements.end(), querry.s);

        if( it != elements.end())
            elements.erase(it);
    }

    void processQuery(const Query& query) {
        if (query.type == "check")
            CheckCommand(query.ind);
        else if (query.type == "find")
            writeSearchResult(query);
        else if (query.type == "add")
            AddCommand(query);
        else if (query.type == "del")
            DeleteCommand(query);

    }

    void processQueries() {
        int n;
        cin >> n;
        for (int i = 0; i < n; ++i)
            processQuery(readQuery());
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    int bucket_count;
    cin >> bucket_count;
    QueryProcessor proc(bucket_count);
    proc.processQueries();
    return 0;
}
