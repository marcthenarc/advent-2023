#include <iostream>
#include <algorithm>
#include <functional>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

// Number extraction while cleaning up extra bits at the end.
int ToNumber(const std::string& num)
{
    std::ostringstream oss;

    for (char c : num)
    {
        if (c < '0' || c > '9')
            continue;

        oss << c;
    }

    return std::atoi(oss.str().c_str());
}

class Set
{
public:
    int r, g, b;
};

class Game
{
public:

    Game()
    {
        sets.push_back(Set());
        it = sets.begin();
    }
    
    bool AddSubset(int amount, std::string color)
    {
        bool changeSet = false;

        // Does it have some ponctuation at the end
        switch (color[color.size() - 1])
        {
            case ';':
                color = color.substr(0, color.size() - 1);
                changeSet = true;
                break;

            case ',':
                color = color.substr(0, color.size() - 1);
                break;
        }

        Set& set = *it;

        if (color == "red")
            set.r = amount;
        else if (color == "green")
            set.g = amount;
        else if (color == "blue")
            set.b = amount;

        bool validate_part1 = set.r <= 12 && set.g <= 13 && set.b <= 14;

        if (changeSet)
        {
            sets.push_back(Set());
            it = sets.begin() + sets.size() - 1;
        }

        return validate_part1;
    }

    int CalculatePower()
    {
        int r = 0, g = 0, b = 0;

        for (const auto& set : sets)
        {
            if (r < set.r)
                r = set.r;

            if (b < set.b)
                b = set.b;

            if (g < set.g)
                g = set.g;
        }

        return r * g * b;
    }

protected:

    std::vector<Set> sets;
    std::vector<Set>::iterator it;
};

void readline(const std::string& filename, std::function<void(std::istringstream&)> predicate)
{
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            predicate(iss);
        }

        file.close();
    }
}

int usage()
{
    std::cout << "usage: a.out -p1|-p2 <input file>\n";
    return 1;
}

int main(int argc, char** argv)
{
    if (argc < 3)
        return usage();

    bool p2 = false;
    bool debug = false;

    if (std::string(argv[1]) == "-p2")
    {
        p2 = true;
    }
    else if (std::string(argv[1]) != "-p1")
    {
        // Fail, neither -p1 not -p2 was selected
        return usage();
    }

    std::string game, id, amount, color;
    int acc = 0;

    readline(argv[2], [&](std::istringstream& iss) {
        iss >> game >> id;

        // For part one
        bool valid = true;

        Game game;

        while (iss >> amount >> color)
            valid &= game.AddSubset(ToNumber(amount), color);

        if (p2)
            acc += game.CalculatePower();
        else
            acc += (valid) ? ToNumber(id) : 0;
    });

    std::cout << acc << "\n";

    return 0;
}
