#include <iostream>
#include <fstream>
#include <cmath>

int solveQuadraticEquation(double coefficientA, double coefficientB, double coefficientC, double& rootOne, double& rootTwo) {
    if (coefficientA == 0) {
        if (coefficientB == 0) {
            return (coefficientC == 0) ? -1 : 0;
        }
        rootOne = -coefficientC / coefficientB;
        return 1;
    }

    double discriminantValue = coefficientB * coefficientB - 4 * coefficientA * coefficientC;

    if (discriminantValue > 0) {
        rootOne = (-coefficientB + std::sqrt(discriminantValue)) / (2 * coefficientA);
        rootTwo = (-coefficientB - std::sqrt(discriminantValue)) / (2 * coefficientA);
        return 2;
    }
    else if (discriminantValue == 0) {
        rootOne = -coefficientB / (2 * coefficientA);
        return 1;
    }
    else {
        return 0;
    }
}

void printResults(std::ostream& outputStream, int rootsCount, double rootOne, double rootTwo) {
    if (rootsCount == 2) {
        outputStream << "Two roots: x1 = " << rootOne << ", x2 = " << rootTwo << std::endl;
    }
    else if (rootsCount == 1) {
        outputStream << "One root: x = " << rootOne << std::endl;
    }
    else if (rootsCount == 0) {
        outputStream << "No real roots." << std::endl;
    }
    else if (rootsCount == -1) {
        outputStream << "Infinite number of roots." << std::endl;
    }
}

int main() {
    int userChoice = 0;
    std::cout << "Select mode:\n1 - Console Input/Output\n2 - File Input/Output\nYour choice: ";
    std::cin >> userChoice;

    double coefficientA = 0, coefficientB = 0, coefficientC = 0;
    double rootOne = 0, rootTwo = 0;
    int rootsCount = 0;

    if (userChoice == 1) {
        std::cout << "Enter coefficients a, b, c: ";
        std::cin >> coefficientA >> coefficientB >> coefficientC;

        rootsCount = solveQuadraticEquation(coefficientA, coefficientB, coefficientC, rootOne, rootTwo);

        std::cout << "Result: ";
        printResults(std::cout, rootsCount, rootOne, rootTwo);
    }
    else if (userChoice == 2) {
        std::ifstream inputFile("input.txt");
        if (!inputFile.is_open()) {
            std::cerr << "Error: Could not open input.txt!" << std::endl;
            return 1;
        }

        inputFile >> coefficientA >> coefficientB >> coefficientC;
        inputFile.close();

        rootsCount = solveQuadraticEquation(coefficientA, coefficientB, coefficientC, rootOne, rootTwo);

        std::ofstream outputFile("output.txt");
        if (!outputFile.is_open()) {
            std::cerr << "Error: Could not open output.txt!" << std::endl;
            return 1;
        }

        printResults(outputFile, rootsCount, rootOne, rootTwo);
        outputFile.close();

        std::cout << "Done! Results saved to output.txt." << std::endl;
    }
    else {
        std::cout << "Invalid choice!" << std::endl;
    }

    return 0;
}