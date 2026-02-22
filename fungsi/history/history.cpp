#include "history.h"

#include <cstddef>
#include <commctrl.h>
#include <string>
#include <vector>

namespace {

struct HistoryItem {
    std::string ekspresi;
    std::string hasil;
};

// Menyimpan daftar operasi yang sudah dilakukan.
std::vector<HistoryItem> gDaftarHistory;
// Menyimpan parent panel history.
HWND gParentWindow = nullptr;
// Menyimpan handle ListView history.
HWND gHistoryListView = nullptr;
// Menyimpan status tampil/sembunyi panel history.
bool gHistoryVisible = false;
// Menyimpan font panel history.
HFONT gHistoryFont = nullptr;

constexpr int kHistoryPanelWidth = 320;
constexpr int kPanelMargin = 12;

// Membuat font panel history agar konsisten dengan komponen lain.
HFONT BuatFontHistory(HWND parent) {
    HDC hdc = GetDC(parent);
    const int tinggiPx = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    ReleaseDC(parent, hdc);

    return CreateFontA(
        tinggiPx,
        0,
        0,
        0,
        FW_NORMAL,
        FALSE,
        FALSE,
        FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS,
        "Segoe UI"
    );
}

// Mengisi ulang konten ListView dari data history saat ini.
void RefreshHistoryList() {
    if (!gHistoryListView) {
        return;
    }

    SendMessageA(gHistoryListView, LVM_DELETEALLITEMS, 0, 0);

    for (std::size_t i = 0; i < gDaftarHistory.size(); ++i) {
        CHAR noBuffer[16] = {};
        wsprintfA(noBuffer, "%u", static_cast<unsigned>(i + 1));

        LVITEMA item = {};
        item.mask = LVIF_TEXT;
        item.iItem = static_cast<int>(i);
        item.iSubItem = 0;
        item.pszText = noBuffer;

        const int row = static_cast<int>(
            SendMessageA(gHistoryListView, LVM_INSERTITEMA, 0, reinterpret_cast<LPARAM>(&item))
        );

        if (row >= 0) {
            LVITEMA subItem = {};
            subItem.iSubItem = 1;
            subItem.pszText = const_cast<char*>(gDaftarHistory[i].ekspresi.c_str());
            SendMessageA(gHistoryListView, LVM_SETITEMTEXTA, static_cast<WPARAM>(row), reinterpret_cast<LPARAM>(&subItem));

            subItem.iSubItem = 2;
            subItem.pszText = const_cast<char*>(gDaftarHistory[i].hasil.c_str());
            SendMessageA(gHistoryListView, LVM_SETITEMTEXTA, static_cast<WPARAM>(row), reinterpret_cast<LPARAM>(&subItem));
        }
    }
}

// Membuat kolom ListView history.
void SetupHistoryColumns() {
    if (!gHistoryListView) {
        return;
    }

    LVCOLUMNA col = {};
    col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

    col.pszText = const_cast<char*>("No");
    col.cx = 50;
    col.iSubItem = 0;
    SendMessageA(gHistoryListView, LVM_INSERTCOLUMNA, 0, reinterpret_cast<LPARAM>(&col));

    col.pszText = const_cast<char*>("Ekspresi");
    col.cx = 150;
    col.iSubItem = 1;
    SendMessageA(gHistoryListView, LVM_INSERTCOLUMNA, 1, reinterpret_cast<LPARAM>(&col));

    col.pszText = const_cast<char*>("Hasil");
    col.cx = 100;
    col.iSubItem = 2;
    SendMessageA(gHistoryListView, LVM_INSERTCOLUMNA, 2, reinterpret_cast<LPARAM>(&col));
}

}  // namespace

// Inisialisasi panel history di dalam window utama.
void InisialisasiHistoryPanel(HWND parent) {
    gParentWindow = parent;
    if (!gHistoryFont) {
        gHistoryFont = BuatFontHistory(parent);
    }

    INITCOMMONCONTROLSEX icex = {};
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);

    gHistoryListView = CreateWindowExA(
        0,
        WC_LISTVIEWA,
        "",
        WS_CHILD | LVS_REPORT | LVS_SINGLESEL | WS_BORDER,
        0,
        0,
        0,
        0,
        parent,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );

    SendMessageA(gHistoryListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    SendMessageA(gHistoryListView, WM_SETFONT, reinterpret_cast<WPARAM>(gHistoryFont), TRUE);
    SendMessageA(gHistoryListView, LVM_SETBKCOLOR, 0, RGB(252, 252, 252));
    SendMessageA(gHistoryListView, LVM_SETTEXTBKCOLOR, 0, RGB(252, 252, 252));
    SendMessageA(gHistoryListView, LVM_SETTEXTCOLOR, 0, RGB(35, 35, 35));
    SetupHistoryColumns();
    RefreshHistoryList();
    ShowWindow(gHistoryListView, SW_HIDE);
}

// Menampilkan/menyembunyikan panel history.
void ToggleHistoryPanel() {
    gHistoryVisible = !gHistoryVisible;
    if (gHistoryListView) {
        ShowWindow(gHistoryListView, gHistoryVisible ? SW_SHOW : SW_HIDE);
    }
}

// Mengatur posisi panel history berdasarkan ukuran client.
void TataLetakHistoryPanel(const RECT& clientRect) {
    if (!gHistoryListView) {
        return;
    }

    const int clientWidth = clientRect.right - clientRect.left;
    const int clientHeight = clientRect.bottom - clientRect.top;

    const int x = clientWidth - kHistoryPanelWidth - kPanelMargin;
    const int y = kPanelMargin;
    const int w = kHistoryPanelWidth;
    const int h = clientHeight - (kPanelMargin * 2);

    MoveWindow(gHistoryListView, x, y, w, h > 0 ? h : 0, TRUE);
}

// Mengecek apakah panel history sedang tampil.
bool IsHistoryPanelVisible() {
    return gHistoryVisible;
}

// Mengambil lebar panel history.
int AmbilLebarHistoryPanel() {
    return kHistoryPanelWidth + kPanelMargin;
}

// Menambahkan satu baris riwayat operasi.
void TambahHistory(const std::string& ekspresi, const std::string& hasil) {
    gDaftarHistory.push_back({ ekspresi, hasil });
    RefreshHistoryList();
}

// Menghapus seluruh data riwayat.
void BersihkanHistory() {
    gDaftarHistory.clear();
    RefreshHistoryList();
}
