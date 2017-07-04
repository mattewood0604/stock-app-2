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

class FileManager {
public:
//    static const std::string mainDirectory;
//
//    static void init();
//
    static void readTicks(Stock& _stock, const std::string& _date);
//    static bool readQuoteAtStockIndex(const unsigned int& _index);
//
//    static void writeTickToFile(const Tick& _tick);
//
//    static std::string readStockSymbolsForQuotes();
//
//    static void writeTimeSpanProfitsForSymbol(const std::string& _symbol, const std::string& _data);
//    static void writeProfitsForSymbol(const std::string& _symbol, const std::string& _data);

private:
//    static std::string quotesDirectory;
//    static const std::string stockSymbolsForQuotesDirectory;
//
//    static std::map<std::string, std::ofstream*> symbolFiles;

//    static void writeDataForSymbol(const std::string& _symbol, const std::string& _data);
//    static void writeDataToFile(const std::string& _data, std::ofstream& _file);
};

#endif /* File_hpp */
