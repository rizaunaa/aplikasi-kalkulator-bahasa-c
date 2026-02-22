#ifndef FUNGSI_TOMBOL_KEYBOARD_H
#define FUNGSI_TOMBOL_KEYBOARD_H

#include <windows.h>

// Mengubah tombol keyboard non-karakter menjadi ID tombol kalkulator.
int AmbilIdTombolDariKeyDown(WPARAM wParam);

// Mengubah input karakter keyboard menjadi ID tombol kalkulator.
int AmbilIdTombolDariChar(WPARAM wParam);

#endif
