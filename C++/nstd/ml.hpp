#pragma once

#ifdef NEURALNETWORK

#include <iostream>

#endif

#include <algorithm>
#include <map>
#include <random>
#include <memory>

namespace nstd
{

/**
 * @brief Machine Learning.
 */
namespace ML
{

/**
 * @brief A class for performing Linear Regression with L2 Regularization.
 */
class LinearRegression
{
public:
    /**
     * @brief Constructs a LinearRegression object.
     * 
     * @param lambda The L2 regularization parameter.
     */
    LinearRegression(double lambda = 0.0) : m_slope(0), m_intercept(0), m_lambda(lambda) {}

    /**
     * @brief Fits the linear regression model to the provided data.
     * 
     * @param x A vector of input features.
     * @param y A vector of target values.
     */
    inline void fit(const std::vector<double>& x, const std::vector<double>& y) noexcept
    {
        int n = x.size();
        if (n == 0) return;

        double x_mean = std::accumulate(x.begin(), x.end(), 0.0) / n;
        double y_mean = std::accumulate(y.begin(), y.end(), 0.0) / n;

        double numerator = 0, denominator = 0;
        for (int i = 0; i < n; ++i)
        {
            numerator += (x[i] - x_mean) * (y[i] - y_mean);
            denominator += (x[i] - x_mean) * (x[i] - x_mean);
        }

        m_slope = numerator / (denominator + m_lambda);
        m_intercept = y_mean - m_slope * x_mean;
    }

    /**
     * @brief Predicts the target value for a given input feature.
     * 
     * @param x The input feature.
     * 
     * @return The predicted target value.
     */
    inline double predict(double x) const
    {
        return m_slope * x + m_intercept;
    }

private:
    double m_slope;     // Slope of the regression line.
    double m_intercept; // Intercept of the regression line.
    double m_lambda;    // L2 regularization parameter.
}; // class LinearRegression

/**
 * @brief A class for performing Logistic Regression with support for multi-class classification.
 */
class LogisticRegression
{
public:
    /**
     * @brief Constructs a LogisticRegression object.
     * 
     * @param num_classes The number of classes for classification.
     */
    LogisticRegression(int num_classes, int input_size) : num_classes(num_classes), m_inputSize(input_size)
    {
        m_weights.resize(num_classes, std::vector<double>(input_size));
        m_biases.resize(num_classes);
        initializeWeights();
    }

    /**
     * @brief Fits the logistic regression model to the provided data.
     * 
     * @param x A 2D vector of input features.
     * @param y A vector of target class labels.
     * @param learning_rate The learning rate for gradient descent.
     * @param epochs The number of iterations for training.
     */
    inline void fit(const std::vector<std::vector<double>>& x,
                    const std::vector<int>& y,
                    double learning_rate = 0.01,
                    int epochs = 10000) noexcept
    {
        for (int epoch = 0; epoch < epochs; ++epoch)
        {
            for (std::size_t i = 0; i < x.size(); ++i)
            {
                std::vector<double> scores = computeScores(x[i]);
                std::vector<double> probs = softmax(scores);

                // Update weights and biases
                for (int j = 0; j < num_classes; ++j)
                {
                    double error = (j == y[i]) ? 1.0 : 0.0;
                    for (std::size_t k = 0; k < static_cast<std::size_t>(m_inputSize); ++k)
                    {
                        m_weights[j][k] += learning_rate * (error - probs[j]) * x[i][k];
                    }

                    m_biases[j] += learning_rate * (error - probs[j]);
                }
            }
        }
    }

    /**
     * @brief Predicts the class probabilities for a given input sample.
     * 
     * @param sample A vector representing the input features.
     * 
     * @return A vector of predicted probabilities for each class.
     */
    inline std::vector<double> predict(const std::vector<double>& sample) const
    {
        return softmax(computeScores(sample));
    }

    /**
     * @brief Predicts the class label for a given input sample.
     * 
     * @param sample A vector representing the input features.
     * 
     * @return The predicted class label.
     */
    inline int predictClass(const std::vector<double>& sample) const
    {
        std::vector<double> probs = predict(sample);
        return std::ranges::distance(probs.begin(), std::ranges::max_element(probs));
    }

private:
    int num_classes;                            // Number of classes for classification.
    int m_inputSize;                            // Number of input features.
    std::vector<std::vector<double>> m_weights; // Weights for each class.
    std::vector<double> m_biases;               // Biases for each class.

    /**
     * @brief Computes the raw scores for each class given an input sample.
     * 
     * @param sample A vector representing the input features.
     * 
     * @return A vector of raw scores for each class.
     */
    inline std::vector<double> computeScores(const std::vector<double>& sample) const
    {
        std::vector<double> scores(num_classes, 0.0);
        for (int i = 0; i < num_classes; ++i)
        {
            for (std::size_t j = 0; j < static_cast<std::size_t>(m_inputSize); ++j)
            {
                scores[i] += m_weights[i][j] * sample[j];
            }
            scores[i] += m_biases[i];
        }

        return scores;
    }

    /**
     * @brief Computes the softmax activation for multi-class classification.
     * 
     * @param scores A vector of raw scores for each class.
     * 
     * @return A vector of softmax probabilities.
     */
    inline std::vector<double> softmax(const std::vector<double>& scores) const
    {
        std::vector<double> exp_scores(scores.size());
        double max_score = *std::ranges::max_element(scores);
        double sum_exp = 0.0;

        for (std::size_t i = 0; i < scores.size(); ++i)
        {
            exp_scores[i] = std::exp(scores[i] - max_score);
            sum_exp += exp_scores[i];
        }

        for (std::size_t i = 0; i < exp_scores.size(); ++i)
        {
            exp_scores[i] /= sum_exp;
        }

        return exp_scores;
    }

    /**
     * @brief Initializes weights and biases randomly.
     */
    void initializeWeights()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-0.01, 0.01);
        for (std::vector<double>& w : m_weights)
        {
            for (double& weight : w)
            {
                weight = dis(gen);
            }
        }

        for (double& bias : m_biases)
        {
            bias = dis(gen);
        }
    }
}; // class LogisticRegression

/**
 * @brief A struct representing a node in the decision tree.
 */
struct TreeNode
{
    double m_value;                    // Value of the node (used for leaf nodes).
    int m_featureIndex;                // Index of the feature used for splitting.
    double m_threshold;                // Threshold value for the split.
    std::shared_ptr<TreeNode> m_left;  // Pointer to the left child node.
    std::shared_ptr<TreeNode> m_right; // Pointer to the right child node.
    bool m_isLeaf;                     // Indicates if the node is a leaf.

    TreeNode() : m_value(0), m_featureIndex(-1), m_threshold(0), m_left(nullptr), m_right(nullptr), m_isLeaf(true) {}
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
    DecisionTree(int max_depth = 5) : m_root(nullptr), m_maxDepth(max_depth) {}

    /**
     * @brief Fits the decision tree model to the provided data.
     * 
     * @param x A 2D vector of input features.
     * @param y A vector of target values.
     */
    inline void fit(const std::vector<std::vector<double>>& x, const std::vector<int>& y)
    {
        m_root = buildTree(x, y, 0);
    }

    /**
     * @brief Predicts the class label for a given input sample.
     * 
     * @param sample A vector representing the input features.
     * 
     * @return The predicted class label.
     */
    inline int predict(const std::vector<double>& sample) const
    {
        return predict(sample, m_root);
    }

private:
    std::shared_ptr<TreeNode> m_root; // Pointer to the root node of the tree.
    int m_maxDepth;                   // Maximum depth of the tree.

    /**
     * @brief Builds the decision tree recursively.
     * 
     * @param x A 2D vector of input features.
     * @param y A vector of target values.
     * @param depth The current depth of the tree.
     * 
     * @return A shared pointer to the root node of the constructed subtree.
     */
    inline std::shared_ptr<TreeNode> buildTree(const std::vector<std::vector<double>>& x, const std::vector<int>& y, int depth)
    {
        if (x.empty() || depth >= m_maxDepth)
        {
            return createLeafNode(y);
        }

        int best_feature = -1;
        double best_threshold = 0.0;
        double best_gain = 0.0;
        std::vector<std::vector<double>> best_left_x, best_right_x;
        std::vector<int> best_left_y, best_right_y;

        for (std::size_t feature_index = 0; feature_index < x[0].size(); ++feature_index)
        {
            std::vector<double> thresholds;
            for (const std::vector<double>& sample : x)
            {
                thresholds.push_back(sample[feature_index]);
            }

            std::sort(thresholds.begin(), thresholds.end());
            thresholds.erase(std::unique(thresholds.begin(), thresholds.end()), thresholds.end());

            for (const double threshold : thresholds)
            {
                std::vector<std::vector<double>> left_x, right_x;
                std::vector<int> left_y, right_y;

                for (std::size_t i = 0; i < x.size(); ++i)
                {
                    if (x[i][feature_index] <= threshold)
                    {
                        left_x.push_back(x[i]);
                        left_y.push_back(y[i]);
                    }
                    else
                    {
                        right_x.push_back(x[i]);
                        right_y.push_back(y[i]);
                    }
                }

                double gain = informationGain(y, left_y, right_y);
                if (gain > best_gain)
                {
                    best_gain = gain;
                    best_feature = feature_index;
                    best_threshold = threshold;
                    best_left_x = left_x;
                    best_right_x = right_x;
                    best_left_y = left_y;
                    best_right_y = right_y;
                }
            }
        }

        if (best_gain == 0.0)
        {
            return createLeafNode(y);
        }

        std::shared_ptr<TreeNode> node = std::make_shared<TreeNode>();
        node->m_featureIndex = best_feature;
        node->m_threshold = best_threshold;
        node->m_left = buildTree(best_left_x, best_left_y, depth + 1);
        node->m_right = buildTree(best_right_x, best_right_y, depth + 1);
        node->m_isLeaf = false;

        return node;
    }

    /**
     * @brief Creates a leaf node with the most common class label.
     * 
     * @param y A vector of target values.
     * 
     * @return A shared pointer to the created leaf node.
     */
    inline std::shared_ptr<TreeNode> createLeafNode(const std::vector<int>& y)
    {
        std::shared_ptr<nstd::ML::TreeNode> node = std::make_shared<TreeNode>();
        std::map<int, int> counts;
        for (const int label : y)
        {
             counts[label]++;
        }

        node->m_value = std::max_element(counts.begin(), counts.end(), [](const auto& a, const auto& b)
        {
            return a.second < b.second;
        })->first;

        node->m_isLeaf = true;

        return node;
    }

    /**
     * @brief Computes the information gain from a split.
     * 
     * @param parent_y A vector of target values before the split.
     * @param left_y A vector of target values for the left child.
     * @param right_y A vector of target values for the right child.
     * 
     * @return The information gain from the split.
     */
    inline double informationGain(const std::vector<int>& parent_y, const std::vector<int>& left_y, const std::vector<int>& right_y)
    {
        double parent_entropy = entropy(parent_y);
        double left_entropy = entropy(left_y);
        double right_entropy = entropy(right_y);
        double weighted_entropy = (left_y.size() * left_entropy + right_y.size() * right_entropy) / parent_y.size();
        return parent_entropy - weighted_entropy;
    }

    /**
     * @brief Computes the entropy of a set of labels.
     * 
     * @param y A vector of target values.
     * 
     * @return The entropy of the labels.
     */
    inline double entropy(const std::vector<int>& y)
    {
        std::map<int, int> counts;
        for (const int label : y)
        {
            counts[label]++;
        }

        double entropy = 0.0;
        for (const auto& count : counts)
        {
            double probability = static_cast<double>(count.second) / y.size();
            entropy -= probability * std::log2(probability);
        }

        return entropy;
    }

    /**
     * @brief Predicts the class label for a given input sample using the decision tree.
     * 
     * @param sample A vector representing the input features.
     * @param node A pointer to the current node in the tree.
     * 
     * @return The predicted class label.
     */
    inline int predict(const std::vector<double>& sample, const std::shared_ptr<TreeNode>& node) const
    {
        if (node->m_isLeaf)
        {
            return static_cast<int>(node->m_value);
        }
        if (sample[node->m_featureIndex] <= node->m_threshold)
        {
            return predict(sample, node->m_left);
        }
        else
        {
            return predict(sample, node->m_right);
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
     * @param m_trees The number of trees in the forest.
     * @param max_depth The maximum depth of each tree.
     */
    RandomForest(int m_trees, int max_depth = 5) : m_trees(m_trees), m_maxDepth(max_depth) {}

    /**
     * @brief Fits the random forest model to the provided data.
     * 
     * @param x A 2D vector of input features.
     * @param y A vector of target values.
     */
    inline void fit(const std::vector<std::vector<double>>& x, const std::vector<int>& y)
    {
        for (int i = 0; i < m_trees; ++i)
        {
            DecisionTree tree(m_maxDepth);
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
    inline int predict(const std::vector<double>& sample) const
    {
        std::map<int, int> votes;
        for (const DecisionTree& tree : trees)
        {
            int prediction = tree.predict(sample);
            votes[prediction]++;
        }

        return std::ranges::max_element(votes, [](const auto& a, const auto& b)
        {
            return a.second < b.second;
        })->first;
    }

private:
    int m_trees;                          // Number of trees in the forest.
    int m_maxDepth;                       // Maximum depth of each tree.
    std::vector<DecisionTree> trees;      // Vector of decision trees.

    /**
     * @brief Creates a bootstrap sample from the provided data.
     * 
     * @param x A 2D vector of input features.
     * @param y A vector of target values.
     * @param sample_x A reference to a vector to store the sampled input features.
     * @param sample_y A reference to a vector to store the sampled target values.
     */
    inline void bootstrapSample(const std::vector<std::vector<double>>& x, const std::vector<int>& y,
                                std::vector<std::vector<double>>& sample_x, std::vector<int>& sample_y) noexcept
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

#ifdef NEURALNETWORK

/**
 * @todo REWORK
 * 
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
        : m_inputSize(input_size), m_hiddenSize(hidden_size), m_outputSize(output_size)
    {
        m_weightsInputHidden.resize(hidden_size, std::vector<double>(input_size));
        m_weightsHiddenOutput.resize(output_size, std::vector<double>(hidden_size));
        m_biasesHidden.resize(hidden_size);
        m_biasesOutput.resize(output_size);
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
    inline void fit(const std::vector<std::vector<double>>& x, const std::vector<std::vector<double>>& y, 
                    double learning_rate = 0.01, int epochs = 10000) noexcept
    {
        for (int epoch = 0; epoch < epochs; ++epoch)
        {
            for (std::size_t i = 0; i < x.size(); ++i)
            {
                // Forward pass
                std::vector<double> hidden_input = addBias(x[i], m_biasesHidden);
                std::vector<double> hidden_output = activate(hidden_input, m_weightsInputHidden, true); // ReLU
                std::vector<double> output_input = addBias(hidden_output, m_biasesOutput);
                std::vector<double> final_output = activate(output_input, m_weightsHiddenOutput, true); // Sigmoid

                // Backward pass
                std::vector<double> output_error = computeError(y[i], final_output);
                std::vector<double> hidden_error = backpropagate(output_error, m_weightsHiddenOutput, hidden_output);

                updateWeights(m_weightsHiddenOutput, hidden_output, output_error, learning_rate);
                updateWeights(m_weightsInputHidden, x[i], hidden_error, learning_rate);
                updateBiases(m_biasesOutput, output_error, learning_rate);
                updateBiases(m_biasesHidden, hidden_error, learning_rate);
            }

            std::cout << "Epoch " << epoch << ", Loss: " << computeLoss(x, y) << std::endl;
        }
    }

    /**
     * @brief Predicts the output for a given input sample.
     * 
     * @param sample A vector representing the input features.
     * 
     * @return A vector of predicted output values.
     */
    inline std::vector<double> predict(const std::vector<double>& sample) noexcept
    {
        std::vector<double> hidden_output = activate(addBias(sample, m_biasesHidden), m_weightsInputHidden, true); // ReLU
        return activate(addBias(hidden_output, m_biasesOutput), m_weightsHiddenOutput, false);                     // Sigmoid
    }

private:
    int m_inputSize;                                        // Number of input features.
    int m_hiddenSize;                                       // Number of hidden neurons.
    int m_outputSize;                                       // Number of output neurons.
    std::vector<std::vector<double>> m_weightsInputHidden;  // Weights from input to hidden layer.
    std::vector<std::vector<double>> m_weightsHiddenOutput; // Weights from hidden to output layer.
    std::vector<double> m_biasesHidden;                     // Biases for hidden layer.
    std::vector<double> m_biasesOutput;                     // Biases for output layer.

    /**
     * @brief Initializes weights and biases randomly.
     */
    inline void initializeWeights() noexcept
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-1.0, 1.0);
        for (std::vector<double>& row : m_weightsInputHidden)
        {
            for (double& weight : row)
            {
                weight = dis(gen);
            }
        }

        for (std::vector<double>& row : m_weightsHiddenOutput)
        {
            for (double& weight : row)
            {
                weight = dis(gen);
            }
        }

        for (double& bias : m_biasesHidden)
        {
            bias = dis(gen);
        }

        for (double& bias : m_biasesOutput)
        {
            bias = dis(gen);
        }
    }

    /**
     * @brief Applies the activation function (ReLU for hidden layer, Sigmoid for output layer).
     * 
     * @param input A vector of input values.
     * @param weights A matrix of weights.
     * @param is_hidden_layer A boolean that indicates whether the layer is a hidden layer.
     * 
     * @return A vector of activated output values.
     */
    std::vector<double> activate(const std::vector<double>& input, 
                                 const std::vector<std::vector<double>>& weights, 
                                 bool is_hidden_layer) noexcept
    {
        std::vector<double> output(weights[0].size());
        for (std::size_t j = 0; j < weights[0].size(); ++j)
        {
            output[j] = 0;
            for (std::size_t i = 0; i < input.size(); ++i)
            {
                output[j] += input[i] * weights[j][i];
            }
            output[j] = is_hidden_layer ? std::max(0.0, output[j]) : 1.0 / (1.0 + std::exp(-output[j])); // ReLU for hidden, Sigmoid for output
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
    inline std::vector<double> addBias(const std::vector<double>& input, 
                                       const std::vector<double>& biases) noexcept
    {
        std::vector<double> output(input);
        for (std::size_t i = 0; i < biases.size(); ++i)
        {
            output.push_back(input[i] + biases[i]);
        }
        return output;
    }

    /**
     * @brief Computes the error between the predicted and actual output.
     * 
     * @param actual The actual output values.
     * @param predicted The predicted output values.
     * 
     * @return A vector of errors.
     */
    inline std::vector<double> computeError(const std::vector<double>& actual, 
                                            const std::vector<double>& predicted) noexcept
    {
        std::vector<double> error(actual.size());
        for (std::size_t i = 0; i < actual.size(); ++i)
        {
            error[i] = actual[i] - predicted[i];
        }
        return error;
    }

    inline double computeLoss(const std::vector<std::vector<double>>& x, 
                              const std::vector<std::vector<double>>& y) noexcept
    {
        double loss = 0.0;
        for (std::size_t i = 0; i < x.size(); ++i)
        {
            std::vector<double> prediction = predict(x[i]);
            for (std::size_t j = 0; j < y[i].size(); ++j)
            {
                loss += std::pow(y[i][j] - prediction[j], 2); // Mean Squared Error
            }
        }
        return loss / x.size();
    }

    /**
     * @brief Backpropagates the error to compute the hidden layer error.
     * 
     * @param output_error The error from the output layer.
     * @param weights The weights connecting the hidden and output layers.
     * @param hidden_output The output from the hidden layer.
     * 
     * @return A vector of hidden layer errors.
     */
    inline std::vector<double> backpropagate(const std::vector<double>& output_error, 
                                             const std::vector<std::vector<double>>& weights, 
                                             const std::vector<double>& hidden_output) noexcept
    {
        std::vector<double> hidden_error(m_hiddenSize);
        for (std::size_t j = 0; j < m_hiddenSize; ++j)
        {
            hidden_error[j] = 0;
            for (std::size_t k = 0; k < output_error.size(); ++k)
            {
                hidden_error[j] += output_error[k] * weights[k][j];
            }
            hidden_error[j] *= (hidden_output[j] > 0 ? 1 : 0); // Derivative of ReLU
        }
        return hidden_error;
    }

    /**
     * @brief Updates the weights based on the input and error.
     * 
     * @param weights The weights to be updated.
     * @param input The input values.
     * @param error The error values.
     * @param learning_rate The learning rate for the update.
     */
    inline void updateWeights(std::vector<std::vector<double>>& weights, 
                              const std::vector<double>& inputs, 
                              const std::vector<double>& errors, 
                              double learning_rate) noexcept
    {
        for (std::size_t j = 0; j < weights.size(); ++j)
        {
            for (std::size_t k = 0; k < weights[j].size(); ++k)
            {
                weights[j][k] += learning_rate * errors[k] * inputs[j];
            }
        }
    }

    /**
     * @brief Updates the biases based on the error.
     * 
     * @param biases The biases to be updated.
     * @param error The error values.
     * @param learning_rate The learning rate for the update.
     */
    inline void updateBiases(std::vector<double>& biases, 
                             const std::vector<double>& errors, 
                             double learning_rate) noexcept
    {
        for (std::size_t i = 0; i < biases.size(); ++i)
        {
            biases[i] += learning_rate * errors[i];
        }
    }
}; // class NeuralNetwork

#endif

} // namespace ML

} // namespace nstd
