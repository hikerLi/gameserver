#include "inireader.h"
#include <fstream>
#include <algorithm>    // std::find_if
IniReader::IniReader(std::string path) : path(path)
{

}

bool IniReader::ParseIniFile(const std::string &filename)
{
    std::string fullpath = path + "/" + filename;
    std::ifstream infile;
    infile.open(fullpath.c_str());

    //Does the file exist?
    if (infile.is_open() != true){
        return false;
    }

    std::string key, value, option;

    std::unordered_map<std::string, std::string> sectionmap;
    std::string sectionname;

    while (!infile.eof()){ // To get you all the lines.
        getline(infile, option); // Saves the line in STRING.

        if(option.empty()){
            continue;
        }

        //Is the option a comment
        if (option.substr(0, 1) == "#"){
            continue;
        }

        if(option.substr(0, 1) == "["){
            if(!sectionname.empty() && !sectionmap.empty()){
                IniItemsMap[sectionname] = sectionmap;
            }
            GetSection(option, sectionname);
            sectionmap.clear();
            continue;
        }

        if(ParseOptionKeyAndValue(option, key, value)){
            sectionmap[key] = value;
        }
    }

    if(!sectionname.empty() && !sectionmap.empty()){
        IniItemsMap[sectionname] = sectionmap;
    }

    infile.close();
    return true;
}

std::string IniReader::GetString(const std::string &section, const std::string &key)
{
    if(!section.empty() && !key.empty() && IniItemsMap.find(section) != IniItemsMap.end() &&
            IniItemsMap[section].find(key) != IniItemsMap[section].end()){
        return IniItemsMap[section][key];
    }
    return "";
}

int32_t IniReader::GetInt(const std::string &section, const std::string &key)
{
    std::string value = GetString(section, key);
    if(!value.empty()){
        return atoi(value.c_str());
    }
    return 0;
}

int64_t IniReader::GetLong(const std::string &section, const std::string &key)
{
    std::string value = GetString(section, key);
    if(!value.empty()){
        return atol(value.c_str());
    }
    return 0;
}

double IniReader::GetDouble(const std::string &section, const std::string &key)
{
    std::string value = GetString(section, key);
    if(!value.empty()){
        return atof(value.c_str());
    }
    return 0.f;
}

bool IniReader::GetSection(const std::string &Option, std::string &section)
{
    size_t found = Option.find(']');
    if(found != std::string::npos){
        section = Option.substr(1, found - 1);
        return true;
    }else{
        return false;
    }
}

bool IniReader::ParseOptionKeyAndValue(const std::string &Option, std::string &Key, std::string &Value)
{
    size_t found = Option.find('=');
    if(found > 100 || found == std::string::npos){
        Key = Value = "";
        return false;
    }
    Key = Trim(Option.substr(0, found - 1));
    Value = Trim(Option.substr(found + 1));
    return true;
}

std::string IniReader::Trim(std::string item)
{
    item.erase(item.begin(), std::find_if(item.begin(), item.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    item.erase(std::find_if(item.rbegin(), item.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), item.end());
    return item;
}
