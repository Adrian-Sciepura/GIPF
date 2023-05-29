#include "MoveException.h"

MoveException::MoveException(std::string errorMessage) : errorMessage(errorMessage)
{
}

MoveException::~MoveException()
{
}

const char* MoveException::what() const noexcept
{
    return errorMessage.c_str();
}
