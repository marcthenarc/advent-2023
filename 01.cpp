#include <iostream>
#include <algorithm>
#include <functional>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <map>

static const std::vector<std::string> numbers = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

void readline(const std::string& filename, std::function<void(std::string)> predicate)
{
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            predicate(line);
        }

        file.close();
    }
}

void find_numbers_with_letters(const std::string& str, const std::string& search, std::map<size_t, int>& loc)
{
    size_t pos = 0;

    while (true)
    {
        pos = search.find(str, pos);

        if (pos == std::string::npos)
            break;

        loc[pos++] = (int)(std::find(numbers.begin(), numbers.end(), str) - numbers.begin());
    }
}

std::string find_numbers(const std::string& str, bool withLetters, bool debug)
{
    std::map<size_t, int> loc;

    if (withLetters)
    {
        for (auto num : numbers)
            find_numbers_with_letters(num, str, loc);
    }

    size_t pos = 0;

    for (auto c : str)
    {
        if (c >= '0' && c <= '9')
        {
            std::string num;
            num[0] = c;
            loc[pos] = std::atoi(num.c_str());
        }

        ++pos;
    }

    std::ostringstream oss;
    auto begin = loc.begin();
    auto end = loc.end();
    --end;
    oss << (*begin).second << (*end).second;
    return oss.str();
}

int usage()
{
    std::cout << "usage: a.out -p1|-p2 <input file> (-d)\n";
    return 1;
}

int main(int argc, char** argv)
{
    if (argc < 3)
        return usage();

    bool withLetters = false;
    bool debug = false;

    if (std::string(argv[1]) == "-p2")
    {
        withLetters = true;
    }
    else if (std::string(argv[1]) != "-p1")
    {
        // Fail, neither -p1 not -p2 was selected
        return usage();
    }

    if (argc == 4)
    {
        debug = std::string(argv[3]) == "-d";
    }

    int acc = 0;

    readline(argv[2], [&](std::string line) {

        std::string buf = find_numbers(line, withLetters, debug);

        acc += std::atoi(buf.c_str());

        if (debug)
            std::cout << line << ": " << buf << ": " << acc << "\n";
        });

    std::cout << acc << "\n";

    return 0;
}
