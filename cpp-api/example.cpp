#include <iostream>
#include <vector>
#include "sglocator.cpp"

using namespace std;

int main(){
	BOOL err;
	string msg;
	string gameFolderName = "GarrysMod";
	vector<string> paths = getGameLocation(gameFolderName, err, msg);
	if(err){
		cout<<msg<<endl;
		return 0;
	}
	for(auto it=paths.begin(); it!=paths.end(); it++){
		cout<<*it<<endl;
	}
	return 0;
}


