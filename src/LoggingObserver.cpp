#include "LoggingObserver.h"

void Subject::attach(Observer *observer) {
    observers.push_back(observer);
}

void Subject::detach(Observer* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void LogObserver::update(const ILoggable& loggable) {
    logFile << loggable.stringToLog() << std::endl;
} 

void Subject::notify(const ILoggable& loggable) {
    for (Observer* observer : observers) {
        observer->update(loggable);
    }
}

LogObserver::LogObserver(const string& logFileName) {
    logFile.open(logFileName, ios::app);
}

LogObserver::~LogObserver() {
    logFile.close();
}