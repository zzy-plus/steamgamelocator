// C++11
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <regex>

using namespace std;

string getSteamLocation(BOOL& err, string& msg){
	err = false;
	HKEY hSubKey;
	DWORD dwType, dwSize = 255;
	char szBuffer[256];
	char* path = NULL;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Valve\\Steam", 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
		if (RegQueryValueEx(hSubKey, "SteamPath", NULL, &dwType, (LPBYTE)szBuffer, &dwSize) == ERROR_SUCCESS) {
			if (dwType == REG_SZ)
				path = szBuffer;
		}
		RegCloseKey(hSubKey);
	}
	if(path == NULL){
		err = true;
		msg = "Can not find steam installation path";
		return "";
	}else{
		return path;
	}
}

vector<string> getLibraryFolders(BOOL& err, string& msg){
	err = false;
	BOOL error;
	string error_msg;
	vector<string> libraryFolders;
	string steamLocation = getSteamLocation(error, error_msg);
	if(error){
		err = true;
		msg = error_msg;
		return libraryFolders;
	}
	string libraryFoldersVdf = steamLocation + "/config/libraryfolders.vdf";
	if(!filesystem::exists(libraryFoldersVdf)){
		err = true;
		msg = "File \"libraryfolders.vdf\" not found";
		return libraryFolders;
	}
	ifstream file(libraryFoldersVdf);
	if(!file){
		err = true;
		msg = "Open \"libraryfolders.vdf\" failed";
		return libraryFolders;
	}
	stringstream buffer;
	buffer << file.rdbuf();
	string content = buffer.str();
	
	// 正则匹配
	regex pattern("\"path\".+\"(.+)\"");
	std::smatch match;
	while (std::regex_search(content, match, pattern)) {
		string commonPath = match.str(1) + "\\steamapps\\common";
		if(filesystem::exists(commonPath) && filesystem::is_directory(commonPath)){
			libraryFolders.push_back(commonPath);
		}
		content = match.suffix();
	}
	return libraryFolders;
}

/**
 * @brief Get installation location of the game.
 * 
 * @param err: Setted to true when any error occurs.
 * @param msg: Message about error info.
 * @param gameFolderName: Must be the right folder name of the game, which you can get by right-click on the game in steam. 
 * 
 * @return locations: Paths where the game installed.
 * Cautions: It may contain more than one location because of the installation or uninstallation history.
 * Therefore, you need to verify for yourself which location is right.
 */
vector<string> getGameLocation(const string& gameFolderName, BOOL& err, string& msg){
	err = false;
	BOOL error;
	string error_msg;
	vector<string> locations;
	vector<string> folders = getLibraryFolders(error, error_msg);
	if(error){
		err = true;
		msg = error_msg;
		return locations;
	}
	for(auto it = folders.begin(); it != folders.end(); it++){
		string gamePath = *it + "\\" + gameFolderName;
		if(filesystem::exists(gamePath) && filesystem::is_directory(gamePath)){
			locations.push_back(gamePath);
		}
	}
	return locations;
}






