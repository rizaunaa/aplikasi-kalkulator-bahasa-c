#include "logika.h"

#include <cctype>
#include <cstdlib>
#include <string>
#include <vector>

#include "history/history.h"
#include "tombol/tombol.h"

namespace {

// Menyimpan teks yang tampil di display.
std::string gDisplayText = "0";
// Menandai bahwa nilai saat ini adalah hasil perhitungan.
bool gBaruHasil = false;

// Mengecek apakah karakter adalah operator matematika.
bool IsOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// Mengubah angka bertitik desimal titik menjadi format koma untuk display.
std::string FormatHasil(double value) {
    std::string text = std::to_string(value);
    while (!text.empty() && text.back() == '0') {
        text.pop_back();
    }
    if (!text.empty() && text.back() == '.') {
        text.pop_back();
    }
    if (text.empty()) {
        text = "0";
    }
    for (char& c : text) {
        if (c == '.') {
            c = ',';
        }
    }
    return text;
}

// Mengubah token angka berkoma menjadi nilai double.
bool ParseTokenAngka(const std::string& token, double* out) {
    if (token.empty() || out == nullptr) {
        return false;
    }
    std::string normalized = token;
    for (char& c : normalized) {
        if (c == ',') {
            c = '.';
        }
    }
    char* endPtr = nullptr;
    const double value = std::strtod(normalized.c_str(), &endPtr);
    if (endPtr == normalized.c_str() || *endPtr != '\0') {
        return false;
    }
    *out = value;
    return true;
}

// Menghitung ekspresi sederhana dengan prioritas * dan /.
bool HitungEkspresi(const std::string& ekspresi, double* out) {
    if (ekspresi.empty() || out == nullptr) {
        return false;
    }

    std::vector<double> angka;
    std::vector<char> op;
    std::string token;

    for (size_t i = 0; i < ekspresi.size(); ++i) {
        const char c = ekspresi[i];
        const bool unaryMinus = c == '-' && (i == 0 || IsOperator(ekspresi[i - 1]));
        if (std::isdigit(static_cast<unsigned char>(c)) || c == ',' || unaryMinus) {
            token.push_back(c);
            continue;
        }
        if (!IsOperator(c)) {
            return false;
        }
        double value = 0.0;
        if (!ParseTokenAngka(token, &value)) {
            return false;
        }
        angka.push_back(value);
        op.push_back(c);
        token.clear();
    }

    double lastValue = 0.0;
    if (!ParseTokenAngka(token, &lastValue)) {
        return false;
    }
    angka.push_back(lastValue);

    std::vector<double> angkaPrioritas;
    std::vector<char> opPrioritas;
    angkaPrioritas.push_back(angka[0]);

    for (size_t i = 0; i < op.size(); ++i) {
        const char operasi = op[i];
        const double nilaiBerikut = angka[i + 1];
        if (operasi == '*' || operasi == '/') {
            double& lhs = angkaPrioritas.back();
            if (operasi == '/') {
                if (nilaiBerikut == 0.0) {
                    return false;
                }
                lhs /= nilaiBerikut;
            } else {
                lhs *= nilaiBerikut;
            }
            continue;
        }
        opPrioritas.push_back(operasi);
        angkaPrioritas.push_back(nilaiBerikut);
    }

    double hasil = angkaPrioritas[0];
    for (size_t i = 0; i < opPrioritas.size(); ++i) {
        if (opPrioritas[i] == '+') {
            hasil += angkaPrioritas[i + 1];
        } else {
            hasil -= angkaPrioritas[i + 1];
        }
    }

    *out = hasil;
    return true;
}

// Menambah digit angka ke teks display.
void TambahDigit(char digit) {
    if (gDisplayText == "0" || gBaruHasil) {
        gDisplayText.assign(1, digit);
        gBaruHasil = false;
        return;
    }
    gDisplayText.push_back(digit);
}

// Menambah operator, sekaligus mencegah operator ganda di akhir ekspresi.
void TambahOperator(char operasi) {
    if (gDisplayText.empty()) {
        return;
    }
    if (IsOperator(gDisplayText.back())) {
        gDisplayText.back() = operasi;
        return;
    }
    gDisplayText.push_back(operasi);
    gBaruHasil = false;
}

// Menambah koma desimal pada angka aktif.
void TambahKoma() {
    if (gBaruHasil) {
        gDisplayText = "0";
        gBaruHasil = false;
    }
    size_t pos = gDisplayText.find_last_of("+-*/");
    const size_t start = (pos == std::string::npos) ? 0 : (pos + 1);
    if (gDisplayText.substr(start).find(',') != std::string::npos) {
        return;
    }
    if (start == gDisplayText.size() || IsOperator(gDisplayText.back())) {
        gDisplayText += "0,";
        return;
    }
    gDisplayText.push_back(',');
}

// Menghapus satu karakter terakhir.
void HapusSatuKarakter() {
    if (gBaruHasil) {
        gDisplayText = "0";
        gBaruHasil = false;
        return;
    }
    if (!gDisplayText.empty()) {
        gDisplayText.pop_back();
    }
    if (gDisplayText.empty()) {
        gDisplayText = "0";
    }
}

// Mereset tampilan ke nilai awal.
void ResetDisplay() {
    gDisplayText = "0";
    gBaruHasil = false;
}

// Mengeksekusi perhitungan saat tombol sama dengan ditekan.
void HitungHasil() {
    if (gDisplayText.empty() || IsOperator(gDisplayText.back())) {
        return;
    }
    const std::string ekspresi = gDisplayText;
    double hasil = 0.0;
    if (!HitungEkspresi(gDisplayText, &hasil)) {
        gDisplayText = "Error";
        gBaruHasil = true;
        TambahHistory(ekspresi, "Error");
        return;
    }
    gDisplayText = FormatHasil(hasil);
    TambahHistory(ekspresi, gDisplayText);
    gBaruHasil = true;
}

}  // namespace

// Mengembalikan teks display saat ini.
const char* AmbilDisplayText() {
    return gDisplayText.c_str();
}

// Memproses aksi berdasarkan ID tombol kalkulator.
void ProsesTombolKalkulator(int idTombol) {
    switch (idTombol) {
        case ID_BTN_0: TambahDigit('0'); break;
        case ID_BTN_1: TambahDigit('1'); break;
        case ID_BTN_2: TambahDigit('2'); break;
        case ID_BTN_3: TambahDigit('3'); break;
        case ID_BTN_4: TambahDigit('4'); break;
        case ID_BTN_5: TambahDigit('5'); break;
        case ID_BTN_6: TambahDigit('6'); break;
        case ID_BTN_7: TambahDigit('7'); break;
        case ID_BTN_8: TambahDigit('8'); break;
        case ID_BTN_9: TambahDigit('9'); break;
        case ID_BTN_TAMBAH: TambahOperator('+'); break;
        case ID_BTN_KURANG: TambahOperator('-'); break;
        case ID_BTN_KALI: TambahOperator('*'); break;
        case ID_BTN_BAGI: TambahOperator('/'); break;
        case ID_BTN_BACKSPACE: HapusSatuKarakter(); break;
        case ID_BTN_C: ResetDisplay(); break;
        case ID_BTN_KOMA: TambahKoma(); break;
        case ID_BTN_SAMA_DENGAN: HitungHasil(); break;
        default: break;
    }
}
