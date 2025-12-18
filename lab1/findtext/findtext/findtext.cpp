#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <cstdint>
#include <optional>

struct Args 
{
    std::string inputFileName;
    std::string desiredLine;
};

std::optional<Args> ParseArgs(int argc, char* argv[])
{
    if (argc != 3)
    {
        return std::nullopt;
    }

    Args args{};
    args.inputFileName = argv[1];
    args.desiredLine = argv[2];
    return args;
}

int FindTextInFile(std::istream& input, const std::string desiredLine)
{
    std::string line{};
    uint32_t lineNumber{ 0 };
    bool stringIsFound{ false };

    while (std::getline(input, line)) 
    {
        lineNumber++;   
        if (line.find(desiredLine) != std::string::npos) 
        {
            std::cout << lineNumber << "\n";
            stringIsFound = true;
        }
    }

    if (!stringIsFound) 
    {
        std::cout << "Text not found" << "\n";
        return 1;
    }

    return 0;
}

bool IsReadFileError(std::istream& input)
{
    if (input.bad()) 
    {
        std::cerr << "Failed to read input file" << "\n";
        return true;
    }
    return false;
}

int main(int argc, char* argv[]) 
{
    auto args = ParseArgs(argc, argv);
    if (!args) 
    {
        std::cerr << "Invalid params number" << "\n";
        std::cerr << "Usage: findtext.exe <file name> <text to search>" << "\n";
        return 1;
    }

    std::ifstream input(args->inputFileName);

    if (!input.is_open())
    {
        std::cerr << "Input file does not exist" << "\n";
        return 1;
    }

    if (IsReadFileError(input)) 
    {
        input.close();
        return 1;
    }

    return FindTextInFile(input, args->desiredLine);
}