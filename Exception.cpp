#ifndef UNTITLED4_EXCEPTIONS_H
#define UNTITLED4_EXCEPTIONS_H

#include <exception>
#include <string>

class TreeException : public std::exception {
private:
    std::string message;
public:
    explicit TreeException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class NotFoundException : public TreeException {
public:
    explicit NotFoundException(const std::string& msg) : TreeException(msg) {}
};

class InvalidInputException : public TreeException {
public:
    explicit InvalidInputException(const std::string& msg) : TreeException(msg) {}
};

#endif //UNTITLED4_EXCEPTIONS_H
