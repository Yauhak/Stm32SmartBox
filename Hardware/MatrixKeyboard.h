#ifndef MATRIX_KEY
#define MATRIX_KEY

extern const char *mode_prompt[];
extern short mode;
extern short capslock;

void initialize_gpio_about_key();
char scan_key();
#endif
