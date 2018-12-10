#pragma once
#include <string>
using namespace std;

class DisplayAsset {
private:
	string name;
	string resource_name;
public:
	DisplayAsset(string name, string resource_name) : name(name), resource_name(resource_name) {
	}
};