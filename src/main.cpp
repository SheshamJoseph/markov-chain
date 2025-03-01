#include "../include/MarkovChain.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main()
{
    MarkovChain markovChain;
    std::string filePath, startWord;
    
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
    markovChain.train(absPath.string());

    std::cout << "Enter the start word: " << std::endl;
    std::cin >> startWord;
    markovChain.generateSentence(startWord);


    return 0;
}
