//
//  Constants.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/2/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include "Constants.hpp"

const bool Constants::logTicks = false;
const bool Constants::logCandles = false;
const bool Constants::logBuySell = false;
const bool Constants::logProfits = true;
const bool Constants::logResponse = true;

const bool Constants::clearOrderAtDayEnd = false;

const std::string Constants::readDirectory = "/Users/Matt/Desktop/symbol_data/";
const std::string Constants::writeDirectory = "/Users/Matt/Desktop/";

const std::string Constants::stocksForQuotesAsCsv = "JNUG,JDST,NUGT,DUST";
