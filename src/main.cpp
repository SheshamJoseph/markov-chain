#include "../include/MarkovChain.h"
#include <iostream>
#include <filesystem>
#include <getopt.h>

namespace fs = std::filesystem;

static struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {"file-path", no_argument, 0, 'f'},
    {"n-grams", no_argument, 0, 'n'},
    {"length", no_argument, 0, 'l'},
    {"start", no_argument, 0, 's'},
    {0,0,0,0}
};

void displayHelp()
{
    std::cout << "Usage: ./markov-chain [option]\n";
    std::cout << "Options:\n";
    std::cout << "\t-h, --help                          - displays help.\n";
    std::cout << "\t-f <path>,  --file-path <path>      - path to the input file.\n";
    std::cout << "\t-n <num>,   --n-grams <num>         - N-grams size.\n";
    std::cout << "\t-l <num>,  --length <num>           - Number of words to generate.\n";
    std::cout << "\t-s <word>, --start <word>           - Start word for the sentence.\n";
    std::cout << "\tExample: ./markov-chain --file data.txt --n 3 --start \"love\" --length 50\n";
}

int main(int argc, char* argv[])
{
    MarkovChain markovChain;
    std::string filePath, startWord;
    fs::path absPath;
    int n_grams {};
    int sentenceLength {};

    int option_index = 0;
    int option;
    while ((option = getopt_long(argc, argv, "hf:l:n:s:", long_options, &option_index)) != -1) {
        switch (option) {
        case 'h': // --help or -h
            displayHelp();
            return 0;
        case 'f': // --file-path or -f
            filePath = static_cast<std::string>(optarg);
            absPath = fs::absolute(filePath);
            if (!fs::exists(absPath)) {
                std::cerr << "Error: File does not exist!" << std::endl;
                return 1;
            }
            break;
        case 'n': // --n-grams or -n
            try {
                n_grams = std::atoi(optarg);
            } catch (const std::exception& e) {
                std::cerr << "Error: Invalid number for --n-grams.\n";
                return 1;
            }
            break;
        case 'l': // --length or -l
            try {
                sentenceLength = std::atoi(optarg);
            } catch (const std::exception& e) {
                std::cerr << "Error: Invalid number for --length.\n";
                return 1;
            }
            break;
        case 's': // --start or -s
            startWord = static_cast<std::string>(optarg);
            break;

        default: // Invalid option
            std::cerr << "Error: Invalid option entered.\n";
            return 1;
        }
    }
 
    markovChain.train(absPath.string(), n_grams);
    markovChain.generateSentence(startWord, n_grams, sentenceLength);


    return 0;
}
