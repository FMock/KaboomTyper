#include "DBMessanger.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> // For std::find_if
#include <cctype>    // For std::isspace

using namespace KaboomTyperDB;

DBMessanger::DBMessanger()
{
    m_configFilePath = "../../Config/DB_config.txt";
    m_dbPath = ReadConfigFile(m_configFilePath);
}

// Helper function to trim whitespace from the start and end of a string
std::string DBMessanger::Trim(const std::string& str)
{
    auto start = str.find_first_not_of(' ');
    auto end = str.find_last_not_of(' ');

    if (start == std::string::npos || end == std::string::npos)
    {
        return ""; // string is all spaces or empty
    }
    return str.substr(start, end - start + 1);
}

bool DBMessanger::GetWords(std::vector<std::string>& container, WordCategories category)
{
    sqlite3* db;
    int rc;

    // Open a database connection using the path from the configuration file
    rc = sqlite3_open(m_dbPath.c_str(), &db);
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

    //const char* sampleQuery = "SELECT TYPE FROM mammal LIMIT 5;"; // Query to select only TYPE column
    const char* sampleQuery = "SELECT TYPE FROM mammal;"; // Query to select only TYPE column
    rc = sqlite3_prepare_v2(db, sampleQuery, -1, &stmt, nullptr);
    if (rc == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            const unsigned char* typeText = sqlite3_column_text(stmt, 0);
            if (typeText != nullptr)
            {
                std::string word = reinterpret_cast<const char*>(typeText);
                container.push_back(Trim(word)); // Trim and add word to the container
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


// Function to read the database path from a configuration file
std::string DBMessanger::ReadConfigFile(const std::string& filePath) // Corrected scope resolution
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

void KaboomTyperDB::DBMessanger::ExecuteSQL(sqlite3* db, const std::string& sql)
{
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}
