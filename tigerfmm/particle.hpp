#pragma once

#include <tigerfmm/defs.hpp>

#include <vector>

struct particle {
	space_vector<real> X;
	space_vector<real> g;
	space_vector<real> g_analytic;
	real M;
	real phi;
	real phi_analytic;
	bool operator<(const particle& other) {
		if (X[0] < other.X[0]) {
			return true;
		} else if (X[0] == other.X[0]) {
			if (X[1] < other.X[1]) {
				return true;
			} else if (X[1] == other.X[1]) {
				if (X[2] < other.X[2]) {
					return true;
				}
			}
		}
		return false;
	}

};
