#include <iostream>
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::cin;

struct Query {
    string type, name;
    int number;
};

struct Contact{
	int number;
	string name;
	Contact():number(-1), name("")
			{}
};

vector<Query> read_queries() {
    int n;
    cin >> n;
    vector<Query> queries(n);
    for (int i = 0; i < n; ++i) {
        cin >> queries[i].type;
        if (queries[i].type == "add")
            cin >> queries[i].number >> queries[i].name;
        else
            cin >> queries[i].number;
    }
    return queries;
}

void write_responses(const vector<string>& result) {
    for (size_t i = 0; i < result.size(); ++i)
        std::cout << result[i] << "\n";
}

vector<string> process_queries(const vector<Query>& queries) {
    vector<string> result;
    vector<Contact> contacts(10000000);     //keep contacts of this phone book manager
    int entry;

    for (size_t i = 0; i < queries.size(); ++i){
        entry = queries[i].number;

        if (queries[i].type == "add") {
            // if we already have contact with such number,
            // we should rewrite contact's name
            if(contacts[entry].number == queries[i].number)
                contacts[entry].name = queries[i].name;
            else
                // otherwise, just add it
                contacts[entry].name = queries[i].name;
                contacts[entry].number = queries[i].number;

        }
        else if (queries[i].type == "del") {
            if(contacts[entry].number == queries[i].number)
                contacts[entry].number = -1;

        }
        else {
            string response = "not found";
            if(contacts[entry].number == queries[i].number)
                response = contacts[entry].name;
            result.push_back(response);
        }
    }
    return result;
}

int main() {
    write_responses(process_queries(read_queries()));
    return 0;
}
