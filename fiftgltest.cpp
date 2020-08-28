#include "fiftgl.hpp"
#include <iostream>

void display(placement &ref_placement, int nx, int ny) {
	for (int j = 0; j < ny; ++j) {
		for (int i = 0; i < nx; ++i) {
			std::cout << "\t" << ref_placement[j*nx + i].value;
		}
	std::cout << std::endl;
	}
}

int main(int argc, char *argv[]) {
	int nx(4), ny(4), depth(9);

	Gamefield gf(nx, ny, depth);

	std::cout << "correct: " << std::endl;		
	gf.display(gf.get_corr_placement());
	std::cout << std::endl;

	std::cout << "initial: " << std::endl;
	gf.display(gf.get_init_placement());
	std::cout << std::endl;

	std::cout << "current: " << std::endl;
	gf.display(gf.get_curr_placement());
	std::cout << std::endl;

	std::cout << "init_history: " << std::endl;
	gf.display_init_history();
	std::cout << std::endl;
	
	placement p = gf.get_state_init(0);
	std::vector<int> h = gf.get_init_history();
	for (size_t i = 0; i < h.size(); ++i) {
		p = gf.get_state_init(i);
		std::cout << "Step " << i << ": " << std::endl;
		display(p, nx, ny);
	}

	// gf.movedice(4);
	// gf.movedice(1);
	// gf.display_curr_history();

	// std::cout << "current: " << std::endl;
	// gf.display(gf.get_curr_placement());
	// std::cout << std::endl;

	/*std::cout << "step 0: " << std::endl;
	placement p0 = gf.get_state_init(0);
	display(p0, nx, ny);
	std::cout << std::endl;

	std::cout << "step 1: " << std::endl;
	placement p1 = gf.get_state_init(1);
	display(p1, nx, ny);
	std::cout << std::endl;

	std::cout << "step -1: " << std::endl;
	placement p_1 = gf.get_state_init(gf.get_init_history().size() - 2);
	display(p_1, nx, ny);
	std::cout << std::endl;*/


	/*int i = gf.get_indexOfdice(gf.get_curr_placement(), -1);
	std::cout << "free dice: " << i << std::endl;
	std::cout << std::endl;

	std::cout << "gf.movedice(3, 2): " << gf.movedice(3, 2) << std::endl;
	std::cout << std::endl;
	std::cout << "gf.movedice(3, 1): " << gf.movedice(3, 1) << std::endl;
	std::cout << std::endl;
	std::cout << "gf.movedice(0, 0): " << gf.movedice(0, 0) << std::endl;
	std::cout << std::endl;


	i = gf.get_indexOfdice(gf.get_curr_placement(), -1);
	std::cout << "free dice: " << i << std::endl;
	std::cout << std::endl;
	

	std::cout << "current: " << std::endl;
	gf.display(gf.get_curr_placement());
	std::cout << std::endl;*/


	//std::cout << "history: " << std::endl;

	//gf.display_history();

	//gf.makeinit(1000, true);

	//gf.display_history();
	/*std::cout << "initial: " << std::endl;
	gf.display(gf.get_init_placement());
	std::cout << std::endl;*/

	
	return 0;
}
