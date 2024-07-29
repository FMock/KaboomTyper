#pragma once

/* WordManager uses a KaboomTyperDB to get words from the database*/

#include "../KaboomTyperDB/DBMessanger.h"
#include <memory>
#include <vector>

using namespace KaboomTyperDB;

namespace GameEngine
{
	class WordManager
	{
	public:
		WordManager();
		~WordManager() = default;
		void ChangeWordCategory(DBMessanger::WordCategory category);
		std::vector<std::string> GetWordCategories();
		std::string GetNextWord();
		DBMessanger::WordCategory GetWordCategory(const std::string& category) const;

	private:
		std::unique_ptr<KaboomTyperDB::DBMessanger> m_dbMessanger;
		std::vector<std::string> m_words;
		std::vector<std::string> m_defaultWords;
		DBMessanger::WordCategory m_currentCategory;
		int m_currentWordIndex;
	};
}
