#include "utilities.h"
#include <QDebug>

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

ConfigInfo readConfigFile(){
    ConfigInfo configValues;
    std::string line;
    std::fstream fileStream;
    fileStream.open("/home/stefano/Documenti/Programmazione/QT/pds/pds.conf");
    while (std::getline(fileStream, line)){
        std::istringstream is_line(line);
        std::string key;
        if (std::getline(is_line, key, '=')){
            std::string value;
            if (key[0] == '#')
                continue;
            if (std::getline(is_line, value)){
                configValues[key] = value;
//                qDebug() << QString::fromStdString(key) << "=" << QString::fromStdString(value);
            }
        }
    }
    return configValues;
}
