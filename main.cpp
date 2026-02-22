#include <windows.h>
#include "fungsi/display/display.h"
#include "fungsi/history/history.h"
#include "fungsi/logika.h"
#include "fungsi/tombol/tombol.h"

// Menangani event utama window, termasuk proses gambar ulang.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            BuatTombolKalkulator(hwnd);
            return 0;
        case WM_SIZE:
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            TataLetakTombol(hwnd, clientRect);
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        case WM_COMMAND: {
            const int id = LOWORD(wParam);
            if (id == ID_BTN_HISTORY) {
                TampilkanHistoryTabel(hwnd);
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
            DisplayAngka(hdc, rect, AmbilDisplayText());

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

    WNDCLASSA wc = {};
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(
        0,
        CLASS_NAME,
        "Kalkulator",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        400,
        500,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hwnd == nullptr) {
        return 0;
    }

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
