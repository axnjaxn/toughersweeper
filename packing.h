#ifndef _BPJ_TOUGHERSWEEPER_PACKING_H
#define _BPJ_TOUGHERSWEEPER_PACKING_H

#include <vector>

//TODO: Methods for generating a pair of grids

class PackingGrid {
protected:
  std::vector<int> cells;
  int w, h;

  static void factor(std::vector<int> nj, int N, std::vector< std::vector<int> >& solns, 
		     int i = 0, std::vector<int> soln = std::vector<int>());
  static std::vector<int> bestFactorization(std::vector<int> nj, int N);

public:
  PackingGrid() {w = h = 0;}
  PackingGrid(int h, int w);

  static PackingGrid initial(int h, int w, int m, int n);//m >= n
  static PackingGrid random(int h, int w, int m, int n);//m >= n
  
  inline int at(int r, int c) const {return cells[r * w + c];}
  inline void set(int r, int c, int v) {cells[r * w + c] = v;}

  void mutate();
};

#endif
