#include "keyboard.h"

#include "tombol.h"

// Mengubah tombol keyboard non-karakter menjadi ID tombol kalkulator.
int AmbilIdTombolDariKeyDown(WPARAM wParam) {
    switch (wParam) {
        case VK_NUMPAD0: return ID_BTN_0;
        case VK_NUMPAD1: return ID_BTN_1;
        case VK_NUMPAD2: return ID_BTN_2;
        case VK_NUMPAD3: return ID_BTN_3;
        case VK_NUMPAD4: return ID_BTN_4;
        case VK_NUMPAD5: return ID_BTN_5;
        case VK_NUMPAD6: return ID_BTN_6;
        case VK_NUMPAD7: return ID_BTN_7;
        case VK_NUMPAD8: return ID_BTN_8;
        case VK_NUMPAD9: return ID_BTN_9;
        case VK_DECIMAL: return ID_BTN_KOMA;
        case VK_ADD: return ID_BTN_TAMBAH;
        case VK_SUBTRACT: return ID_BTN_KURANG;
        case VK_MULTIPLY: return ID_BTN_KALI;
        case VK_DIVIDE: return ID_BTN_BAGI;
        case VK_RETURN: return ID_BTN_SAMA_DENGAN;
        case VK_BACK: return ID_BTN_BACKSPACE;
        case VK_ESCAPE: return ID_BTN_C;
        default: return 0;
    }
}

// Mengubah input karakter keyboard menjadi ID tombol kalkulator.
int AmbilIdTombolDariChar(WPARAM wParam) {
    const char c = static_cast<char>(wParam);
    switch (c) {
        case '0': return ID_BTN_0;
        case '1': return ID_BTN_1;
        case '2': return ID_BTN_2;
        case '3': return ID_BTN_3;
        case '4': return ID_BTN_4;
        case '5': return ID_BTN_5;
        case '6': return ID_BTN_6;
        case '7': return ID_BTN_7;
        case '8': return ID_BTN_8;
        case '9': return ID_BTN_9;
        case '+': return ID_BTN_TAMBAH;
        case '-': return ID_BTN_KURANG;
        case '*': return ID_BTN_KALI;
        case '/': return ID_BTN_BAGI;
        case ',':
        case '.': return ID_BTN_KOMA;
        case '=': return ID_BTN_SAMA_DENGAN;
        case 'c':
        case 'C': return ID_BTN_C;
        default: return 0;
    }
}
