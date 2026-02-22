#ifndef FUNGSI_HISTORY_H
#define FUNGSI_HISTORY_H

#include <windows.h>
#include <string>

// Inisialisasi panel history di dalam window utama.
void InisialisasiHistoryPanel(HWND parent);

// Menampilkan/menyembunyikan panel history.
void ToggleHistoryPanel();

// Mengatur posisi panel history berdasarkan ukuran client.
void TataLetakHistoryPanel(const RECT& clientRect);

// Mengecek apakah panel history sedang tampil.
bool IsHistoryPanelVisible();

// Mengambil lebar panel history.
int AmbilLebarHistoryPanel();

// Menambahkan satu baris riwayat operasi.
void TambahHistory(const std::string& ekspresi, const std::string& hasil);

// Menghapus seluruh data riwayat.
void BersihkanHistory();

#endif
