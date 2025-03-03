#include "../include/MarkovChain.h"
#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <random>
#include <sstream>  // std::basic_stringstream

MarkovChain::MarkovChain()
{}

MarkovChain::~MarkovChain()
{}

void MarkovChain::train(std::string_view filePath, int n)
{
    std::ifstream file(filePath.data());
    if(!file.is_open())
    {
        std::cerr << "Error in opening file..." << std::endl;
        return;
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    // use boost to split the strings in the file
    boost::split(m_wordVec, content, boost::is_any_of(" "), boost::token_compress_on);

    generateTransitionMatrix(n); // input is m_wordVec
    if (!m_transitionMatrix.empty())
        std::cout << "Transition Matrix generated successfully..." << std::endl;
    else
    {
        std::cerr << "Error while generating Transition matrix..." << std::endl;
        // throw 1;
    }
}

void MarkovChain::generateTransitionMatrix(int n)
{
    for(size_t i{0}; i<m_wordVec.size()-1; ++i)
    {
        // find (n-1) words and the next word
        // sample text: "I love to code in C++"
        std::string state;
        for(int j=0; j<n-1; ++j)
        {
            state += (j==0 ? "" : " ") + m_wordVec[i+j];
        }
        std::string nextWord = m_wordVec[i+n-1];
        m_transitionMatrix[state][nextWord]++;
    }

    // normalize to probabilites
    for (auto& [state, next_state]: m_transitionMatrix)
    {
        double total {0};
        for(auto& [_, count]: next_state)
        {
            total += count;
        }
        for(auto& [_, count]: next_state)
        {
            count /= total;
        }
    }
    debugPrintMatrix();
}

void MarkovChain::generateSentence(const std::string& start, int n, int length)
{
    std::string current;
    
    // Find a valid starting key that contains 'start'
    for (const auto& [key, _] : m_transitionMatrix)
    {
        if (key.find(start) == 0)  // If 'start' is at the beginning of the key
        {
            current = key;
            break;
        }
    }

    // If no valid key is found, pick a random one
    if (current.empty())
    {
        std::cerr << "Start word not found, picking a random key..." << std::endl;
        auto it = m_transitionMatrix.begin();
        std::advance(it, rand() % m_transitionMatrix.size());
        current = it->first;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::cout << current << " ";

    for (int i = 0; i < length; ++i)
    {
        if (m_transitionMatrix.find(current) == m_transitionMatrix.end())
            break;  // Stop if no next words are available

        double r = dis(gen);
        double sum = 0;
        std::string nextWord;

        for (auto& [word, prob] : m_transitionMatrix[current])
        {
            sum += prob;
            if (r <= sum)
            {
                nextWord = word;
                break;
            }
        }

        std::cout << nextWord << " ";

        // Update current phrase for (n-1)-gram lookup
        std::stringstream ss(current);
        std::vector<std::string> words;
        std::string word;
        while (ss >> word) words.push_back(word);

        if (words.size() >= n - 1)
            words.erase(words.begin());  // Remove the oldest word

        words.push_back(nextWord);
        current = words[0];
        for (size_t j = 1; j < words.size(); ++j)
            current += " " + words[j];
    }

    std::cout << std::endl;
}


void MarkovChain::debugPrintMatrix() {
    // this is a helper function to print the transition matrix
    std::ofstream outFile("debug_transition_matrix.txt");

    for (const auto& [prefix, nextWords] : m_transitionMatrix) {
        outFile << prefix << " -> ";
        for (const auto& [word, prob] : nextWords) {
            outFile << "[" << word << ": " << prob << "] ";
        }
        outFile << "\n";
    }

    outFile.close();
}
