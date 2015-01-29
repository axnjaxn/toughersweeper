#include "field.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

int Field::computeMinesInNeighborhood(int r, int c) const {
  if (cells[r * w + c].computed);
  else if (cells[r * w + c].bomb) {
    cells[r * w + c].computed = 1;
    cells[r * w + c].numbers = 0;
  }
  else {
    std::vector<int> neighborhood = getNeighbors(r, c);
    cells[r * w + c].numbers = 0;
    for (int i = 0; i < neighborhood.size(); i++)
      if (cells[neighborhood[i]].bomb)
	cells[r * w + c].numbers++;
    cells[r * w + c].computed = 1;
  }
  
  return cells[r * w + c].numbers;
}

Field::Field(int w, int h, int m, int n, int nmines) {
  this->w = w;
  this->h = h;
  this->m = m;
  this->n = n;
  if (nmines > w * h) nmines = w * h;
  this->nmines = nmines;
  cells = new Cell [w * h];
  //TODO: Packing grids
  randomize();
}

Field::Field(const Field& f) {
  cells = NULL;
  *this = f;
}

Field::~Field() {
  if (cells) delete [] cells;
}

Field& Field::operator=(const Field& f) {
  w = f.w; h = f.h;
  m = f.m; n = f.n;
  nmines = f.nmines;
  nminesleft = f.nminesleft;
  nflagsleft = f.nflagsleft;

  if (cells) delete [] cells;
  cells = new Cell [w * h];
  memcpy(cells, f.cells, w * h * sizeof(Cell));

  //TODO: Packing grids

  return *this;
}

//TODO below here

void Field::randomize() {
  memset(cells, 0, w * h * sizeof(Cell));  
  nminesleft = nflagsleft = nmines;
  //TODO: Generate packing grids

  //TODO
  //Generate a permutation of nmines indices
  //Set them all to be mines
  //It's possible to compute all mines in all neighborhoods, but probably should do that lazily
}

void Field::setVisible(int r, int c) {
  computeMinesInNeighborhood(r, c);
  cells[r * w + c].visible = 1;
  if (!cells[r * w + c].bomb && cells[r * w + c].numbers == 0)
    traverseZero(r, c);
}

std::vector<int> Field::getNeighbors(int r, int c) const {
  std::vector<int> neighborhood;

  //TODO: Use packing grid
  for (int r1 = r - 1; r1 <= r + 1; r1++)
    for (int c1 = c - 1; c1 <= c + 1; c1++) {
      if (r1 >= 0 && r1 < h && c1 >= 0 && c1 < w)
	neighborhood.push_back(r1 * w + c1);
    }
  //endel

  return neighborhood;
}

void Field::traverseZero(int r, int c) {
  std::vector<int> neighborhood = getNeighbors(r, c);
  for (int i = 0, r, c; i < neighborhood.size(); i++) {
    r = neighborhood[i] / w; c = neighborhood[i] % w;
    setVisible(r, c);
    if (cells[r * w + c].numbers == 0) traverseZero(r, c);
  }
}

void Field::toggleFlag(int r, int c) {
  if (cells[r * w + c].flagged) {
    cells[r * w + c].flagged = !cells[r * w + c].flagged;
    if (cells[r * w + c].bomb) nminesleft++;
    nflagsleft++;
  }
  else {
    if (cells[r * w + c].bomb) nminesleft--;
    nflagsleft--;
  }
}

