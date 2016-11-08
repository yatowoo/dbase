#ifndef FAIRDBLOGUTIL_H
#define FAIRDBLOGUTIL_H

// <DB>
// Declare some global getter and setter for streams

namespace FairDbLogUtil {

bool LogLevel(const char* stream, const char* level);

const char* GetLogLevel(const char* stream);

}
#endif  // FairDbLogUtil
