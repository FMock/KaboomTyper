#include "WordManager.h"
#include <algorithm>
#include <random>
#include <chrono>

using namespace GameEngine;
using namespace KaboomTyperDB;

WordManager::WordManager() : m_currentWordIndex(0), m_dbMessanger(std::make_unique<DBMessanger>())
{
    m_currentCategory = DBMessanger::WordCategory::Bird; // default word category

    if (!m_dbMessanger->GetDefaultWords(m_words))
        throw std::exception("WordManager(): Error GetWords returned false");

    // Obtain a time-based seed
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    // Use the default random engine and the seed to shuffle the vector
    std::shuffle(m_words.begin(), m_words.end(), std::default_random_engine(seed));
}

void WordManager::ChangeWordCategory(DBMessanger::WordCategory category)
{
    m_currentCategory = category;

    if (!m_dbMessanger->GetWords(m_words, m_currentCategory))
        throw std::exception("WordManager(): Error GetWords returned false");
}

std::vector<std::string> WordManager::GetWordCategories()
{
    static std::vector<std::string> wordCategories;
    static bool isInitialized = false;

    if (!isInitialized)
    {
        wordCategories.push_back("Default");

        if (!m_dbMessanger->GetWordCategories(wordCategories))
            throw std::exception("WordManager::GetWordCategories(): Error GetWords returned false");

        isInitialized = true;
    }

    return wordCategories;
}

std::string WordManager::GetNextWord()
{
    if (m_currentWordIndex + 1 < m_words.size() - 1)
        m_currentWordIndex += 1;
    else
        m_currentWordIndex = 0;

    return m_words[m_currentWordIndex];
}

DBMessanger::WordCategory GameEngine::WordManager::GetWordCategory(const std::string& category) const
{
    return m_dbMessanger->GetWordCategoryFromString(category);
}
