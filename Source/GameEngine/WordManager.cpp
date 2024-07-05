#include "WordManager.h"
#include <algorithm>
#include <random>
#include <chrono>

using namespace GameEngine;
using namespace KaboomTyperDB;

WordManager::WordManager() : m_currentWordIndex(0), m_dbMessanger(std::make_unique<DBMessanger>())
{
    m_currentCategory = DBMessanger::WordCategories::Bird; // default word category

    if (!m_dbMessanger->GetWords(m_words, m_currentCategory))
        throw std::exception("WordManager(): Error GetWords returned false");

    // Obtain a time-based seed
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    // Use the default random engine and the seed to shuffle the vector
    std::shuffle(m_words.begin(), m_words.end(), std::default_random_engine(seed));
}

void WordManager::ChangeWordCategory(DBMessanger::WordCategories category)
{
    m_currentCategory = category;
}

std::vector<std::string> WordManager::GetWordCategories()
{
    if (!m_dbMessanger->GetWordCategories(m_words))
        throw std::exception("WordManager(): Error GetWords returned false");

    return m_words;
}

std::string WordManager::GetNextWord()
{
    if (m_currentWordIndex + 1 < m_words.size() - 1)
        m_currentWordIndex += 1;
    else
        m_currentWordIndex = 0;

    return m_words[m_currentWordIndex];
}
