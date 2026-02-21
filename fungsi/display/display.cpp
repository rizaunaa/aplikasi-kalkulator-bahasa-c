#include "display.h"

// Menampilkan teks angka di tengah area yang diberikan.
void DisplayAngka(HDC hdc, const RECT& rect, const char* angka) {
    RECT area = rect;
    area.left += 40;
    area.top += 40;
    area.right -= 40;
    area.bottom = area.top + 90;

    HPEN borderPen = CreatePen(PS_SOLID, 2, RGB(60, 60, 60));
    HGDIOBJ oldPen = SelectObject(hdc, borderPen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));

    Rectangle(hdc, area.left, area.top, area.right, area.bottom);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(borderPen);

    SetBkMode(hdc, TRANSPARENT);
    DrawTextA(hdc, angka, -1, &area, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
