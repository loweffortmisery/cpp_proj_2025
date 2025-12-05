/*
 * Задача: (23)DataComparison
 * Група: Комп'ютерна математика 2 курс
 * Студент: Лазаренко
 * Опис: Заголовний файл класу Comparison(Порівняння). Описує структуру,  її поля, та методи її аналізу включаючи метрики бінарної класифікації та функції відстані. Мова - С
 */
#ifndef COMPARISON_C_H
#define COMPARISON_C_H

#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef double (*CompFunc)(double);

typedef struct {
    double *R1;
    double *R2;
    size_t n;
    size_t capacity;
    long long TP, FP, FN, TN;
    CompFunc func_distr;
    CompFunc func_loss;
} ComparisonC;
// ComparisonC instead of Comparison to avoid ambiguity in cross test
void comparison_init(ComparisonC *comp);
void comparison_free(ComparisonC *comp);

void comparison_set_distr(ComparisonC *comp, CompFunc f);
void comparison_set_loss(ComparisonC *comp, CompFunc f);

void comparison_add(ComparisonC *comp, double v1, double v2);
void comparison_add_random(ComparisonC *comp, size_t count, double mean, double stddev);
void comparison_clear(ComparisonC *comp);

void comparison_print(const ComparisonC *comp);
void comparison_save(const ComparisonC *comp, const char *filename);
void comparison_load(ComparisonC *comp, const char *filename);

double comparison_get_tp(const ComparisonC *comp);
double comparison_get_fp(const ComparisonC *comp);
double comparison_get_fn(const ComparisonC *comp);
double comparison_get_tn(const ComparisonC *comp);

double comparison_tpr(const ComparisonC *comp);
double comparison_fpr(const ComparisonC *comp);
double comparison_ppv(const ComparisonC *comp);
double comparison_npv(const ComparisonC *comp);
double comparison_acc(const ComparisonC *comp);
double comparison_f1(const ComparisonC *comp);

double comparison_dist(const ComparisonC *comp);
double comparison_corr(const ComparisonC *comp);
double comparison_dist_f(const ComparisonC *comp);
double comparison_loss(const ComparisonC *comp);
double comparison_dist_f_loss(const ComparisonC *comp);

#ifdef __cplusplus
}
#endif

#endif
