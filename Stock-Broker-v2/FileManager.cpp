//
//  File.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/2/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include "Constants.hpp"
#include "FileManager.hpp"

#include <iostream>

std::map<std::string, std::map<std::string, std::vector<Tick>>> FileManager::stockTicksForDate;
std::map<std::string, std::ofstream*> FileManager::writeFiles;

std::string FileManager::writeDirectory = "";


void FileManager::initForWriting() {
    FileManager::writeDirectory = Constants::writeDirectory;

    time_t currentTime = time(0);
    struct tm* now = localtime(&currentTime);

    std::string month = (now->tm_mon + 1 < 10) ? std::to_string(now->tm_mon + 1).insert(0, "0") : std::to_string(now->tm_mon + 1);
    std::string day = (now->tm_mday < 10) ? std::to_string(now->tm_mday).insert(0, "0") : std::to_string(now->tm_mday);

    FileManager::writeDirectory.append(month);
    FileManager::writeDirectory.append("_");
    FileManager::writeDirectory.append(day);
    FileManager::writeDirectory.append("_");
    FileManager::writeDirectory.append(std::to_string(now->tm_year + 1900));
    FileManager::writeDirectory.append("/");

    std::string makeDirectory = "mkdir ";
    makeDirectory.append(FileManager::writeDirectory);

    system(makeDirectory.c_str());
}

void FileManager::readTicks(Stock& _stock, const std::string& _date) {
    std::map<std::string, std::vector<Tick>>& datesTicksForStock = stockTicksForDate[_stock.getSymbol()];
    std::vector<Tick>& ticksForDate = datesTicksForStock[_date];
    if (ticksForDate.size()) {
        for (unsigned int i = 0; i < ticksForDate.size(); i++) {
            _stock.addTick(ticksForDate[i]);
        }

        return;
    }
    else {
        std::cout << "NO TICKS FOR " << _stock.getSymbol() << " on " << _date << std::endl;
    }

    std::string name = Constants::readDirectory + "/" + _date + "/" + _stock.getSymbol() + ".csv";
    std::ifstream* symbolFile = new std::ifstream(name);
    if (!symbolFile->is_open())
    {
        std::cout << "ERROR OPENING SYMBOL FILE FOR READING QUOTES" << std::endl;
        std::cout << strerror(errno) << std::endl;
        return;
    }

    symbolFile->seekg(0, symbolFile->end);
    unsigned int fileLength = (unsigned int)symbolFile->tellg();
    symbolFile->seekg(0, symbolFile->beg);

    char* symbolFileData = new char[fileLength];
    symbolFile->read(symbolFileData, fileLength);

    std::string quotes = std::string(symbolFileData);
    int lastNewlineIndex = -1;
    for (unsigned int i = 0; i < fileLength; i++) {
        if (quotes[i] == '\n') {
            std::string quote = quotes.substr(lastNewlineIndex + 1, i - lastNewlineIndex - 1);
            Tick tick = Tick(quote, _stock.getSymbol(), Tick::FROM::CSV);
            _stock.addTick(tick);
            lastNewlineIndex = i;

            ticksForDate.push_back(tick);
        }
    }

    symbolFile->close();

    delete [] symbolFileData;
}

void FileManager::writeTicks(const std::vector<Tick>& _ticks) {
    for (unsigned int i = 0; i < _ticks.size(); i++) {
        FileManager::writeTick(_ticks[i]);
    }
}

void FileManager::writeTick(const Tick& _tick) {
    std::string symbol = _tick.getSymbol();
    std::string csv = _tick.toCSV();
    writeDataForSymbol(symbol, csv);
}

void FileManager::writeDataForSymbol(const std::string& _symbol, const std::string& _data) {
    std::ofstream* symbolFile = FileManager::writeFiles[_symbol];
    if (symbolFile != NULL) {
        FileManager::writeDataToFile(_data, *symbolFile);
    }
    else {
        std::string fileName = _symbol + ".csv";
        symbolFile = new std::ofstream(FileManager::writeDirectory + fileName, std::ofstream::out | std::ofstream::app);
        if (symbolFile->is_open()) {
            FileManager::writeDataToFile(_data, *symbolFile);
            FileManager::writeFiles[_symbol] = symbolFile;
        }
        else {
            delete symbolFile;
        }
    }
}

void FileManager::writeDataToFile(const std::string& _data, std::ofstream& _file) {
    uint64_t pos = _file.tellp();
    _file.seekp(pos);
    _file.write(_data.c_str(), _data.size());
    _file.flush();
}

//const std::string FileManager::mainDirectory = "/Users/Matt/Desktop/symbol_data/";
//const std::string FileManager::stockSymbolsForQuotesDirectory = mainDirectory + "StocksForQuotes.txt";
//std::string FileManager::quotesDirectory;
//
//std::map<std::string, std::ofstream*> FileManager::symbolFiles;
//
//void FileManager::init() {
//    quotesDirectory = mainDirectory;
//
//    time_t currentTime = time(0);
//    struct tm* now = localtime(&currentTime);
//
//    std::string month = (now->tm_mon + 1 < 10) ? std::to_string(now->tm_mon + 1).insert(0, "0") : std::to_string(now->tm_mon + 1);
//    std::string day = (now->tm_mday < 10) ? std::to_string(now->tm_mday).insert(0, "0") : std::to_string(now->tm_mday);
//
//    quotesDirectory.append(month);
//    quotesDirectory.append("_");
//    quotesDirectory.append(day);
//    quotesDirectory.append("_");
//    quotesDirectory.append(std::to_string(now->tm_year + 1900));
//    quotesDirectory.append("/");
//
//    std::string makeDirectory = "mkdir ";
//    makeDirectory.append(quotesDirectory);
//
//    system(makeDirectory.c_str());
//}
//
//std::string FileManager::readStockSymbolsForQuotes() {
//    std::ifstream symbolFile(stockSymbolsForQuotesDirectory);
//
//    if (!symbolFile.is_open())
//    {
//        std::cout << "ERROR READING STOCK SYMBOLS FOR QUOTES" << std::endl;
//        return "";
//    }
//
//    symbolFile.seekg(0, symbolFile.end);
//    unsigned int fileLength = (unsigned int)symbolFile.tellg();
//    symbolFile.seekg(0, symbolFile.beg);
//
//    char* symbolQuotesFileData = new char[fileLength];
//    symbolFile.read(symbolQuotesFileData, fileLength);
//
//    std::string symbolsForQuotesAsCSV;
//    std::string symbols = std::string(symbolQuotesFileData);
//    int lastNewlineIndex = -1;
//    for (unsigned int i = 0; i < fileLength; i++)
//    {
//        if (symbols[i] == '\n') {
//            std::string symbolForQuote = symbols.substr(lastNewlineIndex + 1, i - lastNewlineIndex - 1);
//            symbolsForQuotesAsCSV.append(symbolForQuote).append(",");
//            lastNewlineIndex = i;
//        }
//    }
//
//    symbolsForQuotesAsCSV.pop_back();
//    return symbolsForQuotesAsCSV;
//}
//
//bool FileManager::readQuoteAtStockIndex(const unsigned int& _index) {
//    return readQuotes(_index);
//}
//
//void FileManager::writeTimeSpanProfitsForSymbol(const std::string& _symbol, const std::string& _data) {
//    std::string fileName = "/Users/Matt/Desktop/timeSpan_profits/" + _symbol + "-profits.txt";
//    std::ofstream symbolFile(fileName);
//    FileManager::writeDataToFile(_data, symbolFile);
//}
//
//void FileManager::writeProfitsForSymbol(const std::string& _symbol, const std::string& _data) {
//    std::string fileName = FileManager::mainDirectory + "/symbol_profits/" + _symbol + "-profits.txt";
//    std::ofstream symbolFile(fileName);
//    writeDataToFile(_data, symbolFile);
//}
//
//void FileManager::writeDataToFile(const std::string& _data, std::ofstream& _file) {
//    uint64_t pos = _file.tellp();
//    _file.seekp(pos);
//    _file.write(_data.c_str(), _data.size());
//    _file.flush();
//}