#include "LoggingObserver.h"

void Subject::attach(Observer * observer) {
    observers.push_back(observer);
}

void Subject::detach(Observer * observer) {
    auto it = find(observers.begin(), observer.end(), observerda)

}

void Subject::notify(const ILoggable& loggable) {
    for (Observer * observer : observers) {
        observer.update(loggable);
    }
}

LogObserver::LogObserver(const string& logFileName) {
    logFile.open(logFileName, ios::app);
}

LogObsever::~LogObserver() {
    logFile.close();
}

void LogObserver::update(ILoggable& loggable) override {
    logFile << loggable->stringToLog() << std::endl;
} 