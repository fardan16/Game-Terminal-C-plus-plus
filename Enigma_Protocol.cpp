#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const char* RESET    = "\033[0m";
const char* MERAH    = "\033[91m";
const char* HIJAU    = "\033[92m";
const char* KUNING   = "\033[93m";
const char* BIRU     = "\033[94m";
const char* MAGENTA  = "\033[95m";
const char* CYAN     = "\033[96m";
const char* ABU      = "\033[90m";
const char* PUTIH    = "\033[97m";
const char* BOLD     = "\033[1m";

const int PANJANG_KODE = 4;
const int MAX_HISTORY = 20;
const int MAX_NAMA = 50;

const int BASIS_ANGKA = 10;
const int BATAS_KECIL = 4;
const long DURASI_LOADING = 30000000;

const int KODE_MENYERAH = -99;
const int KODE_KALAH = -1;

const int CLUE_LOCKED = 0;
const int CLUE_GENAP_KECIL = 2;
const int CLUE_GENAP_BESAR = 3;
const int CLUE_GANJIL_KECIL = 4;
const int CLUE_GANJIL_BESAR = 5;

const int WAKTU_MUDAH = 0;
const int WAKTU_NORMAL = 60;
const int WAKTU_SULIT = 30;

struct Jejak {
    int angka[PANJANG_KODE];
    int pas;
    int ada;
};

struct SettingLevel {
    int nyawa;
    int clue;
    int waktu;
    const char* warna;
};

const SettingLevel SETTING_LEVEL[3] = {
    {15, 4, WAKTU_MUDAH, HIJAU},
    {10, 2, WAKTU_NORMAL, KUNING},
    {8, 0, WAKTU_SULIT, MERAH}
};

void bersihkanBuffer() {
    cin.clear();
    char c;
    while (cin.get(c) && c != '\n') {}
}

void bersihkanLayar() {
    cout << "\033c";
    cout << "\033[2J\033[1;1H";
}

void garisPemisah() {
    cout << ABU << "+----+-----------+-----+-----+" << RESET << "\n";
}

bool cekNamaSama(char* a, char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return (a[i] == '\0' && b[i] == '\0');
}

bool cekNamaValid(char* nama) {
    if (nama[0] == '\0') return false;
    if (nama[0] == ' ') return false;
    
    int panjang = 0;
    while (nama[panjang] != '\0' && panjang < MAX_NAMA) {
        panjang++;
    }
    return (panjang >= 1 && panjang < MAX_NAMA);
}

void tampilkanInfoPangkat() {
    bersihkanLayar();
    cout << KUNING << "==========================================\n";
    cout << "          DATABASE PERINGKAT (RANK)          \n";
    cout << "==========================================\n" << RESET;
    
    cout << "\n[1] " << MAGENTA << "DEWA HACKER (ONE SHOT)" << RESET << "\n";
    cout << PUTIH << "    Syarat: Menang dalam 1 kali tebakan.\n";
    cout << ABU   << "    Status: Hoki atau Genius Mutlak.\n";

    cout << "\n[2] " << CYAN << "LOGIKA DEWA (ELITE)" << RESET << "\n";
    cout << PUTIH << "    Syarat: Main di Level SULIT (3) & Sisa Nyawa >= 4.\n";
    cout << ABU   << "    Status: Kemampuan Logika Tingkat Tinggi.\n";

    cout << "\n[3] " << MERAH << "KRITIS (CLUTCH MASTER)" << RESET << "\n";
    cout << PUTIH << "    Syarat: Menang dengan Sisa Nyawa 1.\n";
    cout << ABU   << "    Status: Menang di detik-detik terakhir.\n";

    cout << "\n[4] " << HIJAU << "OPERATOR HANDAL" << RESET << "\n";
    cout << PUTIH << "    Syarat: Menang normal (Level apapun).\n";
    
    cout << "\n[5] " << ABU << "SCRIPT KIDDIE (NOOB)" << RESET << "\n";
    cout << PUTIH << "    Syarat: Nyawa habis (Gagal memecahkan kode).\n";

    cout << "\n" << MERAH << ">> TEKAN ENTER UNTUK KEMBALI..." << RESET;
    cin.get();
}

void tampilkanTutorial() {
    while(true) {
        bersihkanLayar();
        cout << MERAH << "================================\n";
        cout << PUTIH << "    MANUAL SISTEM (TUTORIAL)\n";
        cout << MERAH << "================================\n" << RESET;
        
        cout << ABU << "[" << HIJAU << "1" << ABU << "] " << CYAN << "DASAR PERMAINAN & INPUT" << RESET << "\n";
        cout << ABU << "[" << HIJAU << "2" << ABU << "] " << CYAN << "FITUR C (CLUE) & Q (MENYERAH)" << RESET << "\n";
        cout << ABU << "[" << HIJAU << "3" << ABU << "] " << CYAN << "MEKANISME SKOR & LEVEL" << RESET << "\n";
        cout << ABU << "[" << MERAH << "4" << ABU << "] " << MERAH << "KEMBALI KE MENU UTAMA" << RESET << "\n\n";
        cout << HIJAU << "AKSES DATA >> " << RESET;
        
        int pilihan;
        cin >> pilihan;
        
        if(cin.fail()) { 
            bersihkanBuffer(); 
            cout << "\n" << MERAH << "[ERROR] INPUT RUSAK! PILIH ANGKA 1-4.\n" << RESET;
            cout << ABU << "(Tekan Enter)" << RESET; cin.get(); continue;
        }
        
        if (pilihan < 1 || pilihan > 4) {
            cout << "\n" << MERAH << "[ERROR] DATA TIDAK DITEMUKAN (1-4 ONLY).\n" << RESET;
            bersihkanBuffer();
            cout << ABU << "(Tekan Enter)" << RESET; cin.get(); continue;
        }

        bersihkanBuffer();
        if (pilihan == 4) break;

        if (pilihan == 1) {
            bersihkanLayar();
            cout << CYAN << "=== PROTOKOL DASAR & CARA INPUT ===\n" << RESET;
            cout << "\n" << KUNING << "[1] MISI ANDA:" << RESET << "\n";
            cout << PUTIH << "    Menebak 4 angka rahasia (0-9) dalam urutan yang benar.\n";
            cout << PUTIH << "    Angka dalam kode rahasia TIDAK ADA yang kembar 4 (misal 1111).\n";
            
            cout << "\n" << KUNING << "[2] ATURAN INPUT (WAJIB BACA):" << RESET << "\n";
            cout << PUTIH << "    Sistem hanya menerima angka satuan yang dipisah SPASI.\n";
            cout << "    " << HIJAU << "[BENAR]: 1 2 3 4" << RESET << ABU << " (Aman)" << RESET << "\n";
            cout << "    " << MERAH << "[SALAH]: 1234" << RESET << ABU << " (Digabung -> Error Format)" << RESET << "\n";
            cout << "    " << MERAH << "[SALAH]: 1 2 3 4 5" << RESET << ABU << " (Kelebihan -> Error Overflow)" << RESET << "\n";
            
            cout << "\n" << KUNING << "[3] MEMBACA HASIL:" << RESET << "\n";
            cout << "    " << HIJAU << "PAS" << RESET << " : Angka Benar & Posisi Benar.\n";
            cout << "    " << BIRU  << "ADA" << RESET << " : Angka Benar TAPI Salah Posisi.\n";
            
            cout << "\n" << MERAH << ">> ENTER KEMBALI" << RESET; cin.get();
        }
        else if (pilihan == 2) {
            bersihkanLayar();
            cout << CYAN << "=== PERINTAH KHUSUS (COMMANDS) ===\n" << RESET;
            
            cout << "\n" << KUNING << "[FITUR C - CLUE / PETUNJUK]" << RESET << "\n";
            cout << PUTIH << "    Ketik huruf '" << CYAN << "C" << PUTIH << "' saat diminta input angka.\n";
            cout << ABU   << "    Fungsi: Mengintip sifat angka di posisi tertentu.\n";
            cout << ABU   << "    Biaya : Mengurangi jatah Clue.\n";

            cout << "\n" << KUNING << "[FITUR Q - QUIT / MENYERAH]" << RESET << "\n";
            cout << PUTIH << "    Ketik huruf '" << MERAH << "Q" << PUTIH << "' jika ingin menyerah.\n";
            cout << ABU   << "    Fungsi: Menghentikan permainan (Kalah/Keluar).\n";
            cout << ABU   << "    Info  : Di Multiplayer, lawan otomatis menang.\n";
            
            cout << "\n" << MERAH << ">> ENTER KEMBALI" << RESET; cin.get();
        }
        else if (pilihan == 3) {
            bersihkanLayar();
            cout << CYAN << "=== MEKANISME SKOR & LEVEL ===\n" << RESET;
            
            cout << "\n" << KUNING << "[1] TINGKAT KESULITAN" << RESET << "\n";
            cout << PUTIH << "    LVL 1 (Mudah)  : " << HIJAU << "15 Nyawa" << PUTIH << " | " << HIJAU << "4 Clue" << PUTIH << " | " << HIJAU << "Tanpa Waktu\n" << RESET;
            cout << PUTIH << "    LVL 2 (Normal) : " << KUNING << "10 Nyawa" << PUTIH << " | " << KUNING << "2 Clue" << PUTIH << " | " << KUNING << "60 Detik/Tebakan\n" << RESET;
            cout << PUTIH << "    LVL 3 (Sulit)  : " << MERAH << " 8 Nyawa" << PUTIH << " | " << MERAH << "0 Clue" << PUTIH << " | " << MERAH << "30 Detik/Tebakan\n" << RESET;
            
            cout << "\n" << KUNING << "[2] ATURAN WAKTU" << RESET << "\n";
            cout << PUTIH << "    Jika waktu habis saat menebak, sistem akan mendeteksi anda lambat.\n";
            cout << "    HUKUMAN: Pengurangan 1 Nyawa dan input ronde tersebut dibatalkan.\n";

            cout << "\n" << MERAH << ">> ENTER KEMBALI" << RESET; cin.get();
        }
    }
}

void hitungPasAda(int rahasia[], int tebakan[], int& pas, int& ada) {
    pas = 0;
    int freqR[BASIS_ANGKA] = {0}, freqT[BASIS_ANGKA] = {0};
    
    for (int i = 0; i < PANJANG_KODE; i++) {
        if (tebakan[i] == rahasia[i]) pas++;
        freqR[rahasia[i]]++;
        freqT[tebakan[i]]++;
    }
    
    int tot = 0;
    for (int a = 0; a < BASIS_ANGKA; a++) {
        tot += (freqR[a] < freqT[a] ? freqR[a] : freqT[a]);
    }
    ada = tot - pas;
}

int jalankanMisi(int level, bool modeMultiplayer, char* pembuatKode, char* pemecahKode) {
    int rahasia[PANJANG_KODE], tebakan[PANJANG_KODE];
    Jejak history[MAX_HISTORY];
    int nyawa = 0, maxNyawa = 0, jatahClue = 0;
    int batasWaktu = 0;
    bool menang = false;

    int statusClue[PANJANG_KODE] = {CLUE_LOCKED, CLUE_LOCKED, CLUE_LOCKED, CLUE_LOCKED};

    if (level >= 1 && level <= 3) {
        maxNyawa = SETTING_LEVEL[level-1].nyawa;
        jatahClue = SETTING_LEVEL[level-1].clue;
        batasWaktu = SETTING_LEVEL[level-1].waktu;
    } else {
        maxNyawa = 10;
        jatahClue = 2;
        batasWaktu = WAKTU_NORMAL;
    }
    nyawa = maxNyawa;

    if (modeMultiplayer) {
        bersihkanLayar();
        cout << MERAH << "=== PROTOKOL UPLOAD: " << KUNING << pembuatKode << MERAH << " ===" << RESET << "\n";
        cout << ABU << "ATURAN: " << PUTIH << "HANYA 4 DIGIT (0-9). JANGAN DIGABUNG." << RESET << "\n";
        
        bool kodeValid = false;
        while (!kodeValid) {
            cout << ">> SET SANDI (" << ABU << "SPASI" << RESET << "): " << KUNING;
            
            if (!(cin >> rahasia[0] >> rahasia[1] >> rahasia[2] >> rahasia[3])) {
                cout << MERAH << "\n[ERROR] BUKAN ANGKA / FORMAT SALAH!\n" << RESET;
                cout << ABU << "(Tekan Enter)" << RESET;
                bersihkanBuffer(); cin.get(); continue;
            }

            char sisa;
            bool kelebihan = false;
            while ((sisa = cin.get()) != '\n' && sisa != EOF) {
                if (sisa != ' ' && sisa != '\t' && sisa != '\r') kelebihan = true;
            }

            if (kelebihan) {
                cout << RESET << MERAH << "\n[ERROR] INPUT TERLALU PANJANG!\n" << RESET;
                cout << ABU << ">> HANYA BOLEH 4 ANGKA.\n" << RESET;
                cout << ABU << "(Tekan Enter)" << RESET; cin.get();
                continue;
            }
            cout << RESET;
            
            bool rangeOk = true;
            bool formatGabung = false;
            for(int k=0; k<PANJANG_KODE; k++) {
                if(rahasia[k]<0 || rahasia[k] >= BASIS_ANGKA) {
                    rangeOk = false;
                    if(rahasia[k] >= BASIS_ANGKA) formatGabung = true;
                }
            }
            bool kembar4 = (rahasia[0]==rahasia[1]&&rahasia[1]==rahasia[2]&&rahasia[2]==rahasia[3]);

            if (!rangeOk) {
                cout << "\n" << MERAH << "[ERROR] INPUT TIDAK VALID!" << RESET << "\n";
                if (formatGabung) cout << ABU << ">> GUNAKAN SPASI (CONTOH: 1 2 3 4).\n" << RESET;
                else cout << ABU << ">> HANYA MENERIMA ANGKA SATUAN (0-9).\n" << RESET;
                cout << ABU << "(Tekan Enter)" << RESET; cin.get();
            }
            else if (kembar4) {
                cout << MERAH << "\n[ERROR] JANGAN 4 ANGKA SAMA PERSIS.\n" << RESET;
                cout << ABU << "(Tekan Enter)" << RESET; cin.get();
            }
            else kodeValid = true;
        }
        
        cout << HIJAU << "\nENKRIPSI SUKSES." << RESET << "\n";
        cout << MERAH << ">> TEKAN ENTER UNTUK MENGHAPUS JEJAK..." << RESET;
        cin.get();
        bersihkanLayar();

    } else {
        srand(time(0));
        cout << CYAN << "INITIALIZING RANDOM ENGINE..." << RESET;
        
        bool kodeValid = false;
        while (!kodeValid) {
            for (int i = 0; i < PANJANG_KODE; i++) {
                rahasia[i] = rand() % BASIS_ANGKA;
            }
            if (rahasia[0]==rahasia[1]&&rahasia[1]==rahasia[2]&&rahasia[2]==rahasia[3]) {
                kodeValid = false;
            } else kodeValid = true;
        }
    }

    int giliran = 0;

    while (nyawa > 0 && !menang) {
        bersihkanLayar();
        
        cout << MERAH << "ENIGMA PROTOCOL" << RESET << ABU << " | TARGET: " << MAGENTA << pembuatKode << ABU << " | AGENT: " << CYAN << pemecahKode << RESET << "\n";
        
        cout << ABU << "LEVEL: ";
        if(level==1) cout << HIJAU << "MUDAH";
        else if(level==2) cout << KUNING << "NORMAL";
        else cout << MERAH << "SULIT";
        
        cout << ABU << " | NYAWA: [";
        for(int i=0; i<maxNyawa; i++) {
            if (i < nyawa) {
                if(nyawa <= 3) cout << MERAH << "*" << RESET;
                else cout << HIJAU << "*" << RESET;
            } else {
                cout << ABU << "-" << RESET;
            }
        }
        cout << ABU << "] " << nyawa << ABU << " | CLUE: " << BIRU << jatahClue << RESET << "\n\n";

        garisPemisah();
        cout << ABU << "| NO |  K O D E  | PAS | ADA |" << RESET << "\n";
        garisPemisah();

        for (int i = 0; i < giliran; i++) {
            cout << ABU << "|" << RESET;
            if (i < 9) cout << " " << (i + 1) << "  "; else cout << " " << (i + 1) << " ";
            
            cout << ABU << "|" << RESET;
            cout << "  " << BOLD << history[i].angka[0] << " " << history[i].angka[1] << " " 
            << history[i].angka[2] << " " << history[i].angka[3] << RESET << "  ";
            cout << ABU << "|" << RESET << "  " << HIJAU << history[i].pas << RESET << "  ";
            cout << ABU << "|" << RESET << "  " << BIRU << history[i].ada << RESET << "  ";
            cout << ABU << "|" << RESET << "\n";
        }
        garisPemisah();
        
        if (jatahClue < SETTING_LEVEL[level-1].clue) {
            cout << "\n" << ABU << "::: DECRYPTED DATA (CLUE LOG) :::" << RESET << "\n";
            for(int z=0; z<PANJANG_KODE; z++) {
                cout << ABU << "DIGIT " << (z+1) << ": " << RESET;
                if (statusClue[z] == CLUE_LOCKED) cout << ABU << "LOCKED" << RESET;
                else if (statusClue[z] == CLUE_GENAP_KECIL) cout << BIRU << "GENAP & KECIL (0-4)" << RESET;
                else if (statusClue[z] == CLUE_GENAP_BESAR) cout << BIRU << "GENAP & BESAR (5-9)" << RESET;
                else if (statusClue[z] == CLUE_GANJIL_KECIL) cout << BIRU << "GANJIL & KECIL (0-4)" << RESET;
                else if (statusClue[z] == CLUE_GANJIL_BESAR) cout << BIRU << "GANJIL & BESAR (5-9)" << RESET;
                
                if (z < 3) cout << " | ";
            }
            cout << "\n";
        }

        cout << "\nOPSI: [" << CYAN << "C" << RESET << "]lue | [" << MERAH << "Q" << RESET << "]uit\n";
        if (batasWaktu > 0) cout << KUNING << "[TIMER: " << batasWaktu << " DETIK] " << RESET;
        else cout << KUNING << "[TIMER: - ] " << RESET;

        cout << ABU << "| CONTOH: 1 2 3 4" << RESET << "\n";
        cout << HIJAU << "TERMINAL >> " << RESET;
        
        time_t waktuMulai = time(0);

        cin >> tebakan[0];

        if (cin.fail()) {
            cin.clear(); char cmd; cin >> cmd;
            bersihkanBuffer(); 

            if (cmd == 'q' || cmd == 'Q') {
                bool konfirmasiValid = false;
                while(!konfirmasiValid) {
                    cout << "\n" << MERAH << "[ALERT] ANDA YAKIN INGIN MENYERAH/KELUAR? (Y/N): " << RESET;
                    char confirm; cin >> confirm;
                    bersihkanBuffer();

                    if (confirm == 'y' || confirm == 'Y') {
                        return KODE_MENYERAH;
                    } else if (confirm == 'n' || confirm == 'N') {
                        konfirmasiValid = true;
                        cout << HIJAU << ">> DIBATALKAN. MELANJUTKAN MISI..." << RESET << "\n";
                        for(long i=0; i<DURASI_LOADING; i++){}
                    } else {
                        cout << MERAH << "[ERROR] INPUT TIDAK VALID! KETIK 'Y' ATAU 'N'." << RESET << "\n";
                        cout << ABU << "(Tekan Enter)" << RESET;
                        cin.get();
                    }
                }
                continue;
            }
            else if (cmd == 'c' || cmd == 'C') {
                if (jatahClue > 0) {
                    cout << "\n" << BIRU << "SCANNING POSISI (1-4): " << RESET;
                    int pos; cin >> pos;
                    
                    if (cin.fail()) {
                        cout << MERAH << "\n[ERROR] INPUT BUKAN ANGKA!" << RESET << "\n";
                        cout << ABU << "(Tekan Enter untuk Lanjut)" << RESET;
                        bersihkanBuffer(); cin.get();
                    }
                    else if (pos < 1 || pos > 4) {
                        cout << MERAH << "\n[ERROR] POSISI DI LUAR JANGKAUAN (HARUS 1-4)!" << RESET << "\n";
                        cout << ABU << "(Tekan Enter untuk Lanjut)" << RESET;
                        bersihkanBuffer(); cin.get();
                    }
                    else if (statusClue[pos-1] != CLUE_LOCKED) {
                        cout << MERAH << "\n[ERROR] CLUE SUDAH ADA / TERBUKA!" << RESET << "\n";
                        cout << ABU << ">> SILAKAN PILIH POSISI LAIN.\n" << RESET;
                        cout << ABU << "(Tekan Enter untuk Lanjut)" << RESET;
                        bersihkanBuffer(); cin.get();
                    }
                    else {
                        int t = rahasia[pos-1];
                        
                        if (t % 2 == 0) {
                            if (t <= BATAS_KECIL) statusClue[pos-1] = CLUE_GENAP_KECIL;
                            else statusClue[pos-1] = CLUE_GENAP_BESAR;
                        } else {
                            if (t <= BATAS_KECIL) statusClue[pos-1] = CLUE_GANJIL_KECIL;
                            else statusClue[pos-1] = CLUE_GANJIL_BESAR;
                        }
                        
                        jatahClue--;
                        
                        cout << HIJAU << ">> DATA BERHASIL DIDAPATKAN!" << RESET << "\n";
                        cout << ABU << "(Tekan Enter untuk Lanjut)" << RESET;
                        bersihkanBuffer(); 
                        cin.get();
                    }
                } else {
                    cout << MERAH << "[ERROR] CLUE HABIS.\n" << RESET;
                    cout << ABU << "(Tekan Enter)" << RESET;
                    cin.get();
                }
            } else {
                cout << MERAH << "[ERROR] PERINTAH TIDAK DIKENAL!\n" << RESET;
                cout << ABU << "(Tekan Enter)" << RESET;
                cin.get();
            }
            
            continue; 
        }

        time_t waktuSelesai = time(0);
        double durasi = difftime(waktuSelesai, waktuMulai);

        if (batasWaktu > 0 && durasi > batasWaktu) {
            cout << "\n" << MERAH << "========================================" << RESET << "\n";
            cout << MERAH << "[ALERT] WAKTU HABIS! (" << durasi << "s > " << batasWaktu << "s)" << RESET << "\n";
            cout << ABU   << ">> SISTEM MENDETEKSI AKTIVITAS LAMBAT.\n" << RESET;
            cout << ABU   << ">> HUKUMAN: PENGURANGAN NYAWA.\n" << RESET;
            cout << MERAH << "========================================" << RESET << "\n";
            
            nyawa--;
            bersihkanBuffer();
            cout << "(Tekan Enter)"; cin.get();
            continue;
        }

        cin >> tebakan[1] >> tebakan[2] >> tebakan[3];
        
        char sisa;
        bool kelebihan = false;
        while ((sisa = cin.get()) != '\n' && sisa != EOF) {
            if (sisa != ' ' && sisa != '\t' && sisa != '\r') kelebihan = true;
        }

        if (kelebihan) {
            cout << "\n" << MERAH << "[ERROR] INPUT TERLALU PANJANG!" << RESET << "\n";
            cout << ABU << ">> HANYA BOLEH 4 ANGKA.\n" << RESET;
            cout << ABU << "(Tekan Enter)" << RESET; cin.get(); continue;
        }

        if (cin.fail()) {
            cout << "\n" << MERAH << "[ERROR] FORMAT RUSAK! PASTIKAN 4 ANGKA." << RESET << "\n";
            cout << ABU << "(Tekan Enter)" << RESET; cin.get(); continue;
        }
        
        bool rangeOk = true;
        bool formatGabung = false;
        for(int k=0;k<PANJANG_KODE;k++) {
            if(tebakan[k]<0 || tebakan[k] >= BASIS_ANGKA) {
                rangeOk = false;
                if(tebakan[k] >= BASIS_ANGKA) formatGabung = true;
            }
        }
        
        if(!rangeOk) {
            cout << "\n" << MERAH << "[ERROR] DATA TIDAK VALID!" << RESET << "\n";
            if (formatGabung) cout << ABU << ">> TERDETEKSI ANGKA > 9. WAJIB SPASI (CONTOH: 1 2 3 4).\n" << RESET;
            else cout << ABU << ">> HANYA MENERIMA ANGKA SATUAN (0-9).\n" << RESET;
            cout << ABU << "(Tekan Enter)" << RESET; cin.get(); continue;
        }

        int pas = 0, ada = 0;
        hitungPasAda(rahasia, tebakan, pas, ada);

        if (giliran < MAX_HISTORY) {
            history[giliran].angka[0] = tebakan[0];
            history[giliran].angka[1] = tebakan[1];
            history[giliran].angka[2] = tebakan[2];
            history[giliran].angka[3] = tebakan[3];
            history[giliran].pas = pas;
            history[giliran].ada = ada;
            giliran++;
        } else {
            cout << MERAH << "[ALERT] HISTORY PENUH!\n" << RESET;
        }
        
        if (pas == 4) {
            menang = true;
        } else {
            nyawa--;
        }
    }

    cout << "\n";
    if (menang) {
        cout << HIJAU << "+================================+\n|  SISTEM JEBOL! AKSES DITERIMA  |\n+================================+\n" << RESET;
        
        if (giliran == 1) cout << "PERINGKAT: " << MAGENTA << "DEWA HACKER (ONE SHOT)" << RESET << "\n";
        else if (nyawa == 1) cout << "PERINGKAT: " << MERAH << "KRITIS (CLUTCH MASTER)" << RESET << "\n";
        else if (level == 3 && nyawa >= 4) cout << "PERINGKAT: " << CYAN << "LOGIKA DEWA (ELITE)" << RESET << "\n";
        else cout << "PERINGKAT: " << HIJAU << "OPERATOR HANDAL" << RESET << "\n";
        
        cout << "Sisa Nyawa: " << nyawa << "\n(Tekan Enter)"; cin.get();
        return nyawa;
    } else {
        bersihkanLayar();
        cout << MERAH << "+===========================================+\n";
        cout << "|  PERINGATAN: LOKASI ANDA TELAH DILACAK!   |\n";
        cout << "|         AKSES DITOLAK SECARA PERMANEN        |\n";
        cout << "+===========================================+\n" << RESET;
        
        cout << "STATUS : " << ABU << "KONEKSI DIPUTUS (TERMINATED)" << RESET << "\n";
        cout << "RANK   : " << MERAH << "SCRIPT KIDDIE (PEMULA)" << RESET << "\n";
        
        cout << "\nKODE SEBENARNYA ADALAH: " << KUNING << "[ " 
             << rahasia[0] << " " << rahasia[1] << " " << rahasia[2] << " " << rahasia[3] 
             << " ]" << RESET << "\n";
             
        cout << "\n" << MERAH << "(Tekan Enter untuk Kabur...)" << RESET;
        cin.get();
        return KODE_KALAH;
    }
}

int main() {
    int mode = 0;
    while (true) {
        bersihkanLayar();
        cout << MERAH << "==================================" << RESET << "\n";
        cout << PUTIH << "          ENIGMA PROTOCOL" << RESET << "\n";
        cout << ABU   << "       [ System by " << MAGENTA << "Fardan-Y" << ABU << " ]" << RESET << "\n";
        cout << MERAH << "==================================" << RESET << "\n";
        
        cout << ABU << "[" << BIRU << "0" << ABU << "] " << BIRU << "MANUAL SISTEM (TUTORIAL)" << RESET << "\n";
        cout << ABU << "[" << KUNING << "1" << ABU << "] " << KUNING << "DATABASE PANGKAT (INFO RANK)" << RESET << "\n";
        cout << ABU << "[" << CYAN << "2" << ABU << "] " << CYAN << "RETAS SOLO (SINGLE)" << RESET << "\n";
        cout << ABU << "[" << CYAN << "3" << ABU << "] " << CYAN << "RETAS DUEL (MULTIPLAYER)" << RESET << "\n";
        cout << ABU << "[" << MERAH << "4" << ABU << "] " << MERAH << "PUTUS KONEKSI (KELUAR)" << RESET << "\n\n";
        cout << HIJAU << "PILIH MODUL >> " << RESET;
        
        cin >> mode;
        
        if (cin.fail()) { 
            bersihkanBuffer(); 
            cout << "\n" << MERAH << "[ERROR] HARUS ANGKA!\n" << RESET;
            cout << ABU << "(Tekan Enter)" << RESET; cin.get(); continue;
        }
        
        if (mode < 0 || mode > 4) {
            bersihkanBuffer();
            cout << "\n" << MERAH << "[ERROR] PILIHAN TIDAK ADA (0-4).\n" << RESET;
            cout << ABU << "(Tekan Enter)" << RESET; cin.get(); continue;
        }
        
        if (mode == 4) { cout << "\n" << MERAH << "BYE..." << RESET << "\n"; break; }
        bersihkanBuffer();

        if (mode == 0) { tampilkanTutorial(); continue; }
        if (mode == 1) { tampilkanInfoPangkat(); continue; }

        if (mode == 2) {
            char namaP1[MAX_NAMA]; char namaCPU[]="SERVER PUSAT";
            int lvl = 1; bool jalan = true;
            
            cout << "NAMA HACKER: " << HIJAU; 
            cin.getline(namaP1, MAX_NAMA);
            if (cin.fail()) {
                cin.clear();
                bersihkanBuffer();
                cout << MERAH << "[ERROR] NAMA TERLALU PANJANG! MAX 49 KARAKTER.\n" << RESET;
                cout << ABU << "(Tekan Enter)" << RESET;
                cin.get();
                continue;
            }
            cout << RESET;
            
            if (!cekNamaValid(namaP1)) {
                cout << MERAH << "[ERROR] NAMA TIDAK BOLEH KOSONG ATAU DIAWALI SPASI!\n" << RESET;
                cout << ABU << "(Tekan Enter)" << RESET;
                cin.get();
                continue;
            }
            
            while(true) {
                cout << "\n" << KUNING << "=== PILIH TINGKAT KEAMANAN ===" << RESET << "\n";
                cout << ABU << "[" << HIJAU << "1" << ABU << "] " << HIJAU << "MUDAH  " << RESET << "(15 Nyawa | 4 Clue | No Timer).\n";
                cout << ABU << "[" << KUNING << "2" << ABU << "] " << KUNING << "NORMAL " << RESET << "(10 Nyawa | 2 Clue | 60 Detik).\n";
                cout << ABU << "[" << MERAH << "3" << ABU << "] " << MERAH << "SULIT  " << RESET << "( 8 Nyawa | 0 Clue | 30 Detik).\n";
                cout << HIJAU << "INPUT PILIHAN (1-3) >> " << RESET;
                
                cin >> lvl;
                if(!cin.fail() && lvl>=1 && lvl<=3) {
                    bersihkanBuffer(); break;
                }
                if (cin.fail()) {
                    cout << MERAH << "[ERROR] JANGAN HURUF!\n" << RESET;
                    cout << ABU << "(Tekan Enter)" << RESET;
                    bersihkanBuffer(); cin.get();
                } else {
                    cout << MERAH << "[ERROR] PILIH 1, 2, ATAU 3.\n" << RESET;
                    cout << ABU << "(Tekan Enter)" << RESET;
                    bersihkanBuffer(); cin.get();
                }
            }

            while (jalan) {
                int h = jalankanMisi(lvl, false, namaCPU, namaP1);
                
                if (h == KODE_MENYERAH) { jalan = false; continue; } 
                
                if (h != KODE_KALAH) {
                    if (lvl == 3) {
                        bersihkanLayar(); cout << MAGENTA << "SEMUA SERVER JEBOL. ANDA MENANG.\n" << RESET;
                        if(h==7) cout << BIRU << "[PENCAPAIAN: MODE DEWA]\n" << RESET;
                        cout << "(Enter Menu)"; cin.get(); jalan = false;
                    } else {
                        cout << "\n" << CYAN << ">> OPSI LANJUTAN:\n" << RESET << "1. Level Berikut (" << lvl+1 << ")\n2. Ulang\n3. Menu\n>> ";
                        int pl; cin >> pl; bersihkanBuffer();
                        if (pl==1) lvl++; else if (pl==3) jalan=false;
                    }
                } else {
                    cout << "\n" << MERAH << ">> GAGAL:\n" << RESET << "1. Coba Lagi\n2. Menu\n>> ";
                    int pl; cin >> pl; bersihkanBuffer(); if (pl==2) jalan=false;
                }
            }
        }
        else if (mode == 3) {
            char p1[MAX_NAMA], p2[MAX_NAMA]; 
            cout << "HACKER 1: " << HIJAU; cin.getline(p1, MAX_NAMA); cout << RESET;
            
            if (cin.fail()) {
                cin.clear();
                bersihkanBuffer();
                cout << MERAH << "[ERROR] NAMA TERLALU PANJANG! MAX 49 KARAKTER.\n" << RESET;
                cout << ABU << "(Tekan Enter)" << RESET;
                cin.get();
                continue;
            }
            
            if (!cekNamaValid(p1)) {
                cout << MERAH << "[ERROR] NAMA TIDAK BOLEH KOSONG ATAU DIAWALI SPASI!\n" << RESET;
                cout << ABU << "(Tekan Enter)" << RESET;
                cin.get();
                continue;
            }
            
            do {
                cout << "HACKER 2: " << CYAN;  cin.getline(p2, MAX_NAMA); cout << RESET;
                
                if (cin.fail()) {
                    cin.clear();
                    bersihkanBuffer();
                    cout << MERAH << "[ERROR] NAMA TERLALU PANJANG! MAX 49 KARAKTER.\n" << RESET;
                    cout << ABU << "(Tekan Enter)" << RESET;
                    cin.get();
                    continue;
                }
                
                if (!cekNamaValid(p2)) {
                    cout << MERAH << "[ERROR] NAMA TIDAK VALID! MIN 1 KARAKTER.\n" << RESET;
                    cout << ABU << "(Tekan Enter)" << RESET;
                    cin.get();
                    continue;
                }
                
                if (cekNamaSama(p1, p2)) {
                    cout << MERAH << "[ERROR] IDENTITAS GANDA! NAMA TIDAK BOLEH SAMA.\n" << RESET;
                    cout << ABU << "(Tekan Enter)" << RESET; cin.get();
                }
            } while (cekNamaSama(p1, p2) || !cekNamaValid(p2));
            
            int skorP1 = 0, skorP2 = 0;
            bool main = true;
            while(main) {
                int l; 
                while(true) {
                    cout << "\n" << KUNING << "=== KONFIGURASI DUEL ===" << RESET << "\n";
                    cout << ABU << "[" << HIJAU << "1" << ABU << "] " << HIJAU << "MUDAH  " << RESET << "(15 Nyawa | 4 Clue | No Timer).\n";
                    cout << ABU << "[" << KUNING << "2" << ABU << "] " << KUNING << "NORMAL " << RESET << "(10 Nyawa | 2 Clue | 60 Detik).\n";
                    cout << ABU << "[" << MERAH << "3" << ABU << "] " << MERAH << "SULIT  " << RESET << "( 8 Nyawa | 0 Clue | 30 Detik).\n";
                    cout << HIJAU << "PILIH LEVEL (1-3) >> " << RESET;
                    cin >> l;
                    if (!cin.fail() && l >= 1 && l <= 3) {
                        bersihkanBuffer(); break; 
                    }
                    if (cin.fail()) {
                        cout << MERAH << "[ERROR] ANGKA SAJA!\n" << RESET;
                        cout << ABU << "(Tekan Enter)" << RESET;
                        bersihkanBuffer(); cin.get();
                    } else {
                        cout << MERAH << "[ERROR] PILIH 1-3!\n" << RESET;
                        cout << ABU << "(Tekan Enter)" << RESET;
                        bersihkanBuffer(); cin.get();
                    }
                }
                
                int h2 = jalankanMisi(l, true, p1, p2);
                int poinRondeP2 = (h2 < 0) ? 0 : h2; 
                skorP2 += poinRondeP2;

                if (h2 == KODE_MENYERAH) {
                    cout << "\n" << MERAH << ">> " << p2 << " MENYERAH! SKOR RONDE INI: 0" << RESET << "\n";
                    cout << ABU << "(Tekan Enter Lanjut ke Giliran " << p1 << ")" << RESET; cin.get();
                }

                int h1 = jalankanMisi(l, true, p2, p1);
                int poinRondeP1 = (h1 < 0) ? 0 : h1;
                skorP1 += poinRondeP1;

                if (h1 == KODE_MENYERAH) {
                    cout << "\n" << MERAH << ">> " << p1 << " MENYERAH! SKOR RONDE INI: 0" << RESET << "\n";
                    cout << ABU << "(Tekan Enter untuk Lihat Hasil)" << RESET; cin.get();
                }

                bersihkanLayar();
                cout << MERAH << "========================================\n";
                cout << "            KLASEMEN SEMENTARA             \n";
                cout << "========================================\n" << RESET;
                cout << HIJAU << p1 << " (TOTAL SKOR): " << skorP1 << " " << ABU << "(+" << poinRondeP1 << ")" << RESET << "\n";
                cout << CYAN  << p2 << " (TOTAL SKOR): " << skorP2 << " " << ABU << "(+" << poinRondeP2 << ")" << RESET << "\n";

                cout << "\n" << KUNING << "STATUS SAAT INI: ";
                if (skorP1 > skorP2) cout << MAGENTA << p1 << " MEMIMPIN!" << RESET << "\n";
                else if (skorP2 > skorP1) cout << MAGENTA << p2 << " MEMIMPIN!" << RESET << "\n";
                else cout << PUTIH << "SERI (IMBANG)" << RESET << "\n";

                cout << "\n" << BIRU << "1. Duel Lagi | 2. Kembali ke Menu: " << RESET; 
                int pl; cin >> pl; bersihkanBuffer();
                if(pl==2) main=false;
            }
        }
    }
    return 0;
}
