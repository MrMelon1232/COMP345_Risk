#ifndef LOGGING_OBSERVER_H
#define LOGGING_OBSERVER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

// Interface for loggable class
class ILoggable {
    public:
        virtual string stringToLog();
};

// Observer interface
class Observer {
    public: 
        virtual void update(ILoggable& loggable);
};

// Subject class
class Subject {
    public:
        void attach(Observer *observer);
        void detach(Observer *observer);
        void notify(ILoggable& loggable);
    
    private:
        vector<Observer*> observers;
};

//LogObserver class
class LogObserver: public Observer {
    public:
        LogObserver(const string& logFileName);
        ~LogObserver();
        virtual void update(ILoggable& loggable) override;
        
    private:
        ofstream logFile;
};

#endif