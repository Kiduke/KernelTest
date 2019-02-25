/* Headers from GCC */
#include <stddef.h>
#include <stdint.h>

/* Checks to make sure kernel is compiled correctly */
#if defined(__linux__)
  #error "This code must be compiled with a cross-compiler"
#elif !defined(__i386__)
  #error "This code must be compiled with an x86-elf compiler"
#endif

/* VGA textmode buffer */
volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;
const int VGA_COLS = 80;
const int VGA_ROWS = 25;

/* Terminal basics */
/* Text display starts in top left */
int term_col = 0;
int term_row = 0;
uint8_t term_color = 0x0F;

/* Initiate and clear terminal */
void term_init() {
  for(int col = 0; col < VGA_COLS; col++) {
    for(int row = 0; row < VGA_ROWS; row++) {
      const size_t index = (VGA_COLS * row) + col;
      vga_buffer[index] = ((uint16_t)term_color << 8) | ' ';
    }
  }
}

/* Place char c on screen */
void term_putc(char c) {
  switch(c) {
    /* \n returns col to 0 and increments row */
    case '\n': {
      term_col = 0;
      term_row++;
      break;
    }
    /* By default char c is displayed, and col is incremented */
    default: {
      const size_t index = (VGA_COLS * term_row) + term_col;
      vga_buffer[index] = ((uint16_t)term_color << 8) | c;
      term_col++;
      break;
    }
  }
  /* If line is longer than max, move to next line */
  if(term_col >= VGA_COLS) {
    term_col = 0;
    term_row++;
  }
  /* If rows are longer than max, return to top left */
  if(term_row >= VGA_ROWS) {
    term_col = 0;
    term_row = 0;
  }
}

/* Print string on terminal */
void term_print(const char* str) {
  for(size_t i = 0; str[i] != '\0'; i++) {
    term_putc(str[i]);
  }
}

/* Main */
void kernel_main() {
  /* Initialize terminal */
  term_init();

  /* Display some text */
  term_print("Hello world!\n");
  term_print("Welcome to Jon's kernel.\n");
}
