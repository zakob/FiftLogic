#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <unordered_map>
#include <queue>
// #include <utility>
// #include <random>
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

// vertex::vertex(placement state_, int Nx_, int Ny_) : state(state_), Nx(Nx_), Ny(Ny_), value(0), parent(nullptr), G(0), H(calc_H()), F(H) {}
vertex::vertex(placement &state_, int &Nx_, int &Ny_) : state(state_), Nx(Nx_), Ny(Ny_), value(0), G(0), H(calc_H()), F(H) {}

// vertex::vertex(placement state_, int value_, int Nx_, int Ny_, vertex &parent_) {
vertex::vertex(placement &state_, int &value_, int &Nx_, int &Ny_, std::shared_ptr<vertex> &parent_) {
	state = state_;
	parent = parent_;
	Nx = Nx_;
	Ny = Ny_;
	value = value_;
	G = calc_G();
	H = calc_H();
	F = G + H;
}

int vertex::calc_G() {
	return getParent()->G + 1;
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

const std::shared_ptr<vertex> vertex::getParent() const {
	return parent.lock();
}

size_t VertexHash::operator()(const vertex &v) const {
	/* factorial kernel of a matrix by K.I. Zaytsev */
	size_t h(0);
	int index(0);
	std::vector<int> fak;

	for(auto dice : v.state) {
		if (dice.value != -1)
			fak.push_back(dice.value);
		else
			fak.push_back(0);
	}

	for(int value = 0; value < static_cast<int>(v.state.size()); ++value) {

		for(index = 0; index < static_cast<int>(fak.size()); ++index) {
			if (fak[index] == value)
				break;
		}

		h = h*(static_cast<int>(v.state.size()) - value) + index;
		fak.erase(fak.begin() + index);
	}

	return h;
}

size_t VertexHash::operator()(const std::shared_ptr<vertex> &pv) const {
	/* factorial kernel of a matrix by K.I. Zaytsev */
	size_t h(0);
	int index(0);
	std::vector<int> fak;

	for(auto dice : pv->state) {
		if (dice.value != -1)
			fak.push_back(dice.value);
		else
			fak.push_back(0);
	}

	for(int value = 0; value < static_cast<int>(pv->state.size()); ++value) {

		for(index = 0; index < static_cast<int>(fak.size()); ++index) {
			if (fak[index] == value)
				break;
		}

		h = h*(static_cast<int>(pv->state.size()) - value) + index;
		fak.erase(fak.begin() + index);
	}

	return h;
}

bool VertexHashEqual::operator() (const std::shared_ptr<vertex> &lhs, const std::shared_ptr<vertex> &rhs) const {
	return *lhs == *rhs;
}

bool VertexSorter::operator() (std::shared_ptr<vertex> &lhs, std::shared_ptr<vertex> &rhs) {
	// return lhs->F > rhs->F;
	return lhs->F >= rhs->F;
}

Gamefield::Gamefield() {
}

Gamefield::Gamefield(int &nx, int &ny) {
	Nx = nx;
	Ny = ny;
	int MAX_VALUE = Nx*Ny;

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
	curr = init;
}

Gamefield::Gamefield(int &nx, int &ny, int &depth) : Gamefield(nx, ny) {
	// std::vector<int> h {12, 11, 7, 6, 10, 9, 5, 10, 9, 7, 6, 3, 4, 8, 3, 6, 15, 12, 11, 3, 8, 4, 2, 1, 10, 9, 1, 10, 9, 1, 6, 8, 3, 11};
	// std::vector<int> h {12, 8, 4, 3, 2, 1, 5, 9, 10, 11, 8, 4, 7, 8, 15, 12, 4, 7, 3, 2, 2, 3, 7, 4};
	// std::vector<int> h {12, 11, 10, 9, 5, 6, 9, 10, 7, 8, 4, 3, 8, 9, 10, 14, 15, 7, 11, 4, 4, 12};
	// std::vector<int> h {12, 11, 7,  6,  10, 14, 13, 9,  14, 7,  15, 12, 11, 8,  4,  3,  6,  15, 7,  13, 12, 7,  13, 10, 5,  14, 10, 12, 9,  10, 12, 13, 8, 11};
	// std::vector<int> h {15, 14, 13, 9, 10, 13, 14, 11, 12, 8, 4, 3, 2, 1, 5, 6, 7, 4, 3, 2, 4, 3, 8, 15, 11, 14, 9, 10, 13, 12, 15, 11};
	// std::vector<int> h {12, 8, 7, 11, 15, 12, 8, 7, 11, 3, 2, 1, 5, 6, 3, 15, 10, 3, 1, 2, 15, 1, 3, 10, 12, 8, 7, 11, 1, 3, 3, 1, 11, 7}; // not find optimal path !!!
	// std::vector<int> h {15, 14, 10, 9, 5, 1, 2, 6, 1, 2, 6, 3, 7, 1, 3, 7, 4, 8, 1, 3, 2, 6, 7, 4, 8, 1, 12, 11, 3, 12, 1, 8, 4, 2, 9, 5, 13, 10, 5, 3, 11, 15};
	// std::vector<int> h {15, 11, 12, 8, 4, 3, 7, 12, 10, 6, 2, 1, 5, 9, 6, 2, 1, 5, 9, 6, 13, 14, 11, 10, 8, 15, 10, 11, 2, 1, 5, 9, 6, 5, 12, 8, 1, 12, 8, 4, 3, 7, 4, 8, 5, 6, 9, 4, 8, 5, 12, 2, 11, 1, 15, 10, 1, 11, 14, 13, 2, 15, 11, 1, 10, 11, 5, 8, 7, 3, 11, 10}; // hard
	// std::vector<int> h {12, 11, 10, 9, 5, 6, 7, 3, 4, 8, 3, 4, 8, 3, 4, 7, 2, 1, 6, 5, 13, 14, 15, 10, 11, 12, 10, 15, 9, 11, 7, 8, 3, 4, 8, 3, 4, 8, 3, 7, 15, 10, 12, 15, 11, 2, 1, 4, 8, 3, 15, 12, 10, 11, 7, 15, 12, 10, 11, 7, 10, 11, 7, 9, 2, 10, 9, 2, 10, 9, 11, 7};
	//15 11 10 14 13 9 14 13 11 10 12 8 7 12 8 7 4 3 2 1 5 14 13 8 10 11 8 6 1 2 3 4 7 15 11 8 6 13 14 5 2 1 5 14 13 5 12 10 15 11 8 15 10 12 1 2 14 13 5 6 15 8 11 7 12 1 13 14 2 13 6 10 7 11 8 7 10 15 9 5 14 6 1 3 4 12 3 4 12 3 4 10 7 9 15 7 11 4 10 12 13 2 6 1 12 11 7 12 1 14 12 7 9 8 4 10 3 13 11 1 2 6 14 2 1 11 6 1 11 9 10 3 13 6 9 11 2 12 7 2 1 9 6 13 11 6 9 1 6 10 2 6 10 2 6 7 5 15 8 4 3 11 13 9 2 10 1 2 9 13 10 9 13 10 11 3 4 6 7 5 12 14 2 1 5 12 15 8 12 15 8 12 6 7 3 11 10 13 9 3 15 8 14 2 1 5 2 1 5 2 3 10 11 4 7 6 8 14 1 3 14 8 12 1 3 5 2 9 13 11 10 13 9 2 5 3 1 12 8 1 12 8 6 7 4 10 13 9 2 14 1 6 8 12 3 1 14 2 11 13 10 4 7 8 6 3 12 6 3 14 2 5 1 12 6 3 14 2 5 11 9 15 4 7 8 14 3 6 2 5 11 9 13 10 15 13 9 1 12 11 13 15 7 8 step 56
	//12 8 7 3 4 7 3 11 15 12 8 3 11 6 2 1 5 2 6 15 12 8 3 11 15 4 7 15 11 12 8 3 12 11 15 7 4 8 3 12 11 15 7 4 8 6 2 5 1 2 10 14 13 9 5 1 2 10 1 5 9 13 14 1 6 7 15 11 12 14 1 6 7 8 4 15 8 3 14 1 6 9 5 2 10 7 3 14 9 6 1 12 11 8 15 4 14 3 7 14 3 7 2 10 14 2 6 9 8 11 12 1 9 8 11 15 4 3 2 14 10 6 8 11 15 4 3 2 14 8 11 9 1 15 7 14 2 3 14 2 8 11 9 1 13 5 6 10 11 9 10 6 1 7 15 13 5 1 7 15 2 14 4 2 13 5 1 7 15 13 5 1 13 15 6 11 9 8 3 4 14 5 2 12 1 13 15 2 5 10 2 6 7 15 6 7 15 6 7 2 8 9 11 15 6 7 13 1 12 5 10 8 2 6 7 13 6 2 15 11 9 3 4 14 5 12 1 10 8 5 12 8 10 6 13 7 11 15 3 9 15 3 9 4 5 10 8 1 6 8 2 11 3 9 10 2 8 6 1 12 2 10 4 5 10 8 6 1 12 6 8 2 6 8 2 6 8 2 1 13 11 4 9 15 5 10 6 9 15 5 10 15 5 3 7 11 13 12 2 8 8 2 step 58
	// init_history = h;
	makeinit(depth, true);
	// init = get_state_init(init_history.size() - 1);
	// display(init);
	// opti_history = Astar_slow(init);
	opti_history = Astar(init);
	// optini_history = IDAstar(init);
	optini_history = make_optini();
	curr = init;
}

Gamefield::Gamefield(int &nx, int &ny, std::vector<int> &placement_) : Gamefield(nx, ny) {
	int MAX_VALUE = Nx*Ny;
	for (int i = 0; i < MAX_VALUE; ++i) {
		if (placement_[i] > 0)
			init[i] = dice(i % Nx, i/Nx, (placement_[i] - 1) % Nx, (placement_[i] - 1)/Nx, placement_[i]);
		else
			init[i] = dice(i % Nx, i/Nx, Nx - 1, Ny - 1, placement_[i]);
	}
	curr = init;
	opti_history = Astar(init);
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
				if (j*Nx + i != static_cast<int>(ref_placement.size()) - 1) {
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

std::vector<std::shared_ptr<vertex>> Gamefield::successors(std::shared_ptr<vertex> &v) {
	std::vector<std::shared_ptr<vertex>> new_vertices;

	int s[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
	int new_i(0), new_j(0);
	placement new_state;

	dice fd = get_free_dice(v->state);

	for (int i = 0; i < 4; ++i) {
		new_i = s[i][0] + fd.pos.i;
		new_j = s[i][1] + fd.pos.j;

		if ((new_i > -1) && (new_i < Nx) && (new_j > -1) && (new_j < Ny)) {
			new_state = v->state;
			new_state[fd.pos.j*Nx + fd.pos.i] = dice(fd.pos.i, fd.pos.j, v->state[new_j*Nx + new_i].corr_pos.i, v->state[new_j*Nx + new_i].corr_pos.j, v->state[new_j*Nx + new_i].value);
			new_state[new_j*Nx + new_i] = dice(new_i, new_j, fd.corr_pos.i, fd.corr_pos.j, fd.value);
			// display(new_state);
			// std::cout << i << std::endl;
			// new_vertices.push_back(new vertex(new_state, v.state[new_j*Nx + new_i].value, Nx, Ny, v));
			new_vertices.push_back(std::make_shared<vertex>(new_state, v->state[new_j*Nx + new_i].value, Nx, Ny, v));
		}
	}
	return new_vertices;
}

std::vector<int> Gamefield::Astar_slow(placement &ref_placement) {
	std::vector<int> history;
	
	// std::vector<vertex*> open;
	// std::vector<vertex*> close;
	std::vector<std::shared_ptr<vertex>> open;
	std::vector<std::shared_ptr<vertex>> close;

	std::vector<std::shared_ptr<vertex>> test_vertices;
	// vertex start_vertex(ref_placement, Nx, Ny);
	
	if (!iscorrect(ref_placement)) {
		open.push_back(std::make_shared<vertex>(ref_placement, Nx, Ny));
		// open.push_back(new vertex(ref_placement, Nx, Ny));
	}
	else {
		return history;
	}

	while (!open.empty()) {
		size_t i(0);
		size_t j(0);
		int F(-1);
		
		for (i = 0; i < open.size(); ++i) {
			if (F < 0) {
				F = open[i]->F;
				j = i;
			}
			if (F >= open[i]->F) {
			// if (F > open[i].F) {
				F = open[i]->F;
				j = i;
			}
		}

		close.push_back(open[j]);
		open.erase(open.begin() + j);

		test_vertices = successors(close.back());

		std::vector<int> check_close(test_vertices.size(), -1);
		std::vector<int> check_open(test_vertices.size(), -1);

		int count(0);
		if (!close.empty()) {
			for (i = 0; i < close.size(); ++i) {
				for (size_t n = 0; n < test_vertices.size(); ++n) {
					if (*close[i] == *test_vertices[n]) {
						check_close[n] = i;
						count += 1;
					}
				}
				if (count == static_cast<int>(test_vertices.size()))
					break;
			}
		}
		count = 0;
		if (!open.empty()) {
			for (i = 0; i < open.size(); ++i) {
				for (size_t n = 0;  n < test_vertices.size(); ++n) {
					if (*open[i] == *test_vertices[n]) {
						check_open[n] = i;
						count += 1;
					}
				}
				if (count == static_cast<int>(test_vertices.size()))
					break;
			}
		}

		for (size_t n = 0; n < test_vertices.size(); ++n) {
			if (check_close[n] > 0) {
				// delete test_vertices[n];
				continue;
			}
			else {
				if (check_open[n] < 0) {
					open.push_back(test_vertices[n]);
				}
				else {
					if (open[check_open[n]]->G > test_vertices[n]->G) {
						open[check_open[n]] = test_vertices[n];
						// open[check_open[n]]->G = test_vertices[n]->G;
						// open[check_open[n]]->H = test_vertices[n]->H;
						// open[check_open[n]]->F = test_vertices[n]->F;
						// open[check_open[n]]->value = test_vertices[n]->value;
						// open[check_open[n]]->parent = test_vertices[n]->parent;
						// delete test_vertices[n];
					}	
				}
			}
		}

		for (i = 0; i < test_vertices.size(); ++i) {
			if (test_vertices[i]->H == 0) {
				std::cout << "open: " << open.size() << " close: " << close.size() << std::endl;
				history.push_back(test_vertices[i]->value);
				// vertex *p = test_vertices[i]->parent;
				// std::shared_ptr<vertex> p = test_vertices[i].getParent();
				std::shared_ptr<vertex> p = test_vertices[i]->getParent();
				while (p->getParent() != nullptr) {
					history.push_back(p->value);
					// p = p->parent;
					p = p->getParent();
				}
				return history;
			}
		}

		// if (open.size() > 10000) {
		// 	std::cout << open.size() << " " << close.size() << std::endl;
		// }
	}
	
	return history;
}

std::vector<int> Gamefield::Astar(placement &ref_placement) {
	/* https://www.redblobgames.com/pathfinding/a-star/implementation.html */

	std::priority_queue<std::shared_ptr<vertex>,
	std::deque<std::shared_ptr<vertex>>,
	VertexSorter> frontier;
	
	std::unordered_map<std::shared_ptr<vertex>,
	std::shared_ptr<vertex>,
	VertexHash,
	VertexHashEqual> came_from;

	std::vector<std::shared_ptr<vertex>> test_vertices;

	std::vector<int> history;
	
	std::shared_ptr<vertex> start = std::make_shared<vertex>(ref_placement, Nx, Ny);

	frontier.push(start);
	came_from[start] = start;

	int c(0);

	while (!frontier.empty()) {
		std::shared_ptr<vertex> current = frontier.top();
		frontier.pop();

		if (current->H == 0) {
			std::cout << "num vertices: " << c << std::endl;
			std::cout << "H: " << current->H;
			history.push_back(current->value);
			std::shared_ptr<vertex> p = current->getParent();
			while (p->getParent() != nullptr) {
				std::cout << " " << p->H;
				history.push_back(p->value);
				p = p->getParent();
			}
			std::cout << std::endl;
			return history;
		}
		
		test_vertices = successors(current);

		for (auto next : test_vertices) {
			if (came_from.find(next) == came_from.end() || next->G < came_from[next]->G) {
				frontier.push(next);
				came_from[next] = current;
			}
		}
		c++;
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
		placement p = get_state_init(i);
		// states.push_back(vertex(get_state_init(i), Nx, Ny));
		states.push_back(vertex(p, Nx, Ny));
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

bool Gamefield::IsCombinationAssembled() {
	int s = 0;
	for (size_t i = 0; i < corr.size(); i++) {
		if (corr[i].value == curr[i].value) {
			s = s + 1;
		}
	}
	if (s == Nx * Ny) {
		return true;
	}
	else {
		return false;
	}
}
void Gamefield::ClearHistory() {
	curr = init;
	curr_history.clear();
	init_history.clear();
	last_history.clear();
}
void Gamefield::UE4Astar() {
	opti_history = Astar(init);
}