#ifndef MOVE_EXCEPTION_H
#define MOVE_EXCEPTION_H

#include <exception>
#include <string>

class MoveException : public std::exception
{
private:
	std::string errorMessage;

public:
	MoveException(std::string errorMessage);
	~MoveException();

	virtual const char* what() const noexcept;
};

#endif