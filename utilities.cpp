#include "utilities.h"

std::vector<std::string> split(const std::string& s, char delimiter){
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter)){
      tokens.push_back(token);
   }
   return tokens;
}


std::string hashFunction(std::string str){
    std::vector<std::string> tokens = split(str, ',');
    std::hash<std::string> hasher;
    std::stringstream data;

    for(int i = 0; i < tokens.size()-1; i++)
        data << tokens.at(i);

    auto hashed = hasher(data.str()); //returns std::size_t
    std::stringstream ss;
    ss << hashed << "\r\n";
    std::string st = ss.str();
    return st;
}
