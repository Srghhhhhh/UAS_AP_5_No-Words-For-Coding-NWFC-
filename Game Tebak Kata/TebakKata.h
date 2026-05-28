#ifndef TEBAKKATA_H
#define TEBAKKATA_H

#include <string>
using namespace std;
struct KataGame {
    string kataRahasia;
    string statusTebakan;
    int sisaNyawa;
    int jumlahSalah;
    char hurufSalah[26];
};

struct Skor {
    string nama;
    int nilai;
};

// Konstanta
const int JUMLAH_KATA     = 15;
const int MAX_LEADERBOARD = 5;

// Deklarasi fungsi
void tampilJudul();
void initGame(KataGame* game, int difficulty);
void tampilGame(KataGame* game);
bool cekDuplikat(KataGame* game, char tebakan);
void prosesTebakan(KataGame* game, char tebakan);
bool apakahMenang(KataGame* game);
int  hitungSkor(int sisaNyawa, int difficulty);
void updateLeaderboard(Skor leaderboard[], int& jumlahEntry, Skor skorBaru);
void tampilLeaderboard(Skor leaderboard[], int jumlahEntry);
void prosesHint(KataGame* game);

#endif
