#include <iostream>
#ifndef DUMMY_CLASS_H
#define DUMMY_CLASS_H

class DefaultRuleOfFive
{
public:
    DefaultRuleOfFive() = default;                                     // Default constructor
    ~DefaultRuleOfFive() = default;                                    // Destructor
    DefaultRuleOfFive(const DefaultRuleOfFive &) = default;            // Copy constructor
    DefaultRuleOfFive(DefaultRuleOfFive &&) = default;                 // Move constructor
    DefaultRuleOfFive &operator=(const DefaultRuleOfFive &) = default; // Copy assignment operator
    DefaultRuleOfFive &operator=(DefaultRuleOfFive &&) = default;      // Move assignment operator
};

class StringRuleOfFive
{
public:
    StringRuleOfFive()
    {
        std::cout << "StringRuleOfFive constructor" << std::endl;
    }
    ~StringRuleOfFive()
    {
        std::cout << "StringRuleOfFive destructor" << std::endl;
    }
    StringRuleOfFive(const StringRuleOfFive &other)
    {
        std::cout << "StringRuleOfFive copy constructor" << std::endl;
    }
    StringRuleOfFive(StringRuleOfFive &&other)
    {
        std::cout << "StringRuleOfFive move constructor" << std::endl;
    }
    StringRuleOfFive &operator=(const StringRuleOfFive &other)
    {
        std::cout << "StringRuleOfFive copy assignment operator" << std::endl;
        return *this;
    }
    StringRuleOfFive &operator=(StringRuleOfFive &&other)
    {
        std::cout << "StringRuleOfFive move assignment operator" << std::endl;
        return *this;
    }
    void print()
    {
        std::cout << "StringRuleOfFive print" << std::endl;
    }
};

class StringDeletedCopyAndMove
{
public:
StringDeletedCopyAndMove()
    {
        std::cout << "StringDeletedCopyAndMove constructor" << std::endl;
    }
    ~StringDeletedCopyAndMove()
    {
        std::cout << "StringDeletedCopyAndMove destructor" << std::endl;
    }
    StringDeletedCopyAndMove(const StringDeletedCopyAndMove &other) = delete;            // Copy constructor
    StringDeletedCopyAndMove(StringDeletedCopyAndMove &&other) = delete;                 // Move constructor
    StringDeletedCopyAndMove &operator=(const StringDeletedCopyAndMove &other) = delete; // Copy assignment operator
    StringDeletedCopyAndMove &operator=(StringDeletedCopyAndMove &&other) = delete;      // Move assignment operator
    void print()
    {
        std::cout << "StringDeletedCopyAndMove print" << std::endl;
    }
};

#endif
