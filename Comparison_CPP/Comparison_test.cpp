/*
 * Задача: (23)DataComparison
 * Група: Комп'ютерна математика 2 курс
 * Студент: Лазаренко
 * Опис: Тестовий файл. Містить меню для вибору режиму роботи (консоль/файл),
 * демонстрацію роботи функцій та вивід результатів. Мова С++
 */

#include "Comparison.h"
#include <iostream>
#include <limits>

void displayMenu() {
    std::cout << "\n=== MENU ===\n";
    std::cout << "1. Add manual element (pair)\n";
    std::cout << "2. Add random data (Normal Dist)\n";
    std::cout << "3. Load from file (Comparison_test.txt)\n";
    std::cout << "4. Save to file (Comparison_test.txt)\n";
    std::cout << "5. Show Data\n";
    std::cout << "6. Calculate Metrics (Classification & Math)\n";
    std::cout << "0. Exit\n";
    std::cout << "Select: ";
}

int main() {
    Comparison comp;
    
    comp.setFuncDistr([](double i) { return 1.0 / (1.0 + i * 0.1); });
    comp.setFuncLoss([](double diff) { return diff * diff; });

    int choice;
    do {
        displayMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
        }

        switch (choice) {
            case 1: {
                double v1, v2;
                std::cout << "Enter R1 val and R2 val: ";
                std::cin >> v1 >> v2;
                comp.addElement(v1, v2);
                break;
            }
            case 2: {
                size_t n;
                double mean, stddev;
                std::cout << "Enter count, mean, stddev: ";
                std::cin >> n >> mean >> stddev;
                comp.addRandomData(n, mean, stddev);
                std::cout << "Added " << n << " random elements.\n";
                break;
            }
            case 3:
                comp.loadFromFile("Comparison_test.txt");
                break;
            case 4:
                comp.saveToFile("Comparison_test.txt");
                break;
            case 5:
                comp.printData();
                break;
            case 6: {
                if (comp.getSize() == 0) {
                    std::cout << "No data to analyze.\n";
                    break;
                }
                std::cout << "\n--- Binary Classification Metrics (Threshold > 0.5) ---\n";
                std::cout << "TP: " << comp.TP() << ", FP: " << comp.FP() << "\n";
                std::cout << "FN: " << comp.FN() << ", TN: " << comp.TN() << "\n";
                std::cout << "Accuracy (ACC): " << comp.ACC() << "\n";
                std::cout << "Precision (PPV): " << comp.PPV() << "\n";
                std::cout << "Recall (TPR): " << comp.TPR() << "\n";
                std::cout << "F1 Score: " << comp.F1() << "\n";

                std::cout << "\n--- Math & Distance Metrics ---\n";
                std::cout << "Euclidean Dist: " << comp.dist() << "\n";
                std::cout << "Correlation: " << comp.corr() << "\n";
                std::cout << "Dist_F (Weighted Indices): " << comp.dist_f() << "\n";
                std::cout << "Loss (Func on Diff): " << comp.loss() << "\n";
                std::cout << "Dist_F_Loss (Combined): " << comp.dist_f_loss() << "\n";
                break;
            }
            case 0:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
