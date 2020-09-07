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
	int nx(4), ny(4), depth(16);

	Gamefield gf(nx, ny, depth);

	// std::cout << "correct: " << std::endl;		
	// gf.display(gf.get_corr_placement());
	// std::cout << std::endl;

	// std::cout << "initial: " << std::endl;
	// gf.display(gf.get_init_placement());
	// std::cout << std::endl;

	// std::cout << "current: " << std::endl;
	// gf.display(gf.get_curr_placement());
	// std::cout << std::endl;

	// std::cout << "init_history: " << std::endl;
	// gf.display_init_history();
	// std::cout << std::endl;

	std::cout << "opti_history: " << std::endl;
	gf.display_opti_history();
	std::cout << std::endl;

	placement p = gf.get_state_init(0);
	std::vector<int> h = gf.get_opti_history();
	for (size_t i = 0; i < h.size(); ++i) {
		p = gf.get_state_opti(i);
		// std::cout << "Step " << i << ": " << std::endl;
		// display(p, nx, ny);
	}
	std::cout << "opti: " << std::endl;
	display(p, nx, ny);

	// std::cout << gf.iscorrect(gf.get_corr_placement()) << std::endl;
	// std::cout << std::endl;
	// std::cout << gf.iscorrect(gf.get_init_placement()) << std::endl;


	// std::vector<int> v;
	// v.push_back(1);
	// if (v.empty())
	// 	std::cout << "pusto" << std::endl;
	// else
	// 	std::cout << "ne pusto" << std::endl;
	// v.clear();
	// if (v.empty())
	// 	std::cout << "pusto" << std::endl;
	// else
	// 	std::cout << "ne pusto" << std::endl;

	// placement test;
	// std::vector<int> fh;

	// test.push_back(dice(0, 0, 1));
	// test.push_back(dice(1, 0, 2));
	// test.push_back(dice(2, 0, 3));
	// test.push_back(dice(3, 0, -1));

	// test.push_back(dice(0, 1, 5));
	// test.push_back(dice(1, 1, 6));
	// test.push_back(dice(2, 1, 7));
	// test.push_back(dice(3, 1, 4));

	// test.push_back(dice(0, 2, 9));
	// test.push_back(dice(1, 2, 10));
	// test.push_back(dice(2, 2, 11));
	// test.push_back(dice(3, 2, 8));

	// test.push_back(dice(0, 3, 13));
	// test.push_back(dice(1, 3, 14));
	// test.push_back(dice(2, 3, 15));
	// test.push_back(dice(3, 3, 12));


	// display(test, nx, ny);
	// gf.movedice(3, -1, test, fh);
	// display(test, nx, ny);
	
	
	// placement p = gf.get_state_init(0);
	// std::vector<int> h = gf.get_init_history();
	// for (size_t i = 0; i < h.size(); ++i) {
	// 	p = gf.get_state_init(i);
	// 	std::cout << "Step " << i << ": " << std::endl;
	// 	display(p, nx, ny);
	// }

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
