//
//  Maximizer.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/13/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include "Dates.hpp"
#include "FileManager.hpp"
#include "Maximizer.hpp"
#include "Stock.hpp"

#include <future>
#include <iostream>

std::string Maximizer::stockSymbol = "";

Maximizer::Maximizer(const std::string& _stockSymbol) {
    Maximizer::stockSymbol = _stockSymbol;
    this->readStockTicks();
}

void Maximizer::readStockTicks() {
    Dates dates;
    Stock stock(this->stockSymbol, 0, ZigZag(0, 0), OnYourMark(0, 0, 0, 0));
    for (unsigned int i = 0; i < dates.dates.size(); i++) {
        FileManager::readTicks(stock, dates.dates[i]);
    }
}

ZigZagMaximize* Maximizer::runZigZag(ZigZagMaximize* _zigZagMaximize) {
    unsigned int lengthStart = 1;
    unsigned int lengthIterations = 60;

    Dates dates;
    for (unsigned int movingAverageLength = lengthStart; movingAverageLength <= lengthStart + lengthIterations; movingAverageLength++) {
        _zigZagMaximize->movingAverageLength = movingAverageLength;

        Stock stock(Maximizer::stockSymbol, _zigZagMaximize->percentage, ZigZag(_zigZagMaximize->candleOffset, _zigZagMaximize->movingAverageLength), OnYourMark(0, 0, 0, 0));

        for (unsigned int date = 0; date < dates.dates.size(); date++) {
            stock.clearData();

            FileManager::readTicks(stock, dates.dates[date]);

            for (unsigned int j = 0; j < stock.getTotalTicks(); j++) {
                stock.mockRestCall(j);
            }

            stock.resetDailyProfits();
        }

        _zigZagMaximize->profit = stock.getTotalProfits();
        _zigZagMaximize->totalTrades = stock.getTotalTrades();
        _zigZagMaximize->positiveTrades = stock.getPositiveTrades();
        _zigZagMaximize->negativeTrades = stock.getNegativeTrades();
        _zigZagMaximize->log();
    }

    return _zigZagMaximize;
}

void Maximizer::maximizeZigZag() {
    float percentageIncrement = 0.005;

    unsigned int offsetStart = 1;
    float percentageStart = 0.01;
    unsigned int offsetIterations = 120;
    unsigned int percentageIterations = 1;

    uint64_t totalTime = 0;

    const unsigned int NUMBER_OF_THREADS = 8;
    unsigned int threadsUsed = 0;
    std::future<ZigZagMaximize*> results[NUMBER_OF_THREADS];
    ZigZagMaximize maximizers[NUMBER_OF_THREADS];

    uint64_t startTime = (uint64_t)time(0);
    const float percentageMax = percentageStart + percentageIncrement * percentageIterations;
    for (float percentage = percentageStart; percentage < percentageMax; percentage += percentageIncrement) {
        for (unsigned int candleOffset = offsetStart; candleOffset <= offsetStart + offsetIterations; candleOffset++) {
            maximizers[threadsUsed].candleOffset = candleOffset;
            maximizers[threadsUsed].percentage = percentage;

            results[threadsUsed] = std::async(Maximizer::runZigZag, &maximizers[threadsUsed]);
            threadsUsed++;

            if (threadsUsed >= NUMBER_OF_THREADS) {
                for (unsigned int i = 0; i < threadsUsed; i++) {
                    results[i].get(); // This might be needed to close the threads but I don't know
                }
                threadsUsed = 0;
            }
        }
    }

    if (threadsUsed >= NUMBER_OF_THREADS) {
        for (unsigned int i = 0; i < threadsUsed; i++) {
            results[i].get();
        }
        threadsUsed = 0;
    }

    uint64_t endTime = (uint64_t)time(0);
    totalTime = (endTime - startTime);

    std::cout << ":-:-:-:-:-:-:-:-:-:-:-:" << std::endl;
    std::cout << "Total Duration: " << totalTime << std::endl;
    std::cout << ":-:-:-:-:-:-:-:-:-:-:-:" << std::endl;
}

void Maximizer::maximizeOnYourMark() {
    unsigned int crossStart = 5;
    unsigned int kijunStart = 5;
    unsigned int overStart = 5;
    float readyStart = 1;

    unsigned int crossIterations = 10;
    unsigned int kijunIterations = 10;
    unsigned int overIterations = 10;
    unsigned int readyIterations = 10;

    Dates dates;
    uint64_t totalTime = 0;

    for (unsigned int crossOffset = crossStart; crossOffset < crossStart + crossIterations; crossOffset++) {
        for (unsigned int kijunOffset = kijunStart; kijunOffset < kijunStart + kijunIterations; kijunOffset++) {
            for (unsigned int overOffset = overStart; overOffset < overStart + overIterations; overOffset++) {
                for (float readyOffset = readyStart; readyOffset < readyStart + readyIterations; readyOffset++) {
                    Stock stock(this->stockSymbol, 0.1, ZigZag(72, 34), OnYourMark(crossOffset, kijunOffset, overOffset, readyOffset));

                    std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
                    std::cout << "Cross: " << crossOffset << std::endl;
                    std::cout << "Kijun: " << kijunOffset << std::endl;
                    std::cout << "Over : " << overOffset << std::endl;
                    std::cout << "Ready: " << readyOffset << std::endl;

                    uint64_t startTime = (uint64_t)time(0);
                    for (unsigned int date = 0; date < dates.dates.size(); date++) {
                        stock.clearData();

                        FileManager::readTicks(stock, dates.dates[date]);

                        for (unsigned int j = 0; j < stock.getTotalTicks(); j++) {
                            stock.mockRestCall(j);
                        }

                        stock.resetDailyProfits();
                    }

                    uint64_t endTime = (uint64_t)time(0);
                    totalTime += (endTime - startTime);
                    std::cout << "Profit: " << stock.getTotalProfits() << std::endl;
                    std::cout << "Duration: " << endTime - startTime << std::endl;
                    std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
                }
            }
        }
    }

    std::cout << ":-:-:-:-:-:-:-:-:-:-:-:" << std::endl;
    std::cout << "Total Duration: " << totalTime << std::endl;
    std::cout << ":-:-:-:-:-:-:-:-:-:-:-:" << std::endl;
}
