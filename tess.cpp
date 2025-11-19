/*
KELOMPOK 10
FARDAN
LINO
NAZWA
YUDHO
*/

// [MODUL 1] Pengenalan C++ & Library
// Menggunakan library standar untuk Input (cin) dan Output (cout)
#include <iostream> 

using namespace std;

// [MODUL 2] Konstanta, Tipe Data & Variabel
// Penggunaan 'const' untuk nilai tetap yang tidak boleh berubah
const int JUMLAH_BARIS = 6;
const int JUMLAH_KOLOM = 7;

// [MODUL 7] Fungsi & Prosedur (Prototype)
// Deklarasi fungsi agar bisa dipanggil dari main()
void bersihkanLayar();
void tampilkanTutorial();
void inisialisasiPapan(char papan[JUMLAH_BARIS][JUMLAH_KOLOM]);
void gambarPapan(char papan[JUMLAH_BARIS][JUMLAH_KOLOM], int skorP1, int skorP2);
int prosesJatuhkanKeping(char papan[JUMLAH_BARIS][JUMLAH_KOLOM], int kolomPilihan, char keping);
bool cekApakahMenang(char papan[JUMLAH_BARIS][JUMLAH_KOLOM], char keping);
bool cekApakahSeri(int totalLangkah);

// ==========================================
//             PROGRAM UTAMA
// ==========================================
int main() {
    // [MODUL 6] Array (Larik)
    // Implementasi Array 2 Dimensi untuk papan permainan
    char papan[JUMLAH_BARIS][JUMLAH_KOLOM];
    
    // [MODUL 2] Variabel Integer untuk Skor
    int skorP1 = 0;
    int skorP2 = 0;
    char jawabanMainLagi;

    // Memanggil Prosedur Tutorial Lengkap di awal
    tampilkanTutorial();

    // [MODUL 5] Perulangan (Do-While Loop)
    // Digunakan agar game minimal jalan sekali, lalu cek kondisi main lagi di akhir
    do {
        // Reset papan dan variabel setiap ronde baru
        inisialisasiPapan(papan);
        
        int pemainAktif = 1;
        char keping = 'X';
        int totalLangkah = 0;
        
        // [MODUL 2] Tipe Data Boolean (True/False)
        bool permainanSelesai = false;

        // [MODUL 5] Perulangan (While Loop)
        // Loop gameplay, berjalan terus selama 'permainanSelesai' masih false
        while (permainanSelesai == false) {
            
            // Update gambar papan
            gambarPapan(papan, skorP1, skorP2);

            // [MODUL 1] Input & Output
            int inputAngka;
            cout << "Giliran Pemain " << pemainAktif << " (" << keping << ")\n";
            cout << "Ketik nomor kolom (1-7) atau ketik 0 untuk Bantuan: ";
            cin >> inputAngka;

            // [MODUL 4] Percabangan (If-Else) & Validasi Input
            // --- [TAMBAHAN] PENANGANAN INPUT BUKAN ANGKA ---
            if (cin.fail()) {
                cin.clear();             // Hapus status error
                cin.ignore(10000, '\n'); // Buang input ngawur
                
                cout << "\n[ERROR] Input harus berupa ANGKA!\n";
                cout << "Tekan [Enter] untuk mencoba lagi...";
                cin.get(); // Pause agar pesan terbaca
                continue;  // Kembali ke awal loop
            }

            // Cek apakah user minta tutorial
            if (inputAngka == 0) {
                tampilkanTutorial();
                continue; // Jangan ganti giliran, kembali ke gambar papan
            }

            // [MODUL 3] Operator Relasional (<, >) & Logika (||)
            // --- [TAMBAHAN] PENANGANAN INPUT DI LUAR JANGKAUAN ---
            if (inputAngka < 1 || inputAngka > 7) {
                cout << "\n[ERROR] Kolom " << inputAngka << " tidak ada! Pilih 1-7.\n";
                cout << "Tekan [Enter] untuk mencoba lagi...";
                
                cin.ignore(10000, '\n'); // Bersihkan sisa enter sebelumnya
                cin.get();               // Pause agar pesan terbaca
                continue;
            }

            // [MODUL 3] Operator Aritmatika (-)
            // User input 1, tapi index Array mulai dari 0. Jadi dikurang 1.
            // Fungsi ini juga mengembalikan nilai (return value)
            int hasilJatuh = prosesJatuhkanKeping(papan, inputAngka - 1, keping);

            // --- [TAMBAHAN] PENANGANAN KOLOM PENUH ---
            if (hasilJatuh == -1) {
                cout << "\n[ERROR] Kolom sudah PENUH! Pilih kolom lain.\n";
                cout << "Tekan [Enter] untuk lanjut...";
                
                cin.ignore(10000, '\n'); // Bersihkan buffer
                cin.get();               // Pause agar pesan terbaca
                // Tidak perlu 'continue' di sini karena dia akan otomatis 
                // melewati blok 'else' di bawah dan looping lagi
            } 
            else {
                // [MODUL 3] Operator Increment (++)
                totalLangkah++; 

                // [MODUL 8] Algoritma & Logika Lanjut (Cek Menang)
                if (cekApakahMenang(papan, keping) == true) {
                    gambarPapan(papan, skorP1, skorP2);
                    cout << "\n!!! SELAMAT! PEMAIN " << pemainAktif << " MENANG !!!\n";
                    
                    // Update Skor Pemenang
                    if (pemainAktif == 1) skorP1++; 
                    else skorP2++;
                    
                    permainanSelesai = true; // Menghentikan while loop
                } 
                // Cek kondisi Seri
                else if (cekApakahSeri(totalLangkah) == true) {
                    gambarPapan(papan, skorP1, skorP2);
                    cout << "\n!!! PERMAINAN SERI (PAPAN PENUH) !!!\n";
                    permainanSelesai = true;
                } 
                else {
                    // [MODUL 4] Percabangan untuk Tukar Giliran
                    if (pemainAktif == 1) {
                        pemainAktif = 2;
                        keping = 'O';
                    } else {
                        pemainAktif = 1;
                        keping = 'X';
                    }
                }
            }
        }

        // Menampilkan Skor Akhir Ronde
        cout << "Skor Saat Ini -> P1: " << skorP1 << " | P2: " << skorP2 << endl;
        
        // --- [BAGIAN INI YANG SAYA TAMBAHKAN AGAR TIDAK LANGSUNG KELUAR] ---
        while (true) {
            cout << "Apakah ingin main lagi? (y/t): ";
            cin >> jawabanMainLagi;

            // Bersihkan buffer barangkali user ngetik "yesss" panjang
            if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); }

            // Cek apakah inputnya valid (y/Y/t/T)
            if (jawabanMainLagi == 'y' || jawabanMainLagi == 'Y' || 
                jawabanMainLagi == 't' || jawabanMainLagi == 'T') {
                break; // KELUAR dari loop validasi ini dan lanjut ke pengecekan do-while
            } else {
                cout << "[ERROR] Jawaban tidak dikenali! Hanya ketik 'y' atau 't'.\n";
            }
        }
        // ------------------------------------------------------------------

    } while (jawabanMainLagi == 'y' || jawabanMainLagi == 'Y');

    cout << "\nTerima kasih sudah bermain!\n";
    return 0;
}


// ==========================================
//        IMPLEMENTASI FUNGSI (MODUL 7)
// ==========================================

void bersihkanLayar() {
    // [MODUL 5] Perulangan (For Loop)
    // Loop sederhana 60 kali untuk membersihkan layar terminal
    for (int i = 0; i < 60; i++) cout << "\n";
}

// Ini adalah Tutorial Lengkap yang tidak dihapus
void tampilkanTutorial() {
    bersihkanLayar();
    cout << "===============================================\n";
    cout << "        PANDUAN LENGKAP CONNECT FOUR           \n";
    cout << "===============================================\n";
    
    cout << "\n[ 1. MISI UTAMA ]\n";
    cout << "  Jadilah pemain pertama yang menyusun 4 keping\n";
    cout << "  milikmu secara berurutan.\n";

    cout << "\n[ 2. TANDA PEMAIN ]\n";
    cout << "  - Pemain 1 menggunakan tanda (X)\n";
    cout << "  - Pemain 2 menggunakan tanda (O)\n";

    cout << "\n[ 3. ARAH KEMENANGAN ]\n";
    cout << "  Kamu menang jika kepingmu berjejer 4 buah:\n";
    cout << "  a. Mendatar  (Horizontal) : X X X X\n";
    cout << "  b. Tegak     (Vertikal)   : X disusun ke atas\n";
    cout << "  c. Miring    (Diagonal)   : Miring kanan/kiri\n";

    cout << "\n[ 4. CARA BERGERAK ]\n";
    cout << "  - Papan memiliki efek 'Gravitasi'.\n";
    cout << "  - Kamu hanya perlu memilih NOMOR KOLOM (1-7).\n";
    cout << "  - Kepingan akan jatuh ke baris terbawah yang kosong.\n";
    
    cout << "\n[ 5. KONTROL KHUSUS ]\n";
    cout << "  - Ketik angka 1-7 : Untuk menjatuhkan keping.\n";
    cout << "  - Ketik angka 0   : Untuk membaca panduan ini lagi.\n";

    cout << "\n===============================================\n";
    cout << "Sudah paham? Tekan [Enter] untuk mulai bermain!";
    cin.ignore(); cin.get(); // Menunggu user tekan enter
}

void inisialisasiPapan(char papan[JUMLAH_BARIS][JUMLAH_KOLOM]) {
    // [MODUL 6] Akses Array 2 Dimensi dengan Nested Loop (Loop dalam Loop)
    for (int brs = 0; brs < JUMLAH_BARIS; brs++) {
        for (int klm = 0; klm < JUMLAH_KOLOM; klm++) {
            papan[brs][klm] = ' '; // Mengisi spasi kosong
        }
    }
}

void gambarPapan(char papan[JUMLAH_BARIS][JUMLAH_KOLOM], int skorP1, int skorP2) {
    bersihkanLayar();

    cout << "P1 (X): " << skorP1 << "  VS  P2 (O): " << skorP2 << "\n\n";
    cout << "  1   2   3   4   5   6   7 \n"; 
    cout << "+---+---+---+---+---+---+---+\n"; 

    // Menggambar papan baris demi baris
    for (int brs = 0; brs < JUMLAH_BARIS; brs++) {
        cout << "|"; 
        for (int klm = 0; klm < JUMLAH_KOLOM; klm++) {
            cout << " " << papan[brs][klm] << " |"; 
        }
        cout << "\n";
        cout << "+---+---+---+---+---+---+---+\n"; 
    }
}

// [MODUL 7] Fungsi yang mengembalikan nilai integer (return int)
int prosesJatuhkanKeping(char papan[JUMLAH_BARIS][JUMLAH_KOLOM], int kolomPilihan, char keping) {
    // Algoritma Gravitasi: Loop mundur dari bawah (index 5) ke atas (index 0)
    for (int brs = JUMLAH_BARIS - 1; brs >= 0; brs--) {
        // Jika ketemu kotak kosong (' '), isi dengan keping
        if (papan[brs][kolomPilihan] == ' ') {
            papan[brs][kolomPilihan] = keping;
            return brs; // Beritahu baris ke berapa keping itu jatuh
        }
    }
    return -1; // Kode -1 artinya kolom penuh
}

bool cekApakahSeri(int totalLangkah) {
    // [MODUL 3] Operator Perkalian (*)
    // Jika total langkah sama dengan luas papan (6x7 = 42), berarti penuh
    if (totalLangkah == JUMLAH_BARIS * JUMLAH_KOLOM) return true;
    else return false;
}

// [MODUL 8] Algoritma Pencarian Pola (Pattern Matching) pada Matriks
bool cekApakahMenang(char papan[JUMLAH_BARIS][JUMLAH_KOLOM], char keping) {
    for (int brs = 0; brs < JUMLAH_BARIS; brs++) {
        for (int klm = 0; klm < JUMLAH_KOLOM; klm++) {
            
            // 1. CEK MENDATAR (HORIZONTAL)
            // [MODUL 3] Operator Logika AND (&&) memastikan 4 kotak isinya sama
            if (klm + 3 < JUMLAH_KOLOM) { 
                if (papan[brs][klm] == keping && 
                    papan[brs][klm + 1] == keping &&
                    papan[brs][klm + 2] == keping && 
                    papan[brs][klm + 3] == keping) return true;
            }

            // 2. CEK TEGAK (VERTIKAL)
            if (brs + 3 < JUMLAH_BARIS) { 
                if (papan[brs][klm] == keping && 
                    papan[brs + 1][klm] == keping &&
                    papan[brs + 2][klm] == keping && 
                    papan[brs + 3][klm] == keping) return true;
            }

            // 3. CEK MIRING KANAN (DIAGONAL)
            if (brs + 3 < JUMLAH_BARIS && klm + 3 < JUMLAH_KOLOM) {
                if (papan[brs][klm] == keping && 
                    papan[brs + 1][klm + 1] == keping &&
                    papan[brs + 2][klm + 2] == keping && 
                    papan[brs + 3][klm + 3] == keping) return true;
            }

            // 4. CEK MIRING KIRI (DIAGONAL)
            if (brs - 3 >= 0 && klm + 3 < JUMLAH_KOLOM) {
                if (papan[brs][klm] == keping && 
                    papan[brs - 1][klm + 1] == keping &&
                    papan[brs - 2][klm + 2] == keping && 
                    papan[brs - 3][klm + 3] == keping) return true;
            }
        }
    }
    return false; // Tidak ada yang menang
}
