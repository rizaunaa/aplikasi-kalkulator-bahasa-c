#include "tombol.h"

namespace {

struct TombolDef {
    int id;
    const char* teks;
    int row;
    int col;
    int rowSpan;
    int colSpan;
    HWND hwnd;
};

constexpr int kTotalCols = 4;
constexpr int kTotalRows = 5;
constexpr int kMargin = 40;
constexpr int kGap = 10;
constexpr int kDisplayTop = 40;
constexpr int kDisplayHeight = 90;
constexpr int kHistoryWidth = 90;
constexpr int kHistoryHeight = 36;

TombolDef gTombol[] = {
    { ID_BTN_C, "C", 0, 0, 1, 1, nullptr },
    { ID_BTN_BACKSPACE, "<-", 0, 1, 1, 1, nullptr },
    { ID_BTN_BAGI, "/", 0, 2, 1, 1, nullptr },
    { ID_BTN_KALI, "x", 0, 3, 1, 1, nullptr },
    { ID_BTN_7, "7", 1, 0, 1, 1, nullptr },
    { ID_BTN_8, "8", 1, 1, 1, 1, nullptr },
    { ID_BTN_9, "9", 1, 2, 1, 1, nullptr },
    { ID_BTN_KURANG, "-", 1, 3, 1, 1, nullptr },
    { ID_BTN_4, "4", 2, 0, 1, 1, nullptr },
    { ID_BTN_5, "5", 2, 1, 1, 1, nullptr },
    { ID_BTN_6, "6", 2, 2, 1, 1, nullptr },
    { ID_BTN_TAMBAH, "+", 2, 3, 1, 1, nullptr },
    { ID_BTN_1, "1", 3, 0, 1, 1, nullptr },
    { ID_BTN_2, "2", 3, 1, 1, 1, nullptr },
    { ID_BTN_3, "3", 3, 2, 1, 1, nullptr },
    { ID_BTN_SAMA_DENGAN, "=", 3, 3, 2, 1, nullptr },
    { ID_BTN_0, "0", 4, 0, 1, 2, nullptr },
    { ID_BTN_KOMA, ",", 4, 2, 1, 1, nullptr }
};

HWND gHistoryButton = nullptr;

}  // namespace

// Membuat semua tombol kalkulator sebagai child window.
void BuatTombolKalkulator(HWND parent) {
    for (auto& tombol : gTombol) {
        tombol.hwnd = CreateWindowExA(
            0,
            "BUTTON",
            tombol.teks,
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            0,
            0,
            0,
            0,
            parent,
            reinterpret_cast<HMENU>(static_cast<INT_PTR>(tombol.id)),
            GetModuleHandle(nullptr),
            nullptr
        );
    }

    gHistoryButton = CreateWindowExA(
        0,
        "BUTTON",
        "History",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        0,
        0,
        0,
        0,
        parent,
        reinterpret_cast<HMENU>(static_cast<INT_PTR>(ID_BTN_HISTORY)),
        GetModuleHandle(nullptr),
        nullptr
    );
}

// Menghitung dan menerapkan posisi tombol dalam grid.
void TataLetakTombol(HWND parent, const RECT& clientRect) {
    const int clientWidth = clientRect.right - clientRect.left;
    const int clientHeight = clientRect.bottom - clientRect.top;
    const int gridTop = kDisplayTop + kDisplayHeight + 30;
    const int usableWidth = clientWidth - (kMargin * 2);
    const int usableHeight = clientHeight - gridTop - kMargin;

    if (usableWidth <= 0 || usableHeight <= 0) {
        return;
    }

    const int cellWidth = (usableWidth - (kGap * (kTotalCols - 1))) / kTotalCols;
    const int cellHeight = (usableHeight - (kGap * (kTotalRows - 1))) / kTotalRows;

    for (auto& tombol : gTombol) {
        if (!tombol.hwnd) {
            continue;
        }

        const int x = kMargin + tombol.col * (cellWidth + kGap);
        const int y = gridTop + tombol.row * (cellHeight + kGap);
        const int w = tombol.colSpan * cellWidth + (tombol.colSpan - 1) * kGap;
        const int h = tombol.rowSpan * cellHeight + (tombol.rowSpan - 1) * kGap;

        MoveWindow(tombol.hwnd, x, y, w, h, TRUE);
    }

    if (gHistoryButton) {
        const int historyX = clientWidth - kMargin - kHistoryWidth;
        const int historyY = kDisplayTop + (kDisplayHeight - kHistoryHeight) / 2;
        MoveWindow(gHistoryButton, historyX, historyY, kHistoryWidth, kHistoryHeight, TRUE);
    }

    InvalidateRect(parent, nullptr, TRUE);
}
