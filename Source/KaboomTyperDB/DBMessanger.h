#pragma once

#ifdef KABOOMTYPERDB_EXPORTS
#define KABOOMTYPERDB_API __declspec(dllexport)
#else
#define KABOOMTYPERDB_API __declspec(dllimport)
#endif

#include <vector>
#include <string>

namespace KaboomTyperDB
{
    class DBMessangerImpl; // Forward declaration of the implementation class (hidden from public API)

    class KABOOMTYPERDB_API DBMessanger
    {
    public:
        enum WordCategory
        {
            Mammal,
            Reptile,
            Amphibian,
            Bird,
            Fish,
            Insect,
            Arachnid,
            Crustacean,
            Myriapod,
            Annelid,
            Cnidarian,
            Mollusk,
            Echinoderm,
            Poriferan,
            Nematode,
            Eye,
            Anime,
            WORDCATEGORIESCOUNT
        };

        DBMessanger();
        ~DBMessanger();

        bool GetWords(std::vector<std::string>& container, WordCategory = WordCategory::Mammal);
        bool GetWordCategories(std::vector<std::string>& container);
        DBMessanger::WordCategory GetWordCategoryFromString(const std::string& category);

    private:
        DBMessangerImpl* m_impl; // Pointer to the implementation
    };
}
