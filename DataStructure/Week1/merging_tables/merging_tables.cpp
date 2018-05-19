#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::max;
using std::vector;

struct DisjointSetsElement {
	int size, parent, rank;

	DisjointSetsElement(int size = 0, int parent = -1, int rank = 0):
	    size(size), parent(parent), rank(rank) {}
};

struct DisjointSets {
	int size;
	int max_table_size;
	vector <DisjointSetsElement> sets;

	DisjointSets(int size): size(size), max_table_size(0), sets(size) {
		for (int i = 0; i < size; i++)
			sets[i].parent = i;
	}

	int getParent(int table) {
		// find parent and compress path
		// we apply compress path heuristic to reduce time of find in the
		// future. when ask the same item again.
		if(sets[table].parent == table)
            return table;
        else
            return sets[table].parent = getParent(sets[table].parent);
	}

	void UnionByRank(int des, int src){
	    //union by rank mean add table with less rank to
	    // table with high rank

         // merge two components
	    sets[des].size += sets[src].size;
	    sets[src].size = 0;
	    // update max_table_size
	    max_table_size = max(max_table_size, sets[des].size);
	    // use union by rank heuristic
        if(sets[des].rank < sets[src].rank)
            sets[des].parent = src;
        else{
            sets[src].parent = des;
            if(src == des)
                sets[des].rank++;
        }
	}

	void merge(int destination, int source) {
		int realDestination = getParent(destination);
		int realSource = getParent(source);
		if (realDestination != realSource)
			UnionByRank(realDestination, realSource);
	}
};

int main() {
	int n, m;
	cin >> n >> m;

	DisjointSets tables(n);
	for (auto &table : tables.sets) {
		cin >> table.size;
		tables.max_table_size = max(tables.max_table_size, table.size);
	}

	for (int i = 0; i < m; i++) {
		int destination, source;
		cin >> destination >> source;
                --destination;
                --source;

		tables.merge(destination, source);
	        cout << tables.max_table_size << endl;
	}

	return 0;
}
