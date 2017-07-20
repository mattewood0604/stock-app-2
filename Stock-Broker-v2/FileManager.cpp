//
//  File.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/2/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include <algorithm>
#include <cstring>
#include <dirent.h>
#include <iostream>

#include "Constants.hpp"
#include "FileManager.hpp"

FileManager::FileManager() {
    this->dayWriteDirectory = FileManager::writeDirectory;
    this->stockTicksForDate = std::map<std::string, std::map<std::string, std::vector<Tick>>>();
    this->writeFiles = std::map<std::string, std::ofstream*>();
}

void FileManager::initForWriting() {
    time_t currentTime = time(0);
    struct tm* now = localtime(&currentTime);

    this->dayWriteDirectory = writeDirectory;
    this->writeFiles = std::map<std::string, std::ofstream*>();

    std::string month = (now->tm_mon + 1 < 10) ? std::to_string(now->tm_mon + 1).insert(0, "0") : std::to_string(now->tm_mon + 1);
    std::string day = (now->tm_mday < 10) ? std::to_string(now->tm_mday).insert(0, "0") : std::to_string(now->tm_mday);

    this->dayWriteDirectory.append("/");
    this->dayWriteDirectory.append(month);
    this->dayWriteDirectory.append("_");
    this->dayWriteDirectory.append(day);
    this->dayWriteDirectory.append("_");
    this->dayWriteDirectory.append(std::to_string(now->tm_year + 1900));
    this->dayWriteDirectory.append("/");

    std::string makeDirectory = "mkdir ";
    makeDirectory.append(this->dayWriteDirectory);

    system(makeDirectory.c_str());
}

std::vector<std::string> FileManager::getDatesWithData() {
    std::vector<std::string> dates;

    DIR* directory = opendir(this->readDirectory.c_str());
    struct dirent* entry = readdir(directory);

    while (entry != NULL) {
        std::string directoryName = std::string(entry->d_name);
        if (entry->d_type == DT_DIR && directoryName.length() > 2) { // > 2 for . and .. directories
            dates.push_back(entry->d_name);
        }

        entry = readdir(directory);
    }
    
    closedir(directory);
    return dates;
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

    std::string name = this->readDirectory + "/" + _date + "/" + _stock.getSymbol() + ".csv";
    std::ifstream* symbolFile = new std::ifstream(name);
    if (!symbolFile->is_open()) {
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
        this->writeTick(_ticks[i]);
    }
}

void FileManager::writeTick(const Tick& _tick) {
    std::string symbol = _tick.getSymbol();
    std::string csv = _tick.toCSV();
    writeDataForSymbol(symbol, csv);
}

void FileManager::writeDataForSymbol(const std::string& _symbol, const std::string& _data) {
    std::ofstream* symbolFile = this->writeFiles[_symbol];
    if (symbolFile != NULL) {
        this->writeDataToFile(_data, *symbolFile);
    }
    else {
        std::string fileName = _symbol + ".csv";
        std::string filePath = this->dayWriteDirectory + fileName;
        filePath.erase(std::remove(filePath.begin(), filePath.end(), '\\'), filePath.end());
        symbolFile = new std::ofstream(filePath, std::ofstream::out | std::ofstream::app);
        if (symbolFile->is_open()) {
            this->writeDataToFile(_data, *symbolFile);
            this->writeFiles[_symbol] = symbolFile;
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

std::string FileManager::readStockSymbolsAsCSV() {
    std::string name = this->readDirectory + "/StocksForData.txt";
    std::ifstream* symbolFile = new std::ifstream(name);

    if (!symbolFile->is_open()) {
        std::cout << "FileManager::readStockSymbols - Error opening stock symbols file" << std::endl;
        std::cout << strerror(errno) << std::endl;
        return "";
    }

    symbolFile->seekg(0, symbolFile->end);
    unsigned int fileLength = (unsigned int)symbolFile->tellg();
    symbolFile->seekg(0, symbolFile->beg);

    char* symbolFileData = new char[fileLength];
    symbolFile->read(symbolFileData, fileLength);

    std::string symbols = std::string(symbolFileData);
    int lastNewlineIndex = -1;
    std::string stockSymbols;
    for (unsigned int i = 0; i < fileLength; i++) {
        if (symbols[i] == '\n') {
            std::string symbol = symbols.substr(lastNewlineIndex + 1, i - lastNewlineIndex - 1);
            stockSymbols.append(symbol);
            stockSymbols.append(",");
            lastNewlineIndex = i;
        }
    }

    stockSymbols.pop_back();
    return stockSymbols;
}
