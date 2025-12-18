#include <iostream>
#include <optional>
#include <fstream>
#include <vector>
#include <queue>
#include <string>

constexpr int MAX_FIELD_SIZE = 100;

struct Args
{
    std::optional<std::string> input{};
    std::optional<std::string> output{};
    bool showHelp{ false };
};

std::optional<Args> ParseArgs(int argc, char* argv[])
{
    Args args{};
    if (argc == 1)
    {
        return args;
    }

    if (argc == 2 && std::string(argv[1]) == "-h")
    {
        args.showHelp = true;
        return args;
    }

    if (argc == 3)
    {
        args.input = argv[1];
        args.output = argv[2];
        return args;
    }

    return std::nullopt;
}

void PrintHelp()
{
    std::cout << "Usage:" << "\n\n";

    std::cout << "\t" << "To work with stdin & stdout:  fill.exe" << "\n";
    std::cout << "\t" << "To get brief help:            fill.exe -h" << "\n";
    std::cout << "\t" << "To work with two text files:  fill.exe <input.txt> <output.txt>" << "\n\n";
}

using Grid = std::vector<std::string>;

void NormalizeGrid(Grid& grid)
{
    size_t maxLength = 0;
    for (const auto& row : grid)
    {
        if (row.length() > maxLength)
        {
            maxLength = row.length();
        }
    }

    for (auto& row : grid)
    {
        if (row.length() < maxLength)
        {
            row.append(maxLength - row.length(), ' ');
        }
    }
}

void FloodFill(Grid& grid, int x, int y)
{
    const int rows = grid.size();
    if (rows == 0) return;

    const int cols = grid[0].size();
    std::queue<std::pair<int, int>> que{};
    que.push({ x, y });
    char initialChar = grid[x][y];

    while (!que.empty())
    {
        auto [cx, cy] = que.front();
        que.pop();

        if (cx < 0 || cx >= rows || cy < 0 || cy >= cols)
        {
            continue;
        }

        if (grid[cx][cy] == '#' || grid[cx][cy] == '.')
        {
            continue;
        }

        grid[cx][cy] = '.';

        que.push({ cx + 1, cy });
        que.push({ cx - 1, cy });
        que.push({ cx, cy + 1 });
        que.push({ cx, cy - 1 });
    }

    grid[x][y] = initialChar;
}

void ProcessGrid(Grid& grid)
{
    NormalizeGrid(grid);

    const int rows = grid.size();
    const int cols = grid[0].size();

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (grid[i][j] == 'O')
            {
                FloodFill(grid, i, j);
            }
        }
    }
}

bool ReadGrid(std::istream& input, Grid& grid)
{
    std::string line{};
    while (std::getline(input, line))
    {
        if (grid.size() >= MAX_FIELD_SIZE)
        {
            break;
        }

        if (line.size() > MAX_FIELD_SIZE)
        {
            line.resize(MAX_FIELD_SIZE);
        }

        grid.push_back(line);
    }

    return true;
}

bool PrintGrid(std::ostream& output, const Grid& grid)
{
    for (const auto& row : grid)
    {
        output << row << "\n";
    }

    return true;
}

int RunProcessWithFiles(const std::string& input, const std::string& output)
{
    std::ifstream inputFile{ input };
    if (!inputFile.is_open())
    {
        std::cerr << "Failed to open input file" << "\n";
        return 1;
    }

    std::ofstream outputFile{ output };
    if (!outputFile.is_open())
    {
        std::cerr << "Failed to open output file" << "\n";
        return 1;
    }

    Grid grid{};
    if (!ReadGrid(inputFile, grid))
    {
        std::cerr << "Failed to scan grid" << "\n";
        return 1;
    }

    ProcessGrid(grid);
    if (!PrintGrid(outputFile, grid))
    {
        std::cerr << "Failed to print grid" << "\n";
        return 1;
    }

    return 0;
}

int RunProcessWithStdIO()
{
    Grid grid{};
    if (!ReadGrid(std::cin, grid))
    {
        std::cerr << "Failed to scan grid" << "\n";
        return 1;
    }

    ProcessGrid(grid);
    if (!PrintGrid(std::cout, grid))
    {
        std::cerr << "Failed to print grid" << "\n";
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[])
{
    auto args = ParseArgs(argc, argv);
    if (!args)
    {
        std::cerr << "ERROR! To show brief manual:  fill.exe -h" << "\n\n";
        return 1;
    }

    if (args->showHelp)
    {
        PrintHelp();
        return 0;
    }

    if (args->input && args->output)
    {
        return RunProcessWithFiles(*args->input, *args->output);
    }
    else
    {
        return RunProcessWithStdIO();
    }
}
