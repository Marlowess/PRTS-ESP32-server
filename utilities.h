#ifndef UTILITIES_H
#define UTILITIES_H

#include <unistd.h>
#include <functional>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

typedef std::map<std::string, std::string> ConfigInfo;

std::vector<std::string> split(const std::string& s, char delimiter);
std::string hashFunction(std::string str);
std::string myHash( const std::string &key);
ConfigInfo readConfigFile();



#endif // UTILITIES_H
