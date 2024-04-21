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
			std::string pathToFontFile("../../Resources/images/fonts/"); // TODO: ADJUST THE ABOVE STRING TOKENIZER CODE TO READ PATH FROM FontParameters.xml 
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
		std::string pathToFontFile("../../Resources/images/Colors/"); // TODO: ADJUST THE ABOVE STRING TOKENIZER CODE TO READ PATH FROM TextBlockParameters.xml

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
		else if (row[0] == "DarkRedBlockFileName") // DARK RED
		{
			parameters.darkRedBlockFileName = row[1];
		}
		else if (row[0] == "DarkRedBlockTexture")
		{
			pathToFontFile.append(parameters.darkRedBlockFileName);
			parameters.darkRedBlockTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.blockWidth, &parameters.blockHeight);
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
		if (row[0] == "DarkBlueBlockFileName") // DARK BLUE
		{
			parameters.darkBlueBlockFileName = row[1];
		}
		else if (row[0] == "DarkBlueBlockTexture")
		{
			pathToFontFile.append(parameters.darkBlueBlockFileName);
			parameters.darkBlueBlockTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.blockWidth, &parameters.blockHeight);
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
		if (row[0] == "DarkGreenBlockFileName") // DARK GREEN
		{
			parameters.darkGreenBlockFileName = row[1];
		}
		else if (row[0] == "DarkGreenBlockTexture")
		{
			pathToFontFile.append(parameters.darkGreenBlockFileName);
			parameters.darkGreenBlockTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.blockWidth, &parameters.blockHeight);
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
		if (row[0] == "DarkYellowBlockFileName") // DARK YELLOW
		{
			parameters.darkYellowBlockFileName = row[1];
		}
		else if (row[0] == "DarkYellowBlockTexture")
		{
			pathToFontFile.append(parameters.darkYellowBlockFileName);
			parameters.darkYellowBlockTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.blockWidth, &parameters.blockHeight);
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
		if (row[0] == "DarkPurpleBlockFileName") // DARK PURPLE
		{
			parameters.darkPurpleBlockFileName = row[1];
		}
		else if (row[0] == "DarkPurpleBlockTexture")
		{
			pathToFontFile.append(parameters.darkPurpleBlockFileName);
			parameters.darkPurpleBlockTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.blockWidth, &parameters.blockHeight);
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
		if (row[0] == "BrownBlockFileName") // BROWN
		{
			parameters.brownBlockFileName = row[1];
		}
		else if (row[0] == "BrownBlockTexture")
		{
			pathToFontFile.append(parameters.brownBlockFileName);
			parameters.brownBlockTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.blockWidth, &parameters.blockHeight);
		}
		if (row[0] == "DarkGrayBlockFileName") // DARK GRAY
		{
			parameters.darkGrayBlockFileName = row[1];
		}
		else if (row[0] == "DarkGrayBlockTexture")
		{
			pathToFontFile.append(parameters.darkGrayBlockFileName);
			parameters.darkGrayBlockTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.blockWidth, &parameters.blockHeight);
		}
		if (row[0] == "BlackBlockFileName") // BLACK
		{
			parameters.blackBlockFileName = row[1];
		}
		else if (row[0] == "BlackBlockTexture")
		{
			pathToFontFile.append(parameters.blackBlockFileName);
			parameters.blackBlockTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.blockWidth, &parameters.blockHeight);
		}
	}
	in.close();
	return true;
}

/// <summary>
/// Read the parameter values for Color
/// </summary>
/// <param name="fileName">the name of the XML file for the color parameters</param>
/// <param name="parameters">A reference to a ColorParameters object</param>
/// <returns></returns>
bool Utilities::ReadXmlFile(const char* fileName, ColorParameters& parameters)
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
		// YIKES, A HARD-CODED PATH! CAN THIS GO IN A CONFIG FILE?
		std::string pathToFontFile("../../Resources/images/Colors/");

		if (row[0] == "TextureWidth") 
		{
			parameters.textureWidth = stoi(row[1]);
		}
		else if (row[0] == "TextureHeight")
		{
			parameters.textureHeight = stoi(row[1]);
		}
		else if (row[0] == "RedFileName") // RED
		{
			parameters.redFileName = row[1];
		}
		else if (row[0] == "RedTexture")
		{
			pathToFontFile.append(parameters.redFileName);
			parameters.redTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.textureWidth, &parameters.textureHeight);
		}
		else if (row[0] == "DarkRedFileName") // DARK RED
		{
			parameters.darkRedFileName = row[1];
		}
		else if (row[0] == "DarkRedTexture")
		{
			pathToFontFile.append(parameters.darkRedFileName);
			parameters.darkRedTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.textureWidth, &parameters.textureHeight);
		}
		if (row[0] == "BlueFileName") // BLUE
		{
			parameters.blueFileName = row[1];
		}
		else if (row[0] == "BlueTexture")
		{
			pathToFontFile.append(parameters.blueFileName);
			parameters.blueTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.textureWidth, &parameters.textureHeight);
		}
		if (row[0] == "DarkBlueFileName") // DARK BLUE
		{
			parameters.darkBlueFileName = row[1];
		}
		else if (row[0] == "DarkBlueTexture")
		{
			pathToFontFile.append(parameters.darkBlueFileName);
			parameters.darkBlueTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.textureWidth, &parameters.textureHeight);
		}
		if (row[0] == "GreenFileName") // GREEN
		{
			parameters.greenFileName = row[1];
		}
		else if (row[0] == "GreenTexture")
		{
			pathToFontFile.append(parameters.greenFileName);
			parameters.greenTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.textureWidth, &parameters.textureHeight);
		}
		if (row[0] == "DarkGreenFileName") // DARK GREEN
		{
			parameters.darkGreenFileName = row[1];
		}
		else if (row[0] == "DarkGreenTexture")
		{
			pathToFontFile.append(parameters.darkGreenFileName);
			parameters.darkGreenTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.textureWidth, &parameters.textureHeight);
		}
		if (row[0] == "YellowFileName") // YELLOW
		{
			parameters.yellowFileName = row[1];
		}
		else if (row[0] == "YellowTexture")
		{
			pathToFontFile.append(parameters.yellowFileName);
			parameters.yellowTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.textureWidth, &parameters.textureHeight);
		}
		if (row[0] == "DarkYellowFileName") // DARK YELLOW
		{
			parameters.darkYellowFileName = row[1];
		}
		else if (row[0] == "DarkYellowTexture")
		{
			pathToFontFile.append(parameters.darkYellowFileName);
			parameters.darkYellowTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.textureWidth, &parameters.textureHeight);
		}
		if (row[0] == "PurpleFileName") // PURPLE
		{
			parameters.purpleFileName = row[1];
		}
		else if (row[0] == "PurpleTexture")
		{
			pathToFontFile.append(parameters.purpleFileName);
			parameters.purpleTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.textureWidth, &parameters.textureHeight);
		}
		if (row[0] == "DarkPurpleFileName") // DARK PURPLE
		{
			parameters.darkPurpleFileName = row[1];
		}
		else if (row[0] == "DarkPurpleTexture")
		{
			pathToFontFile.append(parameters.darkPurpleFileName);
			parameters.darkPurpleTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.textureWidth, &parameters.textureHeight);
		}
		if (row[0] == "WhiteFileName") // WHITE
		{
			parameters.whiteFileName = row[1];
		}
		else if (row[0] == "WhiteTexture")
		{
			pathToFontFile.append(parameters.whiteFileName);
			parameters.whiteTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.textureWidth, &parameters.textureHeight);
		}
		if (row[0] == "OrangeFileName") // ORANGE
		{
			parameters.orangeFileName = row[1];
		}
		else if (row[0] == "OrangeTexture")
		{
			pathToFontFile.append(parameters.orangeFileName);
			parameters.orangeTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.textureWidth, &parameters.textureHeight);
		}
		if (row[0] == "BrownFileName") // BROWN
		{
			parameters.brownFileName = row[1];
		}
		else if (row[0] == "BrownTexture")
		{
			pathToFontFile.append(parameters.brownFileName);
			parameters.brownTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.textureWidth, &parameters.textureHeight);
		}
		if (row[0] == "DarkGrayFileName") // DARK GRAY
		{
			parameters.darkGrayFileName = row[1];
		}
		else if (row[0] == "DarkGrayTexture")
		{
			pathToFontFile.append(parameters.darkGrayFileName);
			parameters.darkGrayTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.textureWidth, &parameters.textureHeight);
		}
		if (row[0] == "BlackFileName") // BLACK
		{
			parameters.blackFileName = row[1];
		}
		else if (row[0] == "BlackTexture")
		{
			pathToFontFile.append(parameters.blackFileName);
			parameters.blackTexture = DrawUtilities::glTexImageTGAFile(pathToFontFile.c_str(), &parameters.textureWidth, &parameters.textureHeight);
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

