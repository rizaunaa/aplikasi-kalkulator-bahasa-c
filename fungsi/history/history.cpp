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
// Menyimpan handle jendela history agar bisa di-reuse.
HWND gHistoryWindow = nullptr;
// Menyimpan handle ListView di dalam jendela history.
HWND gHistoryListView = nullptr;

// Mengisi ulang konten tabel ListView dari data history saat ini.
void RefreshHistoryList() {
    if (!gHistoryListView) {
        return;
    }

    ListView_DeleteAllItems(gHistoryListView);

    for (std::size_t i = 0; i < gDaftarHistory.size(); ++i) {
        CHAR noBuffer[16] = {};
        wsprintfA(noBuffer, "%u", static_cast<unsigned>(i + 1));

        LVITEMA item = {};
        item.mask = LVIF_TEXT;
        item.iItem = static_cast<int>(i);
        item.iSubItem = 0;
        item.pszText = static_cast<LPSTR>(noBuffer);
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

// Menambahkan kolom ListView untuk tabel history.
void SetupHistoryColumns() {
    LVCOLUMNA col = {};
    col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

    col.pszText = const_cast<char*>("No");
    col.cx = 50;
    col.iSubItem = 0;
    ListView_InsertColumn(gHistoryListView, 0, &col);

    col.pszText = const_cast<char*>("Ekspresi");
    col.cx = 170;
    col.iSubItem = 1;
    ListView_InsertColumn(gHistoryListView, 1, &col);

    col.pszText = const_cast<char*>("Hasil");
    col.cx = 130;
    col.iSubItem = 2;
    ListView_InsertColumn(gHistoryListView, 2, &col);
}

// Menangani message jendela history.
LRESULT CALLBACK HistoryWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            gHistoryListView = CreateWindowExA(
                0,
                WC_LISTVIEWA,
                "",
                WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | WS_BORDER,
                10,
                10,
                460,
                300,
                hwnd,
                nullptr,
                GetModuleHandle(nullptr),
                nullptr
            );

            ListView_SetExtendedListViewStyle(gHistoryListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
            SetupHistoryColumns();
            RefreshHistoryList();
            return 0;
        }
        case WM_SIZE: {
            if (gHistoryListView) {
                const int width = LOWORD(lParam);
                const int height = HIWORD(lParam);
                MoveWindow(gHistoryListView, 10, 10, width - 20, height - 20, TRUE);
            }
            return 0;
        }
        case WM_DESTROY:
            gHistoryListView = nullptr;
            gHistoryWindow = nullptr;
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

}  // namespace

// Menambahkan satu baris riwayat operasi.
void TambahHistory(const std::string& ekspresi, const std::string& hasil) {
    gDaftarHistory.push_back({ ekspresi, hasil });
    RefreshHistoryList();
}

// Menampilkan history dalam bentuk tabel GUI.
void TampilkanHistoryTabel(HWND parent) {
    if (gHistoryWindow && IsWindow(gHistoryWindow)) {
        ShowWindow(gHistoryWindow, SW_SHOW);
        SetForegroundWindow(gHistoryWindow);
        RefreshHistoryList();
        return;
    }

    INITCOMMONCONTROLSEX icex = {};
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);

    const char* className = "HistoryWindowClass";
    WNDCLASSA wc = {};
    wc.lpfnWndProc = HistoryWindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = className;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    RegisterClassA(&wc);

    gHistoryWindow = CreateWindowExA(
        WS_EX_DLGMODALFRAME,
        className,
        "History Operasi",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        520,
        380,
        parent,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );
}

// Menghapus seluruh data riwayat.
void BersihkanHistory() {
    gDaftarHistory.clear();
    RefreshHistoryList();
}
