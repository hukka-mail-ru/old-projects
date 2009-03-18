#include "System.h"

using namespace std;

#ifdef LINUX_BUILD

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <fstream>


void writePID(const char* pidfile)
{
    ofstream out(pidfile);
    out << getpid();
}

pid_t readPID(const char* pidfile)
{
    ifstream in(pidfile);
    
    string str;
    getline(in, str);
    
    return atoi(str.c_str());
}

bool isRunning(pid_t pid)
{
    return (kill(pid,0)==0) ? true : false;  // return 0 if the process exists, else -1
}

#endif



#ifdef WIN_BUILD
time_t time( time_t *inTT )
{
    SYSTEMTIME sysTimeStruct;
    FILETIME fTime;
    ULARGE_INTEGER int64time;
    time_t locTT = 0;

    if ( inTT == NULL ) 
    {
        inTT = &locTT;
    }

    GetSystemTime( &sysTimeStruct );
    if ( SystemTimeToFileTime( &sysTimeStruct, &fTime ) ) 
    {
        memcpy( &int64time, &fTime, sizeof( FILETIME ) );
        /* Subtract the value for 1970-01-01 00:00 (UTC) */
        int64time.QuadPart -= 0x19db1ded53e8000;
        /* Convert to seconds. */
        int64time.QuadPart /= 10000000;
        *inTT = (time_t)int64time.QuadPart;
    }
    return *inTT;
} 
#endif

