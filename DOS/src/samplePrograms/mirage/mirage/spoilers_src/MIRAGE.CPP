#include <stdio.h>
#include <dos.h>
#include <io.h>
#include <graphics.h>
#include <mem.h>
#include <conio.h>
#include <math.h>
#include <string.h>

const char *title = "CONFidence CTF 2016";
#define TITLE_LEN 19

void enter_13h() {
  asm {
    mov ax, 13h
    int 10h
  }
}

void enter_textmode() {
  asm {
    mov ax, 3
    int 10h
  }
}

void skip_line(FILE *f) {
  while(fgetc(f) != '\n');
}

void set_pal_color(int i, unsigned char r, unsigned char g, unsigned char b) {
  outportb(0x3c8, i);
  outportb(0x3c9, r);
  outportb(0x3c9, g);
  outportb(0x3c9, b);
}

void map_font() {
  outpw(0x3ce, 5);
  outpw(0x3ce, 0x406);
  outpw(0x3c4, 0x402);
  outpw(0x3c4, 0x604);  
}

void unmap_font() {
  outpw(0x3c4, 0x302);
  outpw(0x3c4, 0x204);    
  outpw(0x3ce, 0x1005);
  outpw(0x3ce, 0xe06);
}

int load_pal(const char *fname) {
  FILE *f = fopen(fname, "r");
  if (!f) {
    return 1;
  }

  skip_line(f);
  skip_line(f);
  skip_line(f);

  int i;
  for (i = 0; i < 256; i++) {
    int r, g, b;
    fscanf(f, "%i %i %i", &r, &g, &b);
    set_pal_color(i, r >> 2, g >> 2, b >> 2);
  }

  fclose(f);
  return 0;
}

unsigned char far* far_alloc(unsigned sz) {
  unsigned sz_seg = (sz + 15) >> 4;  // Will fail on sz around 64K limit.
  unsigned seg = 0;
  if (_dos_allocmem(sz_seg, &seg) != 0) {
    return 0;
  }

  return (unsigned char far*)(((unsigned long)seg) << 16);
}

void far_free(void far* p) {
  if (p) {
    _dos_freemem((unsigned long)p >> 16);
  }
}

unsigned char far* load_raw(const char *fname, unsigned w, unsigned h) {
  unsigned char far* fb = far_alloc(w * h);
  if (!fb) {
    return 0;
  }

  unsigned f = open(fname, 0);
  unsigned bytes_read = 0;
  _dos_read(f, fb, w * h, &bytes_read);
  close(f);

  return fb;
}

unsigned char far* checker;

unsigned char far* img;
unsigned char far* vga_fb;
unsigned char far* fb;
unsigned char far* sprites;
unsigned char far* font;
char passwd[32];

int snatch_font() {
  font = far_alloc(256 * 16);
  if (!font) {
    return 1;
  }
  map_font();

  unsigned char far *vga_font = (unsigned char far*)0xa0000000;
  int i;
  for (i = 0; i < 255; i++) {
    _fmemcpy(font + i * 16, vga_font + i * (16 + 16), 16);
  }
  
  unmap_font();
  return 0;
}

int init_draw() {
  snatch_font();
  enter_13h();

  img = load_raw("MIRAGE.RAW", 320, 200);
  if (!img) {
    return 1;
  }

  if (load_pal("MIRAGE.PAL") != 0) {
    return 1;
  }

  vga_fb = (unsigned char far*)0xa0000000;
  fb = far_alloc(320 * 200);
  sprites = far_alloc(320 * 200);

  // 205 - 255 are free-to-use palette entries for me.
  int i;
  for (i = 0; i < 32; i++) {
    set_pal_color(205 + i, 16 + i, 0, 0);
  }

  for (i = 0; i < 16; i++) {
    set_pal_color(237 + i, 0, 0, 15 + i * 3);
  }  

  set_pal_color(255, 0, 0, 0);
  return 0;
}

void free_draw() {
  far_free(img);
  far_free(font);  
  far_free(fb);
  far_free(sprites);
}

void draw_char(
    unsigned char far *frame,
    int ch, int x, int y, int color, int coloro=0) {
  ch &= 0xff;
  int i, j;
  for (j = 0; j < 16; j++, color += coloro) {
    unsigned char far* ptr = &frame[(j + y) * 320 + x];    

    int chmap = font[ch * 16 + j];
    for (i = 0; i < 8; i++, ptr++, chmap <<= 1) {
      if ((chmap & 0x80)) {
        *ptr = color;
      }
    }
  }
}

void draw_string(
    unsigned char far *frame,    
    const char *ch, int x, int y, int color, int coloro=0) {
  while (*ch) {
    draw_char(frame, *ch++, x, y, color, coloro);
    x += 9;
  }
}

void draw() {
  static double t = 0.0f;
  int i;
  for (i = 20; i < 180 - 18; i++) {
    int m_off = 0;
    unsigned off = i * 320;

    if (i < 110) {
      
      double ym = 1.0 - ((double)(i - 20) / 90.0);
      double nym = 1.0 - ym;
      double y = i / (1.0 + nym * 0.2);
      m_off = sin(t * 0.5 + y) * 5.0 * ym;
    }

    _fmemcpy(
        fb + 16 + off,
        img + 16 + (i + 20) * 320 + m_off,
        320 - 32);
  }

  // Draw "Mirage".
  for (i = 0; i < 16; i++) {
    double y = i / 16.0;
    double x = sin(t * 0.5 + y * 8) * 2;
    _fmemcpy(
        fb + 38 + (181 + i) * 320 + (int)x,
        sprites + 320 * i + 4,
        6 * 9 + 8);
  }

  //draw_string(fb, "__________________________", 44, 22 + 4, 224 + 8, 0);  

  _fmemcpy(fb + 320 * 163, sprites + 320 * 16, 16 * 320);

  draw_string(fb, passwd, 44, 163, 221, 1);
  draw_string(fb, passwd, 44+1, 163, 221, 1);  

  _fmemcpy(vga_fb, fb, 320 * 200);


  t += 0.1;
 
}

void draw_initial() {
  // Initial.
  _fmemset(fb, 0xff, 320 * 200);
  _fmemset(sprites, 0xff, 320 * 200);

  int i;
  for (i = 20 - 1; i < 180 + 1; i++) {
    double y = 1.0 - (double)(i - (20 - 1)) / (double)(180 + 1 - (20 - 1));
    unsigned c = 205 + y * 31.0;
    _fmemset(fb + i * 320 + 16 - 1, c, 320 - 32 + 2);
  }

  draw_string(fb, title, 80, 2, 237, 1);
  draw_string(fb, "       -=- RE300 by gynvael", 40, 181, 237, 1);


  // Sprites.

  // 0 x 0 - 70 x 15 "Mirage"
  draw_string(sprites, "Mirage", 8, 0, 237, 1);

  // 0 x 16 - 0 x 31 pass bg
  _fmemcpy(sprites + 320 * 16, fb + 320 * 163, 16 * 320);
}

#include "CHECKER.CPP"
void run_checker() {
  // Done.
  int i;
  for (i = 0; i < sizeof(checker_code); i++) {
    checker[i] ^= title[i % TITLE_LEN];
  }

  ((int (far*)(void*))checker)(passwd);

  for (i = 0; i < sizeof(checker_code); i++) {
    checker[i] ^= title[i % TITLE_LEN];
  }
}

void handle_ch(char ch) {
  if (ch == 0) {
    // Ignore extended codes.
    ch = getche();
    return;
  }

  int len = strlen(passwd);  

  if (ch == 8 /* BS */) {
    if (len == 0) {
      return;
    }

    passwd[len - 1] = 0;
    run_checker();
    return;
  }

  if (ch < 0x20 || ch > 0x7e) {
    return;
  }

  if (len >= 26) {
    return;
  }

  passwd[len] = ch;
  passwd[len + 1] = 0;
  run_checker();
}



int init_checker() {
  checker = far_alloc(0x1000);
  if (!checker) {
    return 1;
  }

  _fmemcpy(checker, (unsigned char far*)checker_code, sizeof(checker_code));

  return 0;
}


int main(void) {

  if (init_draw() == 0 && init_checker() == 0) {
    draw_initial();

    while (1) {
      while (kbhit() == 0) {    
        draw();
      }
      int ch = getche();

      if (ch == 27) {
        break;
      }

      handle_ch(ch);
    }
  }

  free_draw();
  enter_textmode();

  far_free(checker);
  
  return 0;
}

