#include "DBMessanger.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> // For std::find_if
#include <cctype>    // For std::isspace>
#include "sqlite3.h"

namespace KaboomTyperDB
{
    // Using the Pimpl idiom here since std::string doesn't export the required symbols for DLL use
    // Define the implementation class
    class DBMessangerImpl
    {
    public:
        std::string m_configFilePath;
        std::string m_dbPath;

        std::string ReadConfigFile(const std::string& filePath);
        void ExecuteSQL(sqlite3* db, const std::string& sql);
        std::string Trim(const std::string& str);
        std::string GetCategoryString(DBMessanger::WordCategories category);

        ~DBMessangerImpl() = default; // Explicitly define the destructor
    };

    DBMessanger::DBMessanger()
        : m_impl(new DBMessangerImpl())
    {
        m_impl->m_configFilePath = "../../Config/DB_config.txt";
        m_impl->m_dbPath = m_impl->ReadConfigFile(m_impl->m_configFilePath);
    }

    DBMessanger::~DBMessanger()
    {
        delete m_impl;
    }

    bool DBMessanger::GetWords(std::vector<std::string>& container, WordCategories category)
    {
        sqlite3* db;
        int rc;

        // Open a database connection using the path from the configuration file
        rc = sqlite3_open(m_impl->m_dbPath.c_str(), &db);
        if (rc)
        {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        else
        {
            std::cout << "Opened database successfully\n";
        }

        sqlite3_stmt* stmt;

        // Get the category string
        std::string categoryString = m_impl->GetCategoryString(category);

        // Build the query string
        std::string query = "SELECT TYPE FROM " + categoryString + ";";

        rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        if (rc == SQLITE_OK)
        {
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                const unsigned char* typeText = sqlite3_column_text(stmt, 0);
                if (typeText != nullptr)
                {
                    std::string word = reinterpret_cast<const char*>(typeText);
                    container.push_back(m_impl->Trim(word)); // Trim and add word to the container
                }
            }
        }
        else
        {
            std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);

        // Close the database connection
        sqlite3_close(db);
        return true;
    }

    // Implementation class methods
    std::string DBMessangerImpl::ReadConfigFile(const std::string& filePath)
    {
        std::ifstream file(filePath);
        if (!file.is_open())
        {
            std::cerr << "Could not open config file: " << filePath << std::endl;
            return "";
        }

        std::string line;
        std::string dbPath;
        while (std::getline(file, line))
        {
            std::istringstream ss(line);
            std::string key, value;
            if (std::getline(ss, key, '=') && std::getline(ss, value))
            {
                if (key == "db_path")
                {
                    dbPath = value;
                    break;
                }
            }
        }
        file.close();
        return dbPath;
    }

    void DBMessangerImpl::ExecuteSQL(sqlite3* db, const std::string& sql)
    {
        char* errMsg = nullptr;
        int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK)
        {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        }
    }

    // Helper function to trim whitespace from the start and end of a string
    std::string DBMessangerImpl::Trim(const std::string& str)
    {
        auto start = str.find_first_not_of(' ');
        auto end = str.find_last_not_of(' ');

        if (start == std::string::npos || end == std::string::npos)
        {
            return ""; // string is all spaces or empty
        }
        return str.substr(start, end - start + 1);
    }

    std::string DBMessangerImpl::GetCategoryString(DBMessanger::WordCategories category)
    {
        switch (category)
        {
        case DBMessanger::Mammal:
            return "Mammal";
        case DBMessanger::Reptile:
            return "Reptile";
        case DBMessanger::Amphibian:
            return "Amphibian";
        case DBMessanger::Bird:
            return "Bird";
        case DBMessanger::Fish:
            return "Fish";
        case DBMessanger::Insect:
            return "Insect";
        case DBMessanger::Arachnid:
            return "Arachnid";
        case DBMessanger::Crustacean:
            return "Crustacean";
        case DBMessanger::Myriapod:
            return "Myriapod";
        case DBMessanger::Annelid:
            return "Annelid";
        case DBMessanger::Mollusk:
            return "Mollusk";
        default:
            return "Unknown";
        }
    }
}
