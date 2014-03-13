/*	
	weblock: semi-automated hosts file maintenance program.
    Copyright (C) 2013 Jdc1197

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Contact Jdc1197 by email at: Jdc1197@gmail.com
*/

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <assert.h>
#include <vector>

#define HOSTS_DIR "C:\\Windows\\System32\\drivers\\etc\\hosts"
#define VERSION 1
#define SUBVERSION 2


using namespace std;

const char* keywordfilepath = "keywords";	// file path to keywords file

enum class ProgramFunction { PrintVersion, PrintUsage, RegisterKeyword, LoadHostfile, Error};

struct KeywordPair { string keyword, path; };
typedef vector<KeywordPair> KeywordList;

KeywordList loadKeywordList(const char* filename)
{
	ifstream infile(filename);
	if (infile.is_open())
	{
		KeywordList list;
		string line;
		while (getline(infile, line))
		{
			bool setkeyword = true;	// for parsing
			string keyword, path;
			for (unsigned int i = 0; i < line.length(); i++)
			{
				if (line[i] == ';' && setkeyword == true)
					setkeyword = false;
				else if (setkeyword == true)
					keyword += line[i];
				else
					path += line[i];
			}
			KeywordPair pair;
			pair.keyword = keyword;
			pair.path = path;
			list.push_back(pair);
		}
		return list;
	}
	else
	{
		cout << "Could not open keyword file " << filename << endl;
	}
}

void saveKeywordList(KeywordList list, const char * filename)
{
	ofstream ofile(filename);
	if (ofile.is_open())
	{
		for (unsigned int i = 0; i < list.size(); i++)
		{
			ofile << list[i].keyword << ";" << list[i].path << endl;
		}
	}
}

void registerKeyword(const char* keyword, const char* filepath,  const char* keywordfilename)
{
	KeywordList list = loadKeywordList(keywordfilename);
	KeywordPair pair;
	pair.keyword = keyword;
	pair.path = filepath;
	list.push_back(pair);
	saveKeywordList(list, keywordfilename);
}

struct ParsedArguments
{
	ProgramFunction function;
	
	// for ProgramFunction::LoadHostfile
	char * hostfileToLoad;	// this could be either a file path or a keyword
	
	// for ProgarmFunction::RegisterKeyword
	char * keywordName;
	char * fileName;
};

void copyFile(const char * src_filename, const char * dst_filename)
{
	ifstream src(src_filename, ios::binary);
	assert(src.is_open()); // assert that the file does exist
	ofstream dst(dst_filename, ios::binary);
	dst << src.rdbuf();
}

void printUsage()
{
	cout << "Usage:\n\tweblock ([hostfile to load]|[registered hostfile])" << endl;
	cout << "\t\t-r, --register [register name] [hostfile to register]\n\t\t\t Binds a keyword to a loadable hostfile" << endl;
	cout << "\n\t\t-v, --version\n\t\t\t Prints version and liscense information" << endl;
}

void printVersion()
{
	cout << "weblock v" << VERSION << "." << SUBVERSION << " (built " << __DATE__ << ", at "<< __TIME__ << ")" << endl;
	cout << "Licensed under the GNU GPL v3.0" << endl;
	cout << "See the included file gpl-3.0.txt or go to <http://www.gnu.org/licenses/>";
}

ParsedArguments parseArguments(int argc, char* argv[])
{
	ParsedArguments arg;
	assert(argc > 0);	// At least one argument is passed by default
	if (argc == 1)		// If no items besides the default were passed, print the usage
	{
		arg.function = ProgramFunction::PrintUsage;
		return arg;
	}
	
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-v") == 0)
		{
			arg.function = ProgramFunction::PrintVersion;
			return arg;
		}
		else if ((strcmp(argv[i], "-r") == 0) || (strcmp(argv[i], "--register") == 0))
		{
			if (i+3 == argc)		// if there are ONLY 2 more arguments past this one
			{
				arg.function = ProgramFunction::RegisterKeyword;
				arg.keywordName = argv[i+1];
				arg.fileName = argv[i+2];
				return arg;
			}
		}
		else
		{
			arg.function = ProgramFunction::LoadHostfile;
			arg.hostfileToLoad = argv[i];
			return arg;
		}
		
		
	}
}

int main(int argc, char* argv[])
{
	saveKeywordList(loadKeywordList("keywords"), "keywords2");
	ParsedArguments arg = parseArguments(argc, argv);
	if (arg.function == ProgramFunction::PrintUsage)
		printUsage();
	else if (arg.function == ProgramFunction::PrintVersion)
		printVersion();
	else if (arg.function == ProgramFunction::RegisterKeyword)
		registerKeyword(arg.keywordName, arg.fileName, keywordfilepath);
	else if (arg.function == ProgramFunction::LoadHostfile)
	{
		KeywordList list = loadKeywordList(keywordfilepath);
		for (unsigned int i = 0; i < list.size(); i++)
		{
			if (list[i].keyword == arg.hostfileToLoad)
			{
				copyFile(list[i].path.c_str(), HOSTS_DIR);
				return 0;
			}
		}
		// if the program reaches this point, then a keyword was NOT passed
		copyFile(arg.hostfileToLoad, HOSTS_DIR);
	}
}