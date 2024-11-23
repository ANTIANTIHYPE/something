// ml.hpp
#pragma once

#include <vector>
#include <algorithm>
#include <map>
#include <random>

namespace nstd
{
    /**
     * @brief Machine Learning
     */
    namespace ML
    {
        /**
         * @brief A class for performing Linear Regression.
         */
        class LinearRegression
        {
        public:
            LinearRegression() : slope(0), intercept(0) {}

            /**
             * @brief Fits the linear regression model to the provided data.
             * 
             * @param x A vector of input features.
             * @param y A vector of target values.
             * 
             * @throws std::invalid_argument If the sizes of x and y do not match.
             */
            constexpr void fit(const std::vector<double>& x, const std::vector<double>& y)
            {
                int n = x.size();
                if (n == 0)
                {
                    return;
                }
                double x_mean = std::accumulate(x.begin(), x.end(), 0.0) / n;
                double y_mean = std::accumulate(y.begin(), y.end(), 0.0) / n;

                double numerator = 0, denominator = 0;
                for (int i = 0; i < n; ++i)
                {
                    numerator += (x[i] - x_mean) * (y[i] - y_mean);
                    denominator += (x[i] - x_mean) * (x[i] - x_mean);
                }

                slope = numerator / denominator;
                intercept = y_mean - slope * x_mean;
            }

            /**
             * @brief Predicts the target value for a given input feature.
             * 
             * @param x The input feature.
             * 
             * @return The predicted target value.
             */
            constexpr double predict(double x) const
            {
                return slope * x + intercept;
            }

        private:
            double slope;
            double intercept;
        }; // class LinearRegression

        /**
         * @brief A class for performing Logistic Regression.
         */
        class LogisticRegression
        {
        public:
            LogisticRegression() : weight(0), bias(0) {}

            /**
             * @brief Fits the logistic regression model to the provided data.
             * 
             * @param x A vector of input features.
             * @param y A vector of binary target values (0 or 1).
             * @param learning_rate The learning rate for gradient descent.
             * @param epochs The number of iterations for training.
             */
            constexpr void fit(const std::vector<double>& x, const std::vector<int>& y, double learning_rate = 0.01, int epochs = 1000)
            {
                int n = x.size();
                for (int epoch = 0; epoch < epochs; ++epoch)
                {
                    for (int i = 0; i < n; ++i)
                    {
                        double linear_output = weight * x[i] + bias;
                        double prediction = sigmoid(linear_output);
                        double error = y[i] - prediction;

                        weight += learning_rate * error * prediction * (1 - prediction) * x[i];
                        bias += learning_rate * error * prediction * (1 - prediction);
                    }
                }
            }

            /**
             * @brief Predicts the probability of the positive class for a given input feature.
             * 
             * @param x The input feature.
             * 
             * @return The predicted probability.
             */
            constexpr double predict(double x) const
            {
                return sigmoid(weight * x + bias);
            }

            /**
             * @brief Predicts the class label for a given input feature.
             * 
             * @param x The input feature.
             * 
             * @return The predicted class label (0 or 1).
             */
            constexpr int predict_class(double x) const
            {
                return predict(x) >= 0.5 ? 1 : 0;
            }

        private:
            double weight;
            double bias;

            /**
             * @brief Computes the sigmoid activation function.
             * 
             * @param z The input value.
             * 
             * @return The output of the sigmoid function.
             */
            constexpr double sigmoid(double z) const
            {
                return 1.0 / (1.0 + std::exp(-z));
            }
        }; // class LogisticRegression

        /**
         * @brief A struct representing a node in the decision tree.
         */
        struct TreeNode
        {
            double value;
            int feature_index;
            double threshold;
            TreeNode* left;
            TreeNode* right;
            bool is_leaf;

            TreeNode() : value(0), feature_index(-1), threshold(0), left(nullptr), right(nullptr), is_leaf(true) {}
        };

        /**
         * @brief A class for performing Decision Tree classification.
         */
        class DecisionTree
        {
        public:
            /**
             * @brief Constructs a DecisionTree object.
             * 
             * @param max_depth The maximum depth of the tree.
             */
            DecisionTree(int max_depth = 5) : root(nullptr), max_depth(max_depth) {}

            /**
             * @brief Fits the decision tree model to the provided data.
             * 
             * @param x A 2D vector of input features.
             * @param y A vector of target values.
             */
            constexpr void fit(const std::vector<std::vector<double>>& x, const std::vector<int>& y)
            {
                root = buildTree(x, y, 0);
            }

            /**
             * @brief Predicts the class label for a given input sample.
             * 
             * @param sample A vector representing the input features.
             * 
             * @return The predicted class label.
             */
            constexpr int predict(const std::vector<double>& sample) const
            {
                return predict(sample, root);
            }

        private:
            TreeNode* root;
            int max_depth;

            /**
             * @brief Builds the decision tree recursively.
             * 
             * @param x A 2D vector of input features.
             * @param y A vector of target values.
             * @param depth The current depth of the tree.
             * 
             * @return A pointer to the root node of the constructed subtree.
             */
            TreeNode* buildTree(const std::vector<std::vector<double>>& x, const std::vector<int>& y, int depth)
            {
                if (y.empty() || depth >= max_depth)
                {
                    TreeNode* leaf = new TreeNode();
                    leaf->value = static_cast<double>(mostCommonLabel(y));
                    leaf->is_leaf = true;
                    return leaf;
                }

                int best_feature = -1;
                double best_threshold = 0;
                double best_gain = -std::numeric_limits<double>::infinity();
                std::vector<int> best_left_y, best_right_y;
                std::vector<std::vector<double>> best_left_x, best_right_x;

                for (int feature = 0; feature < x[0].size(); ++feature)
                {
                    std::vector<std::pair<double, int>> feature_values;
                    for (std::size_t i = 0; i < x.size(); i++)
                    {
                        feature_values.emplace_back(x[i][feature], y[i]);
                    }
                    std::sort(feature_values.begin(), feature_values.end());

                    for (std::size_t i = 1; i < feature_values.size(); ++i)
                    {
                        if (feature_values[i].first != feature_values[i - 1].first)
                        {
                            double threshold = (feature_values[i].first + feature_values[i - 1].first) / 2.0;
                            std::vector<int> left_y, right_y;
                            std::vector<std::vector<double>> left_x, right_x;

                            for (const auto& pair : feature_values)
                            {
                                if (pair.first <= threshold)
                                {
                                    left_y.push_back(pair.second);
                                    left_x.push_back(x[&pair - &feature_values[0]]);
                                }
                                else
                                {
                                    right_y.push_back(pair.second);
                                    right_x.push_back(x[&pair - &feature_values[0]]);
                                }
                            }

                            double gain = informationGain(y, left_y, right_y);
                            if (gain > best_gain)
                            {
                                best_gain = gain;
                                best_feature = feature;
                                best_threshold = threshold;
                                best_left_y = left_y;
                                best_right_y = right_y;
                                best_left_x = left_x;
                                best_right_x = right_x;
                            }
                        }
                    }
                }

                TreeNode* node = new TreeNode();
                node->feature_index = best_feature;
                node->threshold = best_threshold;
                node->left = buildTree(best_left_x, best_left_y, depth + 1);
                node->right = buildTree(best_right_x, best_right_y, depth + 1);
                node->is_leaf = false;
                return node;
            }

            /**
             * @brief Finds the most common label in the provided target values.
             * 
             * @param y A vector of target values.
             * 
             * @return The most common label.
             */
            constexpr int mostCommonLabel(const std::vector<int>& y) const
            {
                std::map<int, int> counts;
                for (int label : y)
                {
                    counts[label]++;
                }
                return std::max_element(counts.begin(), counts.end(),
                    [](const auto& a, const auto& b) { return a.second < b.second; })->first;
            }

            /**
             * @brief Calculates the information gain from a split.
             * 
             * @param parent The parent node's target values.
             * @param left The left child node's target values.
             * @param right The right child node's target values.
             * 
             * @return The information gain from the split.
             */
            constexpr double informationGain(const std::vector<int>& parent, const std::vector<int>& left, const std::vector<int>& right)
            {
                double parent_entropy = entropy(parent);
                double left_entropy = entropy(left);
                double right_entropy = entropy(right);
                double weighted_entropy = (left.size() * left_entropy + right.size() * right_entropy) / parent.size();
                return parent_entropy - weighted_entropy;
            }

            /**
             * @brief Calculates the entropy of a set of target values.
             * 
             * @param y A vector of target values.
             * 
             * @return The entropy value.
             */
            constexpr double entropy(const std::vector<int>& y)
            {
                std::map<int, int> counts;
                for (int label : y)
                {
                    counts[label]++;
                }
                double entropy = 0.0;
                for (const auto& count : counts)
                {
                    double p = static_cast<double>(count.second) / y.size();
                    entropy -= p * std::log2(p);
                }
                return entropy;
            }

            /**
             * @brief Predicts the class label for a given input sample recursively.
             * 
             * @param sample A vector representing the input features.
             * @param node The current node in the tree.
             * 
             * @return The predicted class label.
             */
            constexpr int predict(const std::vector<double>& sample, TreeNode* node) const
            {
                if (node->is_leaf)
                {
                    return static_cast<int>(node->value);
                }
                if (sample[node->feature_index] <= node->threshold)
                {
                    return predict(sample, node->left);
                }
                else
                {
                    return predict(sample, node->right);
                }
            }
        }; // class DecisionTree

        /**
         * @brief A class for performing Random Forest classification.
         */
        class RandomForest
        {
        public:
            /**
             * @brief Constructs a RandomForest object.
             * 
             * @param n_trees The number of trees in the forest.
             * @param max_depth The maximum depth of each tree.
             */
            RandomForest(int n_trees, int max_depth = 5) : n_trees(n_trees), max_depth(max_depth) {}

            /**
             * @brief Fits the random forest model to the provided data.
             * 
             * @param x A 2D vector of input features.
             * @param y A vector of target values.
             */
            constexpr void fit(const std::vector<std::vector<double>>& x, const std::vector<int>& y)
            {
                for (int i = 0; i < n_trees; ++i)
                {
                    DecisionTree tree(max_depth);
                    std::vector<std::vector<double>> sample_x;
                    std::vector<int> sample_y;
                    bootstrapSample(x, y, sample_x, sample_y);
                    tree.fit(sample_x, sample_y);
                    trees.push_back(tree);
                }
            }

            /**
             * @brief Predicts the class label for a given input sample.
             * 
             * @param sample A vector representing the input features.
             * 
             * @return The predicted class label.
             */
            constexpr int predict(const std::vector<double>& sample) const
            {
                std::map<int, int> votes;
                for (const DecisionTree& tree : trees)
                {
                    int prediction = tree.predict(sample);
                    votes[prediction]++;
                }
                return std::max_element(votes.begin(), votes.end(), [](const auto& a, const auto& b)
                {
                    return a.second < b.second;
                })->first;
            }

        private:
            int n_trees;
            int max_depth;
            std::vector<DecisionTree> trees;

            /**
             * @brief Creates a bootstrap sample from the provided data.
             * 
             * @param x A 2D vector of input features.
             * @param y A vector of target values.
             * @param sample_x A reference to a vector to store the sampled input features.
             * @param sample_y A reference to a vector to store the sampled target values.
             */
            constexpr void bootstrapSample(const std::vector<std::vector<double>>& x, const std::vector<int>& y,
                                          std::vector<std::vector<double>>& sample_x, std::vector<int>& sample_y)
            {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, x.size() - 1);
                for (std::size_t i = 0; i < x.size(); ++i)
                {
                    int index = dis(gen);
                    sample_x.push_back(x[index]);
                    sample_y.push_back(y[index]);
                }
            }
        }; // class RandomForest

        /**
         * @brief A class for performing Neural Network classification.
         */
        class NeuralNetwork
        {
        public:

            /**
             * @brief Constructs a NeuralNetwork object.
             * 
             * @param input_size The number of input features.
             * @param hidden_size The number of hidden neurons.
             * @param output_size The number of output neurons.
             */
            NeuralNetwork(int input_size, int hidden_size, int output_size) 
                : input_size(input_size), hidden_size(hidden_size), output_size(output_size)
            {
                weights_input_hidden.resize(input_size, std::vector<double>(hidden_size));
                weights_hidden_output.resize(hidden_size, std::vector<double>(output_size));
                biases_hidden.resize(hidden_size);
                biases_output.resize(output_size);
                initializeWeights();
            }

            /**
             * @brief Fits the neural network model to the provided data.
             * 
             * @param x A 2D vector of input features.
             * @param y A 2D vector of target values.
             * @param learning_rate The learning rate for training.
             * @param epochs The number of iterations for training.
             */
            constexpr void fit(const std::vector<std::vector<double>>& x, const std::vector<std::vector<double>>& y, 
                              double learning_rate = 0.01, int epochs = 1000)
            {
                for (int epoch = 0; epoch < epochs; ++epoch)
                {
                    for (std::size_t i = 0; i < x.size(); ++i)
                    {
                        // Forward pass
                        std::vector<double> hidden_output = activate(addBias(x[i], biases_hidden), weights_input_hidden);
                        std::vector<double> final_output = activate(addBias(hidden_output, biases_output), weights_hidden_output);

                        // Backward pass
                        std::vector<double> output_error = computeError(y[i], final_output);
                        std::vector<double> hidden_error = backpropagate(output_error, weights_hidden_output, hidden_output);

                        updateWeights(weights_hidden_output, hidden_output, output_error, learning_rate);
                        updateWeights(weights_input_hidden, x[i], hidden_error, learning_rate);
                        updateBiases(biases_output, output_error, learning_rate);
                        updateBiases(biases_hidden, hidden_error, learning_rate);
                    }
                }
            }

            /**
             * @brief Predicts the output for a given input sample.
             * 
             * @param sample A vector representing the input features.
             * 
             * @return A vector of predicted output values
             */
            constexpr std::vector<double> predict(const std::vector<double>& sample)
            {
                std::vector<double> hidden_output = activate(addBias(sample, biases_hidden ), weights_input_hidden);
                return activate(addBias(hidden_output, biases_output), weights_hidden_output);
            }

        private:
            int input_size;
            int hidden_size;
            int output_size;
            std::vector<std::vector<double>> weights_input_hidden;
            std::vector<std::vector<double>> weights_hidden_output;
            std::vector<double> biases_hidden;
            std::vector<double> biases_output;

            /**
             * @brief Initializes weights and biases randomly.
             */
            constexpr void initializeWeights()
            {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<> dis(-1.0, 1.0);
                for (auto& row : weights_input_hidden)
                {
                    for (auto& weight : row)
                    {
                        weight = dis(gen);
                    }
                }
                for (auto& row : weights_hidden_output)
                {
                    for (auto& weight : row)
                    {
                        weight = dis(gen);
                    }
                }
                for (auto& bias : biases_hidden)
                {
                    bias = dis(gen);
                }
                for (auto& bias : biases_output)
                {
                    bias = dis(gen);
                }
            }

            /**
             * @brief Applies the activation function to the input.
             * 
             * @param input A vector of input values.
             * @param weights A 2D vector of weights.
             * 
             * @return A vector of activated output values.
             */
            constexpr std::vector<double> activate(const std::vector<double>& input, const std::vector<std::vector<double>>& weights)
            {
                std::vector<double> output(weights[0].size(), 0);
                for (std::size_t i = 0; i < weights[0].size(); ++i)
                {
                    for (std::size_t j = 0; j < input.size(); ++j)
                    {
                        output[i] += input[j] * weights[j][i];
                    }
                    output[i] = sigmoid(output[i]);
                }
                return output;
            }

            /**
             * @brief Adds bias to the input vector.
             * 
             * @param input A vector of input values.
             * @param biases A vector of bias values.
             * 
             * @return A vector with biases added.
             */
            constexpr std::vector<double> addBias(const std::vector<double>& input, const std::vector<double>& biases)
            {
                std::vector<double> output = input;
                output.insert(output.end(), biases.begin(), biases.end());
                return output;
            }

            /**
             * @brief Computes the error between target and output.
             * 
             * @param target The target output values.
             * @param output The predicted output values.
             * 
             * @return A vector of error values.
             */
            constexpr std::vector<double> computeError(const std::vector<double>& target, const std::vector<double>& output)
            {
                std::vector<double> error(output.size());
                for (std::size_t i = 0; i < output.size(); ++i)
                {
                    error[i] = target[i] - output[i];
                }
                return error;
            }

            /**
             * @brief Backpropagates the error to compute hidden layer error.
             * 
             * @param output_error The error from the output layer.
             * @param weights The weights connecting hidden and output layers.
             * @param hidden_output The output from the hidden layer.
             * 
             * @return A vector of hidden layer error values.
             */
            constexpr std::vector<double> backpropagate(const std::vector<double>& output_error, const std::vector<std::vector<double>>& weights, const std::vector<double>& hidden_output)
            {
                std::vector<double> hidden_error(weights.size(), 0);
                for (std::size_t i = 0; i < weights.size(); ++i)
                {
                    for (std::size_t j = 0; j < weights[i].size(); ++j)
                    {
                        hidden_error[i] += output_error[j] * weights[i][j];
                    }
                    hidden_error[i] *= hidden_output[i] * (1 - hidden_output[i]);
                }
                return hidden_error;
            }

            /**
             * @brief Updates weights based on the input and error.
             * 
             * @param weights A reference to the weights to be updated.
             * @param inputs A vector of input values.
             * @param errors A vector of error values.
             * @param learning_rate The learning rate for weight updates.
             */
            constexpr void updateWeights(std::vector<std::vector<double>>& weights, const std::vector<double>& inputs, const std::vector<double>& errors, double learning_rate)
            {
                for (std::size_t i = 0; i < weights.size(); ++i)
                {
                    for (std::size_t j = 0; j < weights[i].size(); ++j)
                    {
                        weights[i][j] += learning_rate * errors[j] * inputs[i];
                    }
                }
            }

            /**
             * @brief Updates biases based on the error.
             * 
             * @param biases A reference to the biases to be updated.
             * @param errors A vector of error values.
             * @param learning_rate The learning rate for bias updates.
             */
            constexpr void updateBiases(std::vector<double>& biases, const std::vector<double>& errors, double learning_rate)
            {
                for (std::size_t i = 0; i < biases.size(); ++i)
                {
                    biases[i] += learning_rate * errors[i];
                }
            }

            /**
             * @brief Computes the sigmoid activation function.
             * 
             * @param x The input value.
             * 
             * @return The output of the sigmoid function.
             */
            constexpr double sigmoid(double x) const
            {
                return 1.0 / (1.0 + std::exp(-x));
            }
        }; // class NeuralNetwork
    } // namespace ML
} // namespace nstd
