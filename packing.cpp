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
  const int nmutations = 40;

  PackingGrid grid = initial(h, w, m, n);

  //TODO: replace this with a method sensitive to the constraints
  for (int i = 0; i < nmutations; i++)
    grid.mutate();

  return grid;
}


typedef struct {
  int r, c, r1, c1;
} Pair;
Pair makePair(int r, int c, int r1, int c1) {
  Pair p;
  p.r = r; p.c = c;
  p.r1 = r; p.c1 = c;
  return p;
}

void PackingGrid::mutate() {
  int r, c, r1, c1, i, g, g1;

  //This process will repeat until success
  for (;;) {
    /* Determine two random adjacent cells  */
    i = rand() % (w * h);
    r = r1 = i / w; c = c1 = i % w;
    
    i = rand() % 4;
    switch (i) {
    case 0: r1--; break;
    case 1: r1++; break;
    case 2: c1--; break;
    case 3: c1++; break;
    }

    /* Enforce constraints: in bounds, and different groups */

    if (r1 < 0 || r1 >= h || c1 < 0 || c1 >= w || at(r, c) == at(r1, c1)) continue;
    
    g = at(r, c);
    g1 = at(r1, c1);

    /* Determine which cells from the respective groups are adjacent */

    std::vector<int> group, group1;
    std::vector<Pair> pairs;
    for (int r = 0; r < h; r++)
      for (int c = 0; c < w; c++)
	if (at(r, c) == g) {
	  group.push_back(r * w + c);
	  if (c - 1 >= 0 && at(r, c - 1) == g1) pairs.push_back(makePair(r, c, r, c - 1));
	  if (c + 1 < w && at(r, c + 1) == g1) pairs.push_back(makePair(r, c, r, c + 1));
	  if (r - 1 >= 0 && at(r - 1, c) == g1) pairs.push_back(makePair(r, c, r - 1, c));
	  if (r + 1 < h && at(r + 1, c) == g1) pairs.push_back(makePair(r, c, r + 1, c));
	}
	else if (at(r, c) == g1) group1.push_back(r * w + c);

#if 0
    /* See if size constraint is satisfied */

    if (group.size() == n || group1.size == m) continue;
#endif
    
    /* Permute the adjacency pairs */

    for (int i = 0, j; i < pairs.size(); i++) {
      j = i + (rand() % (pairs.size() - i));
      Pair t = pairs[i];
      pairs[i] = pairs[j];
      pairs[j] = t;
    }

    /* Determine which pairs are legal grants */

    std::vector<int> connected, unconnected;
    bool isConnected;
    for (int k = 0; k < pairs.size(); k++) {
      set(pairs[k].r, pairs[k].c, g1);//Set it to the other group
      
      unconnected = group;

      for (int i = 0; i < unconnected.size(); i++)
	if (unconnected[i] == pairs[k].r * w + pairs[k].c)
	  unconnected.erase(unconnected.begin() + i);

      connected.resize(1);
      connected[0] = unconnected[0];
      unconnected.erase(unconnected.begin());
      
      for (int i = 0, j, r, c, r1, c1; i < unconnected.size(); i++) {
	r = unconnected[i] / w;
	c = unconnected[i] % w;
	for (j = 0; j < connected.size(); j++) {
	  r1 = connected[j] / w;
	  c1 = connected[j] % w;
	  if ((r1 == r && c1 == c + 1)
	      || (r1 == r && c1 == c - 1)
	      || (r1 == r - 1 && c1 == c)
	      || (r1 == r + 1 && c1 == c)) break;
	}
	if (j < connected.size()) {
	  connected.push_back(unconnected[i]);
	  unconnected.erase(unconnected.begin() + i);
	  i = -1;
	}
      }
      isConnected = unconnected.empty();

      if (isConnected) break;
      set(pairs[k].r, pairs[k].c, g);
    }
    
    //TODO: Swap instead of grant

    if (isConnected) {
      return;
    }
    else {
      continue;
    }
  }
}
