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

#define HOSTS_DIR "C:\\Windows\\System32\\drivers\\etc\\hosts"
#define VERSION 1
#define SUBVERSION 1

using namespace std;

void printUsage()
{
	cout << "Usage:\n\tweblock [block|unblock]";
}

void printVersion()
{
	cout << "weblock v" << VERSION << "." << SUBVERSION << " (built " << __DATE__ << ", at "<< __TIME__ << ")" << endl;
	cout << "Licensed under the GNU GPL v3.0" << endl;
	cout << "See the included file gpl-3.0.txt or see <http://www.gnu.org/licenses/>";
}

int main(int argc, char* argv[])
{
	if (argc == 2)
	{	
		ofstream ofile(HOSTS_DIR);
		if ((strcmp(argv[1], "block") == 0) || (strcmp(argv[1], "lock") == 0))	// Program also accepts "lock" and "unlock"
		{
			string str = "127.0.0.1 63.235.20.187\n127.0.0.1 69.172.201.127\n127.0.0.1 www.reddit.com\n";
			ofile << str;
			ofile.close();
		}
		else if ((strcmp(argv[1], "unblock") == 0) || (strcmp(argv[1], "unlock") == 0))
		{
			string str = "127.0.0.1 63.235.20.187\n127.0.0.1 69.172.201.127\n";
			ofile << str;
			ofile.close();	
		}
		else if ((strcmp(argv[1], "-v") == 0) || (strcmp(argv[1], "--version") == 0))
			printVersion();
		else
			printUsage();
	}
	else
		printUsage();
}