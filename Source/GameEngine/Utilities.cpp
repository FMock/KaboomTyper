#include "Utilities.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include "DrawUtils.h"

using namespace GameEngine;
using namespace GameEngine::Utility;

/*
* Reads an .xml file of font parameters and populates a FontParameters object
*/
bool Utilities::ReadXmlFile(const char* fileName, FontParameters& parameters)
{
	char* pch;

	std::string line;
	std::vector<std::string> row;

	std::ifstream in(fileName);

	if (!in.is_open()) { return false; }

	while (getline(in, line))
	{
		row.clear();

		pch = strtok((char*)line.c_str(), " \t></");
		if (pch != NULL)
			row.push_back(pch);

		while (pch != NULL)
		{
			pch = strtok(NULL, " \t></");
			if (pch != NULL)
				row.push_back(pch);
		}

		if (row.size() < 3)
			continue;

		if (row[0] == "FontsheetName")
		{
			parameters.m_fontsheetName = row[1];
		}
		else if (row[0] == "Texture")
		{                                                          // YIKES, A HARD-CODED PATH!
			std::string pathToFontFile("../../Resources/images/"); // TODO: ADJUST THE ABOVE STRING TOKENIZER CODE TO READ PATH FROM FontParameters.xml 
			pathToFontFile.append(parameters.m_fontsheetName);
			parameters.m_texture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str());
		}
		else if (row[0] == "FontsheetWidth")
		{
			parameters.m_fontsheetWidth = stoi(row[1]);
		}
		else if (row[0] == "FontsheetHeight")
		{
			parameters.m_fontsheetHeight = stoi(row[1]);
		}
		else if (row[0] == "FontWidth")
		{
			parameters.m_fontWidth = stoi(row[1]);
		}
		else if (row[0] == "FontHeight")
		{
			parameters.m_fontHeight = stoi(row[1]);
		}
		else if (row[0] == "Padding")
		{
			parameters.m_padding = stoi(row[1]);
		}
	}
	in.close();
	return true;
}

/*
* Reads an .xml file of TextBlock parameters and populates a TextBlockParameters object
*/
bool Utilities::ReadXmlFile(const char* fileName, TextBlockParameters& parameters)
{
	char* pch;

	std::string line;
	std::vector<std::string> row;

	std::ifstream in(fileName);

	if (!in.is_open()) { return false; }

	while (getline(in, line))
	{
		row.clear();

		pch = strtok((char*)line.c_str(), " \t></");
		if (pch != NULL)
			row.push_back(pch);

		while (pch != NULL)
		{
			pch = strtok(NULL, " \t></");
			if (pch != NULL)
				row.push_back(pch);
		}

		if (row.size() < 3)
			continue;
															   // YIKES, A HARD-CODED PATH!
		std::string pathToFontFile("../../Resources/images/"); // TODO: ADJUST THE ABOVE STRING TOKENIZER CODE TO READ PATH FROM TextBlockParameters.xml

		if (row[0] == "BlockWidth") // WIDTH OF A SINGLE TEXTBLOCK 
		{
			parameters.blockWidth = stoi(row[1]);
		}
		else if (row[0] == "BlockHeight") // HEIGHT OF A SINGLE TEXTBLOCK
		{
			parameters.blockHeight = stoi(row[1]);
		}
		else if (row[0] == "RedBlockFileName") // RED
		{
			parameters.redBlockFileName = row[1];
		}
		else if (row[0] == "RedBlockTexture")
		{                                          
			pathToFontFile.append(parameters.redBlockFileName);
			parameters.redBlockTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.blockWidth, &parameters.blockHeight);
		}
		if (row[0] == "BlueBlockFileName") // BLUE
		{
			parameters.blueBlockFileName = row[1];
		}
		else if (row[0] == "BlueBlockTexture")
		{                   
			pathToFontFile.append(parameters.blueBlockFileName);
			parameters.blueBlockTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.blockWidth, &parameters.blockHeight);
		}
		if (row[0] == "GreenBlockFileName") // GREEN
		{
			parameters.greenBlockFileName = row[1];
		}
		else if (row[0] == "GreenBlockTexture")
		{                              
			pathToFontFile.append(parameters.greenBlockFileName);
			parameters.greenBlockTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.blockWidth, &parameters.blockHeight);
		}
		if (row[0] == "YellowBlockFileName") // YELLOW
		{
			parameters.yellowBlockFileName = row[1];
		}
		else if (row[0] == "YellowBlockTexture")
		{                                           
			pathToFontFile.append(parameters.yellowBlockFileName);
			parameters.yellowBlockTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.blockWidth, &parameters.blockHeight);
		}
		if (row[0] == "PurpleBlockFileName") // PURPLE
		{
			parameters.purpleBlockFileName = row[1];
		}
		else if (row[0] == "PurpleBlockTexture")
		{                                     
			pathToFontFile.append(parameters.purpleBlockFileName);
			parameters.purpleBlockTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.blockWidth, &parameters.blockHeight);
		}
		if (row[0] == "WhiteBlockFileName") // WHITE
		{
			parameters.whiteBlockFileName = row[1];
		}
		else if (row[0] == "WhiteBlockTexture")
		{                                                  
			pathToFontFile.append(parameters.whiteBlockFileName);
			parameters.whiteBlockTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.blockWidth, &parameters.blockHeight);
		}
		if (row[0] == "OrangeBlockFileName") // ORANGE
		{
			parameters.orangeBlockFileName = row[1];
		}
		else if (row[0] == "OrangeBlockTexture")
		{                                        
			pathToFontFile.append(parameters.orangeBlockFileName);
			parameters.orangeBlockTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.blockWidth, &parameters.blockHeight);
		}
	}
	in.close();
	return true;
}

///*
//*  Reads an .csv file which contains AnimationParameter data
//*  and loads each animation into a vector.
//*/
//bool GameEngine::Utility::Utilities::ReadCsvFile(const char* fileName, std::vector<AnimationParameters>& animParamVect)
//{
//	std::ifstream file(fileName);
//
//	if (file.is_open())
//	{
//		std::string line;
//		// Skip the first line (header)
//		std::getline(file, line);
//		while (std::getline(file, line))
//		{
//			std::stringstream ss(line);
//			std::string token;
//			AnimationParameters param;
//			std::getline(ss, token, ',');
//			param.animationName = token;
//			std::getline(ss, token, ',');
//			param.framesInAnimation = std::stoi(token);
//			std::getline(ss, token, ',');
//			param.rowsInAnimation = std::stoi(token);
//			std::getline(ss, token, ',');
//			param.startingRow = std::stoi(token);
//			std::getline(ss, token, ',');
//			param.startingCol = std::stoi(token);
//			std::getline(ss, token, ',');
//			param.animationCount = std::stoi(token);
//			animParamVect.push_back(param);
//		}
//		return true;
//	}
//
//	return false;
//}

