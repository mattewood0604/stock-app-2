//
//  Tick.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/2/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include "Constants.hpp"
#include "FileManager.hpp"
#include "Tick.hpp"

#include <iostream>
#include <ctime>
#include <sstream>

std::string Tick::bidKey = "\"bid_price\":";
std::string Tick::bidQtyKey = "\"bid_size\":";
std::string Tick::offerKey = "\"ask_price\":";
std::string Tick::offerQtyKey = "\"ask_size\":";
std::string Tick::timeKey = "\"time\":";
std::string Tick::priceKey = "\"last_trade_price\":";
std::string Tick::symbolKey = "\"symbol\":";

Tick::Tick() {
    this->json = "";
    this->dayTime = (uint64_t)time(0) * 1000;
    this->symbol = "";
    this->price = 0;
    this->bid = 0;
    this->bidQty = 0;
    this->offer = 0;
    this->offerQty = 0;
}

Tick::Tick(const std::string& _symbol, const std::string& _json, FROM _from) {
    if (_from == CSV) {
        this->fromCSV(_symbol, _json);
        return;
    }

    this->json = _json;

    this->dayTime = (uint64_t)time(0) * 1000;
    this->symbol = parseString(symbolKey);
    this->price = parseFloat(priceKey);
    this->bid = parseFloat(bidKey);
    this->bidQty = parseInt(bidQtyKey);
    this->offer = parseFloat(offerKey);
    this->offerQty = parseInt(offerQtyKey);
}

void Tick::fromCSV(const std::string& _symbol, const std::string& _quote) {
    if (_quote.length() == 0) {
        return;
    }

    if (_quote[0] == 't') {
        return;
    }

    this->symbol = _symbol;

    int lastCommaIndex = -1;
    int commaIndex = (int)_quote.find(',');

    std::string value = _quote.substr(lastCommaIndex + 1, commaIndex - lastCommaIndex - 1);
    this->dayTime = stoll(value);
    lastCommaIndex = commaIndex;

    commaIndex = (int)_quote.find(',', lastCommaIndex + 1);
    value = _quote.substr(lastCommaIndex + 1, commaIndex - lastCommaIndex);
    this->price = stof(value);
    lastCommaIndex = commaIndex;

    commaIndex = (int)_quote.find(',', lastCommaIndex + 1);
    value = _quote.substr(lastCommaIndex + 1, commaIndex - lastCommaIndex);
    this->bid = stof(value);
    lastCommaIndex = commaIndex;

    commaIndex = (int)_quote.find(',', lastCommaIndex + 1);
    value = _quote.substr(lastCommaIndex + 1, commaIndex - lastCommaIndex);
    this->bidQty = stoi(value);
    lastCommaIndex = commaIndex;

    commaIndex = (int)_quote.find(',', lastCommaIndex + 1);
    value = _quote.substr(lastCommaIndex + 1, commaIndex - lastCommaIndex);
    this->offer = stof(value);
    lastCommaIndex = commaIndex;

    commaIndex = (int)_quote.find(',', lastCommaIndex + 1);
    value = _quote.substr(lastCommaIndex + 1, commaIndex - lastCommaIndex);
    this->offerQty = stoi(value);
    lastCommaIndex = commaIndex;
}

void Tick::writeToFile() const {
    //FileManager::writeTickToFile(*this);
}

void Tick::log() const {
    if (!Constants::logTicks) {
        return;
    }

    std::cout << this->symbol << " = {" << std::endl;
    std::cout << "\t" << timeKey << " " << this->dayTime << std::endl;
    std::cout << "\t" << priceKey << " " << this->price << std::endl;
    std::cout << "\t" << bidKey << " " << this->bid << std::endl;
    std::cout << "\t" << bidQtyKey << " " << this->bidQty << std::endl;
    std::cout << "\t" << offerKey << " " << this->offer << std::endl;
    std::cout << "\t" << offerQtyKey << " " << this->offerQty << std::endl;
    std::cout << "}" << std::endl;
}

std::string Tick::toCSV() const {
    std::string csv;

    csv.append(std::to_string(dayTime));
    csv.append(",");
    csv.append(std::to_string(price));
    csv.append(",");
    csv.append(std::to_string(bid));
    csv.append(",");
    csv.append(std::to_string(bidQty));
    csv.append(",");
    csv.append(std::to_string(offer));
    csv.append(",");
    csv.append(std::to_string(offerQty));
    csv.append("\n");

    return csv;
}

const std::string& Tick::getSymbol() const {
    return this->symbol;
}

int Tick::parseInt(const std::string& _key) const {
    int bidIndex = (int)this->json.find(_key);
    int commaIndex = (int)this->json.find(",", bidIndex);
    std::string bidString = this->json.substr(bidIndex + _key.size(), commaIndex - bidIndex - _key.size());
    return atoi(bidString.c_str());
}

float Tick::parseFloat(const std::string& _key) const {
    int keyIndex = (int)this->json.find(_key);
    int commaIndex = (int)this->json.find(",", keyIndex);
    std::string floatString = this->json.substr(keyIndex + _key.size(), commaIndex - keyIndex - _key.size());
    floatString.erase(floatString.begin());
    floatString.pop_back();
    return atof(floatString.c_str());
}

std::string Tick::parseString(const std::string& _key) const {
    int keyIndex = (int)this->json.find(_key);
    int commaIndex = (int)this->json.find(",", keyIndex);
    return this->json.substr(keyIndex + _key.size() + 1, commaIndex - keyIndex - _key.size() - 2); // +1 and -2 for removing quotes
}

uint64_t Tick::parseTime() const {
    uint64_t timeNumber;

    int keyIndex = (int)this->json.find(timeKey);
    int commaIndex = (int)this->json.find(",", keyIndex);

    std::string timeString = this->json.substr(keyIndex + timeKey.size(), commaIndex - keyIndex - timeKey.size());
    std::stringstream convert(timeString);
    convert >> timeNumber;
    
    return timeNumber;
}
