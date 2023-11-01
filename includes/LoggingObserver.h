#ifndef LOGGING_OBSERVER_H
#define LOGGING_OBSERVER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

// Interface for loggable class
class ILoggable {
    public:
        virtual string stringtoLog() const = 0;
};

// Observer interface
class Observer {
    public: 
        virtual void update(const ILoggable loggable) = 0;
};

// Subject class
class Subject {
    public:
        void attach(Observer * observer);
        void detach(Observer * observer)
        void notify(const ILoggable& loggable);
    
    private:
        vector<Observer*> observers;
}

//LogObserver class
class LogObserver: public Observer {
    public:
        LogObserver(const string& logFileName);
        ~LogObserver();
        void update(const ILoggable& loggable) override;
        void logtoFileName(const string& log);

    private:
        ofstream logFile;
}
