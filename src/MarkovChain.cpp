#include "../include/MarkovChain.h"
#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <random>

MarkovChain::MarkovChain()
{}

MarkovChain::~MarkovChain()
{}

void MarkovChain::train(std::string_view filePath)
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

    generateTransitionMatrix(); // input is m_wordVec
    std::cout << "Transition Matrix generated successfully..." << std::endl;

}

void MarkovChain::generateTransitionMatrix()
{
    for(size_t i{0}; i<m_wordVec.size()-1; ++i)
    {
        m_transitionMatrix[m_wordVec[i]][m_wordVec[i+1]]++;
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
}

void MarkovChain::generateSentence(const std::string& start)
{
    if (m_transitionMatrix.find(start) == m_transitionMatrix.end())
    {
        std::cerr << "Start word not found in dataset!" << std::endl;
        return;
    }

    std::string current = start;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::cout << current << " ";
    for (int i = 0; i < 200; ++i)
    {
        if (m_transitionMatrix[current].empty())
            break;  // No transitions available

        double r = dis(gen);
        double sum = 0;
        for (auto& [word, prob] : m_transitionMatrix[current])
        {
            sum += prob;
            if (r <= sum)
            {
                current = word;
                break;
            }
        }

        std::cout << current << " ";
    }
    std::cout << std::endl;
}


