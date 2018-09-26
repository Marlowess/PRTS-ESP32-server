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

std::string myHash( const std::string &key) {
    int hashVal = 17;
    std::stringstream data;

    for(int i = 0; i<key.length();  i++){
        hashVal = (hashVal+key[i])%10;
        data << hashVal;
    }
    return data.str();
}



std::string hashFunction(std::string str){
    std::vector<std::string> tokens = split(str, ',');    
    std::stringstream data;

    for(int i = 0; i < tokens.size()-1; i++)
        data << tokens.at(i);

    std::stringstream ss;
    ss << myHash(data.str());
    std::string st = ss.str();
    return st;
}
