#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>
#include "TebakKata.h"

using namespace std;

// ─────────────────────────────────────────
//  Bank kata (15 kata)
// ─────────────────────────────────────────
static string bankKata[JUMLAH_KATA] = {
    "komputer",
    "algoritma",
    "keyboard",
    "internet",
    "program",
    "jaringan",
    "database",
    "variabel",
    "fungsi",
    "pointer",
    "compiler",
    "memori",
    "looping",
    "struktur",
    "rekursif"
};

void tampilJudul() {
    // Bersihkan layar
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    cout<<"\033[35m"<< " ================================= "<<"\033[0m"<<endl;
    cout<<"\033[35m"<<"|         GAME TEBAK KATA         | "<<"\033[0m"<<endl;
    cout<<"\033[35m"<< " ================================= "<<"\033[0m"<<endl;
}

// ─────────────────────────────────────────
//  Inisialisasi game via pointer
// ─────────────────────────────────────────
void initGame(KataGame* game, int difficulty) {

    int indexRandom = rand() % JUMLAH_KATA;
    game->kataRahasia = bankKata[indexRandom];

    // Buat status awal berupa underscore
    game->statusTebakan = "";
    for (int i = 0; i < (int)game->kataRahasia.length(); i++) {
        game->statusTebakan += '_';
    }

    // Set nyawa sesuai difficulty
    if (difficulty == 1) {
        game->sisaNyawa=8;
    }else if (difficulty == 2){
        game->sisaNyawa=6;
    }else{
        game->sisaNyawa=4;
    }                      
    // Reset riwayat salah
    game->jumlahSalah = 0;
    for (int i = 0; i < 26; i++) {
        game->hurufSalah[i] = '\0';
    }
}

// ─────────────────────────────────────────
//  Tampil state permainan
// ─────────────────────────────────────────
void tampilGame(KataGame* game) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    cout<<"\033[35m"<< " ================================= "<<"\033[0m"<<endl;
    cout<<"\033[35m"<<"|         GAME TEBAK KATA         | "<<"\033[0m"<<endl;
    cout<<"\033[35m"<< " ================================= "<<"\033[0m"<<endl;
    
    // Tampil kata dengan spasi antar huruf
    cout<<"\033[96m"<< "\nKata  : ";
    for (int i = 0; i < (int)game->statusTebakan.length(); i++) {
        cout << game->statusTebakan[i] << " ";
    }
    cout << endl;

    cout << "Nyawa : " << game->sisaNyawa << endl;

    cout << "Salah : "<<"\033[0m";
    for (int i = 0; i < game->jumlahSalah; i++) {
        cout << game->hurufSalah[i];
        if (i < game->jumlahSalah - 1) cout << ", ";
    }
    cout <<"\033[35m"<<"\n===============================" <<"\033[0m"<<endl;
}

// ─────────────────────────────────────────
//  Cek duplikat (sudah pernah ditebak)
// ─────────────────────────────────────────
bool cekDuplikat(KataGame* game, char tebakan) {

    // Cek di huruf yang sudah terbuka (benar)
    for (int i = 0; i < (int)game->statusTebakan.length(); i++) {
        if (game->statusTebakan[i] == tebakan) return true;
    }

    // Cek di riwayat huruf salah
    for (int i = 0; i < game->jumlahSalah; i++) {
        if (game->hurufSalah[i] == tebakan) return true;
    }

    return false;
}

// ─────────────────────────────────────────
//  Proses tebakan via pointer
// ─────────────────────────────────────────
void prosesTebakan(KataGame* game, char tebakan) {

    bool ditemukan = false;

    for (int i = 0; i < (int)game->kataRahasia.length(); i++) {
        if (game->kataRahasia[i] == tebakan) {
            game->statusTebakan[i] = tebakan;
            ditemukan = true;
        }
    }

    if (!ditemukan) {
        game->hurufSalah[game->jumlahSalah] = tebakan;
        game->jumlahSalah++;
        game->sisaNyawa--;
        cout<<"\033[37m"<< "\n  Huruf '" << tebakan << "' tidak ada dalam kata!\n";
    } else {
        cout << "\n  Huruf '" << tebakan << "' ditemukan!\n"<<"\033[0m";
    }
}

// ─────────────────────────────────────────
//  Cek kondisi menang
// ─────────────────────────────────────────
bool apakahMenang(KataGame* game) {
    return game->statusTebakan == game->kataRahasia;
}

// ─────────────────────────────────────────
//  Hitung skor
// ─────────────────────────────────────────
int hitungSkor(int sisaNyawa, int difficulty) {
    int multiplier = 1;
    if      (difficulty == 2) multiplier = 2;
    else if (difficulty == 3) multiplier = 3;
    return sisaNyawa * 10 * multiplier;
}

// ─────────────────────────────────────────
//  Update leaderboard (insert & sort)
// ─────────────────────────────────────────
void updateLeaderboard(Skor leaderboard[], int& jumlahEntry, Skor skorBaru) {

    if (jumlahEntry < MAX_LEADERBOARD) {
        leaderboard[jumlahEntry] = skorBaru;
        jumlahEntry++;
    } else {
        // Ganti skor terendah jika skor baru lebih tinggi
        int indexMin = 0;
        for (int i = 1; i < MAX_LEADERBOARD; i++) {
            if (leaderboard[i].nilai < leaderboard[indexMin].nilai) {
                indexMin = i;
            }
        }
        if (skorBaru.nilai > leaderboard[indexMin].nilai) {
            leaderboard[indexMin] = skorBaru;
        }
    }

    // Urutkan descending (bubble sort)
    for (int i = 0; i < jumlahEntry - 1; i++) {
        for (int j = 0; j < jumlahEntry - i - 1; j++) {
            if (leaderboard[j].nilai < leaderboard[j + 1].nilai) {
                Skor temp        = leaderboard[j];
                leaderboard[j]   = leaderboard[j + 1];
                leaderboard[j + 1] = temp;
            }
        }
    }
}

// ─────────────────────────────────────────
//  Tampil leaderboard
// ─────────────────────────────────────────
void tampilLeaderboard(Skor leaderboard[], int jumlahEntry) {

    cout<<"\033[35m"<< "\n===============================" << endl;
    cout << "         LEADERBOARD           " << endl;
    cout << "===============================" << endl;

    if (jumlahEntry == 0) {
        cout << "  Belum ada skor tersimpan.\n";
    } else {
        for (int i = 0; i < jumlahEntry; i++) {
            cout << "  " << (i + 1) << ". "
                 << leaderboard[i].nama
                 << " - " << leaderboard[i].nilai
                 << " poin\n";
        }
    }

    cout << "==============================="<<"\033[0m"<<endl;
}

// ─────────────────────────────────────────
//  Fitur hint: buka huruf pertama, -1 nyawa
// ─────────────────────────────────────────
void prosesHint(KataGame* game) {

    if (game->sisaNyawa <= 1) {
        cout << "\n  Nyawa tidak cukup untuk hint!\n";
        return;
    }

    // Buka huruf pertama yang masih underscore
    for (int i = 0; i < (int)game->kataRahasia.length(); i++) {
        if (game->statusTebakan[i] == '_') {
            char hurufHint = game->kataRahasia[i];
            // Buka semua posisi huruf tersebut
            for (int j = 0; j < (int)game->kataRahasia.length(); j++) {
                if (game->kataRahasia[j] == hurufHint) {
                    game->statusTebakan[j] = hurufHint;
                }
            }
            game->sisaNyawa--;
            cout << "\n  [HINT] Huruf '" << hurufHint
                 << "' dibuka! Nyawa berkurang 1.\n";
            return;
        }
    }

    cout << "\n  Tidak ada huruf yang bisa dibuka!\n";
}
