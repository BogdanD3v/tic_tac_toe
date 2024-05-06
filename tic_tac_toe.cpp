#include <iostream>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>

struct Players
{
    int id;
    char sign;
    int wins;
    int loses;
    int draws;
    int points;
};

std::vector<Players> loadPlayersFromFile(const std::string& filename) {
    std::vector<Players> players;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Nie udało się otworzyć pliku!\n";
        return players;
    }

    int id;
    char sign;

    while (file >> id >> sign) {
        Players player;
        player.id = id;
        player.sign = sign;
        player.wins = 0;   // Inicjalizacja wartości dla każdego gracza
        player.loses = 0;
        player.draws = 0;
        player.points = 0;
        players.push_back(player);

        std::cout << "Wczytano gracza: ID: " << id << ", Symbol: " << sign << "\n"; // Komunikat diagnostyczny
    }

    file.close();

    return players;
}

std::vector<std::string> players_results(const std::string& filename) 
{
    std::vector<std::string> boards;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Nie udało się otworzyć pliku!\n";
    }

    std::string currentString;
    char c;
    int count = 0;

    while (file.get(c)) {
        if (c == '\n') {
            boards.push_back(currentString);
            currentString = "";
            count = 0;
        }
        else {
            currentString += c;
            count++;
            if (count == 3) {
                boards.push_back(currentString);
                currentString = "";
                count = 0;
            }
        }
    }

    if (!currentString.empty()) {
        boards.push_back(currentString);
    }

    file.close();

    return boards;
}

void show_table(const std::vector<Players>& players, const std::vector<std::string>& results) {
    // Przydzielanie punktów i aktualizacja statystyk graczy
    for (const auto& result : results) {
        for (auto& player : players) {
            if (player.sign == result[0]) {
                if (result[1] == 'w') {
                    player.wins++;
                    player.points += 3;
                }
                else if (result[1] == 'd') {
                    player.draws++;
                    player.points += 1;
                }
                else if (result[1] == 'l') {
                    player.loses++;
                }
            }
        }
    }

    // Sortowanie graczy malejąco według liczby punktów
    std::sort(players.begin(), players.end(), [](const Players& a, const Players& b) {
        return a.points > b.points;
        });

    // Wyświetlanie tabeli z wynikami
    std::cout << "ID\tSymbol\tWins\tLoses\tDraws\tPoints\n";
    for (const auto& player : players) {
        std::cout << player.id << "\t" << player.sign << "\t" << player.wins << "\t" << player.loses << "\t" << player.draws << "\t" << player.points << "\n";
    }
}

void show_results(const std::vector<std::string>& results) {
    std::cout << "Results of matches:\n";
    for (const auto& result : results) {
        std::cout << result << "\n";
    }
}

int main() 
{
    std::vector<Players> players = loadPlayersFromFile("players.txt");
    std::vector<std::string> results = players_results("tictactoe.txt");

    while (true) 
    {
        int choice;

        std::cout << "1 - show table" << "\n"
            << "2 - show results" << "\n"
            << "3 - show player stats" << "\n"
            << "4 - exit" << "\n"
            << "Choose option:" << "\n";

        std::cin >> choice;

        if (choice >= 1 && choice <= 4) 
        {
            system("cls");

            switch (choice) 
            {
            case 1:
                show_table(players, results);
                break;
            case 2:
                show_results(results);
                break;
            case 4:
                break;
            default:
                std::cout << "This option is not implemented yet." << "\n";
                break;
            }
        }
        else 
        {
            std::cout << "Invalid choice! Please choose a valid option." << "\n";
            std::cin.clear();
        }
    }

    return 0;
}
