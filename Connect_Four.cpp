#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <limits>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;
using namespace std::chrono;

const int JUMLAH_BARIS = 6;
const int JUMLAH_KOLOM = 7;
const int TOTAL_SLOT = JUMLAH_BARIS * JUMLAH_KOLOM;
const int SYARAT_MENANG = 4;
const int OFFSET_MENANG = SYARAT_MENANG - 1;

const int ID_P1 = 1;
const int ID_P2 = 2;
const char SIMBOL_P1 = 'X';
const char SIMBOL_P2 = 'O';
const char SIMBOL_KOSONG = '.';

const int WAKTU_LEVEL_1 = 60;
const int WAKTU_LEVEL_2 = 30;
const int WAKTU_LEVEL_3 = 15;
const int DELAY_REFRESH_MS = 50;
const int DELAY_ERROR_DETIK = 1;

const int GAME_CMD_UNDO = 0;
const int GAME_CMD_INFO = 8;
const int GAME_CMD_NYERAH = 9;

const int GAME_STATUS_INFO = -1;
const int GAME_STATUS_UNDO = -2;
const int GAME_STATUS_NYERAH = -3;
const int GAME_STATUS_TIMEOUT = -4; 
const int GAME_STATUS_ERROR = -99;
const int BUFFER_SIZE = 10000;

const char* WARNA_RESET    = "\033[0m";
const char* WARNA_P1       = "\033[95m";
const char* WARNA_P2       = "\033[93m";
const char* WARNA_GRID     = "\033[96m";
const char* WARNA_MENANG   = "\033[92m";
const char* WARNA_SYS      = "\033[97m";
const char* WARNA_ABU      = "\033[90m";
const char* WARNA_ALERT    = "\033[91m";
const char* WARNA_BOLD     = "\033[1m";
const char* WARNA_REVERSE  = "\033[7m";

typedef char TipePapan;

void inisialisasiPapan(TipePapan papan[][JUMLAH_KOLOM]);
void gambarPapan(TipePapan papan[][JUMLAH_KOLOM], int lastBaris, int lastKolom);
void jedaEnter();
void bersihkanLayar();
void tampilkanAturanPermainan();
char mintaInputKonfirmasi(const char* pesan); 
int pilihLevelPermainan();

int jatuhkanKeping(TipePapan papan[][JUMLAH_KOLOM], int kolom, char keping);
bool cekDanTandaiKemenangan(TipePapan papan[][JUMLAH_KOLOM], char keping);
bool cekSeri(int jumlahGiliran);
int mintaInputKolomDenganTimer(char keping, int pAktif, TipePapan papan[][JUMLAH_KOLOM], int batasWaktuDetik);

int kbhit_cross();
char getch_cross();

int main() {
    TipePapan papan[JUMLAH_BARIS][JUMLAH_KOLOM];

    int pemainAktif;
    char kepingAktif;
    int jumlahGiliran;
    bool gameSelesai;
    char mainLagi;  
    
    int skorP1 = 0;
    int skorP2 = 0;
    
    int batasWaktu = 0;

    tampilkanAturanPermainan();

    do {
        bersihkanLayar();
        batasWaktu = pilihLevelPermainan();

        inisialisasiPapan(papan);
        pemainAktif = ID_P1;
        kepingAktif = SIMBOL_P1;
        jumlahGiliran = 0;
        gameSelesai = false;
        
        int lastBaris = -1;
        int lastKolom = -1;
        
        int riwayatLangkah[TOTAL_SLOT]; 
        int indeksRiwayat = 0;  
        
        do {
            bersihkanLayar();
            cout << WARNA_GRID << "=========================================================" << WARNA_RESET << endl;
            cout << WARNA_BOLD << WARNA_SYS << "                CONNECT FOUR (BY Fardan-Y)" << WARNA_RESET << endl;
            cout << WARNA_GRID << "=========================================================" << WARNA_RESET << endl;
            
            cout << "            " << WARNA_SYS << "STATUS SKOR: " << WARNA_P1 << "P1 (" << SIMBOL_P1 << ") = " << skorP1 << WARNA_RESET 
                 << WARNA_SYS << " | " << WARNA_P2 << "P2 (" << SIMBOL_P2 << ") = " << skorP2 << WARNA_RESET << endl;
            
            if (batasWaktu > 0) {
                 cout << "            " << WARNA_ALERT << "   MODE TIMER: " << batasWaktu << " DETIK / GILIRAN" << WARNA_RESET << endl;
            } else {
                 cout << "            " << WARNA_MENANG << "MODE SANTAI (TANPA TIMER)" << WARNA_RESET << endl;
            }

            cout << WARNA_ABU << "---------------------------------------------------------" << WARNA_RESET << endl;

            gambarPapan(papan, lastBaris, lastKolom);

            int inputPemain = mintaInputKolomDenganTimer(kepingAktif, pemainAktif, papan, batasWaktu); 

            if (inputPemain == GAME_STATUS_TIMEOUT) {
                cout << "\a"; 
                cout << "\n" << WARNA_ALERT << "!!! WAKTU HABIS !!! GILIRAN DILEWATKAN." << WARNA_RESET << endl;
                std::this_thread::sleep_for(std::chrono::seconds(DELAY_ERROR_DETIK));
                
                pemainAktif = (pemainAktif == ID_P1) ? ID_P2 : ID_P1;
                kepingAktif = (kepingAktif == SIMBOL_P1) ? SIMBOL_P2 : SIMBOL_P1;
                continue;
            }

            else if (inputPemain == GAME_STATUS_UNDO) {
                if (indeksRiwayat == 0) { 
                    cout << WARNA_ALERT << "[!] ERROR SISTEM: Tidak ada langkah untuk di-Undo!" << WARNA_RESET << endl;
                    jedaEnter(); 
                    continue; 
                }
                
                indeksRiwayat--; 
                int kolomTarget = riwayatLangkah[indeksRiwayat];

                for (int i = 0; i < JUMLAH_BARIS; i++) {
                    if (papan[i][kolomTarget] != SIMBOL_KOSONG) {
                        papan[i][kolomTarget] = SIMBOL_KOSONG; 
                        break; 
                    }
                }

                if (indeksRiwayat > 0) {
                    lastKolom = riwayatLangkah[indeksRiwayat - 1];
                    lastBaris = -1; 
                    for (int r = 0; r < JUMLAH_BARIS; r++) {
                        if (papan[r][lastKolom] != SIMBOL_KOSONG) {
                            lastBaris = r;
                            break;
                        }
                    }
                } else {
                    lastBaris = -1;
                    lastKolom = -1;
                }

                jumlahGiliran--;
                pemainAktif = (pemainAktif == ID_P1) ? ID_P2 : ID_P1;
                kepingAktif = (kepingAktif == SIMBOL_P1) ? SIMBOL_P2 : SIMBOL_P1;
                
                continue; 
            }

            else if (inputPemain == GAME_STATUS_INFO) {
                tampilkanAturanPermainan();
                continue;                        
            }
            else if (inputPemain == GAME_STATUS_NYERAH) {
                gameSelesai = true; 
                cout << "\n" << WARNA_ALERT << "!!! KONEKSI TERPUTUS: PEMAIN " << pemainAktif << " MENYERAH !!!" << WARNA_RESET << endl;
                if (pemainAktif == ID_P1) skorP2++; else skorP1++;
            }
            else {
                int kolom = inputPemain;
                int barisJatuh = jatuhkanKeping(papan, kolom, kepingAktif); 
                
                if (barisJatuh != -1) { 
                    riwayatLangkah[indeksRiwayat] = kolom;
                    indeksRiwayat++; 
                    jumlahGiliran++; 

                    lastBaris = barisJatuh;
                    lastKolom = kolom;

                    if (cekDanTandaiKemenangan(papan, kepingAktif)) {
                        gameSelesai = true;
                        if (pemainAktif == ID_P1) skorP1++; else skorP2++;
                        
                        bersihkanLayar();
                        cout << "\n" << WARNA_MENANG << "!!! DETEKSI KEMENANGAN: POSITIF !!!" << WARNA_RESET << endl;
                        
                        gambarPapan(papan, lastBaris, lastKolom); 
                        
                        const char* warnaPemenang = (pemainAktif == ID_P1) ? WARNA_P1 : WARNA_P2;
                        cout << "\n" << WARNA_SYS << "PEMAIN " << pemainAktif << warnaPemenang << " (" << kepingAktif << ") " << WARNA_RESET << WARNA_SYS << "MENANG!" << WARNA_RESET << endl;

                    } else if (cekSeri(jumlahGiliran)) {
                        gameSelesai = true;
                        bersihkanLayar();
                        gambarPapan(papan, lastBaris, lastKolom);
                        cout << "\n" << WARNA_ALERT << "!!! SISTEM DEADLOCK (SERI) !!!" << WARNA_RESET << endl;
                    } else {
                        pemainAktif = (pemainAktif == ID_P1) ? ID_P2 : ID_P1;
                        kepingAktif = (kepingAktif == SIMBOL_P1) ? SIMBOL_P2 : SIMBOL_P1;
                    }
                } else {
                    cout << WARNA_ALERT << "[!] ERROR: KOLOM " << (kolom + 1) << " PENUH (OVERFLOW) !!!" << WARNA_RESET << endl;
                    std::this_thread::sleep_for(std::chrono::seconds(DELAY_ERROR_DETIK));
                }
            }
        } while (!gameSelesai);

        jedaEnter(); 

        bersihkanLayar();
        mainLagi = mintaInputKonfirmasi("Reboot Sistem (Main Lagi)?");

    } while (mainLagi == 'Y' || mainLagi == 'y');
    
    bersihkanLayar();
    cout << WARNA_GRID << "=========================================================" << WARNA_RESET << endl;
    cout << WARNA_BOLD << WARNA_SYS << "                    MEMATIKAN SISTEM..." << WARNA_RESET << endl;
    cout << WARNA_GRID << "=========================================================" << WARNA_RESET << endl;
    cout << WARNA_SYS << "            Skor Akhir: " << WARNA_P1 << "P1: " << skorP1 << WARNA_RESET << WARNA_SYS << " - " << WARNA_P2 << "P2: " << skorP2 << WARNA_RESET << endl;
    
    return 0; 
}

void bersihkanLayar() {
#ifdef _WIN32
    system("cls"); 
#else
    cout << "\033c";
    cout << "\033[2J\033[1;1H";
#endif
}

void jedaEnter() {
    cout << WARNA_SYS << "\nTEKAN [ENTER] UNTUK LANJUT..." << WARNA_RESET;
    cin.get();
}

char mintaInputKonfirmasi(const char* pesan) {
    char input;
    while (true) {
        cout << WARNA_BOLD << WARNA_SYS << pesan << WARNA_GRID << " (Y/T): " << WARNA_RESET;
        cin >> input;
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (input == 'Y' || input == 'y' || input == 'T' || input == 't') {
            return input;
        } else {
            cout << WARNA_ALERT << "[!] Input Invalid. Ketik 'Y' atau 'T'." << WARNA_RESET << endl;
        }
    }
}

int pilihLevelPermainan() {
    int level;
    while (true) {
        cout << WARNA_GRID << "=========================================================" << WARNA_RESET << endl;
        cout << WARNA_BOLD << WARNA_SYS << "                 PILIH TINGKAT KESULITAN" << WARNA_RESET << endl;
        cout << WARNA_GRID << "=========================================================" << WARNA_RESET << endl;
        cout << WARNA_SYS << " [ 1 ] " << WARNA_MENANG << "MUDAH"  << WARNA_RESET << "  : Batas waktu " << WAKTU_LEVEL_1 << " Detik\n";
        cout << WARNA_SYS << " [ 2 ] " << WARNA_P2     << "NORMAL" << WARNA_RESET << " : Batas waktu " << WAKTU_LEVEL_2 << " Detik\n";
        cout << WARNA_SYS << " [ 3 ] " << WARNA_ALERT  << "SULIT"  << WARNA_RESET << "  : Batas waktu " << WAKTU_LEVEL_3 << " Detik\n";
        cout << WARNA_GRID << "=========================================================" << WARNA_RESET << endl;
        cout << WARNA_SYS << ">> PILIH LEVEL (1-3): " << WARNA_RESET;
        
        if (!(cin >> level)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << WARNA_ALERT << "[!] ERROR: Input harus berupa angka!" << WARNA_RESET << endl;
            jedaEnter();
            bersihkanLayar();
            continue;
        }
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (level == 1) return WAKTU_LEVEL_1;
        else if (level == 2) return WAKTU_LEVEL_2;
        else if (level == 3) return WAKTU_LEVEL_3;
        else {
            bersihkanLayar();
            cout << WARNA_ALERT << "[!] Level tidak valid. Pilih 1, 2, atau 3." << WARNA_RESET << endl;
        }
    }
}

void tampilkanAturanPermainan() {
    bersihkanLayar();
    cout << WARNA_GRID << "=========================================================" << WARNA_RESET << endl;
    cout << WARNA_BOLD << WARNA_SYS << "             PANDUAN SISTEM CONNECT FOUR" << WARNA_RESET << endl;
    cout << WARNA_GRID << "=========================================================" << WARNA_RESET << endl;
    
    cout << "\n" << WARNA_SYS << "[ IDENTITAS USER ]" << WARNA_RESET << endl;
    cout << "  " << WARNA_P1 << "PEMAIN 1" << WARNA_RESET << WARNA_SYS << " : Simbol '" << SIMBOL_P1 << "' (Neon Pink)\n" << WARNA_RESET;
    cout << "  " << WARNA_P2 << "PEMAIN 2" << WARNA_RESET << WARNA_SYS << " : Simbol '" << SIMBOL_P2 << "' (Neon Yellow)\n" << WARNA_RESET;

    cout << "\n" << WARNA_SYS << "[ PROTOKOL MISI ]" << WARNA_RESET << endl;
    cout << WARNA_SYS << "  Misi kamu adalah menjadi yang pertama menyusun\n";
    cout << "  " << WARNA_MENANG << SYARAT_MENANG << " KEPING SECARA BERURUTAN" << WARNA_SYS << " di papan.\n";
    
    cout << "\n" << WARNA_SYS << "[ ARAH KEMENANGAN ]" << WARNA_RESET << endl;
    cout << WARNA_SYS << "  Kamu bisa menang dengan menyusun " << SYARAT_MENANG << " keping ke arah:\n";
    cout << "  1. " << WARNA_GRID << "Horizontal (Mendatar)" << WARNA_SYS << " : X X X X\n";
    cout << "  2. " << WARNA_GRID << "Vertikal (Tegak)" << WARNA_SYS << "      : X disusun ke atas\n";
    cout << "  3. " << WARNA_GRID << "Diagonal (Miring)" << WARNA_SYS << "      : Miring kanan atau kiri\n" << WARNA_RESET;

    cout << "\n" << WARNA_SYS << "[ MEKANISME WAKTU & LEVEL ]" << WARNA_RESET << endl;
    cout << "  " << WARNA_MENANG << "LEVEL 1 (MUDAH)" << WARNA_SYS << "  : Waktu " << WAKTU_LEVEL_1 << " detik.\n";
    cout << "  " << WARNA_P2 << "LEVEL 2 (NORMAL)" << WARNA_SYS << " : Waktu " << WAKTU_LEVEL_2 << " detik.\n";
    cout << "  " << WARNA_ALERT << "LEVEL 3 (SULIT)" << WARNA_SYS << "  : Waktu " << WAKTU_LEVEL_3 << " detik.\n";

    cout << "\n" << WARNA_SYS << "[ KONTROL TERMINAL ]" << WARNA_RESET << endl;
    cout << WARNA_SYS << "  Selain angka 1-" << JUMLAH_KOLOM << ", tersedia perintah khusus:\n";
    cout << "  [ " << WARNA_P1 << GAME_CMD_UNDO << WARNA_SYS << " ] " << WARNA_BOLD << "UNDO" << WARNA_SYS << "       : Batalkan langkah (Mundur).\n";
    cout << "  [ " << WARNA_P1 << GAME_CMD_INFO << WARNA_SYS << " ] " << WARNA_BOLD << "INFO" << WARNA_SYS << "       : Buka panduan ini lagi.\n";
    cout << "  [ " << WARNA_P1 << GAME_CMD_NYERAH << WARNA_SYS << " ] " << WARNA_BOLD << "NYERAH" << WARNA_SYS << "     : Keluar dari permainan.\n" << WARNA_RESET;
    
    cout << "\n" << WARNA_GRID << "=========================================================" << WARNA_RESET << endl;
    cout << WARNA_BOLD << WARNA_SYS << "Inisialisasi Sistem? Tekan [Enter] untuk mulai!" << WARNA_RESET;
    cin.get(); 
}

void inisialisasiPapan(TipePapan papan[][JUMLAH_KOLOM]) {
    for (int i = 0; i < JUMLAH_BARIS; i++) {
        for (int j = 0; j < JUMLAH_KOLOM; j++) papan[i][j] = SIMBOL_KOSONG; 
    }
}

void gambarPapan(TipePapan papan[][JUMLAH_KOLOM], int lastBaris, int lastKolom) {
    cout << "\n"; 
    
    for (int j = 0; j < JUMLAH_KOLOM; j++) {
        if (papan[0][j] != SIMBOL_KOSONG) {
             cout << "    " << WARNA_ABU << (j + 1) << WARNA_RESET << "   ";
        } else {
             cout << "    " << WARNA_SYS << (j + 1) << WARNA_RESET << "   ";
        }
    }
    cout << "\n";

    cout << WARNA_GRID << "+" << WARNA_RESET;
    for (int j = 0; j < JUMLAH_KOLOM; j++) cout << WARNA_GRID << "-------+" << WARNA_RESET; 
    cout << "\n";

    for (int i = 0; i < JUMLAH_BARIS; i++) {
        cout << WARNA_GRID << "|" << WARNA_RESET; 
        for (int j = 0; j < JUMLAH_KOLOM; j++) {
            cout << "   "; 
            char sel = papan[i][j];
            
            bool isLastMove = (i == lastBaris && j == lastKolom);

            if (isLastMove) cout << WARNA_REVERSE; 

            if (sel == SIMBOL_P1) cout << WARNA_P1 << SIMBOL_P1 << WARNA_RESET;
            else if (sel == SIMBOL_P2) cout << WARNA_P2 << SIMBOL_P2 << WARNA_RESET;
            else if (sel == SIMBOL_KOSONG ) cout << WARNA_ABU << SIMBOL_KOSONG << WARNA_RESET;
            else cout << WARNA_MENANG << sel << WARNA_RESET; 
            
            if (isLastMove) cout << WARNA_RESET;

            cout << "   " << WARNA_GRID << "|" << WARNA_RESET; 
        }
        cout << "\n" << WARNA_GRID << "+" << WARNA_RESET;
        for (int j = 0; j < JUMLAH_KOLOM; j++) cout << WARNA_GRID << "-------+" << WARNA_RESET; 
        cout << "\n";
    }
}

int jatuhkanKeping(TipePapan papan[][JUMLAH_KOLOM], int kolom, char keping) {
    for (int i = JUMLAH_BARIS - 1; i >= 0; i--) {
        if (papan[i][kolom] == SIMBOL_KOSONG) {
            papan[i][kolom] = keping; return i; 
        }
    }
    return -1; 
}

bool cekSeri(int jumlahGiliran) {
    return jumlahGiliran == TOTAL_SLOT;
}

bool cekDanTandaiKemenangan(TipePapan papan[][JUMLAH_KOLOM], char keping) {
    for (int i = 0; i < JUMLAH_BARIS; i++) {
        for (int j = 0; j < JUMLAH_KOLOM; j++) {
            if (j + OFFSET_MENANG < JUMLAH_KOLOM &&
                papan[i][j] == keping && papan[i][j + 1] == keping &&
                papan[i][j + 2] == keping && papan[i][j + 3] == keping) {
                papan[i][j] = '-'; papan[i][j + 1] = '-';
                papan[i][j + 2] = '-'; papan[i][j + 3] = '-';
                return true; 
            }
            if (i + OFFSET_MENANG < JUMLAH_BARIS &&
                papan[i][j] == keping && papan[i + 1][j] == keping &&
                papan[i + 2][j] == keping && papan[i + 3][j] == keping) {
                papan[i][j] = '|'; papan[i + 1][j] = '|';
                papan[i + 2][j] = '|'; papan[i + 3][j] = '|';
                return true;
            }
            if (i + OFFSET_MENANG < JUMLAH_BARIS && j + OFFSET_MENANG < JUMLAH_KOLOM &&
                papan[i][j] == keping && papan[i + 1][j + 1] == keping &&
                papan[i + 2][j + 2] == keping && papan[i + 3][j + 3] == keping) {
                papan[i][j] = '\\'; papan[i + 1][j + 1] = '\\';
                papan[i + 2][j + 2] = '\\'; papan[i + 3][j + 3] = '\\';
                return true;
            }
            if (i - OFFSET_MENANG >= 0 && j + OFFSET_MENANG < JUMLAH_KOLOM &&
                papan[i][j] == keping && papan[i - 1][j + 1] == keping &&
                papan[i - 2][j + 2] == keping && papan[i - 3][j + 3] == keping) {
                papan[i][j] = '/'; papan[i - 1][j + 1] = '/';
                papan[i - 2][j + 2] = '/'; papan[i - 3][j + 3] = '/';
                return true;
            }
        }
    }
    return false;
}

#ifdef _WIN32
int kbhit_cross() { return _kbhit(); }
char getch_cross() { return _getch(); }
#else
int kbhit_cross() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}
char getch_cross() { return getchar(); }
#endif

int mintaInputKolomDenganTimer(char keping, int pAktif, TipePapan papan[][JUMLAH_KOLOM], int batasWaktuDetik) {
    const char* warnaPemain = (pAktif == ID_P1) ? WARNA_P1 : WARNA_P2;
    
    cout << "\n" << WARNA_SYS << "[AKSES SISTEM] " << warnaPemain << "Pemain " << pAktif << " (" << keping << ")" << WARNA_RESET << endl;
    cout << WARNA_SYS << ">> INPUT PERINTAH (1-" << JUMLAH_KOLOM << ") [" << GAME_CMD_UNDO << "=Undo, " << GAME_CMD_INFO << "=Info, " << GAME_CMD_NYERAH << "=Nyerah]: " << WARNA_RESET;

    auto start = high_resolution_clock::now();
    string inputBuffer = "";
    
    while (true) {
        if (batasWaktuDetik > 0) {
            auto now = high_resolution_clock::now();
            auto duration = duration_cast<seconds>(now - start);
            if (duration.count() >= batasWaktuDetik) {
                return GAME_STATUS_TIMEOUT;
            }
        }

        if (kbhit_cross()) {
            char ch = getch_cross();
            
            if (ch == '\r' || ch == '\n') { 
                if (inputBuffer.empty()) continue;
                
                cout << endl; 
                int kolom = 0;
                try {
                    kolom = stoi(inputBuffer);
                } catch (...) {
                     cout << WARNA_ALERT << "[!] ERROR: Input harus berupa angka!" << WARNA_RESET << endl;
                     inputBuffer = "";
                     cout << ">> INPUT ULANG: ";
                     continue;
                }

                if (kolom == GAME_CMD_UNDO) return GAME_STATUS_UNDO;
                if (kolom == GAME_CMD_INFO) return GAME_STATUS_INFO;
                if (kolom == GAME_CMD_NYERAH) {
                    char k = mintaInputKonfirmasi("Nyerah?");
                    if (k == 'Y' || k == 'y') return GAME_STATUS_NYERAH;
                    else { inputBuffer = ""; cout << "\n>> INPUT ULANG: "; continue; }
                }

                if (kolom >= 1 && kolom <= JUMLAH_KOLOM) {
                    if (papan[0][kolom - 1] != SIMBOL_KOSONG) {
                        cout << WARNA_ALERT << "[!] Kolom Penuh!" << WARNA_RESET << endl;
                        inputBuffer = "";
                        cout << ">> INPUT: ";
                        continue;
                    }
                    return kolom - 1;
                } else {
                    cout << WARNA_ALERT << "[!] Invalid (1-" << JUMLAH_KOLOM << ")" << WARNA_RESET << endl;
                    inputBuffer = "";
                    cout << ">> INPUT: ";
                }
            } 
            else if (ch == '\b' || ch == 127) { 
                if (!inputBuffer.empty()) {
                    inputBuffer.pop_back();
                    cout << "\b \b";
                }
            }
            else {
                inputBuffer += ch;
                cout << ch; 
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_REFRESH_MS));
    }
}