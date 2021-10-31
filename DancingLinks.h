/**
* DancingLinks.h declares the dancing links data structure and algorithm.
* @author Charlie Keaney
*/

#ifndef DANCING_LINKS_H
#define DANCING_LINKS_H 1

#include <iostream>

#define DEBUG_DISPLAY_CONSTRUCTION_INFO			0
#define DEBUG_DISPLAY_LINKING_INFO				0
#define DEBUG_DISPLAY_COVER_INFO				0
#define DEBUG_DISPLAY_OPTIONS_TO_CONSTRAINTS	0
#define DEBUG_DISPLAY_CONSTRAINTS_TO_OPTIONS	0

#define DEBUG_DISPLAY_SEARCH_CALLS				0
#define DEBUG_DISPLAY_SEARCH_CHOICE				0
#define DEBUG_DISPLAY_SEARCH_ACCESSES			0
#define DEBUG_DISPLAY_SEARCH_INFO				0

using namespace std;

class FourWayLinkedNode;
class DancingLinksNode;
class DancingLinksColumn;
class DancingLinksMatrix;

/** FourWayLinkedNode
*   A class of data object used to represent generic nodes within the matrix.
*   Serves as the foundation of all other objects to represent their four way
*   doubly nature
*/
class FourWayLinkedNode {
private:
	FourWayLinkedNode* up;
	FourWayLinkedNode* down;
	FourWayLinkedNode* left;
	FourWayLinkedNode* right;
public:
	FourWayLinkedNode();

	FourWayLinkedNode* get_up();
	FourWayLinkedNode* get_down();
	FourWayLinkedNode* get_left();
	FourWayLinkedNode* get_right();

	void set_up(FourWayLinkedNode* n);
	void set_down(FourWayLinkedNode* n);
	void set_left(FourWayLinkedNode* n);
	void set_right(FourWayLinkedNode* n);
};

/** DancingLinksNode
*	A class of data object used to represent a non-top layer node
*	within the dancing links matrix. Not a generic type for all
*	nodes, since headers and the root do not have data and a column.
*/
class DancingLinksNode : public FourWayLinkedNode {
private:
	DancingLinksColumn* column;
	int data;
public:
	DancingLinksNode(int, DancingLinksColumn*);

	DancingLinksColumn* get_column();
	int	get_data();

	DancingLinksNode* get_left();
	DancingLinksNode* get_right();
};

/** DancingLinksColumn
*	This class represents a column (a node on the top row) within the
*   matrix used by the dancing links algorithm.
*/
class DancingLinksColumn : public FourWayLinkedNode {
private:
	char* name;
	int size;
public:
	DancingLinksColumn(char* n);
	~DancingLinksColumn();

	char* get_name();
	int	 get_size();

	DancingLinksColumn* get_left();
	DancingLinksColumn* get_right();

	void insert_left(DancingLinksColumn* l);
	void insert_right(DancingLinksColumn* r);

	void cover();
	void uncover();

	void set_size(int s);
};

/** DancingLinksMatrix
*   A class used to represent the entire matrix used by the dancing
*   links algorithm as a data object.
*/
class DancingLinksMatrix {	
private:
	DancingLinksColumn* h;
public:
	DancingLinksMatrix(bool piece_at_pos[], int num_columns, int num_rows);
	~DancingLinksMatrix();

	int full_search(int k, DancingLinksNode** o);	
	int partial_search(int k, DancingLinksNode** o);
	int* solve();
	int get_num_columns();
	int get_num_rows();

	void print_grid();
	void print_setwise(DancingLinksNode** o);
};

#endif