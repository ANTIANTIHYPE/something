#include <iostream>
#include <ml.hpp>

int main()
{
    std::cout << "=== Linear Regression Test ===" << std::endl;
    nstd::ML::LinearRegression lr(0.01); // L2 regularization
    std::vector<double> x_lr = {1, 2, 3, 4, 5};
    std::vector<double> y_lr = {2, 3, 5, 7, 11};
    lr.fit(x_lr, y_lr);
    std::cout << "Linear Regression Prediction for 6: " << lr.predict(6) << std::endl; // Expected: around 12

    std::cout << "\n=== Logistic Regression Test ===" << std::endl;
    nstd::ML::LogisticRegression log_reg(2, 1); // 2 classes, 1 feature
    std::vector<double> x_log = {0.1, 0.4, 0.6, 0.8};
    std::vector<int> y_log = {0, 0, 1, 1};
    log_reg.fit({{0.1}, {0.4}, {0.6}, {0.8}}, y_log);
    std::cout << "Logistic Regression Prediction for 0.5: " << log_reg.predictClass({0.5}) << std::endl; // Expected: 1

    std::cout << "\n=== Decision Tree Test ===" << std::endl;
    nstd::ML::DecisionTree dt(3); // Max depth of 3
    std::vector<std::vector<double>> x_dt = {{1, 2}, {2, 3}, {3, 1}, {4, 2}, {5, 5}};
    std::vector<int> y_dt = {0, 0, 1, 1, 1};
    dt.fit(x_dt, y_dt);
    std::cout << "Decision Tree Prediction for {3, 2}: " << dt.predict({3, 2}) << std::endl; // Actual output: 1
    std::cout << "Decision Tree Prediction for {5, 5}: " << dt.predict({5, 5}) << std::endl; // Expected: 1

    std::cout << "\n=== Random Forest Test ===" << std::endl;
    nstd::ML::RandomForest rf(10, 3); // 10 trees, max depth of 3
    rf.fit(x_dt, y_dt);
    std::cout << "Random Forest Prediction for {3, 2}: " << rf.predict({3, 2}) << std::endl; // Actual output: 1
    std::cout << "Random Forest Prediction for {5, 5}: " << rf.predict({5, 5}) << std::endl; // Expected: 1

    // NeuralNetwork doesn't print out anything (???)

    // std::cout << "\n=== Neural Network Test ===" << std::endl;
    // nstd::ML::NeuralNetwork nn(2, 3, 1); // 2 input features, 3 hidden neurons, 1 output
    // std::vector<std::vector<double>> x_nn = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    // std::vector<std::vector<double>> y_nn = {{0}, {1}, {1}, {0}}; // XOR problem
    // nn.fit(x_nn, y_nn);
    // std::vector<double> prediction_1 = nn.predict({1, 0});
    // std::vector<double> prediction_2 = nn.predict({0, 1});
    // std::cout << "Neural Network Prediction for {1, 0}: " << prediction_1[0] << " (Expected close to 1)" << std::endl;
    // std::cout << "Neural Network Prediction for {0, 1}: " << prediction_2[0] << " (Expected close to 1)" << std::endl;

    return 0;
}