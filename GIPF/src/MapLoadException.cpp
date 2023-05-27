#include "MapLoadException.h"

MapLoadException::MapLoadException(std::string errorMessage) : errorMessage(errorMessage)
{
}

MapLoadException::~MapLoadException()
{
}

const char* MapLoadException::what() const noexcept
{
    return errorMessage.c_str();
}
