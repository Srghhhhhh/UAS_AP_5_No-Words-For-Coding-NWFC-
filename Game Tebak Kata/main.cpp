#include <iostream>
#include <string>
#include <cctype>
#include <ctime>
#include <cstdlib>
#include "TebakKata.h"

using namespace std;

int main() {

    srand(time(0));

    // ── Data leaderboard untuk sesi ini ──
    Skor leaderboard[MAX_LEADERBOARD];
    int  jumlahEntry = 0;

    // ── Minta nama pemain sekali di awal ──
    tampilJudul();
    cout<<"\033[94m"<< "Tekan Enter untuk mulai...";
    cin.ignore(1000, '\n');  // ganti cin.get() dengan ini
    string namaPemain;
    cout << "Masukkan nama Anda : "<<"\033[0m";
    getline(cin, namaPemain);

    char ulang = 'Y';

    while (ulang == 'Y' || ulang == 'y') {

        // ── Pilih difficulty ──
        tampilJudul();
        cout << "\033[94m"<<"Halo, " << "\033[0m"<<namaPemain << "!\n\n";
        cout << "\033[94m"<<"Pilih Tingkat Kesulitan:\n";
        cout << "  1. Easy   (8 nyawa)\n";
        cout << "  2. Medium (6 nyawa)\n";
        cout << "  3. Hard   (4 nyawa)\n";
        cout << "\nPilihan : ";

        int difficulty;
        while (!(cin >> difficulty) || difficulty < 1 || difficulty > 3) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Input tidak valid! Masukkan 1, 2, atau 3 : ";
        }
        cin.ignore(1000, '\n');
        cout<<"\033[0m";

        // ── Inisialisasi ronde baru ──
        KataGame game;
        initGame(&game, difficulty);

        // ── Loop putaran ──
        while (!apakahMenang(&game) && game.sisaNyawa > 0) {

            tampilGame(&game);

            cout <<"\033[96m"<< "Tebak huruf (atau '?' untuk hint) : ";
            string inputStr;
            getline(cin, inputStr);

            // Validasi: harus tepat 1 karakter
            if (inputStr.length() != 1) {
                cout << "\n  Masukkan tepat 1 huruf!\n";
                cout << "  Tekan Enter untuk lanjut...";
                cin.get();
                continue;
            }

            char tebakan = tolower(inputStr[0]);

            // Fitur hint
            if (tebakan == '?') {
                prosesHint(&game);
                cout << "  Tekan Enter untuk lanjut...";
                cin.get();
                continue;
            }

            // Validasi: harus huruf alfabet
            if (!isalpha(tebakan)) {
                cout << "\n  Input harus berupa huruf!\n";
                cout << "  Tekan Enter untuk lanjut...";
                cin.get();
                continue;
            }

            // Cek duplikat
            if (cekDuplikat(&game, tebakan)) {
                cout << "\n  Huruf '" << tebakan
                     << "' sudah pernah ditebak!\n";
                cout << "  Tekan Enter untuk lanjut...";
                cin.get();
                continue;
            }

            // Proses tebakan
            prosesTebakan(&game, tebakan);
            cout << "  Tekan Enter untuk lanjut..."<<"\033[0m";
            cin.get();
        }

        // ── Hasil akhir ronde ──
        tampilGame(&game);

        if (apakahMenang(&game)) {

            int skor = hitungSkor(game.sisaNyawa, difficulty);

            cout<<"\033[92m"<< "\n  *** SELAMAT, ANDA MENANG! ***\n";
            cout << "  Skor Anda : " << skor << " poin\n"<<"\033[0m";

            // Simpan ke leaderboard
            Skor skorBaru;
            skorBaru.nama  = namaPemain;
            skorBaru.nilai = skor;
            updateLeaderboard(leaderboard, jumlahEntry, skorBaru);

        } else {

            cout<<"\033[91m" << "\n  GAME OVER! Nyawa habis.\n";
            cout << "  Kata rahasia : "
                 << game.kataRahasia << "\n"<<"\033[0m";
        }

        // ── Tampil leaderboard ──
        tampilLeaderboard(leaderboard, jumlahEntry);

        // ── Tanya replay ──
        cout << "\nApakah Anda ingin bermain lagi? (Y/N) : ";
        string replayStr;
        getline(cin, replayStr);
        ulang = replayStr.empty() ? 'N' : toupper(replayStr[0]);
    }

    cout << "\nTerima kasih sudah bermain, "
         << namaPemain << "! Sampai jumpa!\n\n";

    return 0;
}
