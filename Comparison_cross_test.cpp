/*
 * Задача: (23)DataComparison
 * Група: Комп'ютерна математика 2 курс
 * Студент: Лазаренко
 * Опис: перехресне тестування С++ та С реалізацій.
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <iomanip>

// Include headers
#include "Comparison_CPP/Comparison.h"
#include "Comparison_C/Comparison.h"


// Test distribution function (weight depends on index)
double test_distr_func(double i) {
    return 1.0 + (i * 0.1); 
}

// Test loss function (quadratic)
double test_loss_func(double diff) {
    return diff * diff;
}

// Global counters
int total_tests = 0;
int passed_tests = 0;

// Function to check value equality
void check_val(const std::string& name, double cpp_val, double c_val) {
    total_tests++;
    double diff = std::abs(cpp_val - c_val);
    
    // Use small epsilon for double comparison
    if (diff < 1e-9) {
        std::cout << "[PASS] " << std::setw(20) << std::left << name 
                  << " | C++: " << std::setw(10) << cpp_val 
                  << " == C: " << c_val << "\n";
        passed_tests++;
    } else {
        std::cout << "[FAIL] " << std::setw(20) << std::left << name 
                  << " | C++: " << cpp_val 
                  << " != C: " << c_val << " (Diff: " << diff << ")\n";
    }
}

int main() {
    std::cout << "=== CROSS TEST STARTED (FULL COVERAGE) ===\n\n";

    // 1. Data  
    // Threshold = 0.5
    std::vector<std::pair<double, double>> data = {
        {1.0, 0.9},  
        {-0.1, 0.1},
        {0.2, 0.8},
        {0.8, 0.2}, 
        {5.0, 5.0},
        {-0.5, -0.6} 
    };

    // 2. C++ Initialization
    Comparison cpp_comp;
    // Set custom functions to test dist_f and loss
    cpp_comp.setFuncDistr(test_distr_func);
    cpp_comp.setFuncLoss(test_loss_func);

    // 3. C Initialization
    ComparisonC c_comp;
    comparison_init(&c_comp);
    // Set the same functions for C
    comparison_set_distr(&c_comp, test_distr_func);
    comparison_set_loss(&c_comp, test_loss_func);

    // 4. Populate data
    for (auto p : data) {
        cpp_comp.addElement(p.first, p.second);
        comparison_add(&c_comp, p.first, p.second);
    }

    std::cout << "--- Group 1: Basic Counters ---\n";
    check_val("TP", cpp_comp.TP(), comparison_get_tp(&c_comp));
    check_val("FP", cpp_comp.FP(), comparison_get_fp(&c_comp));
    check_val("FN", cpp_comp.FN(), comparison_get_fn(&c_comp));
    check_val("TN", cpp_comp.TN(), comparison_get_tn(&c_comp));

    std::cout << "\n--- Group 2: Classification Metrics ---\n";
    check_val("Accuracy (ACC)", cpp_comp.ACC(), comparison_acc(&c_comp));
    check_val("Precision (PPV)", cpp_comp.PPV(), comparison_ppv(&c_comp));
    check_val("Recall (TPR)",    cpp_comp.TPR(), comparison_tpr(&c_comp));
    check_val("F1 Score",        cpp_comp.F1(),  comparison_f1(&c_comp));
    check_val("FPR",             cpp_comp.FPR(), comparison_fpr(&c_comp));
    check_val("NPV",             cpp_comp.NPV(), comparison_npv(&c_comp));

    std::cout << "\n--- Group 3: Math & Distance Metrics ---\n";
    check_val("Euclidean Dist",  cpp_comp.dist(), comparison_dist(&c_comp));
    check_val("Correlation",     cpp_comp.corr(), comparison_corr(&c_comp));
    
    check_val("Dist F (Weighted)", cpp_comp.dist_f(), comparison_dist_f(&c_comp));
    check_val("Loss (Function)",   cpp_comp.loss(),   comparison_loss(&c_comp));
    check_val("Dist F + Loss",     cpp_comp.dist_f_loss(), comparison_dist_f_loss(&c_comp));

    // 5. Summary
    std::cout << "\n==========================================\n";
    std::cout << passed_tests << "/" << total_tests << " tests passed.\n";
    
    if (passed_tests == total_tests) {
        std::cout << "All passed\n";
    } else {
        std::cout << "Some failed\n";
    }
    std::cout << "==========================================\n";

    comparison_free(&c_comp);
    return 0;
}
