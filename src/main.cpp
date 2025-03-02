#include "../include/MarkovChain.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main()
{
    MarkovChain markovChain;
    std::string filePath, startWord;
    int n_grams {};
    
    std::cout << "Enter the path to the training file: ";
    std::getline(std::cin, filePath);

    // Convert to absolute path
    fs::path absPath = fs::absolute(filePath);
    std::cout << absPath << std::endl;

    // Validate file existence
    if (!fs::exists(absPath)) {
        std::cerr << "Error: File does not exist!" << std::endl;
        return 1;
    }
    // get number of grams
    std::cout << "Enter number of grams: ";
    std::cin >> n_grams;

    markovChain.train(absPath.string(), n_grams);

    std::cout << "Enter the start word: " << std::endl;
    std::cin >> startWord;
    markovChain.generateSentence(startWord, n_grams);


    return 0;
}
