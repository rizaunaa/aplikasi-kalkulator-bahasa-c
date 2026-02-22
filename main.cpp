#include <windows.h>
#include "fungsi/display/display.h"
#include "fungsi/history/history.h"
#include "fungsi/logika.h"
#include "fungsi/tombol/tombol.h"

// Memuat icon dari file proyek, fallback ke icon aplikasi default.
HICON MuatIconAplikasi(int size) {
    HICON icon = static_cast<HICON>(
        LoadImageA(nullptr, "icon.ico", IMAGE_ICON, size, size, LR_LOADFROMFILE)
    );
    if (!icon) {
        icon = LoadIcon(nullptr, IDI_APPLICATION);
    }
    return icon;
}

// Menghitung posisi tengah layar berdasarkan work area desktop.
POINT HitungPosisiTengahLayar(int width, int height) {
    RECT workArea = {};
    SystemParametersInfoA(SPI_GETWORKAREA, 0, &workArea, 0);

    POINT p = {};
    p.x = workArea.left + ((workArea.right - workArea.left) - width) / 2;
    p.y = workArea.top + ((workArea.bottom - workArea.top) - height) / 2;
    return p;
}

// Menghitung area kalkulator di sisi kiri saat history panel tampil.
RECT AmbilAreaKalkulator(const RECT& clientRect) {
    RECT area = clientRect;
    if (IsHistoryPanelVisible()) {
        area.right -= AmbilLebarHistoryPanel();
    }
    return area;
}

// Menangani event utama window, termasuk proses gambar ulang.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            BuatTombolKalkulator(hwnd);
            InisialisasiHistoryPanel(hwnd);
            return 0;
        case WM_SIZE: {
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            const RECT areaKalkulator = AmbilAreaKalkulator(clientRect);
            TataLetakTombol(hwnd, areaKalkulator);
            TataLetakHistoryPanel(clientRect);
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        }
        case WM_COMMAND: {
            const int id = LOWORD(wParam);
            if (id == ID_BTN_HISTORY) {
                const bool sebelumnyaVisible = IsHistoryPanelVisible();
                ToggleHistoryPanel();

                RECT windowRect;
                GetWindowRect(hwnd, &windowRect);
                const int tinggiWindow = windowRect.bottom - windowRect.top;
                int lebarWindowBaru = windowRect.right - windowRect.left;

                if (!sebelumnyaVisible && IsHistoryPanelVisible()) {
                    lebarWindowBaru += AmbilLebarHistoryPanel();
                } else if (sebelumnyaVisible && !IsHistoryPanelVisible()) {
                    lebarWindowBaru -= AmbilLebarHistoryPanel();
                }

                if (lebarWindowBaru < 400) {
                    lebarWindowBaru = 400;
                }

                SetWindowPos(hwnd, nullptr, windowRect.left, windowRect.top, lebarWindowBaru, tinggiWindow, SWP_NOZORDER);

                RECT clientRect;
                GetClientRect(hwnd, &clientRect);
                const RECT areaKalkulator = AmbilAreaKalkulator(clientRect);
                TataLetakTombol(hwnd, areaKalkulator);
                TataLetakHistoryPanel(clientRect);
                InvalidateRect(hwnd, nullptr, TRUE);
                return 0;
            }
            if (id >= ID_BTN_C && id <= ID_BTN_KOMA) {
                ProsesTombolKalkulator(id);
                InvalidateRect(hwnd, nullptr, TRUE);
            }
            return 0;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT rect;
            GetClientRect(hwnd, &rect);
            const RECT areaKalkulator = AmbilAreaKalkulator(rect);
            DisplayAngka(hdc, areaKalkulator, AmbilDisplayText());

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

// Membuat window aplikasi lalu menjalankan message loop.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    const char CLASS_NAME[] = "EmptyWindowClass";
    const int windowWidth = 400;
    const int windowHeight = 500;
    const POINT posisiAwal = HitungPosisiTengahLayar(windowWidth, windowHeight);
    HICON iconBesar = MuatIconAplikasi(32);
    HICON iconKecil = MuatIconAplikasi(16);

    WNDCLASSEXA wc = {};
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.hIcon = iconBesar;
    wc.hIconSm = iconKecil;

    RegisterClassExA(&wc);

    HWND hwnd = CreateWindowExA(
        0,
        CLASS_NAME,
        "Kalkulator",
        WS_OVERLAPPEDWINDOW,
        posisiAwal.x,
        posisiAwal.y,
        windowWidth,
        windowHeight,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hwnd == nullptr) {
        return 0;
    }

    SendMessage(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(iconBesar));
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(iconKecil));
    SetWindowTextA(hwnd, "Kalkulator");
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
