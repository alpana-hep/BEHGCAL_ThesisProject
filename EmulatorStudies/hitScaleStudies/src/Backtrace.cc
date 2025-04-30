#ifndef Backtrace_HH
#define Backtrace_HH

#include <execinfo.h>
#include <cxxabi.h>
#include <dlfcn.h>

#include <iostream>
#include <sstream>

#include "Backtrace.h"

void Backtrace()
{
	void *callstack[128];
	const int nMaxFrames = sizeof(callstack) / sizeof(callstack[0]);
	char buf[1024];
	int nFrames = backtrace(callstack, nMaxFrames);
	char **symbols = backtrace_symbols(callstack, nFrames);

	std::ostringstream trace_buf;
	for (int i = 1; i < nFrames; i++) {
		Dl_info info;
		if (dladdr(callstack[i], &info)) {
			char *demangled = NULL;
			int status;
			demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
			snprintf(buf, sizeof(buf), "%-3d %*p %s + %zd \t",
				 i, int(2 + sizeof(void*) * 2), callstack[i],
					 status == 0 ? demangled : info.dli_sname,
					 (char *)callstack[i] - (char *)info.dli_saddr);
			free(demangled);
		} else {
			snprintf(buf, sizeof(buf), "%-3d %*p \t",
				 i, int(2 + sizeof(void*) * 2), callstack[i]);
		}
		trace_buf << buf;

		snprintf(buf, sizeof(buf), "%s\n", symbols[i]);
		trace_buf << buf;
	}
	free(symbols);
	if (nFrames == nMaxFrames)
		trace_buf << "[truncated]\n";
	std::cout << std::endl << std::endl << trace_buf.str() << std::endl;
}

// Originally from http://charette.no-ip.com:81/programming/2010-01-25_Backtrace/2010-01-25_Backtrace.c

int displayBacktrace()
{
	const int size = 20;
	void *buffer[size];
	int ret;
	int idx;
	char **ptr;

	// get the void* pointers to functions; "ret" is the number of items returned
	ret = backtrace( buffer, size );

	// now we want to convert those pointers to text
	ptr = backtrace_symbols( buffer, ret );

	for ( idx = 0; idx < ret; idx ++ )
	{
		// if using C++, look up abi::__cxa_demangle( ... ) in #include <cxxabi.h>
		printf( "%d: %s\n", idx, ptr[ idx ] );
	}

	free( ptr );

	//#ifdef __cplusplus
	// here is an example of C++ demangling
	int status = -1;
	char *demangledName = abi::__cxa_demangle( "_Z16displayBacktracev", NULL, NULL, &status );
	if ( status == 0 )
	{
		printf( "_Z16displayBacktracev demangles to:  %s\n", demangledName );
	}
	free( demangledName ); 
	//#endif

	return ret;
}

#endif
