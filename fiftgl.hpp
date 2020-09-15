#pragma once
#include <vector>
#include <string>
//#include <list>

/* Fift Game Logic */

struct position {
	position();
	position(int x, int y);
	int i;
	int j;

	// bool operator== (const position &pos);
};

struct dice {
	dice(int x, int y, int value_);
	dice(int x, int y, int x0, int y0, int value_);
	dice(position pos_, position corr_pos_, int value_);
	position pos;
	position corr_pos;
	int value;
};

typedef std::vector<dice> placement;

struct vertex {
	vertex(placement state_, int Nx_, int Ny_);
	vertex(placement state_, int value_, int Nx_, int Ny_, std::vector<vertex> &close);
	
	int linearConflicts();
	int manhattan();
	int inversion();
	int calc_G(std::vector<vertex> &close);
	int calc_H();

	bool operator==(const vertex &v);
	bool operator!=(const vertex &v);
	bool operator>(const vertex &v);

	placement state;
	int Nx;
	int Ny;

	int value;
	int parent;
	int G;
	int H;
	int F;
};

class Gamefield {
	private:
		placement corr;
		placement init;
		placement curr;
		int Nx;
		int Ny;
		//std::vector<placement> history; // NEED DELETE
		std::vector<int> curr_history;
		std::vector<int> init_history;
		std::vector<int> last_history;
		std::vector<int> opti_history;
		std::vector<int> optini_history;

		dice& get_free_dice(placement &ref_placement);
		void display_history(std::vector<int> &ref_history);
	public:
		Gamefield(int nx, int ny);
		Gamefield(int nx, int ny, int depth);
		Gamefield(std::string pth_to_file);

		placement& get_corr_placement();
		placement& get_init_placement();
		placement& get_curr_placement();
		placement get_state(int step, placement &ref_placment, std::vector<int> &ref_history);
		placement get_state_init(int step);
		placement get_state_curr(int step);
		placement get_state_opti(int step);
		placement get_state_optini(int step);
		
		int get_indexOfdice(placement &ref_placement, int i, int j);
		int get_indexOfdice(placement &ref_placement, int value);

		std::vector<int>& get_init_history();
		std::vector<int>& get_curr_history();
		std::vector<int>& get_last_history();
		std::vector<int>& get_opti_history();
		std::vector<int>& get_optini_history();
		//std::vector<dice> get_lastStep();
		
		//void normalize_placement(placement &ref_placement);

		bool movedice(int i, int j, placement &ref_placement, std::vector<int> &ref_history);
		bool movedice(int value, placement &ref_placement, std::vector<int> &ref_history);
		bool movedice(int i, int j);
		bool movedice(int value);

		bool iscorrect(placement &ref_placement);
		bool iscorrect();

		void makeinit(int depth, bool make_norm);
		
		/* find optimal solve */
		std::vector<vertex> makeNewVertices(std::vector<vertex> &open, std::vector<vertex> &close);
		// std::vector<vertex> makeNewVertices(std::priority_queue<vertex, std::vector<vertex>, std::greater<vertex>> &open, std::vector<vertex> &close);
		int makeNewVertices(std::vector<vertex> &path, int &F_bound);
		std::vector<int> Astar(placement &ref_placement);
		std::vector<int> IDAstar(placement &ref_placement);
		
		std::vector<int> make_optini();

		void display(placement &ref_placement);
		void display_init_history();
		void display_curr_history();
		void display_opti_history();
		void display_optini_history();
		//void display_history(); // NEED DELETE
};
