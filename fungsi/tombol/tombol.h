#ifndef FUNGSI_TOMBOL_H
#define FUNGSI_TOMBOL_H

#include <windows.h>

// ID tombol kalkulator.
enum TombolId {
    ID_BTN_C = 1001,
    ID_BTN_BACKSPACE,
    ID_BTN_BAGI,
    ID_BTN_KALI,
    ID_BTN_7,
    ID_BTN_8,
    ID_BTN_9,
    ID_BTN_KURANG,
    ID_BTN_4,
    ID_BTN_5,
    ID_BTN_6,
    ID_BTN_TAMBAH,
    ID_BTN_1,
    ID_BTN_2,
    ID_BTN_3,
    ID_BTN_SAMA_DENGAN,
    ID_BTN_0,
    ID_BTN_KOMA,
    ID_BTN_HISTORY
};

// Membuat semua tombol kalkulator.
void BuatTombolKalkulator(HWND parent);

// Mengatur posisi tombol saat ukuran window berubah.
void TataLetakTombol(HWND parent, const RECT& clientRect);

#endif
