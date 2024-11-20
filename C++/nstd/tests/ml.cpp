#include <iostream>
#include <ml.hpp>

int main()
{
    nstd::ML::LinearRegression lr;
    std::vector<double> x = {1, 2, 3, 4, 5};
    std::vector<double> y = {2, 3, 5, 7, 11};
    lr.fit(x, y);
    std::cout << "Linear Regression Prediction for 6: " << lr.predict(6) << std::endl;

    nstd:: ML::LogisticRegression log_reg;
    std::vector<double> x_log = {0.1, 0.4, 0.6, 0.8};
    std::vector<int> y_log = {0, 0, 1, 1};
    log_reg.fit(x_log, y_log);
    std::cout << "Logistic Regression Prediction for 0.5: " << log_reg.predict(0.5) << std::endl;

    nstd:: ML::DecisionTree dt;
    std::vector<std::vector<double>> x_dt = {{1, 2}, {2, 3}, {3, 1}, {4, 2}};
    std::vector<int> y_dt = {0, 0, 1, 1};
    dt.fit(x_dt, y_dt);
    std::cout << "Decision Tree Prediction for {3, 2}: " << dt.predict({3, 2}) << std::endl;

    nstd:: ML::RandomForest rf(10);
    rf.fit(x_dt, y_dt);
    std::cout << "Random Forest Prediction for {3, 2}: " << rf.predict({3, 2}) << std::endl;

    nstd:: ML::NeuralNetwork nn(2, 3, 1);
    std::vector<std::vector<double>> x_nn = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    std::vector<std::vector<double>> y_nn = {{0}, {1}, {1}, {0}};
    nn.fit(x_nn, y_nn);
    std::vector<double> prediction = nn.predict({1, 0});
    std::cout << "Neural Network Prediction for {1, 0}: " << prediction[0] << std::endl;

    return 0;
}