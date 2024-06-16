#pragma once

#ifdef KABOOMTYPERDB_EXPORTS
#define KABOOMTYPERDB_API __declspec(dllexport)
#else
#define KABOOMTYPERDB_API __declspec(dllimport)
#endif

#include "sqlite3.h"
#include <vector>
#include <string>

namespace KaboomTyperDB
{
	class KABOOMTYPERDB_API DBMessanger
	{
	public:
		enum WordCategories
		{
			ANIME,
			ANIMALS,
			PEOPLE,
			PLACES,
			PHRASES,
			THINGS,
			WORDCATEGORIESCOUNT
		};

		DBMessanger();
		~DBMessanger() = default;

		bool GetWords(std::vector<std::string>& container, WordCategories = WordCategories::ANIMALS);

	private:
		std::string m_configFilePath;
		std::string m_dbPath;
		std::string ReadConfigFile(const std::string& filePath);
		void ExecuteSQL(sqlite3* db, const std::string& sql);
		std::string Trim(const std::string& str);
	};
}
