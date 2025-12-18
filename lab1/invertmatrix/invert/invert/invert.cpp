#include <iostream>
#include <fstream>
#include <array>
#include <sstream>
#include <optional>
#include <iomanip>

constexpr int MATRIX_SIZE = 3;

struct Args
{
	std::string input{};
	bool showHelp{ false };
};

void PrintHelp()
{
	std::cout << "Usage:" << "\n";
	std::cout << "\t" << "To work with stdin & stdout:  invert.exe" << "\n";
	std::cout << "\t" << "To show brief manual:         invert.exe -h" << "\n";
	std::cout << "\t" << "To work with input file:      invert.exe <input.txt>" << "\n";
	std::cout << "The matrix is a table with 3 rows and 3 columns, separated by tabs in a row" << "\n";
	std::cout << "If no input file is expected, the matrix is output in the same format via stdin" << "\n\n";
}

std::optional<Args> ParseArgs(int argc, char* argv[])
{
	Args args{};
	if (argc > 2)
	{
		return std::nullopt;
	}

	if (argc == 2)
	{
		std::string argument = argv[1];
		if (argument == "-h")
		{
			args.showHelp = true;
		}
		else
		{
			args.input = argument;
		}
	}

	return args;
}

std::string Trim(const std::string& line)
{
	size_t begIndex = line.find_first_not_of(" \t\n");
	if (begIndex == std::string::npos)
	{
		return "";
	}

	size_t endIndex = line.find_last_not_of(" \t\n");
	return line.substr(begIndex, endIndex - begIndex + 1);
}

bool ParseInputLine(const std::string& line, std::array<double, MATRIX_SIZE>& row)
{
	std::istringstream isstream(line);
	for (int j = 0; j < MATRIX_SIZE; ++j)
	{
		std::string token{};
		if (!std::getline(isstream, token, '\t'))
		{
			return false;
		}

		token = Trim(token);
		try
		{
			size_t pos = 0;
			row[j] = std::stod(token, &pos);
			if (pos != token.length())
			{
				return false;
			}
		}
		catch (const std::exception&)
		{
			return false;
		}
	}

	std::string extra{};
	return !std::getline(isstream, extra) || Trim(extra).empty();
}

using Matrix = std::array<std::array<double, MATRIX_SIZE>, MATRIX_SIZE>;

bool ReadMatrix(std::istream& input, Matrix& matrix)
{
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		std::string line{};
		if (!std::getline(input, line) || Trim(line).empty() || !ParseInputLine(line, matrix[i]))
		{
			std::cout << "Invalid matrix format" << "\n";
			return false;
		}
	}

	std::string extraLine{};
	if (std::getline(input, extraLine) && !Trim(extraLine).empty())
	{
		std::cout << "Invalid matrix format" << "\n";
		return false;
	}

	return true;
}

double CalcDeterminant(const Matrix matrix)
{
	return matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1])
		- matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0])
		+ matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
}

void TransposeMatrix(const Matrix& matrix, Matrix& result)
{
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			result[i][j] = matrix[j][i];
		}
	}
}

void CalcCofactors(const Matrix& matrix, Matrix& cofactors)
{
	cofactors[0][0] = (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]);
	cofactors[0][1] = -(matrix[0][1] * matrix[2][2] - matrix[0][2] * matrix[2][1]);
	cofactors[0][2] = (matrix[0][1] * matrix[1][2] - matrix[0][2] * matrix[1][1]);

	cofactors[1][0] = -(matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]);
	cofactors[1][1] = (matrix[0][0] * matrix[2][2] - matrix[0][2] * matrix[2][0]);
	cofactors[1][2] = -(matrix[0][0] * matrix[1][2] - matrix[0][2] * matrix[1][0]);

	cofactors[2][0] = (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
	cofactors[2][1] = -(matrix[0][0] * matrix[2][1] - matrix[0][1] * matrix[2][0]);
	cofactors[2][2] = (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]);
}

void DivideMatrixByDeterminant(const Matrix& algCofactor, double det, Matrix& result)
{
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			result[i][j] = algCofactor[i][j] / det;
		}
	}
}

void PrintInvertMatrix(const Matrix& matrix)
{
	constexpr int PRECISION = 3;
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			std::cout << std::fixed << std::setprecision(PRECISION) << matrix[i][j];
			if (j < 2)
			{
				std::cout << "\t";
			}
		}
		std::cout << "\n";
	}
}

bool ReadMatrixFromStream(std::istream& input, Matrix& matrix)
{
	return ReadMatrix(input, matrix);
}

bool CalcInvertMatrix(const Matrix& matrix, Matrix& inverseMatrix)
{
	double det = CalcDeterminant(matrix);
	const double EPSILON = 1e-9;
	if (std::abs(det) < EPSILON)
	{
		std::cout << "Matrix is degenerate! Non-invertible" << "\n";
		return false;
	}

	Matrix cofactors{};
	CalcCofactors(matrix, cofactors);
	Matrix adjugate{};
	TransposeMatrix(cofactors, adjugate);

	DivideMatrixByDeterminant(adjugate, det, inverseMatrix);
	return true;
}

int ProcessMatrix(std::istream& input)
{
	Matrix matrix{};
	if (!ReadMatrixFromStream(input, matrix))
	{
		return 1;
	}

	Matrix inverseMatrix{};
	if (!CalcInvertMatrix(matrix, inverseMatrix))
	{
		return 0;
	}

	PrintInvertMatrix(inverseMatrix);
}

int main(int argc, char* argv[]) 
{
	auto args = ParseArgs(argc, argv);
	if (!args)
	{
		PrintHelp();
		return 1;
	}

	if (args->showHelp)
	{
		PrintHelp();
		return 0;
	}

	if (args->input.empty())
	{
		return ProcessMatrix(std::cin);
	}

	std::ifstream input{ args->input };
	if (!input.is_open())
	{
		std::cout << "Failed to open input file!" << "\n";
		return 1;
	}
	return ProcessMatrix(input);
}