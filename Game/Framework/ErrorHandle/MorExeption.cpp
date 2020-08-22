#include "MorExeption.h"
#include <sstream>

MorException::MorException( int line,const char* file ) noexcept
    :
    line( line ),
    file( file )
{
}

const char* MorException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* MorException::GetType() const noexcept
{
    return "Mor Exception";
}

int MorException::GetLine() const noexcept
{
    return line;
}

const std::string& MorException::GetFile() const noexcept
{
    return file;
}

std::string MorException::GetOriginString() const noexcept
{
    std::ostringstream oss;
    oss << "\n[File]\n" << file << std::endl
        << "\n[Line]\n" << line;
    return oss.str();
}
