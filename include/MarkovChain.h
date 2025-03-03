#ifndef MARKOV_CHAIN_H
#define MARKOV_CHAIN_H

#include <unordered_map>
#include <string>
#include <vector>

class MarkovChain
{
private:
    std::unordered_map<std::string, std::unordered_map<std::string, double>> m_transitionMatrix;
    // std::string m_filePath;                                 // path to input file
    std::string m_start;                                    // the word to start generating from
    // int m_length;
    std::vector<std::string> m_wordVec;                     // a vector of all the words in the input
    std::unordered_map<std::string, int> m_wordsCount;      // map of each mord and its frequency
    void debugPrintMatrix();

    void generateTransitionMatrix(int n=1);

public:
    MarkovChain();
    ~MarkovChain();
    void train(std::string_view filePath, int n);
    void generateSentence(const std::string& start, int n, int length);
};

#endif // MARKOV_CHAIN_H
