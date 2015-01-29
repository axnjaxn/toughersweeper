#include <byteimage/byteimage_sdl2.h>
#include <byteimage/render.h>
#include <byteimage/font.h>
#include <ctime>
#include "field.h"

#define TITLE "TougherSweeper by Brian Jackson"
#define SZ 1
#define CELLSIZE 30

//TODO: Win graphic

void displayField(ByteImage& target, const Field& field, TextRenderer* font, int mr, int mc) {
  int x, y, w, h;
  char label[8];
  Matrix bgcolor, fgcolor;

  memset(target.pixels, 0, target.size());
  for (int r = 0; r < field.height(); r++)
    for (int c = 0; c < field.width(); c++) {
      memset(label, 0, 8);
      x = c * CELLSIZE + 1;
      y = r * CELLSIZE + 1;
      w = h = CELLSIZE - 2;

      bgcolor = makeColor(255, 255, 255);
      fgcolor = makeColor(0, 0, 0);

      if (r <= mr + SZ && r >= mr - SZ && c <= mc + SZ && c >= mc - SZ) {
	if (field.isVisible(r, c)) bgcolor = makeColor(255, 255, 192);
	else bgcolor = makeColor(255, 255, 128);
      }

      if (field.isFlagged(r, c))
	bgcolor = makeColor(0, 0, 255);
      else if (field.isVisible(r, c)) {
	if (field.isBomb(r, c))
	  bgcolor = makeColor(255, 0, 0);
	else {
	  sprintf(label, "%d", field.minesInNeighborhood(r, c));
	  fgcolor = makeColor(0, 0, 0);
	  font->drawCentered(target, label, y + h / 2, x + w / 2, 0, 0, 0);
	}
      }      
      else if (field.getMark(r, c)) {
	sprintf(label, "%d", field.getMark(r,c));
	fgcolor = makeColor(0, 0, 255);

      }
      
      DrawRect(target, x, y, w, h, 
	       ByteImage::clip(bgcolor.at(0)), ByteImage::clip(bgcolor.at(1)), ByteImage::clip(bgcolor.at(2)));
      if (label) 
	font->drawCentered(target, label, y + h / 2, x + w / 2, 
			   ByteImage::clip(fgcolor.at(0)), ByteImage::clip(fgcolor.at(1)), ByteImage::clip(fgcolor.at(2)));
    }
}

void readConfig(int* nr, int* nc, int* nm) {
#if 0
  char fn[200];
  sprintf(fn, "%s/.toughersweeper/config.txt", getenv("HOME"));
  FILE* fp = fopen(fn, "r");
  if (!fp) {
#endif
    *nr = 10;
    *nc = 10;
    *nm = 15;
#if 0
    fp = fopen(fn, "w");
    if (fp) {
      fprintf(fp, "%d %d %d", *nr, *nc, *nm);
      fclose(fp);
    }
  }
  else {
    fscanf(fp, "%d %d %d", nr, nc, nm);
    fclose(fp);
  }
#endif
}

class SweeperUI : public ByteImageDisplay {
protected:
  Field* field;

  ByteImage canvas;
  TextRenderer* font;

  int cursor_r, cursor_c;
  bool dead;

  void handleEvent(SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
      if (event.key.keysym.sym == SDLK_ESCAPE) exit(0);
      else if (event.key.keysym.sym == SDLK_F2) {
	srand(time(NULL));
	field->randomize();
	dead = 0;
	render();
      }
      else if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9) {
	field->setMark(cursor_r, cursor_c, event.key.keysym.sym - SDLK_0);
	render();
      }
    }
    else if (event.type == SDL_MOUSEMOTION) {
      cursor_r = event.motion.y / CELLSIZE;
      cursor_c = event.motion.x / CELLSIZE;
      render();
    }
    else if (!dead && event.type == SDL_MOUSEBUTTONDOWN) {
      if (event.button.button == SDL_BUTTON_RIGHT) {
	if (!field->isVisible(cursor_r, cursor_c)) field->toggleFlag(cursor_r, cursor_c);
      }
      else if (event.button.button == SDL_BUTTON_LEFT) {
	field->setVisible(cursor_r, cursor_c);
	if (field->isBomb(cursor_r, cursor_c)) dead = 1;
      }
      render();
    }
    
    ByteImageDisplay::handleEvent(event);
  }
  
  void render() {
    displayField(canvas, *field, font, cursor_r, cursor_c);
    updateImage(canvas);
  }

public:
  SweeperUI(const Field& field) : ByteImageDisplay(field.height() * CELLSIZE, field.width() * CELLSIZE, TITLE) {
    frameDelay = 100;

    this->field = new Field(field);

    canvas = ByteImage(field.height() * CELLSIZE, field.width() * CELLSIZE, 3);
    font = new TextRenderer("res/FreeSans.ttf", 14);

    cursor_r = cursor_c = ~0;
    dead = 0;

    render();
  }
  ~SweeperUI() {
    delete field;
  }
};

int main(int argc, char* argv[]) {
  int nr, nc, nm;
  readConfig(&nr, &nc, &nm);

  srand(time(NULL));

  Field field(nr, nc, 4, 5, nm);
  SweeperUI(field).main();

  return 0;
}
