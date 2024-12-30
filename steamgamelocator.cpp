// C++11
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <regex>

using namespace std;

string getSteamLocation(BOOL& err){
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
		return "";
	}else{
		return path;
	}
}

vector<string> getLibraryFolders(BOOL& err){
	err = false;
	BOOL error;
	vector<string> libraryFolders;
	string steamLocation = getSteamLocation(error);
	if(error){
		err = true;
		return libraryFolders;
	}
	string libraryFoldersVdf = steamLocation + "/config/libraryfolders.vdf";
	if(!filesystem::exists(libraryFoldersVdf)){
		err = true;
		return libraryFolders;
	}
	ifstream file(libraryFoldersVdf);
	if(!file){
		err = true;
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
 * @param gameFolderName: Must be the right folder name of the game, which you can get by right-click on the game in steam. 
 * 
 * @return locations: Paths where the game installed.
 * Cautions: It may contain more than one location because of the installation or uninstallation history.
 * Therefore, you need to verify for yourself which location is right.
 */
vector<string> getGameLocation(const string& gameFolderName, BOOL& err){
	err = false;
	BOOL error;
	vector<string> locations;
	vector<string> folders = getLibraryFolders(error);
	if(error){
		err = true;
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

/*
int main(){
	BOOL err;
	vector<string> v = getGameLocation("killingfloor2",err);
	if(err){
		cout<<"error"<<endl;
		return 0;
	}
	for(auto it=v.begin(); it!=v.end();it++){
		cout<<*it<<endl;
	}
	return 0;
}
*/



