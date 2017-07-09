//
//  main.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/2/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include "Dates.hpp"
#include "FileManager.hpp"
#include "Stock.hpp"
#include "ZigZag.hpp"

#include <iostream>
#include <future>

class ZigZagMaximize {
public:
    ZigZagMaximize() {
        this->percentage = 0;
        this->candleOffset = 0;
        this->movingAverageLength = 0;
    };

    void log() const {
        std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
        std::cout << "Percentage: " << this->percentage << std::endl;
        std::cout << "Candle Offset: " << this->candleOffset << std::endl;
        std::cout << "Moving Average Length: " << this->movingAverageLength << std::endl;
        std::cout << "Profit: " << this->profit << std::endl;
        std::cout << "Total Trades: " << this->totalTrades << std::endl;
        std::cout << "Positive Trades: " << this->positiveTrades << std::endl;
        std::cout << "Negative Trades: " << this->negativeTrades << std::endl;
        std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
    }

    float percentage;
    unsigned int candleOffset;
    unsigned int movingAverageLength;
    float profit;
    float duration;
    unsigned int totalTrades;
    unsigned int positiveTrades;
    unsigned int negativeTrades;
};

void mockRestCall(Stock& _stock, unsigned int& _marketTime) {
    if (_stock.getTotalTicks() == 0) {
        return;
    }

    const Tick& tick = _stock.getTick(_marketTime);
    _stock.addTickToCandle(tick);

    _stock.indicators();
    _stock.order();
}

void runProfits(Stock& _stock) {
    Dates dates;

    for (unsigned int i = 0; i < dates.dates.size(); i++) {
        _stock.clearData();

        std::cout << dates.dates[i] << std::endl;
        std::cout << "------------" << std::endl;
        FileManager::readTicks(_stock, dates.dates[i]);

        for (unsigned int j = 0; j < _stock.getTotalTicks(); j++) {
            mockRestCall(_stock, j);
        }

        _stock.logProfits();
        _stock.resetDailyProfits();
    }

    std::cout << std::endl;
    std::cout << "-+-+-+-+-+-+-+-+-+" << std::endl;
    std::cout << "Days: " << dates.dates.size() << std::endl;
    std::cout << "Profits: " << _stock.getTotalProfits() << std::endl;
    std::cout << "Average Price: " << _stock.getAveragePrice() << std::endl;
    std::cout << "Total Trades: " << _stock.getTotalTrades() << std::endl;
    std::cout << "Positive Trades: " << _stock.getPositiveTrades() << std::endl;
    std::cout << "Negative Trades: " << _stock.getNegativeTrades() << std::endl;
    std::cout << "Trades / Day: " << _stock.getTotalTrades() / dates.dates.size() << std::endl;
    std::cout << "-+-+-+-+-+-+-+-+-+" << std::endl;
    std::cout << std::endl;
}

ZigZagMaximize* runZigZag(ZigZagMaximize* _zigZagMaximize) {
    Dates dates;
    Stock stock("JNUG", _zigZagMaximize->percentage, ZigZag(_zigZagMaximize->candleOffset, _zigZagMaximize->movingAverageLength), OnYourMark(0, 0, 0, 0));

    for (unsigned int date = 0; date < dates.dates.size(); date++) {
        stock.clearData();

        FileManager::readTicks(stock, dates.dates[date]);

        for (unsigned int j = 0; j < stock.getTotalTicks(); j++) {
            mockRestCall(stock, j);
        }

        stock.resetDailyProfits();
    }

    _zigZagMaximize->profit = stock.getTotalProfits();
    _zigZagMaximize->totalTrades = stock.getTotalTrades();
    _zigZagMaximize->positiveTrades = stock.getPositiveTrades();
    _zigZagMaximize->negativeTrades = stock.getNegativeTrades();

    return _zigZagMaximize;

}

void maximizeZigZag() {
    float percentageIncrement = 0.005;

    unsigned int offsetStart = 1;
    unsigned int lengthStart = 1;
    float percentageStart = 0.005;
    unsigned int offsetIterations = 120;
    unsigned int lengthIterations = 60;
    unsigned int percentageIterations = 4;

    uint64_t totalTime = 0;

    const unsigned int NUMBER_OF_THREADS = 4;
    unsigned int threadsUsed = 0;
    std::future<ZigZagMaximize*> results[NUMBER_OF_THREADS];
    ZigZagMaximize maximizers[NUMBER_OF_THREADS];

    uint64_t startTime = (uint64_t)time(0);
    const float percentageMax = percentageStart + percentageIncrement * percentageIterations;
    for (float percentage = percentageStart; percentage < percentageMax; percentage += percentageIncrement) {
        for (unsigned int candleOffset = offsetStart; candleOffset <= offsetStart + offsetIterations; candleOffset++) {
            for (unsigned int movingAverageLength = lengthStart; movingAverageLength <= lengthStart + lengthIterations; movingAverageLength++) {
                maximizers[threadsUsed].candleOffset = candleOffset;
                maximizers[threadsUsed].movingAverageLength = movingAverageLength;
                maximizers[threadsUsed].percentage = percentage;

                results[threadsUsed] = std::async(runZigZag, &maximizers[threadsUsed]);
                threadsUsed++;

                if (threadsUsed >= NUMBER_OF_THREADS) {
                    for (unsigned int i = 0; i < threadsUsed; i++) {
                        ZigZagMaximize* maximizer = results[i].get();
                        maximizer->log();
                    }
                    threadsUsed = 0;
                }
            }
        }
    }

    uint64_t endTime = (uint64_t)time(0);
    totalTime = (endTime - startTime);

    std::cout << ":-:-:-:-:-:-:-:-:-:-:-:" << std::endl;
    std::cout << "Total Duration: " << totalTime << std::endl;
    std::cout << ":-:-:-:-:-:-:-:-:-:-:-:" << std::endl;
}

void maximizeOnYourMark() {
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
                    Stock stock("JNUG", 0.1, ZigZag(28, 52), OnYourMark(crossOffset, kijunOffset, overOffset, readyOffset));

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
                            mockRestCall(stock, j);
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

int main(int argc, const char * argv[]) {
    Stock stock("JNUG", 0.01, ZigZag(75, 29), OnYourMark(0, 0, 0, 0));

//    maximizeOnYourMark();
    maximizeZigZag();
//    runProfits(stock);

    return 0;
}
