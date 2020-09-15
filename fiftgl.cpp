#include <iostream>
#include <vector>
//#include <list>
// #include <map>
#include <queue>
#include <string>
#include <cstdlib>
#include <ctime>
//#include <random>
#include "fiftgl.hpp"

position::position() : i(0), j(0) {}

// bool position::operator== (const position &pos) {
// }

position::position(int x, int y) : i(x), j(y) {}

dice::dice(int x, int y, int value_) {
	pos = position(x, y);
	corr_pos = position(x, y);
	value = value_;
}

dice::dice(int x, int y, int x0, int y0, int value_) {
	pos = position(x, y);
	corr_pos = position(x0, y0);
	value = value_;
}

dice::dice(position pos_, position corr_pos_, int value_) : pos(pos_), corr_pos(corr_pos_), value(value_) {}

vertex::vertex(placement state_, int Nx_, int Ny_) : state(state_), parent(-1), Nx(Nx_), Ny(Ny_), G(0), H(calc_H()), F(H) {}

vertex::vertex(placement state_, int value_, int Nx_, int Ny_, std::vector<vertex> &close) {
	state = state_;
	parent = static_cast<int>(close.size() - 1);
	Nx = Nx_;
	Ny = Ny_;
	value = value_;
	G = calc_G(close);
	H = calc_H();
	F = G + H;
}

int vertex::calc_G(std::vector<vertex> &close) {
	return close[parent].G + 1;
}

// int vertex::calc_H() {
// 	int h(0);
// 	for (size_t i = 0; i < state.size(); ++i) {
// 		if (state[i].value != -1) {
// 			h += abs(static_cast<int>(i) - state[i].value + 1);
// 		}
// 		else {
// 			h += abs(static_cast<int>(i) - static_cast<int>(state.size()) + 1);
// 		}
// 	}
// 	return h;
// }

// int vertex::calc_H() {
// 	/* calculate number of dices which not inplace */
// 	int h(0);
// 	int Nx(4), Ny(4);
// 	for (int j = 0; j < Ny; ++j) {
// 		for (int i = 0; i < Nx; ++i) {
// 			if ((state[j*Nx + i].value - 1 != j*Nx + i) && (state[j*Nx + i].value != -1))
// 				h += 1;
// 			if (state[j*Nx + i].value == -1)
// 				if (j*Nx + i + 1 != Nx*Ny)
// 					h += 1;
// 		}
// 	}
// 	return h;
// }

int vertex::linearConflicts() {
	int conflicts(0);
	// std::vector<int> checked;
	for (size_t i = 0; i < state.size(); ++i) {
		for (size_t j = 0; j < state.size(); ++j) {
			if ((i != j) && (state[i].value != -1) && (state[j].value != -1)) {
				if (((state[i].pos.i != state[i].corr_pos.i) || (state[i].pos.j != state[i].corr_pos.j)) && ((state[j].pos.i != state[j].corr_pos.i) || (state[j].pos.j != state[j].corr_pos.j))) {
					// if ((state[i].corr_pos.i == state[i].pos.i) && (state[j].corr_pos.i == state[j].pos.i) && (state[i].corr_pos.i == state[j].pos.i) && (state[j].corr_pos.i == state[i].pos.i)) {
					if ((state[i].corr_pos.i == state[i].pos.i) && (state[j].corr_pos.i == state[j].pos.i) && (state[i].corr_pos.i == state[j].corr_pos.i)) {
						// if ((((state[i].pos.j - state[i].corr_pos.j) < 0 ) && ((state[j].pos.j - state[j].corr_pos.j) > 0)) || (((state[i].pos.j - state[i].corr_pos.j) > 0 ) && ((state[j].pos.j - state[j].corr_pos.j) < 0))) {
						if (((state[i].pos.j < state[j].pos.j) && (state[i].value > state[j].value)) || ((state[i].pos.j > state[j].pos.j) && (state[i].value < state[j].value))) {
						// if (true) {
							// std::cout << state[i].value << " : " << state[j].value << std::endl;
							conflicts++;
							// conflicts += abs(state[i].pos.j - state[i].corr_pos.j);
						}
					}
					// if ((state[i].corr_pos.j == state[i].pos.j) && (state[j].corr_pos.j == state[j].pos.j) && (state[i].corr_pos.j == state[j].pos.j) && (state[j].corr_pos.j == state[i].pos.j)) {
					if ((state[i].corr_pos.j == state[i].pos.j) && (state[j].corr_pos.j == state[j].pos.j) && (state[i].corr_pos.j == state[j].corr_pos.j)) {
						// if ((((state[i].pos.i - state[i].corr_pos.i) < 0 ) && ((state[j].pos.i - state[j].corr_pos.i) > 0)) || (((state[i].pos.i - state[i].corr_pos.i) > 0 ) && ((state[j].pos.i - state[j].corr_pos.i) < 0))) {
						if (((state[i].pos.i < state[j].pos.i) && (state[i].value > state[j].value)) || ((state[i].pos.i > state[j].pos.i) && (state[i].value < state[j].value))) {
						// if (true) {
							// std::cout << state[i].value << " : " << state[j].value << std::endl;
							conflicts++;
							// conflicts += abs(state[i].pos.i - state[i].corr_pos.i);
						}
					}
				}
			}
		}
	}
	return conflicts;
}

int vertex::manhattan() {
	/* manhattan heuristic */
	int h(0);
	for (size_t i = 0; i < state.size(); ++i) {
		if (state[i].value > 0)
			h += abs(state[i].pos.i - state[i].corr_pos.i) + abs(state[i].pos.j - state[i].corr_pos.j);
	}
	// h = h + linearConflicts();
	return h;
}

int vertex::inversion() {
	/* https://michael.kim/blog/puzzle */
	int vcount(0), hcount(0);
	for (size_t i = 0; i < state.size(); ++i) {
		for (size_t j = 0; j < state.size(); ++j) {
			if ((state[i].value > 0) && (state[j].value > 0)) {
				// if (((i - j) == -1) && (state[i].value > state[j].value))
				if ((i < j) && (state[i].value > state[j].value))
					vcount++;
				// if ((((state[i].pos.i*Ny + state[i].pos.j) - (state[j].pos.i*Ny + state[j].pos.j) == -1)) && ((state[i].corr_pos.i*Ny + state[i].corr_pos.j) > (state[j].corr_pos.i*Ny + state[j].corr_pos.j))) // j*Nx + i + 1 = value
				if (((state[i].pos.i*Ny + state[i].pos.j) < (state[j].pos.i*Ny + state[j].pos.j)) && ((state[i].corr_pos.i*Ny + state[i].corr_pos.j) > (state[j].corr_pos.i*Ny + state[j].corr_pos.j)))
					hcount++;
			}
		}
	}
	// std::cout << "v: " << vcount << " h: " << hcount << std::endl;
	return (vcount / 3 + vcount % 3) + (hcount / 3 + hcount % 3);
}

int vertex::calc_H() {
	// return manhattan();
	// return inversion();
	// return manhattan() + linearConflicts();
	return std::max(inversion(), (manhattan() + linearConflicts()));
}

bool vertex::operator==(const vertex &v) {
	bool check = true;
	for (size_t i = 0; i < v.state.size(); ++i) {
		if (v.state[i].value != this->state[i].value) {
			check = false;
			break;
		}
	}
	return check;
}

bool vertex::operator!=(const vertex &v) {
	return !(operator==(v));
}

bool vertex::operator>(const vertex &v) {
	return this->F > v.F;
}

Gamefield::Gamefield(int nx, int ny, int depth) {
	Nx = nx;
	Ny = ny;
	int MAX_VALUE = Nx*Ny;

	// std::vector<int> h {12, 11, 7, 6, 10, 9, 5, 10, 9, 7, 6, 3, 4, 8, 3, 6, 15, 12, 11, 3, 8, 4, 2, 1, 10, 9, 1, 10, 9, 1, 6, 8, 3, 11};
	// std::vector<int> h {12, 8, 4, 3, 2, 1, 5, 9, 10, 11, 8, 4, 7, 8, 15, 12, 4, 7, 3, 2, 2, 3, 7, 4};
	// std::vector<int> h {12, 11, 10, 9, 5, 6, 9, 10, 7, 8, 4, 3, 8, 9, 10, 14, 15, 7, 11, 4, 4, 12};
	// std::vector<int> h {12, 11, 7,  6,  10, 14, 13, 9,  14, 7,  15, 12, 11, 8,  4,  3,  6,  15, 7,  13, 12, 7,  13, 10, 5,  14, 10, 12, 9,  10, 12, 13, 8, 11};
	// std::vector<int> h {15, 14, 13, 9, 10, 13, 14, 11, 12, 8, 4, 3, 2, 1, 5, 6, 7, 4, 3, 2, 4, 3, 8, 15, 11, 14, 9, 10, 13, 12, 15, 11};
	// std::vector<int> h {12, 8, 7, 11, 15, 12, 8, 7, 11, 3, 2, 1, 5, 6, 3, 15, 10, 3, 1, 2, 15, 1, 3, 10, 12, 8, 7, 11, 1, 3, 3, 1, 11, 7}; // not find optimal path !!!
	// std::vector<int> h {15, 14, 10, 9, 5, 1, 2, 6, 1, 2, 6, 3, 7, 1, 3, 7, 4, 8, 1, 3, 2, 6, 7, 4, 8, 1, 12, 11, 3, 12, 1, 8, 4, 2, 9, 5, 13, 10, 5, 3, 11, 15};
	// init_history = h;
	for (int j = 0; j < Ny; ++j) {
		for (int i = 0; i < Nx; ++i) {
			int value = j*Nx + i + 1;
			if (MAX_VALUE > value) {
				corr.push_back(dice(i, j, value));
				init.push_back(dice(i, j, value));
				//curr.push_back(dice(i, j, value));
			}
			else {
				corr.push_back(dice(i, j, -1));
				init.push_back(dice(i, j, -1));
				//curr.push_back(dice(i, j, -1));
			}
		} 
	}
	//history.push_back(init); // NEED DELETE
	makeinit(depth, true);
	// init = get_state_init(init_history.size() - 1);
	// display(init);
	// std::cout << std::endl;
	// display_init_history();
	// placement p = get_state_init(init_history.size() - 1);
	// display(p);
	opti_history = Astar(init);
	// optini_history = IDAstar(init);
	optini_history = make_optini();
	curr = init;
}

placement& Gamefield::get_corr_placement() {
	return corr;
}

placement& Gamefield::get_init_placement() {
	return init;
}

placement& Gamefield::get_curr_placement() {
	return curr;
}

placement Gamefield::get_state(int step, placement &ref_placement, std::vector<int> &ref_history) {
	placement p = ref_placement;
	std::vector<int> fake_history;
	if (step <= static_cast<int>(ref_history.size())) {
		for (int i = 0; i <= step; ++i) {
			movedice(ref_history[i], p, fake_history);
		}
	}
	return p;	
}

placement Gamefield::get_state_init(int step) {
	return get_state(step, corr, init_history);
}

placement Gamefield::get_state_opti(int step) {
	return get_state(step, corr, opti_history);
}

placement Gamefield::get_state_curr(int step) {
	return get_state(step, init, curr_history);
}

placement Gamefield::get_state_optini(int step) {
	return get_state(step, corr, optini_history);
}

dice& Gamefield::get_free_dice(placement &ref_placement) {
	return ref_placement[get_indexOfdice(ref_placement, -1)];
}

int Gamefield::get_indexOfdice(placement &ref_placement, int i, int j) {
	int index(-1);
	for(index = 0; index < Nx*Ny; ++index) {
		if ((ref_placement[index].pos.i == i) && (ref_placement[index].pos.j == j))
			break;
	}
	return index;
}
int Gamefield::get_indexOfdice(placement &ref_placement, int value) {
	int index(-1);
	for(index = 0; index < Nx*Ny; ++index) {
		if (ref_placement[index].value == value)
			break;
	}
	return index;
}

std::vector<int>& Gamefield::get_init_history() {
	return init_history;
}

std::vector<int>& Gamefield::get_curr_history() {
	return curr_history;
}

std::vector<int>& Gamefield::get_opti_history() {
	return opti_history;
}

std::vector<int>& Gamefield::get_optini_history() {
	return optini_history;
}

bool Gamefield::movedice(int i, int j, placement &ref_placement, std::vector<int> &ref_history) {
	last_history.clear();
	if ((i < Nx) && (j < Ny) && (i > -1) && (j > -1)) {
		dice &fd = get_free_dice(ref_placement);
		if ((fd.pos.i == i) && (fd.pos.j == j)) {
			return false;
		}
		else {
			if (fd.pos.i == i) {
				/* move col */
				placement tmp = ref_placement;
				int delta = fd.pos.j - j;
				int step = 1;
				if (delta < 0) {
					delta = -delta;
					step = -step;
				}
				for (int k = delta - 1; k != -1; --k) {
					dice md = tmp[get_indexOfdice(tmp, i, j + step*k)];
					// int c_value = tmp[get_indexOfdice(tmp, i, j + step*k)].value;
					// position c_pos = tmp[get_indexOfdice(tmp, i, j + step*k)].corr_pos;
					ref_placement[(j + step*k + step)*Nx + i] = dice(i, j + step*k + step, md.corr_pos.i, md.corr_pos.j, md.value);
					ref_history.push_back(md.value);
					last_history.push_back(md.value);
				}
				ref_placement[j*Nx + i] = dice(i, j, -1); // move free dice to new place
				//history.push_back(ref_placement); // NEED DELETE
				return true;
			}
			else if (fd.pos.j == j) {
				/* move raw */
				placement tmp = ref_placement;
				int delta = fd.pos.i - i;
				int step = 1;
				if (delta < 0) {
					delta = -delta;
					step = -step;
				}
				for (int k = delta - 1; k != -1; --k) {
					dice md = tmp[get_indexOfdice(tmp, i + step*k, j)];
					// int c_value = tmp[get_indexOfdice(tmp, i + step*k, j)].value;
					// int c_value = tmp[get_indexOfdice(tmp, i + step*k, j)].value;
					ref_placement[j*Nx + i + step*k + step] = dice(i + step*k + step, j, md.corr_pos.i, md.corr_pos.j, md.value);
					ref_history.push_back(md.value);
					last_history.push_back(md.value);
				}
				ref_placement[j*Nx + i] = dice(i, j, -1); // move free dice to new place
				//history.push_back(ref_placement); // NEED DELETE
				return true;
			}
			else {
				return false;
			}
		}
	}
	else {
		return false;
	}
}

bool Gamefield::movedice(int value, placement &ref_placement, std::vector<int> &ref_history) {
	int index = get_indexOfdice(ref_placement, value);
	int i, j;
	i = ref_placement[index].pos.i;
	j = ref_placement[index].pos.j;
	return movedice(i, j, ref_placement, ref_history);
}

bool Gamefield::movedice(int i, int j) {
	return movedice(i, j, curr, curr_history);
}

bool Gamefield::movedice(int value) {
	return movedice(value, curr, curr_history);
}

bool Gamefield::iscorrect(placement &ref_placement) {
	bool check = true;
	for (int j = 0; j < Ny; ++j) {
		for (int i = 0; i < Nx; ++i) {
			if (ref_placement[j*Nx + i].value != -1) {
				if (ref_placement[j*Nx + i].value != j*Nx + i + 1) {
					check = false;
					break;
				}
			}
			else {
				if (j*Nx + i != ref_placement.size() - 1) {
					check = false;
					break;
				}
			}
		}
	}
	return check;
}

bool Gamefield::iscorrect() {
	return iscorrect(curr);
}

void Gamefield::makeinit(int depth, bool make_norm) {
	std::srand(std::time(nullptr)); // use current time as seed for random generator
	std::vector<position> available_steps;
	int r = 0;
	int step = 1;
	//bool is_mv;
	dice &fd = get_free_dice(init);
	int fd_i, fd_j, old_i(-1), old_j(-1), new_i(0), new_j(0);
	fd_i = fd.pos.i;
	fd_j = fd.pos.j;
	for (int n = 0; n < depth; ++n) {
		//r = std::rand() & 1; //r = std::rand() % 2;
		
		//display(init);
		/* prepare available steps */
		if ((old_i != fd_i + step) && (fd_i + step < Nx))
			available_steps.push_back(position(fd_i + step, fd_j));

		if ((old_i != fd_i - step) && (fd_i - step > -1))
			available_steps.push_back(position(fd_i - step, fd_j));

		if ((old_j != fd_j + step) && (fd_j + step < Ny))
			available_steps.push_back(position(fd_i, fd_j + step));

		if ((old_j != fd_j - step) && (fd_j - step > -1))
			available_steps.push_back(position(fd_i, fd_j - step));

		/* make random step from available steps */
		r = std::rand() % static_cast<int>(available_steps.size());
		new_i = available_steps[r].i;
		new_j = available_steps[r].j;

		r = 0;
		available_steps.clear();

		movedice(new_i, new_j, init, init_history);

		old_i = fd_i;
		old_j = fd_j;

		fd_i = new_i;
		fd_j = new_j;
	}
	if (make_norm) {
		if (fd_i != Nx - 1) {
			movedice(Nx - 1, fd_j, init, init_history);
			fd_i = Nx - 1;
		}
		if (fd_j != Ny - 1)
			movedice(fd_i, Ny - 1, init, init_history);
	}
}

std::vector<vertex> Gamefield::makeNewVertices(std::vector<vertex> &open, std::vector<vertex> &close) {
// std::vector<vertex> Gamefield::makeNewVertices(std::priority_queue<vertex, std::vector<vertex>, std::greater<vertex>> &open, std::vector<vertex> &close) {
	size_t i(0);
	size_t j(0);
	size_t k(0);

	int F(-1);
	for (i = 0; i < open.size(); ++i) {
		if (F < 0) {
			F = open[i].F;
			j = i;
		}
		if (F >= open[i].F) {
		// if (F > open[i].F) {
			F = open[i].F;
			j = i;
		}
	}

	close.push_back(open[j]);
	open.erase(open.begin() + j);

	// close.push_back(open.top());
	// open.pop();

	vertex v = close.back();
	placement p = v.state;
	std::vector<int> fake_history;

	i = 0;

	std::vector<vertex> test_vertices;
	std::vector<vertex> last_added;

	dice fd = get_free_dice(p);

	if (movedice(fd.pos.i + 1, fd.pos.j, p, fake_history)) {
		vertex new_open_vertex1(p, fake_history[0], Nx, Ny, close);
		test_vertices.push_back(new_open_vertex1);
		p = v.state;
		fake_history.clear();	
	}
	if (movedice(fd.pos.i - 1, fd.pos.j, p, fake_history)) {
		vertex new_open_vertex2(p, fake_history[0], Nx, Ny, close);
		test_vertices.push_back(new_open_vertex2);
		p = v.state;
		fake_history.clear();
	}
	if (movedice(fd.pos.i, fd.pos.j + 1, p, fake_history)) {
		vertex new_open_vertex3(p, fake_history[0], Nx, Ny, close);
		test_vertices.push_back(new_open_vertex3);
		p = v.state;
		fake_history.clear();
	}
	if (movedice(fd.pos.i, fd.pos.j - 1, p, fake_history)) {
		vertex new_open_vertex4(p, fake_history[0], Nx, Ny, close);
		test_vertices.push_back(new_open_vertex4);
		p = v.state;
		fake_history.clear();
	}

	std::vector<int> check_close(test_vertices.size(), -1);
	std::vector<int> check_open(test_vertices.size(), -1);

	int count(0);
	if (!close.empty()) {
		for (i = 0; i < close.size(); ++i) {
			for (size_t n = 0; n < test_vertices.size(); ++n) {
				if (close[i] == test_vertices[n]) {
					check_close[n] = i;
					count += 1;
				}
			}
			if (count == test_vertices.size())
				break;
		}
	}
	count = 0;
	if (!open.empty()) {
		for (k = 0; k < open.size(); ++k) {
			for (size_t n = 0;  n < test_vertices.size(); ++n) {
				if (open[k] == test_vertices[n]) {
					check_open[n] = k;
					count += 1;
				}
			}
			if (count == test_vertices.size())
				break;
		}
	}

	for (size_t n = 0; n < test_vertices.size(); ++n) {
		// if (check_close[n] > 0) {
		// 	if (close[check_close[n]].G > test_vertices[n].G) {
		// 		close[check_close[n]] = test_vertices[n];
		// 	}
		// }
		// else {
		// 	if (check_open[n] < 0) {
		// 		open.push_back(test_vertices[n]);
		// 		last_added.push_back(test_vertices[n]);
		// 	}
		// }
		if (check_close[n] > 0) {
			continue;
		}
		else {
			if (check_open[n] < 0) {
				open.push_back(test_vertices[n]);
				last_added.push_back(test_vertices[n]);
			}
			else {
				if (open[check_open[n]].G > test_vertices[n].G) {
				// if (true) {
					open[check_open[n]] = test_vertices[n];
				}	
			}
		}
	}
	// std::cout << open.size() << " " << close.size() << " " << last_added.size() << std::endl;
	return last_added;
}

// int Gamefield::makeNewVertices(std::vector<vertex> &path, int &F_bound) {
// 	return F_bound;
// }

std::vector<int> Gamefield::Astar(placement &ref_placement) {
	std::vector<int> history;
	std::vector<vertex> open;
	// std::priority_queue<vertex, std::vector<vertex>, std::greater<vertex>> open;
	std::vector<vertex> close;

	std::vector<vertex> last_added;
	vertex start_vertex(ref_placement, Nx, Ny);
	
	if (!iscorrect(ref_placement)) {
		open.push_back(start_vertex);
	}

	while (!open.empty()) {

		last_added = makeNewVertices(open, close);

		for (size_t i = 0; i < last_added.size(); ++i) {
			// if (iscorrect(last_added[i].state)) {
			if (last_added[i].H == 0) {
				std::cout << "open: " << open.size() << " close: " << close.size() << std::endl;
				history.push_back(last_added[i].value);
				int parent = last_added[i].parent;
				while (parent > 0) {
					history.push_back(close[parent].value);
					parent = close[parent].parent;
				}
				// open.clear();
				// break;
				return history;
			}
			// if (open.empty()) {
			// 	std::cout << "open empty" << std::endl;
			// }
		}

		// if (open.size() > 10000) {
		// 	std::cout << open.size() << " " << close.size() << std::endl;
			// int F(-1);
			// int indx;
			// for (size_t k = 0; k < open.size(); ++k) {
			// 	if (F < 0) {
			// 		F = open[k].F;
			// 		indx = k;
			// 	}
			// 	if (F > open[k].F) {
			// 		F = open[k].F;
			// 		indx = k;
			// 	}
			// }
			// int parent = open[indx].parent;
			// while (parent > 0) {
			// 		history.push_back(close[parent].value);
			// 		parent = close[parent].parent;
			// }
			// vertex v = open[indx];
			// open.clear();
			// open.push_back(v);
			// open[0].parent = -1;
			// close.clear();
			
			// std::cout << "G: " << open[0].G << " H: " << open[0].H << std::endl;
			// display(open[0].state);
			// std::cout << std::endl;
	// 	}
	}
	
	return history;
}

// std::vector<int> Gamefield::IDAstar(placement &ref_placement) {
// 	std::vector<int> history;
// 	std::vector<vertex> path;
	
// 	return history;
// }

std::vector<int> Gamefield::make_optini() {
	std::vector<int> opti_h = init_history;
	std::vector<vertex> states;
	
	for (size_t i = 0; i < init_history.size(); ++i) {
		states.push_back(vertex(get_state_init(i), Nx, Ny));
	}

	bool check(false);
	while (!check) {
		for (size_t i = 0; i < states.size(); ++i) {
			for (size_t j = 0; j < states.size(); ++j) {
				if ((states[i] == states[j]) && (i != j)) {
					opti_h.erase(opti_h.begin() + i + 1, opti_h.begin() + j + 1);
					states.erase(states.begin() + i + 1, states.begin() + j + 1);
					check = true;
					// std::cout << "delete from init history " << i << " : " << j << std::endl;
					break;
				}
				else {
					check = false;
				}
			}
			if (check) {
				check = false;
				break;
			}
			else {
				check = true;
			}
		}
	}

	return opti_h;
}

void Gamefield::display(placement &ref_placement) {
	for (int j = 0; j < Ny; ++j) {
		for (int i = 0; i < Nx; ++i) {
			std::cout << "\t" << ref_placement[j*Nx + i].value;
		}
		std::cout << std::endl;
	}
	/*std::cout << "_ref: " << &ref_placement << std::endl;
	std::cout << "corr: " << &corr << std::endl;
	std::cout << "init: " << &init << std::endl;
	std::cout << "curr: " << &curr << std::endl;*/
}

void Gamefield::display_history(std::vector<int> &ref_history) {
	int l = 7;
	for (size_t i = 0; i < ref_history.size(); ++i) {
		std::cout << ref_history[i] << " ";
		if ((i + 1 % l) == 0) {
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}

void Gamefield::display_init_history() {
	display_history(init_history);
}

void Gamefield::display_curr_history() {
	display_history(curr_history);
}

void Gamefield::display_opti_history() {
	display_history(opti_history);
}

void Gamefield::display_optini_history() {
	display_history(optini_history);
}

/* NEED DELETE */
/*void Gamefield::display_history() {
	for (size_t i = 0; i < history.size(); ++i) {
		std::cout << "Step " << i << ": " << std::endl;
		display(history[i]);
	}
}*/
