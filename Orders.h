#pragma once
#include <iostream>
#include <vector>
#include <string>


class Order
{
    public: 
        virtual ~Order() = default; 
        virtual void execute() = 0; 
        virtual bool validate() = 0; 
        virtual std::string getDescription() const = 0;
        virtual Order* copy() const = 0;
}; 

class Deploy : public Order
{
    public:
        Deploy();
        Deploy(const Deploy& d);
        ~Deploy() override; 
        Deploy(const Deploy& d);
        void execute() override;
        bool validate() const override;
        std::string getDescription() override; 
};

