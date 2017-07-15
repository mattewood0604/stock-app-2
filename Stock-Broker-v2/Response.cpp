//
//  Response.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/14/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include <iostream>
#include <cstring>

#include "Constants.hpp"
#include "Response.hpp"
#include "Tick.hpp"

Response::Response() {
    this->memory = NULL;
    this->size = 0;
}

void Response::clear() {
    this->size = 0;
}

void Response::log() const {
    if (!Constants::logResponse) {
        return;
    }

    for (unsigned int i = 0; i < this->size; i++) {
        std::cout << this->memory[i];
    }

    std::cout << std::endl;
}

void Response::recreate(void* _contents, size_t& _size) {
    this->memory = (char*)realloc(this->memory, this->size + _size + 1);
    if(this->memory == NULL) {
        std::cout << "!-!-!-!-! Not Enough Memory To Reallocate !-!-!-!-!" << std::endl;
        return;
    }

    memcpy(&(this->memory[this->size]), _contents, _size);
    this->size += _size;
    this->memory[this->size] = 0;
}

std::string Response::parseAuthentication() const {
    std::string jsonString = std::string(this->memory);

    std::string tokenString = "{\"token\":\"";
    std::string curlyString = "\"}";
    int tokenStartIndex = (int)(jsonString.find(tokenString.c_str()) + tokenString.size());
    int tokenEndIndex = (int)(jsonString.size() - curlyString.size());

    if (tokenStartIndex < 0 || tokenEndIndex < 0) {
        return "";
    }
    else if (tokenEndIndex < tokenStartIndex) {
        return "";
    }

    return jsonString.substr(tokenStartIndex, tokenEndIndex - tokenStartIndex);
}

std::vector<Tick> Response::parseQuotes() const {
    static std::string quotes = "\"results\":[";

    std::vector<Tick> ticks;
    std::string jsonString = std::string(this->memory);

    int quotesIndex = (int)jsonString.find(quotes);
    if (quotesIndex < 0) {
        return ticks;
    }

    int closingBracketIndex = (int)jsonString.find("]", quotesIndex);
    if (closingBracketIndex < 0) {
        return ticks;
    }
    std::string quotesString = jsonString.substr(quotesIndex + quotes.size(), closingBracketIndex - quotesIndex - quotes.size());
    if (quotesString.size() <= 0) {
        return ticks;
    }

    int openCurlyIndex = (int)quotesString.find("{");
    int closeCurlyIndex = (int)quotesString.find("}");
    while (openCurlyIndex >= 0 && closeCurlyIndex >= 0) {
        std::string quote = quotesString.substr(openCurlyIndex, closeCurlyIndex - openCurlyIndex + 1);

        ticks.push_back(Tick(quote));

        openCurlyIndex = (int)quotesString.find("{", closeCurlyIndex);
        closeCurlyIndex = (int)quotesString.find("}", openCurlyIndex);
    }

    return ticks;
}

std::string Response::nextUrlForInstruments() const {
    std::string next = "\"next\":\"";

    std::string jsonString = std::string(this->memory);
    int nextIndex = (int)jsonString.find(next);
    if (nextIndex < 0) {
        std::cout << "Unable to parse next url from response for instruments" << std::endl;
        return "";
    }

    int quoteIndex = (int)jsonString.find("\"}", nextIndex);
    if (quoteIndex < nextIndex) {
        std::cout << "Unable to parse next url from response for instruments" << std::endl;
        return "";
    }

    return jsonString.substr(nextIndex + next.size(), quoteIndex - (nextIndex + next.size()));
}

std::vector<std::string> Response::symbolsFromInstruments() const {
    std::string symbol = "\"symbol\":\"";
    std::string state = "\"state\":\"";
    std::string tradeable = "\"tradeable\":";

    std::vector<std::string> foundSymbols;

    std::string jsonString = std::string(this->memory);
    int symbolIndex = (int)jsonString.find(symbol);
    int stateIndex = (int)jsonString.find(state);
    int tradeableIndex = (int)jsonString.find(tradeable);

    while (symbolIndex > 0) {
        int commaIndex = (int)jsonString.find("\",", symbolIndex);
        std::string stockSymbol = jsonString.substr(symbolIndex + symbol.size(), commaIndex - (symbolIndex + symbol.size()));

        commaIndex = (int)jsonString.find("\",", stateIndex);
        std::string stateString = jsonString.substr(stateIndex + state.size(), commaIndex - (stateIndex + state.size()));

        commaIndex = (int)jsonString.find(",\"", tradeableIndex);
        std::string tradeableString = jsonString.substr(tradeableIndex + tradeable.size(), commaIndex - (tradeableIndex + tradeable.size()));

        if (tradeableString.compare("true") == 0 && stateString.compare("active") == 0) {
            foundSymbols.push_back(stockSymbol);
            //std::cout << stockSymbol << std::endl;
            /*std::cout << "STOCK: " << stockSymbol << std::endl;
             std::cout << "--------------" << std::endl;
             std::cout << "DTR: " << dayTradeRatioString << std::endl;
             std::cout << "MR : " << maintenanceRatioString << std::endl;
             std::cout << "MIR: " << marginInitialRatioString << std::endl;
             std::cout << "TRADEABLE: " << tradeableString << std::endl;
             std::cout << std::endl;
             */
        }


        symbolIndex = (int)jsonString.find(symbol, commaIndex);
        stateIndex = (int)jsonString.find(state, commaIndex);
        tradeableIndex = (int)jsonString.find(tradeable, commaIndex);
    }

    return foundSymbols;
}

unsigned int Response::parseAverageVolume() const {
    std::string averageVolume = "\"average_volume\":\"";

    std::string jsonString = std::string(this->memory);
    int averageVolumeIndex = (int)jsonString.find(averageVolume);
    if (averageVolumeIndex < 0) {
        return 0;
    }

    int commaIndex = (int)jsonString.find("\",", averageVolumeIndex);
    if (commaIndex < averageVolumeIndex) {
        return 0;
    }

    std::string averageVolumeString = jsonString.substr(averageVolumeIndex + averageVolume.size(), commaIndex - (averageVolumeIndex + averageVolume.size()));
    return atoi(averageVolumeString.c_str());
}
