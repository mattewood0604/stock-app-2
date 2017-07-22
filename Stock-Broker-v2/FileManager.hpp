//
//  File.hpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/2/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#ifndef FileManager_hpp
#define FileManager_hpp

#include "Stock.hpp"
#include "Tick.hpp"

#include <string>
#include <fstream>
#include <map>
#include <vector>

class FileManager {
public:
    FileManager();
    ~FileManager();
    
    void initForWriting();

    std::vector<std::string> getDatesWithData();

    void readTicks(Stock& _stock, const std::string& _date);
    void writeTicks(const std::vector<Tick>& _ticks);

    std::string readStockSymbolsAsCSV();

private:
    static const std::string writeDirectory;
    static const std::string readDirectory;
    std::string dayWriteDirectory;

    std::map<std::string, std::map<std::string, std::vector<Tick>>> stockTicksForDate;
    std::map<std::string, std::ofstream*> writeFiles;

    void writeTick(const Tick& _tick);
    void writeDataForSymbol(const std::string& _symbol, const std::string& _data);
    void writeDataToFile(const std::string& _data, std::ofstream& _file);
};

#endif /* File_hpp */
