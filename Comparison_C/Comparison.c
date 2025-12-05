/*
 * Задача: (23)DataComparison
 * Група: Комп'ютерна математика 2 курс
 * Студент: Лазаренко
 * Опис: Реалізація методів класу Comparison.
 */
#include "Comparison.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

#define INITIAL_CAPACITY 10
#define THRESHOLD 0.5

double default_distr(double x) { 
    return 1.0 + x * 0.0; 
}
double default_loss(double x) { return fabs(x); }

double rand_normal(double mean, double stddev) {
    static int haveSpare = 0;
    static double randSpare;
    if(haveSpare) {
        haveSpare = 0;
        return mean + stddev * randSpare;
    }
    haveSpare = 1;
    double u, v, s;
    do {
        u = (rand() / ((double)RAND_MAX)) * 2.0 - 1.0;
        v = (rand() / ((double)RAND_MAX)) * 2.0 - 1.0;
        s = u * u + v * v;
    } while(s >= 1.0 || s == 0.0);
    s = sqrt(-2.0 * log(s) / s);
    randSpare = v * s;
    return mean + stddev * (u * s);
}

void comparison_init(ComparisonC *comp) {
    comp->n = 0;
    comp->capacity = INITIAL_CAPACITY;
    comp->R1 = (double*)malloc(comp->capacity * sizeof(double));
    comp->R2 = (double*)malloc(comp->capacity * sizeof(double));
    comp->TP = 0; comp->FP = 0; comp->FN = 0; comp->TN = 0;
    comp->func_distr = default_distr;
    comp->func_loss = default_loss;
}

void comparison_free(ComparisonC *comp) {
    if (comp->R1) free(comp->R1);
    if (comp->R2) free(comp->R2);
    comp->R1 = NULL;
    comp->R2 = NULL;
    comp->n = 0;
}

void comparison_set_distr(ComparisonC *comp, CompFunc f) { comp->func_distr = f; }
void comparison_set_loss(ComparisonC *comp, CompFunc f) { comp->func_loss = f; }

void comparison_clear(ComparisonC *comp) {
    comp->n = 0;
    comp->TP = 0; comp->FP = 0; comp->FN = 0; comp->TN = 0;
}

void comparison_add(ComparisonC *comp, double v1, double v2) {
    if (comp->n >= comp->capacity) {
        comp->capacity *= 2;
        comp->R1 = (double*)realloc(comp->R1, comp->capacity * sizeof(double));
        comp->R2 = (double*)realloc(comp->R2, comp->capacity * sizeof(double));
    }
    comp->R1[comp->n] = v1;
    comp->R2[comp->n] = v2;
    comp->n++;

    int actual = (fabs(v1) > THRESHOLD);
    int pred = (fabs(v2) > THRESHOLD);

    if (actual && pred) comp->TP++;
    else if (!actual && !pred) comp->TN++;
    else if (!actual && pred) comp->FP++;
    else if (actual && !pred) comp->FN++;
}

void comparison_add_random(ComparisonC *comp, size_t count, double mean, double stddev) {
    static int seeded = 0;
    if (!seeded) { srand(time(NULL)); seeded = 1; }
    for (size_t i = 0; i < count; ++i) {
        comparison_add(comp, rand_normal(mean, stddev), rand_normal(mean, stddev));
    }
}

void comparison_print(const ComparisonC *comp) {
    printf("Index\tR1(Exp)\tR2(Calc)\n");
    for (size_t i = 0; i < comp->n; ++i) {
        printf("%zu\t%.4f\t%.4f\n", i, comp->R1[i], comp->R2[i]);
    }
}

void comparison_save(const ComparisonC *comp, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) { perror("Error saving file"); return; }
    fprintf(f, "%zu\n", comp->n);
    for (size_t i = 0; i < comp->n; ++i) fprintf(f, "%.6f ", comp->R1[i]);
    fprintf(f, "\n");
    for (size_t i = 0; i < comp->n; ++i) fprintf(f, "%.6f ", comp->R2[i]);
    fprintf(f, "\n");
    fclose(f);
    printf("Saved to %s\n", filename);
}

void comparison_load(ComparisonC *comp, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { perror("Error loading file"); return; }
    comparison_clear(comp);
    size_t count;
    if (fscanf(f, "%zu", &count) != 1) { fclose(f); return; }
    double *tempR1 = (double*)malloc(count * sizeof(double));
    double *tempR2 = (double*)malloc(count * sizeof(double));
    for (size_t i = 0; i < count; ++i) fscanf(f, "%lf", &tempR1[i]);
    for (size_t i = 0; i < count; ++i) fscanf(f, "%lf", &tempR2[i]);
    for (size_t i = 0; i < count; ++i) comparison_add(comp, tempR1[i], tempR2[i]);
    free(tempR1); free(tempR2);
    fclose(f);
    printf("Loaded %zu items from %s\n", count, filename);
}

double comparison_get_tp(const ComparisonC *comp) { return (double)comp->TP; }
double comparison_get_fp(const ComparisonC *comp) { return (double)comp->FP; }
double comparison_get_fn(const ComparisonC *comp) { return (double)comp->FN; }
double comparison_get_tn(const ComparisonC *comp) { return (double)comp->TN; }

double comparison_tpr(const ComparisonC *comp) {
    double sum = comp->TP + comp->FN;
    return (sum == 0) ? 0.0 : comp->TP / sum;
}
double comparison_fpr(const ComparisonC *comp) {
    double sum = comp->FP + comp->TN;
    return (sum == 0) ? 0.0 : comp->FP / sum;
}
double comparison_ppv(const ComparisonC *comp) {
    double sum = comp->TP + comp->FP;
    return (sum == 0) ? 0.0 : comp->TP / sum;
}
double comparison_npv(const ComparisonC *comp) {
    double sum = comp->TN + comp->FN;
    return (sum == 0) ? 0.0 : comp->TN / sum;
}
double comparison_acc(const ComparisonC *comp) {
    return (comp->n == 0) ? 0.0 : (double)(comp->TP + comp->TN) / comp->n;
}
double comparison_f1(const ComparisonC *comp) {
    double p = comparison_ppv(comp);
    double r = comparison_tpr(comp);
    return (p + r == 0) ? 0.0 : 2.0 * p * r / (p + r);
}

double comparison_dist(const ComparisonC *comp) {
    double sum = 0.0;
    for (size_t i = 0; i < comp->n; ++i) {
        double d = comp->R1[i] - comp->R2[i];
        sum += d * d;
    }
    return sqrt(sum);
}

double comparison_corr(const ComparisonC *comp) {
    if (comp->n == 0) return 0.0;
    double m1 = 0, m2 = 0;
    for(size_t i=0; i<comp->n; ++i) { m1 += comp->R1[i]; m2 += comp->R2[i]; }
    m1 /= comp->n; m2 /= comp->n;
    double num = 0, den1 = 0, den2 = 0;
    for (size_t i = 0; i < comp->n; ++i) {
        double d1 = comp->R1[i] - m1;
        double d2 = comp->R2[i] - m2;
        num += d1 * d2;
        den1 += d1 * d1;
        den2 += d2 * d2;
    }
    if (den1 == 0 || den2 == 0) return 0.0;
    return num / sqrt(den1 * den2);
}

double comparison_dist_f(const ComparisonC *comp) {
    double res = 0;
    for(size_t i=0; i<comp->n; ++i) {
        res += comp->func_distr((double)i) * fabs(comp->R1[i] - comp->R2[i]);
    }
    return res;
}

double comparison_loss(const ComparisonC *comp) {
    double res = 0;
    for(size_t i=0; i<comp->n; ++i) {
        res += comp->func_loss(comp->R1[i] - comp->R2[i]);
    }
    return res;
}

double comparison_dist_f_loss(const ComparisonC *comp) {
    double res = 0;
    for(size_t i=0; i<comp->n; ++i) {
        res += comp->func_distr((double)i) * comp->func_loss(comp->R1[i] - comp->R2[i]);
    }
    return res;
}
