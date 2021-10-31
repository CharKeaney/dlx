
#ifndef DLX_SUDOKU_CPP
#define DLX_SUDOKU_CPP 1

#include <string>
#include <iostream>
#include "DancingLinks.h"

using namespace std;

#define DEBUG_DISPLAY_CONSTRAINTS_TABLE 0
#define DEBUG_DISPLAY_SOLUTION 0

#define get_row(pos)	(pos / 9)
#define get_col(pos)	(pos % 9)
#define get_box(pos)	((get_row(pos) / 3) * 3 + get_col(pos) / 3)

#define rows_connected(pos1, pos2)	(get_row(pos1) == get_row(pos2))
#define cols_connected(pos1, pos2)	(get_col(pos1) == get_col(pos2))
#define box_connected(pos1, pos2)	(get_box(pos1) == get_box(pos2))

bool pieces_are_connected(int a, int b) {
	return (rows_connected(a, b)
			|| cols_connected(a, b)
			|| box_connected(a, b));
}

class SudokuGrid { 
private: 
	bool piece_at_pos[9][81];
	bool map_piece_position_to_piece_group_covered[9][81][9 * 27 + 81];

	void initialize_piece_at_pos(string s) {
		/* For each piece in piece_at_pos */
		for (int p = 0; p < 9; p++) {
			/* For each position in piece_at_pos*/
			for (int n = 0; n < 81; n++) {
				/* initialize to false */
				piece_at_pos[p][n] = false;
			}
		}
		/* For each character in string s */
		for (int i = 0; i < 9 * 9; i++) {
			/* Put string character into piece_at_pos for piece in position */
			char c = s[i];
			int c_sudo_val = c - '0';
			if (c_sudo_val == 0) continue;
			piece_at_pos[c_sudo_val - 1][i] = true;
		}
	}

	void initialize_map_piece_position_to_piece_group_covered() {
		/* For each piece */
		for (int pc = 0; pc < 9; pc++) {
			/* For each position */
			for (int pos = 0; pos < 81; pos++) {
				/* For each other piece */
				for (int i = 0; i < 9 * 27 + 81; i++) {
					map_piece_position_to_piece_group_covered[pc][pos][i] = false;
				}
			}
		}
		/* For each piece */
		for (int pc = 0; pc < 9; pc++) {
			/* For each position */
			for (int pos = 0; pos < 81; pos++) {
				/* For each position that piece could cover */
				/* if this position is already covered by some piece already placed */
				bool piece_already_covered = false;
				if (!piece_at_pos[pc][pos]) {
					for (int i = 0; i < 81; i++) {

						if (piece_at_pos[pc][i] && pieces_are_connected(pos, i)) {
							/* and this is not the piece that was placed */
							if (pos != i) {
								/* then disregard this position. */
								piece_already_covered = true;
							}
							break;
						}
					}
				}
				if (piece_already_covered) continue;
				map_piece_position_to_piece_group_covered[pc][pos][pos] = true;
				map_piece_position_to_piece_group_covered[pc][pos][81 + pc * 27 + get_row(pos)] = true;
				map_piece_position_to_piece_group_covered[pc][pos][81 + pc * 27 + 9 + get_col(pos)] = true;
				map_piece_position_to_piece_group_covered[pc][pos][81 + pc * 27 + 18 + get_box(pos)] = true;
			}
		}
	}

	bool* create_choice_constraint_table() {
		bool* choice_constraint_table = new bool[(9 * 81) * (9 * 27 + 81)];
		/* For each piece */
		for (int pc = 0; pc < 9; pc++) {
			/* For each position */
			for (int pos = 0; pos < 81; pos++) {
				/* For each field in the constraint table entry */
				for (int ci = 0; ci < 9 * 27 + 81; ci++) {
					*(choice_constraint_table + (pos * 9 + pc) * (9 * 27 + 81) + ci) =
						map_piece_position_to_piece_group_covered[pc][pos][ci];
				}
			}
		}
		return choice_constraint_table;
	}

public:
	SudokuGrid(string s) {
		initialize_piece_at_pos(s);
		initialize_map_piece_position_to_piece_group_covered();
		
		/* Display the constraints table if the debugging option is enabled. */
		if (DEBUG_DISPLAY_CONSTRAINTS_TABLE) {
			/* For each position */
			for (int pos = 0; pos < 81; pos++) {
				/* For each piece */
				for (int pc = 0; pc < 9; pc++) {
					cout << "pos-piece:" << pos << ":" << pc << ":" << pos * 9 + pc << ":\t";
					/* for each field in the constraint table row */
					/* starting with the constraint of a piece in each square */
					for (int c = 0; c < 81; c++) {
						cout << map_piece_position_to_piece_group_covered[pc][pos][c] ? 1 : 0;
					}
					/* and continuing with the constraints for a piece in each group */
					cout << " ";
					for (int p = 0; p < 9; p++) {
						for (int i = 0; i < 27; i++) {
							cout << map_piece_position_to_piece_group_covered[pc][pos][81 + p * 27 + i] ? 1 : 0;
						}
						cout << " ";
					}
					cout << endl;
				}
			}
		}

	}

	void print_solved() {
		bool* choice_constraint_table = create_choice_constraint_table();
		DancingLinksMatrix dlx = DancingLinksMatrix(choice_constraint_table, 9 * 27 + 81, 9 * 81);
		
		int* solution = dlx.solve();
		int ans[81];
		
		if (DEBUG_DISPLAY_SOLUTION) {
			for (int i = 0; i < 81; i++) {
				ans[i] = 0;
			}
			for (int* s = solution; *s != -1; s++) {
				cout << "pos-piece:" << *s << ":\t";

				for (int c = 0; c < 81; c++) {
					cout << *(choice_constraint_table + *s * (9 * 27 + 81) + c) ? 1 : 0;
				}
				cout << " ";

				for (int p = 0; p < 9; p++) {
					for (int i = 0; i < 27; i++) {
						cout << *(choice_constraint_table + *s * (9 * 27 + 81) + (p * 27 + i) + 81) ? 1 : 0;
					}
					cout << " ";
				}
				cout << endl;
			}
		}

		for (int* s = solution; *s != -1; s++) {
			int piece = *s % 9;
			int pos = *s / 9;
			ans[pos] = piece + 1;
		}

		for (int r = 0; r < 9; r++) {
			for (int c = 0; c < 9; c++) {
				cout << ans[r * 9 + c];
			}
			cout << endl;
		}
	}
};

void main() {

	SudokuGrid sg = SudokuGrid("100007090030020008009600500005300900010080002600004000300000010040000007007000300");
	sg.print_solved();
}

#endif