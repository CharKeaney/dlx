/**
* DancingLinks.h implements the dancing links data structure and algorithm.
* @author Charlie Keaney
*/

#ifndef DANCING_LINKS_CPP
#define DANCING_LINKS_CPP 1

#include "DancingLinks.h"
#include <string>
#include <istream>

using namespace std;

FourWayLinkedNode::FourWayLinkedNode() {
		up = NULL;
		down = NULL;
		left = NULL;
		right = NULL;
}

FourWayLinkedNode* FourWayLinkedNode::get_up()    { return up;	  }
FourWayLinkedNode* FourWayLinkedNode::get_down()  { return down;  }
FourWayLinkedNode* FourWayLinkedNode::get_left()  {	return left;  }
FourWayLinkedNode* FourWayLinkedNode::get_right() { return right; }

void FourWayLinkedNode::set_up(FourWayLinkedNode* n)    { up = n;	 }
void FourWayLinkedNode::set_down(FourWayLinkedNode* n)  { down = n;	 }
void FourWayLinkedNode::set_left(FourWayLinkedNode* n)  { left = n;	 }
void FourWayLinkedNode::set_right(FourWayLinkedNode* n) { right = n; }

DancingLinksNode::DancingLinksNode(int d, DancingLinksColumn* col) {
	column = col;
	data = d;
	FourWayLinkedNode::set_up(NULL);
	FourWayLinkedNode::set_down(NULL);
	FourWayLinkedNode::set_left(NULL);
	FourWayLinkedNode::set_right(NULL);
	if (DEBUG_DISPLAY_CONSTRUCTION_INFO) {
		cout << "DancingLinksNode: constructed node with data "
			 << data << endl;
	}
}

DancingLinksColumn* DancingLinksNode::get_column() { return column; }
int	DancingLinksNode::get_data()   { return data; }

DancingLinksNode* DancingLinksNode::get_left() {
	return (DancingLinksNode*)FourWayLinkedNode::get_left(); 
}

DancingLinksNode* DancingLinksNode::get_right() {
	return (DancingLinksNode*)FourWayLinkedNode::get_right();
}


DancingLinksColumn::DancingLinksColumn(char* n) {
	name = n;
	size = 0;
	FourWayLinkedNode::set_up(NULL);
	FourWayLinkedNode::set_down(NULL);
	FourWayLinkedNode::set_left(NULL);
	FourWayLinkedNode::set_right(NULL);
	if (DEBUG_DISPLAY_CONSTRUCTION_INFO) {
		cout << "DancingLinksColumn: constructed column with name " 
				<< n << endl;
	}
}

DancingLinksColumn::~DancingLinksColumn() {
	delete name;
}

char* DancingLinksColumn::get_name() { return name; }
int	 DancingLinksColumn::get_size() { return size; }

DancingLinksColumn* DancingLinksColumn::get_left() {
	return (DancingLinksColumn*)FourWayLinkedNode::get_left();
}

DancingLinksColumn* DancingLinksColumn::get_right() {
	return (DancingLinksColumn*)FourWayLinkedNode::get_right();
}

void DancingLinksColumn::insert_left(DancingLinksColumn* l) {
	if (get_left()) {
		get_left()->set_right(l);
	}
	set_left(l);
	l->set_right(get_left());
}

void DancingLinksColumn::insert_right(DancingLinksColumn* r) {
	if (get_right()) {
		get_right()->set_left(r);
	}
	set_right(r);
	r->set_left(get_right());
}

void DancingLinksColumn::cover() {
	if (DEBUG_DISPLAY_COVER_INFO) {
		cout << "DancingLinksColumn:cover: covered column "
			 << get_name()
			 << endl;
	}
	DancingLinksColumn* c = this;
	/* Set L[R[c]] ← L[c] and R[L[c]] ← R[c]. */
	get_right()->set_left(get_left());
	get_left()->set_right(get_right());
	/* For each i ← D[c], D[D[c]], . . . , while i != c,*/
	for (DancingLinksNode* i = (DancingLinksNode*) c->get_down();
		(void*) i != (void*) c;
		i = (DancingLinksNode*)i->get_down()) {

		if (DEBUG_DISPLAY_COVER_INFO) {
			cout << "DancingLinksColumn:cover: covering node i=" << i->get_data() 
				<< ":" << i->get_column()->get_name()
				<< endl;
		}
		/* for each j ← R[i], R[R[i]] , . . . , while j != i,*/
		for (DancingLinksNode* j = (DancingLinksNode*) i->get_right();
			(void*) j != (void*) i;
			j = j->get_right()) {
		
			if (DEBUG_DISPLAY_COVER_INFO) {
				cout << "DancingLinksColumn:cover: covering node j=" << j->get_data() 
					<< ":" << j->get_column()->get_name()
					<< endl;
			}
			/* set U[D[j]] ← U[j], D[U[j]] ← D[j], */
			j->get_down()->set_up(j->get_up());
			j->get_up()->set_down(j->get_down());
			/* and set S[C[j] ← S[C[j]] − 1.*/
			j->get_column()->set_size(j->get_column()->get_size() - 1);
		}
	}
}

void DancingLinksColumn::uncover() {
	if (DEBUG_DISPLAY_COVER_INFO) {
		cout << "DancingLinksColumn:uncover: uncovered column "
	 		 << get_name()
	 		 << endl;
	}
	/* For each i = U[c], U[U[c]], . . . , while i != c, */
	for (DancingLinksNode* i = (DancingLinksNode*) get_up();
		(void*) i != (void*) this;
		i = (DancingLinksNode*) i->get_up()) {

		/* for each j ← L[i], L[L[i]] , . . . , while j != i */
		for (DancingLinksNode* j = i->get_left();
			(void*) j != (void*) i; 
			j = j->get_left()) {
		
			/* set S[C[j]] ← S[C[j]] + 1, */
			j->get_column()->set_size(j->get_column()->get_size() + 1);
			/* and set U[D[j]] ← j, D[U[j]] ← j.*/			
			j->get_up()->set_down(j);
			j->get_down()->set_up(j);
		}
	}
	/* Set L[R[c]] ← c and R[L[c]] ← c. */
	get_right()->set_left(this);
	get_left()->set_right(this);
}

void DancingLinksColumn::set_size(int s) { size = s; }

DancingLinksNode** construct_nodes_from_bitfield(
	DancingLinksColumn* header,  
	bool* piece_at_pos,  
	int num_cols,
	int num_rows) {

	int size = num_cols * num_rows;
	DancingLinksNode** nodefield = new DancingLinksNode * [size];

	DancingLinksColumn* current_col = header;
	for (int c = 0; c < num_cols; c++) {
		current_col = (DancingLinksColumn*) current_col->get_right();

		for (int r = 0; r < num_rows; r++) {

			DancingLinksNode* val_to_write = 
				(*(piece_at_pos + r * num_cols + c)) 
				? (new DancingLinksNode(r, current_col) )
				: NULL;
			*(nodefield + r * num_cols + c) = val_to_write;
		}
	}
	return nodefield;
}

void link_nodes_vertically(DancingLinksNode** nodefield, int num_columns, int num_rows)
{
	/* For each column*/
	for (int c = 0; c < num_columns; c++) {
		DancingLinksNode* first_node = NULL;
		DancingLinksNode* current_node = NULL;
		
		/* for each row */
		for (int r = 0; r < num_rows; r++) {
			DancingLinksNode* found_node = (*(nodefield + r * num_columns + c));
			if (found_node != NULL) {
				if (current_node == NULL) {
					first_node = found_node;
					current_node = found_node;
					continue;
				}
				if (DEBUG_DISPLAY_LINKING_INFO) {
					cout << "link_nodes_vertically: linking node with data "
						<< current_node->get_data()
						<< " to node with data "
						<< found_node->get_data()
						<< endl;
				}
				current_node->set_down(found_node);
				found_node->set_up(current_node);
				current_node = found_node;
			}
		}
		if (first_node != NULL && current_node != NULL) {
			if (DEBUG_DISPLAY_LINKING_INFO) {
				cout << "link_nodes_vertically: linking node with data "
					<< current_node->get_data()
					<< " to node with data "
					<< first_node->get_data()
					<< endl;
			}
			current_node->set_down(first_node);
			first_node->set_up(current_node);
		}
	}
}

void link_nodes_horizontally(DancingLinksNode** nodefield, int num_columns, int num_rows) {
	/* For each row */
	for (int r = 0; r < num_rows; r++) {

		DancingLinksNode* first_node = NULL;
		DancingLinksNode* current_node = NULL;

		/* for each column */
		for (int c = 0; c < num_columns; c++) {

			/* If a node is present at that column in that row */
			DancingLinksNode* found_node = (*(nodefield + r * num_columns + c));
			if (found_node != NULL) {
				if (current_node == NULL) {
					first_node = found_node;
					current_node = found_node;
					continue;
				}
				if (DEBUG_DISPLAY_LINKING_INFO) {
					cout << "link_nodes_horizontally: linking node with data "
						<< current_node->get_data()
						<< " to node with data "
						<< found_node->get_data()
						<< endl;
				}
				/* Links it to the next node in that row. */
				current_node->set_right(found_node);
				found_node->set_left(current_node);
				current_node = found_node;
			}
		}
		if (first_node != NULL && current_node != NULL) {
			if (DEBUG_DISPLAY_LINKING_INFO) {
				cout << "link_nodes_horizontally: linking node with data "
					<< current_node->get_data()
					<< " to node with data "
					<< first_node->get_data()
					<< endl;
			}
			current_node->set_right(first_node);
			first_node->set_left(current_node);
		}
	}
}

void link_columns_to_nodes(
	DancingLinksColumn* h, 
	DancingLinksNode** nodefield, 
	int num_columns,
	int num_rows) {

	DancingLinksColumn* current_col = h->get_right();

	for (int c = 0; c < num_columns; c++) {

		bool seen_first_node = false;
		for (int r = 0; r < num_rows - 1; r++) {
			DancingLinksNode* current_node = *(nodefield + r * num_columns + c);
			if (!seen_first_node) {
				if (current_node) {
					if (DEBUG_DISPLAY_LINKING_INFO) {

						int data = ((DancingLinksNode*) current_node->get_up())->get_data();

						cout << "link_columns_to_nodes: linking column "
							<< current_col->get_name()
							<< " to node with data "
							<< current_node->get_data()
							<< " above and node with data "
							<< data
							<< " below"
							<< endl;
					}

					DancingLinksNode* last_node = 
						(DancingLinksNode*) current_node->get_up();

					last_node->set_down(current_col);
					current_col->set_up(last_node);

					current_col->set_down(current_node);
					current_node->set_up(current_col);

					seen_first_node = true;
				}
			}
			if (current_node) {
				current_col->set_size(current_col->get_size() + 1);
			}
		}
		current_col = (DancingLinksColumn*) current_col->get_right();
	}
}

DancingLinksColumn* construct_header_and_columns(int num_columns) {
	DancingLinksColumn* h = new DancingLinksColumn(_strdup("h"));
	DancingLinksColumn* current_col = h;
	/* Now we create the rest of the columns */
	for (int i = 0; i < num_columns; i++) {	
		char c[8];
		_itoa_s(i, c, 10);
		DancingLinksColumn* n = new DancingLinksColumn(_strdup(c));
		current_col->set_right(n);
		n->set_left(current_col);
		current_col = n;
	}
	current_col->set_right(h);
	h->set_left(current_col);
	return h;
}

DancingLinksMatrix::DancingLinksMatrix(bool piece_at_pos[], int num_columns, int num_rows) {
	/* First we instantiate the first column, h */
	h = construct_header_and_columns(num_columns);
	/* Next we construct the nodes, without arranging them. */
	DancingLinksNode** nodefield = 
		construct_nodes_from_bitfield(h, piece_at_pos, num_columns, num_rows);

	if (DEBUG_DISPLAY_OPTIONS_TO_CONSTRAINTS) {
		for (int r = 0; r < num_rows; r++) {
			cout << r << " = { ";
			for (int c = 0; c < num_columns; c++) {
				if (*(nodefield + r * num_columns + c)) {
					cout << (*(nodefield + r * num_columns + c))->get_column()->get_name() << ", ";
				}
			}
			cout << " }" << endl;
		}
		cout << endl;
	}

	/* Lets arrange each node vertically by linking them. */
	/* for each row */
	link_nodes_vertically(nodefield, num_columns, num_rows);
	/* Lets arrange each node horizontally by linking them. */
	/* for each row */
	link_nodes_horizontally(nodefield, num_columns, num_rows);
	/* Now lets connect our created columns to our nodes we have created. */
	link_columns_to_nodes(h, nodefield, num_columns, num_rows);

	if (DEBUG_DISPLAY_CONSTRAINTS_TO_OPTIONS) {
		for (DancingLinksColumn* c = h->get_right(); c != h; c = c->get_right()) {
			cout << c->get_name() << " is contained within { ";

			for (DancingLinksNode* r = (DancingLinksNode*)c->get_down();
				(void*)r != (void*)c;
				r = (DancingLinksNode*)r->get_down()) {

				cout << r->get_data() << ", ";

			}
			cout << " }" << endl;
		}
		cout << endl;
	}
	/* Memory Management */
	delete[] nodefield;
}

DancingLinksMatrix::~DancingLinksMatrix() {

	/* For each column */
	DancingLinksColumn* next_col = NULL;
	for (DancingLinksColumn* c = h->get_right(); 
		(void*) c != (void*) h; 
		c = next_col) {

		/* For each row */
		DancingLinksNode* next_node = NULL;
		for (DancingLinksNode* r = (DancingLinksNode*) c->get_down(); 
			(void*) r != (void*) c; 
			r = next_node) { 
			/* Delete the row but save its lower neighbour's address. */
			next_node = (DancingLinksNode*) r->get_down();
			delete r;
		}
		/* Delete the column but save its right neighbour's address. */
		next_col = c->get_right();
		delete c;
	}
	delete h;
}


int DancingLinksMatrix::full_search(int k, DancingLinksNode** o) {
	if (DEBUG_DISPLAY_SEARCH_CALLS) {
		cout << "partial_search: called with depth "
			<< k << endl;
	}

	if (DEBUG_DISPLAY_SEARCH_INFO) print_grid();
	/* If R[h] = h, print the current solution (see below) and return. */
	if (h->get_right() == h) {
		//print_solution(o);
		return 0;
	}
	/* Otherwise choose a column object c (see below). */
	DancingLinksColumn* c = h->get_right();
	for (DancingLinksColumn* m = c; m != h; m = m->get_right()) {
		if (m->get_size() < c->get_size()) c = m;
	}

	if (DEBUG_DISPLAY_SEARCH_CHOICE) {
		cout << "partial_search: chosen constraint "
			<< c->get_name()
			<< " with size "
			<< c->get_size()
			<< endl;
	}

	/* Cover column c (see below). */
	c->cover();

	int retval = 1;
	/* For each r ← D[c], D[D[c]] , . . . , while r != c, */
	for (DancingLinksNode* r = (DancingLinksNode*)c->get_down();
		(void*)r != (void*)c;
		r = (DancingLinksNode*)r->get_down()) {

		if (DEBUG_DISPLAY_SEARCH_ACCESSES) {
			cout << "partial_search: r="
				<< r->get_data()
				<< endl;
		}

		/* set Ok ← r; */
		o[k] = r;
		/* for each j ← R[r], R[R[r]] , . . . , while j != r, */
		for (DancingLinksNode* j = r->get_right();
			(void*)j != (void*)r;
			j = j->get_right()) {
			/* cover column j (see below); */
			j->get_column()->cover();

			if (DEBUG_DISPLAY_SEARCH_ACCESSES) {
				cout << "partial_search: considering adding set j ="
					<< j->get_data()
					<< " for constraint "
					<< j->get_column()->get_name()
					<< endl;
			}
		}
		retval = partial_search(k + 1, o);
		/* set r ← Ok and c ← C[r]; */
		r = o[k];
		c = ((DancingLinksNode*)r)->get_column();
		/* for each j ← L[r], L[L[r]], . . . , while j != r, */
		for (DancingLinksNode* j = r->get_left();
			(void*)j != (void*)r;
			j = j->get_left()) {
			/* uncover column j (see below). */
			j->get_column()->uncover();

			if (DEBUG_DISPLAY_SEARCH_ACCESSES) {
				cout << "partial_search: reconsidering adding set j ="
					<< j->get_data()
					<< " for constraint "
					<< j->get_column()->get_name()
					<< endl;
			}
		}
	}
	/* Uncover column c (see below) */
	c->uncover();
	/* and return */
	return retval;
}


int DancingLinksMatrix::partial_search(int k, DancingLinksNode** o) {
	if (DEBUG_DISPLAY_SEARCH_CALLS) {
		cout << "partial_search: called with depth " 
			 << k << endl;
	}
	if (DEBUG_DISPLAY_SEARCH_INFO) print_grid();
	/* If R[h] = h, print the current solution (see below) and return. */
	if (h->get_right() == h) {
		//print_solution(o);
		return 0;
	}
	/* Otherwise choose a column object c (see below). */
	DancingLinksColumn* c = h->get_right();
	for (DancingLinksColumn* m = c; m != h; m = m->get_right()) {
		if (m->get_size() < c->get_size()) c = m;
	}

	if (DEBUG_DISPLAY_SEARCH_CHOICE) {
		cout << "partial_search: chosen constraint "
			 << c->get_name()
			 << " with size "
			 << c->get_size()
			 << endl;
	}

	/* Cover column c (see below). */
	c->cover();

	int retval = 1;
	/* For each r ← D[c], D[D[c]] , . . . , while r != c, */
	for (DancingLinksNode* r = (DancingLinksNode*) c->get_down();
		(void*) r != (void*) c;
		r = (DancingLinksNode*) r->get_down()) {

		if (DEBUG_DISPLAY_SEARCH_ACCESSES) {
			cout << "partial_search: r="
				 << r->get_data()
				 << endl;
		}

		/* set Ok ← r; */
		o[k] = r;
		/* for each j ← R[r], R[R[r]] , . . . , while j != r, */
		for (DancingLinksNode* j = r->get_right();
			(void*) j != (void*) r;
			j = j->get_right()) {
			/* cover column j (see below); */
			j->get_column()->cover();

			if (DEBUG_DISPLAY_SEARCH_ACCESSES) {
				cout << "partial_search: considering adding set j ="
					 << j->get_data()
					 << " for constraint "
					 << j->get_column()->get_name()
					 << endl;
			}
		}
		retval = partial_search(k + 1, o);
		/* set r ← Ok and c ← C[r]; */
		r = o[k];
		c = ((DancingLinksNode*) r)->get_column();
		/* for each j ← L[r], L[L[r]], . . . , while j != r, */
		for (DancingLinksNode* j = r->get_left(); 
			(void*) j != (void*) r;
			j = j->get_left()) {
			/* uncover column j (see below). */
			j->get_column()->uncover();

			if (DEBUG_DISPLAY_SEARCH_ACCESSES) {
				cout << "partial_search: reconsidering adding set j ="
					 << j->get_data()
					 << " for constraint "
					 << j->get_column()->get_name()
					 << endl;
			}
		}
		if (retval == 0) { 
			break;; 
		}
	}
	/* Uncover column c (see below) */
	c->uncover();
	/* and return */
	return retval;
}

int* DancingLinksMatrix::solve() {
	int num_cols = get_num_columns();
	DancingLinksNode** o = new DancingLinksNode*[num_cols];
	for (int i = 0; i < num_cols; i++) o[i] = NULL;
	int result = partial_search(0, o);

	int count = 0;
	for (DancingLinksNode** n = o; *n != NULL; n++) {
		count++;
	}

	int* int_o = new int[count];
	int i = 0;
	for (; i < count; i++) {
		DancingLinksNode *n = *(o + i);
		int val = n->get_data();
		*(int_o + i) = val;
	}	
	*(int_o + i) = -1;
	
	/* Memory Management */
	delete o;

	return int_o;
}

int DancingLinksMatrix::get_num_columns() {
	int count = 0;
	for (DancingLinksColumn* c = h->get_right(); c != h; c = c->get_right()) {
		count++;
	}
	return count;
}

int DancingLinksMatrix::get_num_rows() {
	int max_count = 0;
	for (DancingLinksColumn* c = h->get_right(); c != h; c = c->get_right()) {
		int num_rows_in_col = ((DancingLinksNode*)c->get_up())->get_data();
		if (num_rows_in_col > max_count) max_count = num_rows_in_col;
	}
	return max_count;
}

//
//void DancingLinksMatrix::print_setwise() {
//	/* For each column*/
//DancingLinksColumn* current_col = h;
//for (DancingLinksColumn* c = current_col->get_down(); c != h; c = c->get_right()) {
//	/* For each node */
//	for (DancingLinksNode* n = current_col->get_down())
//}

void DancingLinksMatrix::print_grid() {
	int num_columns = get_num_columns();
	int num_rows = get_num_rows();

	DancingLinksColumn* current_col = h;
	while (1) {
		cout << current_col->get_name() << "\t";
		current_col = (DancingLinksColumn*) current_col->get_right();
		if (current_col == h) break;
	}
	cout << endl;

	while (1) {
		cout << current_col->get_size() << "\t";
		current_col = (DancingLinksColumn*) current_col->get_right();
		if (current_col == h) break;
	}
	cout << endl;

	bool *bnodes = new bool[num_columns * num_rows];
	for (int i = 0; i < num_columns * num_rows; i++) {
		bnodes[i] = false;
	}

	int i = 1;
	for (DancingLinksColumn* c = h->get_right();
		c != h;
		c = c->get_right()) {

		for (DancingLinksNode* j = (DancingLinksNode*) c->get_down();
			(void*)j != (void*) c;
			j = (DancingLinksNode*) j->get_down()) {

			int d = j->get_data() - 1;
			bnodes[d * num_columns + i] = true;
		}
		i++;
	}

	for (int r = 0; r < num_columns; r++) cout << "--------";
	cout << endl;

	for (int r = 0; r < num_rows; r++) {
		for (int c = 0; c < num_columns; c++) {

			if (bnodes[r * num_columns + c] == true) {
				cout << r + 1 << "\t";
			}
			else {
				cout << " " << "\t";
			}
		}
		cout << endl << endl;
	}
	// Memory Management
	delete[] bnodes;
}

void DancingLinksMatrix::print_setwise(DancingLinksNode** o) {
	return;
	for (DancingLinksNode** n = o; *n != NULL; n++) {

		DancingLinksNode* j = *n;
		cout << j->get_data() << " ";
		/*while ((void*) (j = j->get_right()) != (void*) *n) {
			cout << j->get_column()->get_name() << " ";
		}*/
		cout << endl;
	}
}


#endif