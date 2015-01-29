#include "packing.h"
#include <cstdio>
#include <cstdlib>

PackingGrid::PackingGrid(int h, int w) {
  this->w = w;
  this->h = h;
  cells.resize(w * h);
}

void PackingGrid::factor(std::vector<int> nj, int N, std::vector< std::vector<int> >& solns, 
			 int i, std::vector<int> soln) {
  if (i >= nj.size()) {
    if (N == 0)
      solns.push_back(soln);
    return;
  }

  for (int k = 0; k < N; k++) {
    if (nj[i] * k > N) return;
    soln.push_back(k);
    factor(nj, N - nj[i] * k, solns, i + 1, soln);
    soln.pop_back();    
  }
}

std::vector<int> PackingGrid::bestFactorization(std::vector<int> nj, int N) {
  std::vector< std::vector<int> > solns;
  factor(nj, N, solns);

  //TODO: Formula for best factorization should minimize variance of aj
  
  return solns[solns.size() / 2];
}


PackingGrid PackingGrid::initial(int h, int w, int m, int n) {
  PackingGrid grid(h, w);
  
  //Factorization
  std::vector<int> nj;
  for (int j = m; j >= n; j--)
    nj.push_back(j);
  std::vector<int> aj = bestFactorization(nj, w * h);
  
  //Placement
  for (int i = 0, group = 1, r = 0, c = 0; i < nj.size(); i++) {
    for (int j = 0; j < aj[i]; j++) {
      for (int k = 0; k < nj[i]; k++) {
	printf("Cursor: %d %d\n", r, c);
	grid.set(r, c, group);
	if (!(r & 1)) {
	  if (++c >= grid.w) {
	    c--;
	    r++;
	  }
	}
	else {
	  if (--c < 0) {
	    c++;
	    r++;
	    }
	}
      }
      group++;
    }
  }

  return grid;
}

PackingGrid PackingGrid::random(int h, int w, int m, int n) {
  PackingGrid grid = initial(h, w, m, n);

  //TODO: replace this with a method sensitive to the constraints
  for (int i = 0; i < 100; i++)
    grid.mutate();

  return grid;
}

void PackingGrid::mutate() {
  //TODO
}
