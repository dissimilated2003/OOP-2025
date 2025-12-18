/*
    magicNumbers
*/

#include <iostream>
#include <cstdint>
#include <string>
#include <optional>
#include <charconv>
#include <vector>

struct Args 
{
    std::optional<uint64_t> number;
    std::optional<uint64_t> findLimit;
};

bool IsMagicNumber(uint64_t number) 
{
    if (number <= 9) 
    {
        return false;
    }

    uint64_t digitSum = 0;
    uint64_t originalNumber = number;
    while (number != 0) 
    {
        digitSum += number % 10;
        number /= 10;
    }

    return (originalNumber - digitSum) % 9 != 0;
}

std::vector<uint64_t> FindMagicNumbers(uint64_t limit) 
{
    std::vector<uint64_t> magicNumbers;
    for (uint64_t i = 1; i <= limit; ++i) 
    {
        if (IsMagicNumber(i)) 
        {
            magicNumbers.push_back(i);
        }
    }
    return magicNumbers;
}

std::optional<uint64_t> ParseNumber(const std::string& str) 
{
    uint64_t value;
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);

    bool isSuccessfulParsed = ec == std::errc() && ptr == str.data() + str.size();
    if (isSuccessfulParsed) 
    {
        return value;
    }

    return std::nullopt;
}

std::optional<Args> ParseArgs(int argc, char* argv[]) 
{
    Args args;
    if (argc == 2) 
    {
        args.number = ParseNumber(argv[1]);
    }
    else if (argc == 3) 
    {
        std::string flag = argv[1];
        if (flag == "-find") 
        {
            args.findLimit = ParseNumber(argv[2]);
        }
    }

    if (!args.number && !args.findLimit) 
    {
        return std::nullopt;
    }

    return args;
}

int RunProgram(const Args& args) 
{
    if (args.findLimit) 
    {
        auto magicNumbers = FindMagicNumbers(*args.findLimit);
        std::cout << "Magic numbers from 0 to " << *args.findLimit << ":\n";
        for (auto number : magicNumbers) 
        {
            std::cout << number << "\n";
        }
    }
    else if (args.number) 
    {
        if (IsMagicNumber(*args.number)) 
        {
            std::cout << "Magic" << "\n";
        }
        else 
        {
            std::cout << "Non-magic" << "\n";
        }
    }

    return 0;
}

int main(int argc, char* argv[]) 
{
    auto args = ParseArgs(argc, argv);
    if (!args) 
    {
        std::cerr << "Error" << "\n";
        return 1;
    }

    return RunProgram(*args);
}