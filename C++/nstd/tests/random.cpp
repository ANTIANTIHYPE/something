#include <iostream>
#include <random.hpp>

int main()
{
    nstd::random rng;

    float randomFloat = rng.rand(0.0f, 1.0f);
    std::cout << "Random Float: " << randomFloat << std::endl;

    int randomInt = static_cast<int>(rng.rand(1.0, 10.0));
    std::cout << "Random Integer: " << randomInt << std::endl;

    bool randomBool = rng.randbool();
    std::cout << "Random Boolean: " << std::boolalpha << randomBool << std::endl;

    double randomNormal = rng.randnorm(0.0, 1.0);
    std::cout << "Random Normal: " << randomNormal << std::endl;

    double randomExp = rng.randexp(1.0);
    std::cout << "Random Exponential: " << randomExp << std::endl;

    int randomPoisson = rng.randpoisson(5.0);
    std::cout << "Random Poisson: " << randomPoisson << std::endl;

    std::vector<double> randomVec = rng.randvec(5, 1.0, 100.0);
    std::cout << "Random Vector: ";
    for (const auto& num : randomVec)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
