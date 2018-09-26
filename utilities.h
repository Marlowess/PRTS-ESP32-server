#ifndef UTILITIES_H
#define UTILITIES_H

#include <unistd.h>
#include <functional>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

std::vector<std::string> split(const std::string& s, char delimiter);
std::string hashFunction(std::string str);
std::string myHash( const std::string &key);



#endif // UTILITIES_H
