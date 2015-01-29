#ifndef _BPJ_TOUGHERSWEEPER_PACKING_H
#define _BPJ_TOUGHERSWEEPER_PACKING_H

#include <vector>

class PackingGrid {
public:
  std::vector<int> cells;
  int w, h;

  PackingGrid(int h, int w);

  inline int at(int r, int c) const {return cells[r * w + c];}
  inline void set(int r, int c, int v) {cells[r * w + c] = v;}


  static void factor(std::vector<int> nj, int N, std::vector< std::vector<int> >& solns, 
		     int i = 0, std::vector<int> soln = std::vector<int>());
  static std::vector<int> bestFactorization(std::vector<int> nj, int N);
};

#endif
