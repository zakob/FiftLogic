#include "fiftgl.hpp"
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <memory>

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
	// std::vector<int> placement_ {11, 12, 9, 10, 15, -1, 13, 14, 3, 4, 1, 2, 7, 8, 5, 6};
	// std::vector<int> placement_ {-1, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	// std::vector<int> placement_ {1, 2, 15, 3, 7, 4, 8, 11, 5, 10, 6, 12, 9, 13, 14, -1};
	// std::vector<int> placement_ {5, 1, 3, 4, 2, 13, 6, 7, 9, 15, 8, 12, 14, 11, 10, -1};
	// std::vector<int> placement_ {1, 2, 3, 4, 5, 6, 7, 8, 9, 13, 15, -1, 14, 11, 10, 12};
	Gamefield gf(nx, ny, depth);
	// Gamefield gf(nx, ny, placement_);
	
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

void test_pq(int nx, int ny) {
	Gamefield gf(nx, ny);
	
	placement p1 = gf.get_init_placement();
	
	gf.movedice(nx*ny - 1);
	placement p2 = gf.get_curr_placement();

	gf.movedice(nx - 1);
	placement p3 = gf.get_curr_placement();

	std::shared_ptr<vertex> v1 = std::make_shared<vertex>(p1, nx, ny);
	std::shared_ptr<vertex> v2 = std::make_shared<vertex>(p2, nx, ny);
	std::shared_ptr<vertex> v3 = std::make_shared<vertex>(p3, nx, ny);

	std::cout << "v1_F: " << v1->F << std::endl;
	std::cout << "v2_F: " << v2->F << std::endl;
	std::cout << "v3_F: " << v3->F << std::endl;

	std::priority_queue<std::shared_ptr<vertex>,
			    std::deque<std::shared_ptr<vertex>>,
			    VertexSorter> q;
		
	q.push(v3);
	q.push(v2);
	q.push(v1);
	q.push(v1);

	while (!q.empty()) {
		std::cout << q.top()->F << std::endl;
		q.pop();
	}

}

void test_umap(int nx, int ny) {
	Gamefield gf(nx, ny);
	
	placement p1 = gf.get_init_placement();
	
	gf.movedice(nx*ny - 1);
	placement p2 = gf.get_curr_placement();

	gf.movedice(nx - 1);
	placement p3 = gf.get_curr_placement();
	placement p4 = gf.get_curr_placement();

	std::shared_ptr<vertex> v1 = std::make_shared<vertex>(p1, nx, ny);
	std::shared_ptr<vertex> v2 = std::make_shared<vertex>(p2, nx, ny);
	std::shared_ptr<vertex> v3 = std::make_shared<vertex>(p3, nx, ny);
	std::shared_ptr<vertex> v4 = std::make_shared<vertex>(p4, nx, ny);

	std::cout << "v1_F: " << v1->F << std::endl;
	std::cout << "v2_F: " << v2->F << std::endl;
	std::cout << "v3_F: " << v3->F << std::endl;

	//std::unordered_map<std::shared_ptr<vertex>,
	//		   size_t,
	//		   VertexHash> umap;
	std::unordered_map<std::shared_ptr<vertex>,
			   std::shared_ptr<vertex>,
			   VertexHash,
			   VertexHashEqual> umap;

	//VertexHash h;

	//umap.emplace(v1, h(v1));
	//umap.emplace(v2, h(v2));
	//umap.emplace(v3, h(v3));

	umap[v1] = v1;
	umap[v2] = v2;
	umap[v3] = v3;

	for (auto v: umap) {
		std::cout << v.first << " " << v.second->F << std::endl;
	}

	if (umap.find(v4) != umap.end())
		std::cout << "found" << std::endl;
	else
		std::cout << "not found" << std::endl;
	
}

void test_constructor(int nx, int ny) {
	std::vector<int> placement_ {3, 15, 12, 11, 5, 6, 7, 13, 4, 10, 14, 9, 2, 8, 1, -1}; // 4x4 - 60 steps
	// std::vector<int> placement_ {2, -1, 3, 5, 1, 4}; // 2x3
	Gamefield gf(nx, ny, placement_);
	std::cout << "initial: " << std::endl;
	gf.display(gf.get_init_placement());
	std::cout << std::endl;
}

int main(int argc, char *argv[]) {
	int nx(4), ny(4), depth(40);
	
	// test_constructor(nx, ny);
	test_search_algorithm(nx, ny, depth);
	// test_VertexHash(nx, ny);		
	// test_pq(nx, ny);
	//test_umap(nx, ny);

	return 0;
}
