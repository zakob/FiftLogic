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

void test_VertexHash(int nx, int ny) {
	Gamefield gf(nx, ny);

	// std::cout << "initial: " << std::endl;
	// gf.display(gf.get_init_placement());
	// std::cout << std::endl;

	VertexHash hash;

	vertex a(gf.get_curr_placement(), nx, ny);

	std::cout << "current: " << std::endl;
	gf.display(gf.get_curr_placement());
	std::cout << std::endl;
	std::cout << "hash: " << hash(a) << std::endl;

	gf.movedice(nx*ny - 1);

	vertex b(gf.get_curr_placement(), nx, ny);

	std::cout << "current: " << std::endl;
	gf.display(gf.get_curr_placement());
	std::cout << std::endl;
	std::cout << "hash: " << hash(b) << std::endl;

	gf.movedice(nx - 1);

	vertex c(gf.get_curr_placement(), nx, ny);

	std::cout << "current: " << std::endl;
	gf.display(gf.get_curr_placement());
	std::cout << std::endl;
	std::cout << "hash: " << hash(c) << std::endl;


}

void test_search_algorithm(int nx, int ny, int depth) {
	Gamefield gf(nx, ny, depth);

	// std::cout << "correct: " << std::endl;		
	// gf.display(gf.get_corr_placement());
	// std::cout << std::endl;

	std::cout << "initial: " << std::endl;
	gf.display(gf.get_init_placement());
	std::cout << std::endl;

	// std::cout << "current: " << std::endl;
	// gf.display(gf.get_curr_placement());
	// std::cout << std::endl;

	std::cout << "init_history: " << std::endl;
	gf.display_init_history();
	std::cout << gf.get_init_history().size() << std::endl;
	std::cout << std::endl;

	std::cout << "opti_history: " << std::endl;
	gf.display_opti_history();
	std::cout << gf.get_opti_history().size() << std::endl;
	std::cout << std::endl;

	std::vector<int> h = gf.get_opti_history();
	placement p = gf.get_state_opti(h.size() - 1);
	std::cout << "opti: " << std::endl;
	display(p, nx, ny);
	std::cout << std::endl;

	std::cout << "optini_history: " << std::endl;
	gf.display_optini_history();
	std::cout << gf.get_optini_history().size() << std::endl;
	std::cout << std::endl;

	h = gf.get_optini_history();
	p = gf.get_state_optini(h.size() - 1);
	std::cout << "optini: " << std::endl;
	display(p, nx, ny);
	std::cout << std::endl;

	vertex v(gf.get_init_placement(), nx, ny);
	std::cout << "lC: " << v.linearConflicts() << std::endl;
	std::cout << "m: " << v.manhattan() << std::endl;
	std::cout << "i: " << v.inversion() << std::endl;
}

int main(int argc, char *argv[]) {
	int nx(4), ny(4), depth(30);

	test_search_algorithm(nx, ny, depth);
	// test_VertexHash(nx, ny);		

	return 0;
}
