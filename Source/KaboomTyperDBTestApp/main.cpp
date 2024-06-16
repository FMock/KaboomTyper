#include "sqlite3.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Function to execute SQL commands
void executeSQL(sqlite3* db, const std::string& sql)
{
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

// Function to read the database path from a configuration file
std::string readConfigFile(const std::string& filePath)
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

int main()
{
    std::string configFilePath = "../../Config/DB_config.txt"; // Update this path as needed
    std::string dbPath = readConfigFile(configFilePath);
    if (dbPath.empty())
    {
        return 1;
    }

    sqlite3* db;
    int rc;

    // Open a database connection using the path from the configuration file
    rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc)
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Opened database successfully\n";
    }

    // Example query to verify table existence
    const char* sampleQuery = "SELECT * FROM sqlite_master WHERE type='table' AND name='mammal';";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sampleQuery, -1, &stmt, nullptr);
    if (rc == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            std::cout << "Table 'mammal' exists.\n";
        }
        else
        {
            std::cerr << "Table 'mammal' does not exist.\n";
        }
    }
    else
    {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);

    // Sample data query to ensure table is populated
    sampleQuery = "SELECT * FROM mammal LIMIT 5;";
    rc = sqlite3_prepare_v2(db, sampleQuery, -1, &stmt, nullptr);
    if (rc == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            std::cout << "ID: " << sqlite3_column_text(stmt, 0)
                << ", CLASS_ID: " << sqlite3_column_text(stmt, 1)
                << ", TYPE: " << sqlite3_column_text(stmt, 2) << std::endl;
        }
    }
    else
    {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);

    // Close the database connection
    sqlite3_close(db);

    return 0;
}
