#pragma once
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
		void ChangeWordCategory(DBMessanger::WordCategories category);
		std::string GetNextWord();

	private:
		// Database Access
		std::unique_ptr<KaboomTyperDB::DBMessanger> m_dbMessanger;
		std::vector<std::string> m_words;
		DBMessanger::WordCategories m_currentCategory;
		int m_currentWordIndex;
	};
}
