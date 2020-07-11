/*  
 <<<<<<< HEAD
 Copyright (c) 2016,2017 Dominic C. Marcello
 =======
 Copyright (c) 2016 Dominic C. Marcello
 >>>>>>> d5ace976906503f6304eb1617e48ca83ccb27431

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <tigerfmm/defs.hpp>
#include <tigerfmm/multipole.hpp>
#include <tigerfmm/expansion.hpp>
#include <tigerfmm/simd.hpp>

#include <hpx/hpx_init.hpp>

#include <cassert>
#include <memory>
#include <unistd.h>
#include <cmath>
#include <list>
#include <vector>

#include <fenv.h>



int main(int argc, char *argv[]) {

	std::vector<std::string> cfg = { "hpx.commandline.allow_unknown=1" };

	hpx::init(argc, argv, cfg);
}




#include <tigerfmm/tree.hpp>


int hpx_main(int argc, char *argv[]) {


	feenableexcept(FE_DIVBYZERO);
	feenableexcept(FE_INVALID);
	feenableexcept(FE_OVERFLOW);


	bool found_analytic;
	printf("%16s %16s %16s %16s %16s %16s %16s %16s %16s\n", "theta", "nparts",
			"solve_time", "err", "err99", "terr", "terr99", "gsum", "tsum");
	tree root;
//	srand(time(NULL));
	root.initialize();
	root.output("output.txt");
	if( root.load_analytic() ) {
		printf( "Found analytic.bin\n");
		found_analytic = true;
	} else {
		printf( "Did not find analytic.bin, computing\n");
		found_analytic = false;
	}
	for (real theta = one; theta > 0.15; theta -= 0.1) {
		root.set_theta(theta);
		space_vector<real> g_err;
		for (integer d = 0; d != NDIM; ++d) {
			g_err[d] = zero;
		}
		real solve_time = root.solve();
		//	srand(1);
		const real this_err = root.compute_error(root, false);
		real err99 = root.get_top10();
		const real this_err_torque = root.compute_error(root, true);
		real err99_torque = root.get_top10();
		auto g = root.force_sum(false);
		auto gnorm = root.force_sum(true);
		auto t = root.torque_sum(false);
		auto tnorm = root.torque_sum(true);
		real gsum = zero;
		real tsum = zero;
		real gsum_norm = zero;
		real tsum_norm = zero;
		for (integer d = 0; d != NDIM; ++d) {
			gsum += g[d] * g[d];
			gsum_norm += gnorm[d] * gnorm[d];
			tsum += t[d] * t[d];
			tsum_norm += tnorm[d] * tnorm[d];
		}
		gsum /= gsum_norm;
		tsum /= tsum_norm;
		gsum = std::sqrt(gsum);
		tsum = std::sqrt(tsum);
		for (integer i = 0; i != NDIM; ++i) {
			g[i] /= std::sqrt(gsum_norm);
			t[i] /= std::sqrt(tsum_norm);
		}
		//	printf("Force  sum = %e \n", gsum);
		//	printf("Torque sum = %e \n", tsum);
		FILE* fp = fopen("data.dat", "at");
		printf("%16e %16lli %16e %16e %16e %16e %16e %16e %16e\n", theta,
				nparts, solve_time, this_err, err99, this_err_torque,
				err99_torque, gsum, tsum);
		fprintf(fp, "%e %lli %e %e %e %e %e %e %e\n", theta, nparts, solve_time,
				this_err, err99, this_err_torque, err99_torque, gsum, tsum);
		fclose(fp);
	}
	if( !found_analytic ) {
		root.save_analytic();
	}
	return hpx::finalize();
}

