#ifndef MAP_LOAD_EXCEPTION_H
#define MAP_LOAD_EXCEPTION_H

#include <exception>
#include <string>

class MapLoadException : public std::exception
{
private:
	std::string errorMessage;

public:
	MapLoadException(std::string errorMessage);
	~MapLoadException();
	
	virtual const char* what() const noexcept;
};

#endif