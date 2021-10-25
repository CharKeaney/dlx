/**
* DancingLinks.h implements the dancing links data structure and algorithm.
* @author Charlie Keaney
*/

#ifndef DANCING_LINKS_CPP
#define DANCING_LINKS_CPP 1

#include "DancingLinks.h"

FourWayLinkedNode::FourWayLinkedNode() {
		up = NULL;
		down = NULL;
		left = NULL;
		right = NULL;
}

FourWayLinkedNode* FourWayLinkedNode::get_up() { return up;			}
FourWayLinkedNode* FourWayLinkedNode::get_down() { return down;		}
FourWayLinkedNode* FourWayLinkedNode::get_left() { return left;		}
FourWayLinkedNode* FourWayLinkedNode::get_right() { return right;	}

void FourWayLinkedNode::set_up(FourWayLinkedNode* n) { up = n;		}
void FourWayLinkedNode::set_down(FourWayLinkedNode* n) { down = n;	}
void FourWayLinkedNode::set_left(FourWayLinkedNode* n) { left = n;	}
void FourWayLinkedNode::set_right(FourWayLinkedNode* n) { right = n;}

DancingLinksNode::DancingLinksNode(int d, DancingLinksColumn* col) {
	FourWayLinkedNode(*this);
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
int	DancingLinksNode::get_data() { return data; }

DancingLinksNode* DancingLinksNode::get_left() {
	return (DancingLinksNode*)FourWayLinkedNode::get_left(); 
}

DancingLinksNode* DancingLinksNode::get_right() {
	return (DancingLinksNode*)FourWayLinkedNode::get_right();
}


DancingLinksColumn::DancingLinksColumn(char n) {
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

char DancingLinksColumn::get_name() { return name; }
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
	DancingLinksColumn* c = this;
	/* Set L[R[c]] ← L[c] and R[L[c]] ← R[c]. */
	get_right()->set_left(get_left());
	get_left()->set_right(get_right());
	/* For each i ← D[c], D[D[c]], . . . , while i != c,*/
	for (DancingLinksNode* i = (DancingLinksNode*)c->get_down();
		(void*)i != (void*)c;
		i = (DancingLinksNode*)i->get_down()) {
		/* for each j ← R[i], R[R[i]] , . . . , while j != i,*/
		for (DancingLinksNode* j = (DancingLinksNode*)i->get_right();
			(void*)j != (void*)i;
			j = j->get_right()) {

			/* set U[D[j]] ← U[j], D[U[j]] ← D[j], */
			j->get_down()->set_up(j->get_up());
			j->get_up()->set_down(j->get_down());
			/* and set S[C[j] ← S[C[j]] − 1.*/
			j->get_column()->set_size(j->get_column()->get_size() - 1);
		}
	}
}

void DancingLinksColumn::uncover() {
	/* For each i = U[c], U[U[c]], . . . , while i != c, */
	for (DancingLinksNode* i = (DancingLinksNode*)get_up();
		(void*)i != (void*)this;
		i = (DancingLinksNode*)i->get_up()) {
		/* for each j ← L[i], L[L[i]] , . . . , while j != i */
		for (DancingLinksNode* j = i->get_left();
			(void*)j != (void*)i;
			j = j->get_left()) {
			/* set S[C[j]] ← S[j] + 1, */
			j->get_column()->set_size(j->get_column()->get_size() + 1);
			/* and set U[D[j]] ← j, D[U[j]] ← j.*/
			j->get_down()->set_up(j);
			j->get_up()->set_down(j);
		}
	}
	/* Set L[R[c]] ← c and R[L[c]] ← c. */
	get_right()->set_left(this);
	get_left()->set_right(this);
}

void DancingLinksColumn::set_size(int s) { size = s; }

DancingLinksNode** construct_nodes_from_bitfield(
	DancingLinksColumn* header,  
	bool* bitfield,  
	int num_cols) {

	int size = num_cols * num_cols;
	DancingLinksNode** nodefield = new DancingLinksNode * [size];

	DancingLinksColumn* current_col = header;
	for (int j = 0; j < num_cols; j++) {
		current_col = (DancingLinksColumn*)current_col->get_right();

		for (int i = 0; i < num_cols; i++) {

			DancingLinksNode* val_to_write = 
				(*(bitfield + i * num_cols + j)) 
				? new DancingLinksNode(i + 1, current_col) 
				: NULL;
			*(nodefield + i * num_cols + j) = val_to_write;
		}
	}
	return nodefield;
}

void link_nodes_vertically(DancingLinksNode** nodefield, int num_columns)
{
	for (int c = 0; c < num_columns; c++) {

		DancingLinksNode* first_node = NULL;
		DancingLinksNode* current_node = NULL;
		/* for each col */
		for (int r = 0; r < num_columns - 1; r++) {

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

void link_nodes_horizontally(DancingLinksNode** nodefield, int num_columns) {
	for (int r = 0; r < num_columns - 1; r++) {
		DancingLinksNode* first_node = NULL;
		DancingLinksNode* current_node = NULL;

		/* for each col*/
		for (int c = 0; c < num_columns; c++) {

			DancingLinksNode* found_node = (*(nodefield + r * num_columns + c));
			if (found_node) {
				if (!current_node) {
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
				current_node->set_right(found_node);
				found_node->set_left(current_node);
				current_node = found_node;
			}
		}
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

void link_columns_to_nodes(
	DancingLinksColumn* h, 
	DancingLinksNode** nodefield, 
	int num_columns) {

	DancingLinksColumn* current_col = h->get_right();

	for (int c = 0; c < num_columns; c++) {

		bool seen_first_node = false;
		for (int r = 0; r < num_columns - 1; r++) {
			DancingLinksNode* current_node = *(nodefield + (r) * (num_columns)+c);
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
		current_col = (DancingLinksColumn*)current_col->get_right();
	}
}

DancingLinksColumn* construct_header_and_columns(int num_columns) {
	DancingLinksColumn* h = new DancingLinksColumn('h');
	DancingLinksColumn* current_col = h;
	/* Now we create the rest of the columns */
	for (int i = 0; i < num_columns; i++) {
		DancingLinksColumn* n = new DancingLinksColumn((char)i + 'a');
		current_col->set_right(n);
		n->set_left(current_col);
		current_col = n;
	}
	current_col->set_right(h);
	h->set_left(current_col);
	return h;
}

DancingLinksMatrix::DancingLinksMatrix(bool bitfield[], int num_columns) {
	/* First we instantiate the first column, h */
	h = construct_header_and_columns(num_columns);
	/* Next we construct the nodes, without arranging them. */
	DancingLinksNode** nodefield = 
		construct_nodes_from_bitfield(h, bitfield, num_columns);
	/* Lets arrange each node vertically by linking them. */
	/* for each row */
	link_nodes_vertically(nodefield, num_columns);
	/* Lets arrange each node horizontally by linking them. */
	/* for each row */
	link_nodes_horizontally(nodefield, num_columns);
	/* Now lets connect our created columns to our nodes we have created. */
	link_columns_to_nodes(h, nodefield, num_columns);
}

int DancingLinksMatrix::search(int k, DancingLinksNode** o) {
	if (DEBUG_DISPLAY_SEARCH_CALLS) {
		cout << "search: called with depth " << k << endl;
	}
	if (DEBUG_DISPLAY_SEARCH_INFO) print();
	/* If R[h] = h, print the current solution (see below) and return. */
	if (h->get_right() == h) {
		print_solution(o);
		return 0;
	}
	/* Otherwise choose a column object c (see below). */
	DancingLinksColumn* c = h->get_right();
	/* Cover column c (see below). */
	c->cover();

	/* For each r ← D[c], D[D[c]] , . . . , while r != c, */
	for (DancingLinksNode* r = (DancingLinksNode*)c->get_down();
		(void*)r != (void*)c;
		r = (DancingLinksNode*)r->get_down()) {

		/* set Ok ← r; */
		o[k] = r;
		/* for each j ← R[r], R[R[r]] , . . . , while j != r, */
		for (DancingLinksNode* j = r->get_right();
			(void*)j != (void*)r;
			j = j->get_right()) {
			/* cover column j (see below); */
			j->get_column()->cover();
		}
		search(k + 1, o);
		/* set r ← Ok and c ← C[r]; */
		r = o[k];
		c = ((DancingLinksNode*)r)->get_column();
		/* for each j ← L[r], L[L[r]], . . . , while j != r, */
		for (DancingLinksNode* j = r->get_left(); j != r; j = j->get_left()) {
			/* uncover column j (see below). */
			j->get_column()->uncover();
		}
	}
	/* Uncover column c (see below) */
	c->uncover();
	/* and return */
	return 1;
}

void DancingLinksMatrix::print() {
	DancingLinksColumn* current_col = h;
	while (1) {
		cout << current_col->get_name() << "\t";
		current_col = (DancingLinksColumn*)current_col->get_right();
		if (current_col == h) break;
	}
	cout << endl;

	while (1) {
		cout << current_col->get_size() << "\t";
		current_col = (DancingLinksColumn*)current_col->get_right();
		if (current_col == h) break;
	}
	cout << endl;

	bool bnodes[8 * 6];
	for (int i = 0; i < 8 * 6; i++) {
		bnodes[i] = false;
	}

	int i = 1;
	for (DancingLinksColumn* c = h->get_right();
		c != h;
		c = c->get_right()) {

		for (DancingLinksNode* j = (DancingLinksNode*)c->get_down();
			(void*)j != (void*)c;
			j = (DancingLinksNode*)j->get_down()) {

			int d = j->get_data() - 1;
			bnodes[d * 8 + i] = true;
		}
		i++;
	}

	for (int r = 0; r < i; r++) cout << "--------";
	cout << endl;

	for (int r = 0; r < 6; r++) {
		for (int c = 0; c < i; c++) {

			if (bnodes[r * 8 + c] == true) {
				cout << r + 1 << "\t";
			}
			else {
				cout << " " << "\t";
			}
		}
		cout << endl << endl;
	}
}

void DancingLinksMatrix::print_solution(DancingLinksNode** o) {
	for (DancingLinksNode** n = o; *n != NULL; n++) {

		DancingLinksNode* j = *n;
		cout << j->get_column()->get_name() << " ";
		while ((void*)(j = j->get_right()) != (void*)*n) {
			cout << j->get_column()->get_name() << " ";
		}
		cout << endl;
	}
}

#endif