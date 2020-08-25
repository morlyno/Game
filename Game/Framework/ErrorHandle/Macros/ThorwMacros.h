#pragma once

#define THROW_MSG(msg) throw Exception( __LINE__,__FILE__,(msg) )
#define THROW() throw Exception( __LINE__,__FILE__ )

#define EXCEPTION_MSG(msg) Exception( __LINE__,__FILE__,(msg) )
#define EXCEPTION() Exception( __LINE__,__FILE__ )