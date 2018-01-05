#include "csvreader.h"
#include <fstream>
#include <vector>
#include <Common/memorypool.h>

CsvReader::CsvReader() : rowcount(0) , readrowcount(0)
{

}

template<class Function>
int CsvReader::EachStartRow(Function callback, int startrow)
{
    readrowcount = startrow;
    while(readrowcount < rowcount){
        callback();
        readrowcount++;
    }

    return readrowcount + 1 - startrow;
}

bool CsvReader::ParseCsvFile(const std::string &fullpath)
{
    std::ifstream infile;
    infile.open(fullpath.c_str());
    if(!infile.is_open()){
        return false;
    }

    std::string line;
    std::vector<std::string> items;

    while(!infile.eof()){
        getline(infile, line);
        items = Trim(line);
        std::unordered_map<int, std::string>  linemap;
        for(size_t i = 0; i < items.size(); i++){
            linemap[i] = items[i].empty() ? "" : items[i];
        }
        csvHandle[rowcount] = std::move(linemap);
        rowcount++;
    }
    infile.close();
    return true;
}

std::vector<std::string> CsvReader::Trim(const std::string &msg, char deli, char quotamark)
{
    std::vector<std::string> items;
    bool bStrFront = false;
    for(size_t pos = 0, lastpos = 0; pos < msg.length(); pos++){
        //" " between
        if(msg[pos] == quotamark && bStrFront){
            std::string sub = msg.substr(lastpos, pos - lastpos);
            items.push_back(sub);
            bStrFront = false;
            pos++;
            continue;
        }else if(msg[pos] == quotamark){
            bStrFront = true;
            lastpos = pos + 1;
            continue;
        }

        if(msg[pos] == deli){
            std::string sub = msg.substr(lastpos, pos - lastpos);
            items.push_back(sub);
            lastpos = pos + 1;
        }
    }
    return items;
}

template<class... Values>
bool CsvReader::Fetch(Values&... values)
{
    if(readrowcount >= rowcount)
        return false;

    FetchImpl(0, std::forward<Values&>(values)...);
}


/*********************** Manager ******************************************/
CsvReaderManager::CsvReaderManager() : folderpath("./configs")
{

}

bool CsvReaderManager::AddCsvFile(const std::string &filename)
{
    if(filename.empty() || Readers.findkey(filename)){
        return false;
    }

    CsvReader *reader = MemPoolIns.newElement<CsvReader>();
    if(nullptr == reader){
        return false;
    }

    return reader->ParseCsvFile(folderpath + "/" + filename);
}

const CsvReader *CsvReaderManager::GetReader(const std::string &filename)
{
    if(Readers.findkey(filename)){
        return Readers[filename];
    }
    return nullptr;
}

void CsvReaderManager::SetFolderPath(const std::string &folderpath)
{
    this->folderpath = folderpath;
}
