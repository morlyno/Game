#pragma once

#define THROW_MSG(msg) throw MorException( __LINE__,__FILE__,(msg) )
#define THROW() throw MorException( __LINE__,__FILE__ )

#define EXCEPTION_MSG(msg) MorException( __LINE__,__FILE__,(msg) )
#define EXCEPTION() MorException( __LINE__,__FILE__ )