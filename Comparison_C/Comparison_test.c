/*
 * Задача: (23)DataComparison
 * Група: Комп'ютерна математика 2 курс
 * Студент: Лазаренко
 * Опис: Тестовий файл. Містить меню для вибору режиму роботи (консоль/файл),
 * демонстрацію роботи функцій та вивід результатів. Мова С
 */
#include "Comparison.h"
#include <stdio.h>
#include <math.h>

double my_decay(double i) {
    return 1.0 / (1.0 + i * 0.1);
}

/* Приклад кастомної функції втрат */
double my_sq_loss(double diff) {
    return diff * diff;
}

void display_menu() {
    printf("\n=== MENU ===\n");
    printf("1. Add manual element (pair)\n");
    printf("2. Add random data (Normal Dist)\n");
    printf("3. Load from file (Comparison_test.txt)\n");
    printf("4. Save to file (Comparison_test.txt)\n");
    printf("5. Show Data\n");
    printf("6. Calculate Metrics (Classification & Math)\n");
    printf("0. Exit\n");
    printf("Select: ");
}

int main() {
    ComparisonC comp;
    comparison_init(&comp);

    comparison_set_distr(&comp, my_decay);
    comparison_set_loss(&comp, my_sq_loss);

    const char* filename = "Comparison_test.txt";

    int choice;
    do {
        display_menu();
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            choice = -1;
        }

        switch(choice) {
            case 1: {
                double v1, v2;
                printf("Enter R1 val and R2 val: "); 
                scanf("%lf %lf", &v1, &v2);
                comparison_add(&comp, v1, v2);
                break;
            }
            case 2: {
                size_t n;
                double m, s;
                printf("Enter count, mean, stddev: ");
                scanf("%zu %lf %lf", &n, &m, &s);
                comparison_add_random(&comp, n, m, s);
                printf("Added %zu random elements.\n", n);
                break;
            }
            case 3:
                comparison_load(&comp, filename);
                break;
            case 4:
                comparison_save(&comp, filename);
                break;
            case 5:
                comparison_print(&comp);
                break;
            case 6: {
                if (comp.n == 0) {
                    printf("No data to analyze.\n"); 
                    break;
                }
                
                printf("\n--- Binary Classification Metrics (Threshold > 0.5) ---\n");
                printf("TP: %g, FP: %g\n", comparison_get_tp(&comp), comparison_get_fp(&comp));
                printf("FN: %g, TN: %g\n", comparison_get_fn(&comp), comparison_get_tn(&comp));
                
                printf("Accuracy (ACC): %g\n", comparison_acc(&comp));
                printf("Precision (PPV): %g\n", comparison_ppv(&comp));
                printf("Recall (TPR): %g\n", comparison_tpr(&comp));
                printf("F1 Score: %g\n", comparison_f1(&comp));

                printf("\n--- Math & Distance Metrics ---\n");
                printf("Euclidean Dist: %g\n", comparison_dist(&comp));
                printf("Correlation: %g\n", comparison_corr(&comp));
                printf("Dist_F (Weighted Indices): %g\n", comparison_dist_f(&comp));
                printf("Loss (Func on Diff): %g\n", comparison_loss(&comp));
                printf("Dist_F_Loss (Combined): %g\n", comparison_dist_f_loss(&comp));
                break;
            }
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while(choice != 0);

    comparison_free(&comp);
    return 0;
}
