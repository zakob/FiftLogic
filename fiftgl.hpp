#pragma once
#include <vector>

/* Fift Game Logic */

struct position {
	position();
	position(int x, int y);
	int i;
	int j;
};

struct dice {
	dice(int x, int y, int n);
	dice(position pos, int n);
	position pos;
	int value;
};

typedef std::vector<dice> placement;

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

		dice& get_free_dice(placement &ref_placement);
		void display_history(std::vector<int> &ref_history);
	public:
		Gamefield(int nx, int ny, int depth);

		placement& get_corr_placement();
		placement& get_init_placement();
		placement& get_curr_placement();
		placement get_state(int step, placement &ref_placment, std::vector<int> &ref_history);
		placement get_state_init(int step);
		placement get_state_curr(int step);
		//std::vector<placement>& get_history();
		
		int get_indexOfdice(placement &ref_placement, int i, int j);
		int get_indexOfdice(placement &ref_placement, int value);

		std::vector<int>& get_init_history();
		std::vector<int>& get_curr_history();
		//std::vector<dice> get_lastStep();
		
		//void normalize_placement(placement &ref_placement);

		bool movedice(int i, int j, placement &ref_placement, std::vector<int> &ref_history);
		bool movedice(int value, placement &ref_placement, std::vector<int> &ref_history);
		bool movedice(int i, int j);
		bool movedice(int value);

		void makeinit(int depth, bool make_norm);
		//void get_solve();

		void display(placement &ref_placement);
		void display_init_history();
		void display_curr_history();
		//void display_history(); // NEED DELETE
};
