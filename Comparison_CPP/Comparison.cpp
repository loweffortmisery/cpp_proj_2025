/*
 * Задача: (23)DataComparison
 * Група: Комп'ютерна математика 2 курс
 * Студент: Лазаренко
 * Опис: Реалізація методів класу Comparison. Мова С++
 */
#include "Comparison.h"
#include <cmath>
#include <numeric>
#include <fstream>
#include <random>
#include <iomanip>


const double THRESHOLD = 0.5;

Comparison::Comparison() : n(0), cm{0,0,0,0} {
    func_distr = [](double) { return 1.0; };
    func_loss = [](double x) { return std::abs(x); };
}

void Comparison::updateCM_single(double v1, double v2) {
    bool actual = (std::abs(v1) > THRESHOLD);
    bool pred = (std::abs(v2) > THRESHOLD);

    if (actual && pred) cm.TP++;
    else if (!actual && !pred) cm.TN++;
    else if (!actual && pred) cm.FP++;
    else if (actual && !pred) cm.FN++;
}

void Comparison::addElement(double val1, double val2) {
    R1.push_back(val1);
    R2.push_back(val2);
    n++;
    // recounting after each added element instead of recounting on direct call
    updateCM_single(val1, val2);
}

void Comparison::addRandomData(size_t count, double mean, double stddev) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(mean, stddev);
    for (size_t i = 0; i < count; ++i) {
        addElement(d(gen), d(gen));
    }
}

void Comparison::clear() {
    R1.clear();
    R2.clear();
    n = 0;
    cm = {0, 0, 0, 0}; // matrix reset
}

void Comparison::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    file << n << "\n";
    for (double v : R1) file << v << " ";
    file << "\n";
    for (double v : R2) file << v << " ";
    file << "\n";
}

void Comparison::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    clear();
    size_t count;
    file >> count;
    std::vector<double> tempR1(count), tempR2(count);
    for(size_t i=0; i<count; ++i) file >> tempR1[i];
    for(size_t i=0; i<count; ++i) file >> tempR2[i];
    
    for(size_t i=0; i<count; ++i) {
        addElement(tempR1[i], tempR2[i]);
    }
}

double Comparison::TP() const { return (double)cm.TP; }
double Comparison::FP() const { return (double)cm.FP; }
double Comparison::FN() const { return (double)cm.FN; }
double Comparison::TN() const { return (double)cm.TN; }

double Comparison::TPR() const { return (cm.TP + cm.FN == 0) ? 0.0 : TP() / (TP() + FN()); }
double Comparison::FPR() const { return (cm.FP + cm.TN == 0) ? 0.0 : FP() / (FP() + TN()); }
double Comparison::PPV() const { return (cm.TP + cm.FP == 0) ? 0.0 : TP() / (TP() + FP()); }
double Comparison::NPV() const { return (cm.TN + cm.FN == 0) ? 0.0 : TN() / (TN() + FN()); }
double Comparison::FDR() const { return 1.0 - PPV(); }
double Comparison::FOR() const { return 1.0 - NPV(); }
double Comparison::ACC() const { return (n == 0) ? 0.0 : (TP() + TN()) / n; }
double Comparison::F1() const {
    double p = PPV();
    double r = TPR();
    return (p + r == 0) ? 0.0 : 2.0 * p * r / (p + r);
}

double Comparison::dist() const {
    double sum = 0.0;
    for (size_t i = 0; i < n; ++i) sum += std::pow(R1[i] - R2[i], 2);
    return std::sqrt(sum);
}

double Comparison::corr() const {
    if (n == 0) return 0.0;
    double m1 = std::accumulate(R1.begin(), R1.end(), 0.0) / n;
    double m2 = std::accumulate(R2.begin(), R2.end(), 0.0) / n;
    double num = 0, d1 = 0, d2 = 0;
    for(size_t i=0; i<n; ++i) {
        num += (R1[i]-m1)*(R2[i]-m2);
        d1 += pow(R1[i]-m1, 2);
        d2 += pow(R2[i]-m2, 2);
    }
    return (d1*d2 == 0) ? 0 : num / std::sqrt(d1*d2);
}

size_t Comparison::getSize() const { return n; }
void Comparison::printData() const {
    std::cout << "Ind\tR1\tR2\n";
    for(size_t i=0; i<n; ++i) std::cout << i << "\t" << R1[i] << "\t" << R2[i] << "\n";
}
void Comparison::setFuncDistr(std::function<double(double)> f) { func_distr = f; }
void Comparison::setFuncLoss(std::function<double(double)> f) { func_loss = f; }

double Comparison::dist_f() const {
    double d = 0;
    for(size_t i=0; i<n; ++i) d += func_distr(i) * std::abs(R1[i]-R2[i]);
    return d;
}
double Comparison::loss() const {
    double l = 0;
    for(size_t i=0; i<n; ++i) l += func_loss(R1[i]-R2[i]);
    return l;
}
double Comparison::dist_f_loss() const {
    double r = 0;
    for(size_t i=0; i<n; ++i) r += func_distr(i) * func_loss(R1[i]-R2[i]);
    return r;
}
void Comparison::addVectorToR1(const std::vector<double>& v) { for(auto x : v) R1.push_back(x); }
void Comparison::addVectorToR2(const std::vector<double>& v) { for(auto x : v) R2.push_back(x); }
const std::vector<double>& Comparison::getR1() const { return R1; }
const std::vector<double>& Comparison::getR2() const { return R2; }
