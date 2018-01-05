#ifndef CSVREADER_H
#define CSVREADER_H

#include <Common/singleton.h>
#include <Concurrent/concurrent_unordered_map.h>
#include <unordered_map>
#include <vector>

class CsvReader final
{
public:
    CsvReader();

    template<class Function>
    int EachStartRow(Function callback, int startrow = 0);

    template<class... Values>
    bool Fetch(Values&... values);
private:
    bool ParseCsvFile(const std::string &fullpath);

    template <class Value>
    Value ConvertValue(const std::string& item){
        if(std::is_same<int, Value>::value){
            return std::atoi(item.c_str());
        }if(std::is_same<long, Value>::value){
            return std::atol(item.c_str());
        }if(std::is_same<long long, Value>::value){
            return std::atoll(item.c_str());
        }if(std::is_same<double, Value>::value){
            return std::atof(item.c_str());
        }if(std::is_same<std::string, Value>::value){
            return item;
        }
        return item;
    }

    template <class Value>
    void GetValue(int i, Value& value){
        value = ConvertValue<Value>(csvHandle[readrowcount][i]);
    }

    template <class Value>
    void FetchImpl(int i, Value& value) {
        GetValue(i, value);
    }

    template <typename Value, class... Values>
    void FetchImpl(int i, Value& value, Values&... values) {
        GetValue(i, value);
        FetchImpl(i + 1, std::forward<Values&>(values)...);
    }

    std::vector<std::string> Trim(const std::string &msg, char deli = ',', char quotamark = '"' );
private:
    int rowcount;
    int readrowcount;
    std::unordered_map<int, std::unordered_map<int, std::string>> csvHandle;

    friend class CsvReaderManager;
};

class CsvReaderManager final
{
public:
    CsvReaderManager();

    bool AddCsvFile(const std::string &filename);

    const CsvReader *GetReader(const std::string &filename);

    void SetFolderPath(const std::string &folderpath);
private:
    Concurrent::concurrent_unordered_map<std::string, CsvReader*> Readers;
    std::string folderpath;
};

#endif // CSVREADER_H
