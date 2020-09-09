#include <iostream>
#include <vector>
//#include <list>
// #include <map>
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

vertex::vertex(placement state_) : state(state_), parent(-1), G(0), H(calc_H()), F(H) {}

vertex::vertex(placement state_, int value_, std::vector<vertex> &close) {
	state = state_;
	parent = static_cast<int>(close.size() - 1);
	value = value_;
	G = calc_G(close);
	H = calc_H();
	F = G + H;
}
// vertex::vertex(int value_, int G_, int H_, int F_) : value(value_), G(G_), H(H_), F(F_) {}

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

int vertex::calc_H() {
	/* manhattan heuristic */
	int h(0);
	for (size_t i = 0; i < state.size(); ++i) {
		h += abs(state[i].pos.i - state[i].corr_pos.i) + abs(state[i].pos.j - state[i].corr_pos.j);
	}
	return h;
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

Gamefield::Gamefield(int nx, int ny, int depth) {
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
	//history.push_back(init); // NEED DELETE
	makeinit(depth, true);
	// display(init);
	// std::cout << std::endl;
	// display_init_history();
	// placement p = get_state_init(init_history.size() - 1);
	// display(p);
	opti_history = Astar(init);
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
	// if (step > 0) {
	// 	if (step <= static_cast<int>(ref_history.size())) {
	// 		for (int i = 0; i < step; ++i) {
	// 			movedice(ref_history[i], p, h);
	// 		}
	// 	}
	// }
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
					// std::cout << j*Nx + i << " " << ref_placement[j*Nx + i].value << std::endl;
					break;
				}
			}
			else {
				if (j*Nx + i != ref_placement.size() - 1) {
					check = false;
					// std::cout << j*Nx + i << " " << ref_placement[j*Nx + i].value << std::endl;
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

		/*std::cout << "fd: " << old_i << ", " << old_j << std::endl;
		std::cout << "step: " << new_i << ", " << new_j << std::endl;
		std::cout << is_mv << std::endl;

		std::cout << std::endl;*/
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

// void Gamefield::makeoptisolve() {
// 	std::vector<vertex> open;
// 	std::vector<vertex> close;

// }

std::vector<vertex> Gamefield::makeNewVertexes(std::vector<vertex> &open, std::vector<vertex> &close) {
	size_t i(0);
	size_t j(0);
	size_t k(0);

	int F(-1);
	for (i = 0; i < open.size(); ++i) {
		if (F < 0) {
			F = open[i].F;
			j = i;
		}
		if (F > open[i].F) {
			F = open[i].F;
			j = i;
		}
	}

	close.push_back(open[j]);
	open.erase(open.begin() + j);

	// std::cout << "F = " << F << std::endl;

	vertex v = close.back();
	placement p = v.state;
	std::vector<int> fake_history;

	bool in_open = false;
	bool in_close = false;
	i = 0;

	std::vector<vertex> last_added;

	// vertex new_open_vertex(p);

	dice fd = get_free_dice(p);

	if (movedice(fd.pos.i + 1, fd.pos.j, p, fake_history)) {
		// std::cout << "1" << std::endl;
		// display(p);
		// display(v.state);
		vertex new_open_vertex1(p, fake_history[0], close);

		// movedice(fake_history[0], v.state, fake_history);
		if (!close.empty()) {
			for (i = 0; i < close.size(); ++i) {
				if (close[i] == new_open_vertex1) {
					in_close = true;
					break;
				}
			}
		}
		if (!open.empty()) {
			for (k = 0; k < open.size(); ++k) {
				if (open[k] == new_open_vertex1) {
					in_open = true;
					break;
				}
			}
		}
		if (in_close) {
			if (close[i].G > new_open_vertex1.G) {
				close[i] = new_open_vertex1;
				// last_added.push_back(new_open_vertex1);
			}
		}
		else {
			if (!in_open) {
				open.push_back(new_open_vertex1);
				last_added.push_back(new_open_vertex1);
			}
		}
		i = 0;
		k = 0;
		in_open = false;
		in_close = false;
		p = v.state;
		fake_history.clear();
	}
	if (movedice(fd.pos.i - 1, fd.pos.j, p, fake_history)) {
		// std::cout << "2" << std::endl;
		// display(p);
		// display(v.state);
		vertex new_open_vertex2(p, fake_history[0], close);

		// movedice(fake_history[0], v.state, fake_history);
		if (!close.empty()) {
			for (i = 0; i < close.size(); ++i) {
				if (close[i] == new_open_vertex2) {
					in_close = true;
					break;
				}
			}
		}
		if (!open.empty()) {
			for (k = 0; k < open.size(); ++k) {
				if (open[k] == new_open_vertex2) {
					in_open = true;
					break;
				}
			}
		}
		if (in_close) {
			if (close[i].G > new_open_vertex2.G) {
				close[i] = new_open_vertex2;
				// last_added.push_back(new_open_vertex1);
			}
		}
		else {
			if (!in_open) {
				open.push_back(new_open_vertex2);
				last_added.push_back(new_open_vertex2);
			}
		}
		i = 0;
		k = 0;
		in_open = false;
		in_close = false;
		p = v.state;
		fake_history.clear();
	}
	if (movedice(fd.pos.i, fd.pos.j + 1, p, fake_history)) {
		// std::cout << "3" << std::endl;
		// display(p);
		// display(v.state);
		vertex new_open_vertex3(p, fake_history[0], close);

		// movedice(fake_history[0], v.state, fake_history);
		if (!close.empty()) {
			for (i = 0; i < close.size(); ++i) {
				if (close[i] == new_open_vertex3) {
					in_close = true;
					break;
				}
			}
		}
		if (!open.empty()) {
			for (k = 0; k < open.size(); ++k) {
				if (open[k] == new_open_vertex3) {
					in_open = true;
					break;
				}
			}
		}
		if (in_close) {
			if (close[i].G > new_open_vertex3.G) {
				close[i] = new_open_vertex3;
				// last_added.push_back(new_open_vertex1);
			}
		}
		else {
			if (!in_open) {
				open.push_back(new_open_vertex3);
				last_added.push_back(new_open_vertex3);
			}
		}
		i = 0;
		k = 0;
		in_open = false;
		in_close = false;
		p = v.state;
		fake_history.clear();
	}
	if (movedice(fd.pos.i, fd.pos.j - 1, p, fake_history)) {
		// std::cout << "4" << std::endl;
		// display(p);
		// display(v.state);
		vertex new_open_vertex4(p, fake_history[0], close);

		// movedice(fake_history[0], v.state, fake_history);
		if (!close.empty()) {
			for (i = 0; i < close.size(); ++i) {
				if (close[i] == new_open_vertex4) {
					in_close = true;
					break;
				}
			}
		}
		if (!open.empty()) {
			for (k = 0; k < open.size(); ++k) {
				if (open[k] == new_open_vertex4) {
					in_open = true;
					break;
				}
			}
		}
		if (in_close) {
			if (close[i].G > new_open_vertex4.G) {
				close[i] = new_open_vertex4;
				// last_added.push_back(new_open_vertex1);
			}
		}
		else {
			if (!in_open) {
				open.push_back(new_open_vertex4);
				last_added.push_back(new_open_vertex4);
			}
		}
		i = 0;
		k = 0;
		in_open = false;
		in_close = false;
		p = v.state;
		fake_history.clear();
	}
	// std::cout << std::endl;
	// if (last_added.empty())
	// 	display(v.state);
	// 	std::cout << std::endl;
	// std::cout << open.size() << " " << close.size() << " " << last_added.size() << std::endl;
	return last_added;
}

std::vector<int> Gamefield::Astar(placement &ref_placement) {
	std::vector<int> history;
	std::vector<vertex> open;
	std::vector<vertex> close;

	std::vector<vertex> last_added;
	// display(ref_placement);
	vertex start_vertex(ref_placement);
	
	if (!iscorrect(ref_placement)) {
		open.push_back(start_vertex);
	}

	while (!open.empty()) {

		last_added = makeNewVertexes(open, close);

		for (size_t i = 0; i < last_added.size(); ++i) {
			// std::cout << "check" << std::endl;
			if (iscorrect(last_added[i].state)) {
				std::cout << "open: " << open.size() << " close: " << close.size() << std::endl;
				history.push_back(last_added[i].value);
				int parent = last_added[i].parent;
				// std::cout << parent << std::endl;
				while (parent > 0) {
					// std::cout << close[parent].value << std::endl;
					history.push_back(close[parent].value);
					parent = close[parent].parent;
				}
				open.clear();
				break;
			}
			if (open.empty()) {
				std::cout << "open empyt" << std::endl;
			}
		}
		if (!last_added.empty()) {
			if (open.size() > 1000) {
				std::cout << open.size() << " " << close.size() << std::endl;
				// open = last_added;
				int F_open(-1);
				int indx_open;
				// int F_close(-1);
				// int indx_close;
				for (size_t k = 0; k < last_added.size(); ++k) {
					if (F_open < 0) {
						F_open = last_added[k].F;
						indx_open = k;
					}
					if (F_open > last_added[k].F) {
						F_open = last_added[k].F;
						indx_open = k;
					}
				}
				int parent = open[indx_open].parent;
				while (parent > 0) {
						// std::cout << close[parent].value << std::endl;
						history.push_back(close[parent].value);
						parent = close[parent].parent;
				}
				open.clear();
				close.clear();
				open.push_back(last_added[indx_open]);
				open[0].parent = -1;
				// std::cout << open.size() << " " << close.size() << std::endl;
				std::cout << "G: " << open[0].G << " H: " << open[0].H << std::endl;
				display(open[0].state);
				std::cout << std::endl;
			}
		}
	}
	
	return history;
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

/* NEED DELETE */
/*void Gamefield::display_history() {
	for (size_t i = 0; i < history.size(); ++i) {
		std::cout << "Step " << i << ": " << std::endl;
		display(history[i]);
	}
}*/
