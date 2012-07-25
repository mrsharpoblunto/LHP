#include "StdAfx.h"

#include <iostream>
#include <fstream>

#include "Preprocessor.hpp"
#include "PreprocessorError.hpp"

namespace LHP {

	//load a lhp from disk then
	//if a master page is specified, break the current document into placeholder sections
	//then inject the placeholders into the master page content
	//then convert all parts of the document not enclosed in <% %> tags to lua Response:content+='escapenewlines';  statements
	//return the preprocessed valid lua script.
	Preprocessor::Preprocessor(const std::string &documentRoot,const std::string &scriptFile)
	{
		std::ifstream input;
		input.open(scriptFile.c_str(),std::ios_base::in);

		bool firstPass = true;
		bool usingMasterPage = false;
		std::string masterPageFile;
		std::string currentPlaceHolderId;
		std::map<std::string,std::string> placeholders;
		std::string buffer;
		std::string fragment;

		char current;
		while (input.get(current))
		{
			if (current=='<')
			{
				if (input.get(current)) {
					if (current=='@') {
						buffer+=ConvertToLuaBlock(fragment);
						fragment="";
						std::pair<std::string,std::string> keyvalue = ParseMetaTag(input);
						if (keyvalue.first=="masterpage") {
							if (!firstPass) {
								throw new PreprocessorError("Master page declaration must appear at the beginning of the file");
							}
							else {
								usingMasterPage = true;
								masterPageFile = documentRoot+"\\Views\\"+keyvalue.second;
							}
						}
						else if (keyvalue.first=="placeholder") {
							if (keyvalue.second=="") {
								throw new PreprocessorError("Placeholder declarations must specify a placeholder id");
							}
							else {
								currentPlaceHolderId = keyvalue.second;
								buffer = "";
								fragment="";
							}
						}
						else if (keyvalue.first=="endplaceholder") {
							if (currentPlaceHolderId == "") {
								throw new PreprocessorError("Unexpected end placeholder declaration");
							}
							else {
								buffer+=ConvertToLuaBlock(fragment);
								placeholders[currentPlaceHolderId]=buffer;
								buffer="";
								fragment="";
								currentPlaceHolderId="";
							}
						}
					}
					else if (current=='%')
					{
						if (usingMasterPage && currentPlaceHolderId=="") {
							throw new PreprocessorError("Unexpected lua tag outside of placeholder");
						}
						else {
							buffer+=ConvertToLuaBlock(fragment);
							fragment="";
							buffer+=GetLuaBlock(input);
						}
					}
					else {
						fragment.push_back('<');
						fragment.push_back(current);
					}
				}
				else {
					fragment.push_back('<');
					break;
				}
			}
			else if (current=='\r')
			{
				fragment.push_back('\\');
				fragment.push_back('r');
			}
			else if (current=='\n')
			{
				fragment.push_back('\\');
				fragment.push_back('n');
			}
			else {
				fragment.push_back(current);
			}
			firstPass = false;
		}

		if (usingMasterPage) {
			if (currentPlaceHolderId!="") {
				throw new PreprocessorError("No endplaceholder declaration found for placeholder '"+currentPlaceHolderId+"'");
			}
			_content = MergePlaceHolders(masterPageFile, placeholders);		
		}
		else {
			buffer+=ConvertToLuaBlock(fragment);
			_content = buffer;
		}
		input.close();
	}

	std::string Preprocessor::MergePlaceHolders(const std::string &masterPageFile, std::map<std::string,std::string> &placeholders)  const
	{
		std::ifstream input;
		input.open(masterPageFile.c_str(),std::ios_base::in);

		std::string buffer;
		std::string fragment;
		char current;
		while (input.get(current))
		{
			if (current=='<')
			{
				if (input.get(current)) {
					if (current=='@') {
						buffer+=ConvertToLuaBlock(fragment);
						fragment="";
						std::pair<std::string,std::string> keyvalue = ParseMetaTag(input);
						if (keyvalue.first=="masterpage") {
							throw new PreprocessorError("Unexpected tag 'masterpage' in MasterPage");
						}
						else if (keyvalue.first=="placeholder") {
							if (keyvalue.second=="") {
								throw new PreprocessorError("Placeholder declarations must specify a placeholder id");
							}
							else {
								if (placeholders.find(keyvalue.second)!=placeholders.end()) {
									buffer+=placeholders[keyvalue.second];
								}
								else {
									throw new PreprocessorError("No content defined for this placeholder");
								}
							}
						}
						else if (keyvalue.first=="endplaceholder") {
							throw new PreprocessorError("Unexpected tag 'endplaceholder' in MasterPage");
						}
					}
					else {
						fragment.push_back('<');
						fragment.push_back(current);
					}
				}
				else {
					fragment.push_back('<');
					break;
				}
			}
			else if (current=='\r')
			{
				fragment.push_back('\\');
				fragment.push_back('r');
			}
			else if (current=='\n')
			{
				fragment.push_back('\\');
				fragment.push_back('n');
			}
			else {
				fragment.push_back(current);
			}
		}
		buffer+=ConvertToLuaBlock(fragment);

		return buffer;
	}

	std::string Preprocessor::ConvertToLuaBlock(const std::string &fragment)  const
	{
		if (fragment!="") {
			return "response:write('"+fragment+"')\n";
		}
		else {
			return "";
		}
	}

	std::string Preprocessor::GetLuaBlock(std::ifstream &input)  const
	{
		char current;
		std::string buffer="";
		bool firstPass = true;
		bool wrapInResponseWrite = false;

		while (input.get(current))
		{
			if (current=='%')
			{
				if (input.get(current)) {
					if (current=='>')
					{
						if (wrapInResponseWrite) {
							return "response:write("+buffer+")\n";
						}
						else {
							buffer.push_back('\n');
							return buffer;
						}
					}
					else {
						buffer.push_back('%');
						buffer.push_back(current);
					}
				}
				else {
					throw new PreprocessorError("Unexpected end of file, no closing lua tag found");
				}
			}
			else if (current=='=' && firstPass) {
				wrapInResponseWrite = true;
			}
			else {
				buffer.push_back(current);
			}
			firstPass = false;
		}
		throw new PreprocessorError("Unexpected end of file, no closing lua tag found");
	}

	std::pair<std::string,std::string> Preprocessor::ParseMetaTag(std::ifstream &input)  const
	{
		char current;
		bool processingName = true;
		std::pair<std::string,std::string> result;

		while (input.get(current))
		{
			if (current=='@')
			{
				if (input.get(current)) {
					if (current=='>') {
						return result;
					}
					else {
						throw new PreprocessorError("Unexpected '@' character found");
					}
				}
				else {
					throw new PreprocessorError("Unexpected end of file, no closing tag found");
				}
			}
			if (current=='=') {
				if (processingName) {
					processingName = false;
				}
				else {
					throw new PreprocessorError("Unexpected '=' character found");
				}
			}
			else if (current=='\r' || current=='\n')
			{
				throw new PreprocessorError("Newlines not allowed inside tag");
			}
			else {
				if (processingName) {
					result.first.push_back(current);
				}
				else {
					result.second.push_back(current);
				}
			}
		}
		throw new PreprocessorError("Unexpected end of file, no closing tag found");
	}

	const std::string &Preprocessor::GetProcessedScript() const
	{
		return _content;
	}

}