#include "MorExeption.h"
#include <sstream>

MorException::MorException( int line,const char* file ) noexcept
    :
    line( line ),
    file( file ),
    msg( std::string() )
{
}

MorException::MorException( int line,const char* file,const char* msg ) noexcept
    :
    line( line ),
    file( file ),
    msg( msg )
{
}

const char* MorException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << GetOriginString();
    if ( !msg.empty() )
    {
        oss << "\n[Message]\n"
            << msg;
    }
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

const std::string& MorException::GetMsg() const noexcept
{
    return msg;
}

std::string MorException::GetOriginString() const noexcept
{
    std::ostringstream oss;
    oss << "\n[File]\n" << file << std::endl
        << "\n[Line]\n" << line;
    return oss.str();
}
