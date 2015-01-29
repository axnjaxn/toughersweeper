#include "field.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

int Field::computeMinesInNeighborhood(int r, int c) const {
  //TODO
  //If it has already been computed, return the number
  //If it is a bomb, set computed and return 0
  //Get the neighborhood
  //Sum the number of mines in the neighborhood
  //Set computed and return the result
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

void Field::traverseZero(int r, int c) {
  //TODO
  //Should use getNeighbors
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
