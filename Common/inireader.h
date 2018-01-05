#ifndef INIREADER_H
#define INIREADER_H

#include <Common/singleton.h>
#include <string>
#include <unordered_map>
class IniReader final/*: public Singleton<IniReader>*/
{
public:
    IniReader(std::string path = "./inidir");

    bool ParseIniFile(const std::string &filename);

    std::string GetString(const std::string &section, const std::string &key);

    int32_t GetInt(const std::string &section, const std::string &key);

    int64_t GetLong(const std::string &section, const std::string &key);

    double GetDouble(const std::string &section, const std::string &key);

private:
    bool GetSection(const std::string &Option, std::string & section);

    bool ParseOptionKeyAndValue(const std::string &Option, std::string &Key, std::string &Value);

    std::string Trim(std::string item);
private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> IniItemsMap;
    std::string path;
};

//#define IniReaderIns IniReader::GetInstance()
#endif // INIREADER_H
