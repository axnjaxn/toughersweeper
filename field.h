#ifndef _BPJ_TOUGHERSWEEPER_FIELD_H
#define _BPJ_TOUGHERSWEEPER_FIELD_H

#include "packing.h"

union Cell {
  unsigned short bytes;
  struct {
    bool visible:1;
    bool bomb:1;
    bool flagged:1;
    bool computed:1;
    unsigned int mark:6;
    unsigned int numbers:6;
  };
};

class Field {
protected:
  int w, h, m, n, nmines, nminesleft, nflagsleft;
  Cell* cells;
  PackingGrid A, B;

  int computeMinesInNeighborhood(int r, int c) const;

public:
  Field(int w, int h, int m, int n, int nmines);
  Field(const Field& f);
  virtual ~Field();

  Field& operator=(const Field& f);

  void randomize();

  inline int flags() const {return nflagsleft;}
  inline bool complete() const {return (!nminesleft && !nflagsleft);}
  inline int width() const {return w;}
  inline int height() const {return h;}
  inline void setMark(int r, int c, unsigned int mark) {cells[r * w + c].mark = mark;}
  inline unsigned int getMark(int r, int c) const {return cells[r * w + c].mark;}
  void setVisible(int r, int c);
  inline bool isVisible(int r, int c) const {return cells[r * w + c].visible;}
  inline bool isFlagged(int r, int c) const {return cells[r * w + c].flagged;}
  inline bool isBomb(int r, int c) const {return cells[r * w + c].bomb;}
  inline int minesInNeighborhood(int r, int c) const {return cells[r * w + c].numbers;}//Only defined if visible!
  std::vector<int> getNeighbors(int r, int c) const;

  void traverseZero(int r, int c);//Expands connected 0-neighbor cells
  void toggleFlag(int r, int c);
};

#endif
