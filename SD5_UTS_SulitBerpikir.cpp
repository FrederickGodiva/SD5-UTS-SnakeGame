#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <limits>
#include <string>
#include <windows.h>
#include <iomanip>

class Player {
    private:
        std::string name;
        int position;

    public:
        Player(const std::string& name) : name(name), position(0) {}

        const std::string& getName() const {
            return name;
        }

        int getPosition() const {
            return position;
        }

        void setPosition(int newPos) {
            position = newPos;
        }
};


class SnakeAndLadderGame {
    private:
        void displayBoard() const;
        void takeTurn(Player& player);
        int rollDice() const;
        void displayNarrative(int newPosition) const;

        const static int boardSize = 100;
        std::vector<Player> players;

    public:
        SnakeAndLadderGame(int numPlayers);

        void play();
};

SnakeAndLadderGame::SnakeAndLadderGame(int numPlayers) {
    if (numPlayers < 2 || numPlayers > 4) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        std::cerr << "Jumlah pemain tidak valid. Tekan CTRL + ALT + N, dan masukkan angka antara 2 dan 4." << std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        exit(EXIT_FAILURE);
    }

    std::cin.ignore();

    for (int i = 1; i <= numPlayers; ++i) {
        std::string playerName;
        std::cout << "Masukkan nama Pemain " << i << ": ";
        getline(std::cin, playerName);
        players.emplace_back(playerName);
    }

    std::cout << std::endl;

    // Seed for random number generation
    std::srand(std::time(0));
}

void SnakeAndLadderGame::displayBoard() const {
    std::cout << "Kondisi papan saat ini:" << std::endl;

    for (const auto& player : players) {
        if (player.getPosition() < boardSize) {
            std::cout << player.getName() << " ada di posisi " << player.getPosition() << std::endl;
        }
    }

    std::cout << std::endl;
}

void SnakeAndLadderGame::displayNarrative(int newPosition) const {
    std::string quotes[] = {
        "Tiada hal yang mustahil.",
        "Tetap semangat dan jangan pernah menyerah.",
        "Kamu pasti bisa.",
        "Yok, bisa yokk, semangat.",
        "Percaya pada proses.",
        "Pelan-pelan menuju kemenangan.",
        "Tidak ada kata gagal dalam hidup ini.",
        "Cobaan adalah proses pembelajaran.",
        "Ayo Fokus, kamu sudah memulai, maka selesaikan.",
        "Bermain sportif tetap diutamakan.",
        "Bermimpilah besar dan berani gagal.",
        "Jatuh, agar kita dapat bangkit.",
        "Kenyamanan adalah musuh pencapaian.",
        "Berbahagialah, tapi jangan pernah puas.",
        "Kalau ada usaha, pasti ada jalan."
    };
    int randomQuotes = rand() % 15;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
    if (newPosition!=13 && newPosition!=16 && newPosition!=23 && newPosition!=47 && newPosition!=49 && newPosition!=56 && newPosition!=64 &&
        newPosition!=69 && newPosition!=87 && newPosition!=93 && newPosition!=95 && newPosition!=98 && newPosition!=2 && newPosition!=4 && 
        newPosition!=9 && newPosition!=21 && newPosition!=28 && newPosition!=36 && newPosition!=41 && newPosition!=51 && newPosition!=70 && 
        newPosition!=80)
    {
        std::cout << quotes[randomQuotes] << std::endl;
    }
    
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void SnakeAndLadderGame::takeTurn(Player& player) {
    int roll;
    
    do {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
        std::cout << "\nTekan ENTER untuk melempar dadu...";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        roll = rollDice();
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        std::cout << player.getName() << " mendapat angka " << roll << std::endl << std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

        int newPosition = player.getPosition() + roll;
        displayNarrative(newPosition);

        if (newPosition > boardSize) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            std::cout << "Santai dong " << player.getName() << ", kebanyakan, jadi sisanya jalan mundur ya." << std::endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            int backwardDistance = newPosition - boardSize;
            newPosition = boardSize - backwardDistance;
        }

        // Define snakes and ladders
        static const std::vector<std::pair<int, int>> snakes = { 
            {13, 1}, {16, 6},  {23, 15}, {47, 27}, {49, 11}, {56, 53}, 
            {64, 58}, {69, 13}, {87, 24}, {93, 73}, {95, 75}, {98, 78}
        };
        static const std::vector<std::pair<int, int>> ladders = { 
            {2, 37}, {4, 14}, {9, 31}, {21, 42}, {28, 84}, 
            {36, 44}, {41, 63}, {51, 67}, {70, 91}, {80, 99} 
        };

        // Check for snakes
        auto snake = std::find_if(snakes.begin(), snakes.end(), [newPosition](const auto& s) { return s.first == newPosition; });
        if (snake != snakes.end()) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
            std::cout << "Apaan tuh? " << player.getName() << " Sedang sial dan kepleset di batu berlumut. Turun ke posisi " << snake->second << std::endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            newPosition = snake->second;
        }

        // Check for ladders
        auto ladder = std::find_if(ladders.begin(), ladders.end(), [newPosition](const auto& l) { return l.first == newPosition; });
        if (ladder != ladders.end()) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
            std::cout << "Hari ini " << player.getName() << " sedang merasa beruntung, dan dipindahkan ke posisi " << ladder->second << std::endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            newPosition = ladder->second;
        }

        player.setPosition(newPosition);

        if (roll == 1 && player.getPosition() != boardSize) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
            std::cout<<"Lempar dadu sekali lagi..."<<std::endl;
        }

    } while (roll == 1 && player.getPosition() != boardSize);

        if (player.getPosition() == boardSize && players.size()>=2) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
        std::cout << player.getName() << " telah berhasil menyelesaikan permainan!" << std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        }
}

int SnakeAndLadderGame::rollDice() const {
    return std::rand() % 6 + 1;
}

void SnakeAndLadderGame::play() {
    // Shuffle the players to randomize the playing order
    std::random_shuffle(players.begin(), players.end());

    while (players.size() > 1) {
        for (auto& player : players) {
            displayBoard();
            takeTurn(player);
        }

        // Remove players who have reached or exceeded the board size
        players.erase(std::remove_if(players.begin(), players.end(), [](const auto& p) { return p.getPosition() >= boardSize; }), players.end());

        // Check if there is only one player left
        if (players.size() == 1) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
            std::cout << "Dengan ini " << players[0].getName() << " adalah pemain terakhir dan dinyatakan kalah, silahkan menerima hukuman anda." << std::endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        }
    }
}

int main() {
    system("CLS");

    int numPlayers;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    std::cout << std::string(48, '=') << std::endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
    std::cout << "Selamat Datang di Perjalanan yang akan Disesali!" << std::endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    std::cout << std::string(48, '=') << std::endl;

    std::cout << "Berapa orang yang mau bermain? (2-4): ";
    std::cin >> numPlayers;

    std::cout << std::endl;

    SnakeAndLadderGame game(numPlayers);
    game.play();

    return 0;
}