#include <iostream>

#include "matrix2.h"

int main()
{
    Matrix<int, 2> matrix(0);

    for (int i = 0; i < 10; i++)
    {
        matrix[i][i] = i;
    }

    for (int i = 9; i >= 0; i--)
    {
        matrix[i][9 - i] = i;
    }

    for (int i = 1; i < 9; i++)
    {
        for (int j = 1; j < 9; j++)
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl << "size: " << matrix.size() << std::endl << std::endl;

    for (const auto& pair : matrix)
    {
       auto key = pair.first;
       std::cout << '[' << key[0] << "][" << key[1] << "] = " << pair.second << std::endl;
    }

    ((matrix[100][100] = 314) = 0) = 217;
    std::cout << matrix[100][100] << std::endl;

    return 0;
}
