#ifndef FUNGSI_HISTORY_H
#define FUNGSI_HISTORY_H

#include <windows.h>
#include <string>

// Menambahkan satu baris riwayat operasi.
void TambahHistory(const std::string& ekspresi, const std::string& hasil);

// Menampilkan history dalam bentuk tabel GUI.
void TampilkanHistoryTabel(HWND parent);

// Menghapus seluruh data riwayat.
void BersihkanHistory();

#endif
