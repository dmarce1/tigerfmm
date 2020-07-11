#pragma once

#include <tigerfmm/expansion.hpp>
#include <tigerfmm/multipole.hpp>
#include <tigerfmm/particle.hpp>


class tree {
	space_vector<real> Xcom;
	space_vector<real> Xmax;
	space_vector<real> Xmin;
	real Rmax;
	expansion<real> Lphi;
	multipole<real> M;
	integer level;
	bool is_leaf;
	std::vector<particle> particles;
	tree* children[nchild];
	std::vector<tree> child_vector;
public:


	tree();
	~tree();
	bool operator<(const tree& other);
	space_vector<real> force_sum(bool norm) const;
	space_vector<real> torque_sum(const bool norm) const;
	void output(const char* filename) const ;
	bool is_well_separated_from(const tree& other) const;
	void compute_expansions(const expansion<real>& Lphi0,
			const space_vector<real>& X0);
	bool get_leaf_directory(std::vector<tree*>& dir) ;
	void compute_interactions(std::vector<tree*> cells);
	void P2P(tree* other);
	void P2Pself(const tree* other);
	void compute_multipoles();
	void direct_interaction_at(const space_vector<real>& X, real& phi,
			space_vector<real>& g) const;
	real compute_error(const tree& root, bool torque);
	void initialize();
	integer create_tree(const space_vector<real>& xmin,
			const space_vector<real>& xmax, std::vector<particle> parts,
			integer lev);
	real solve();
	static real get_top10();
	static void set_theta(real theta);
};


