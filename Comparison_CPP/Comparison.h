/*
 * Задача: (23)DataComparison
 * Група: Комп'ютерна математика 2 курс
 * Студент: Лазаренко
 * Опис: Заголовний файл класу Comparison(Порівняння). Описує структуру,  її поля, та методи її аналізу включаючи метрики бінарної класифікації та функції відстані. Мова С++
 */

#ifndef COMPARISON_H
#define COMPARISON_H

#include <vector>
#include <functional>
#include <string>
#include <iostream>

class Comparison {
public:
    struct ConfusionMatrix {
        long long TP;
        long long FP;
        long long FN;
        long long TN;
    };

private:
    size_t n;
    std::vector<double> R1;
    std::vector<double> R2;
    
    ConfusionMatrix cm;
    
    std::function<double(double)> func_distr;
    std::function<double(double)> func_loss;

    void updateCM_single(double v1, double v2);

public:
    Comparison();

    size_t getSize() const;
    void setFuncDistr(std::function<double(double)> f);
    void setFuncLoss(std::function<double(double)> f);
    
    const std::vector<double>& getR1() const;
    const std::vector<double>& getR2() const;

    void addElement(double val1, double val2);
    void addVectorToR1(const std::vector<double>& v);
    void addVectorToR2(const std::vector<double>& v);
    void addRandomData(size_t count, double mean, double stddev);
    void clear();

    void printData() const;
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    double TP() const;
    double FP() const;
    double FN() const;
    double TN() const;

    double TPR() const;
    double FPR() const;
    double PPV() const; 
    double NPV() const;
    double FDR() const;
    double FOR() const;
    double ACC() const;
    double F1() const;

    double dist() const;
    double corr() const;
    double dist_f() const;
    double loss() const;
    double dist_f_loss() const;
};

#endif
