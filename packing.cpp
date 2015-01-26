#include <cstdio>
#include <cstdlib>
#include <vector>

class Grid {
public:
  std::vector<int> cells;
  int w, h;

  Grid(int h, int w) {
    this->w = w;
    this->h = h;
    cells.resize(w * h);
  }

  inline int at(int r, int c) const {return cells[r * w + c];}
  inline void set(int r, int c, int v) {cells[r * w + c] = v;}

  static char getLabel(int g) {
    if (!g) return ' ';
    else if (g <= 26) return g + 'a' - 1;
    else if (g <= 52) return g - 27 + 'A';
    else if (g <= 62) return g - 53 + '0';
    else return '?';
  }

  void print() const {
    for (int r = 0; r < h; r++) {
      for (int c = 0; c < w; c++)
	printf("%c", getLabel(cells[r * w + c]));
      printf("\n");
    }
  }
};

void factor(std::vector<int> nj, int N, std::vector< std::vector<int> >& solns, int i = 0, std::vector<int> soln = std::vector<int>()) {
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

std::vector<int> bestFactorization(std::vector<int> nj, int N) {
  std::vector< std::vector<int> > solns;
  factor(nj, N, solns);

#if 0
  for (int i = 0; i < solns.size(); i++) {
    for (int j = 0; j < nj.size(); j++) {
      if (j > 0) printf(" + ");
      printf("%d*%d", solns[i][j], nj[j]);
    }
    printf("\n");
  }
#endif

  //TODO: Calculate variance, etc.
  
  return solns[solns.size() / 2];
}

int main(int argc, char* argv[]) {
  int m, n;
  printf("Enter m, n, with m >= n: ");
  scanf("%d %d", &m, &n);
  
  int w, h;
  printf("Enter width and height: ");
  scanf("%d %d", &w, &h);

  Grid grid(h, w);
  
  //Factorization
  std::vector<int> nj;
  for (int j = m; j >= n; j--)
    nj.push_back(j);
  std::vector<int> aj = bestFactorization(nj, w * h);

  for (int i = 0; i < nj.size(); i++) {
    if (i > 0) printf(" + ");
    printf("%d*%d", aj[i], nj[i]);
  }
  printf("\n");

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

  grid.print();
  
  return 0;
}