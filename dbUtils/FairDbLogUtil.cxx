#include <FairDbLogUtil.h>
#include <FairDbLog.h>
#include <FairDbLogService.h>

bool FairDbLogUtil::LogLevel(const char* stream, const char* level)
{
    FairDbLogStream *s = FairDbLogService::Instance()->GetStream(stream);
    if (!s) return false;
    FairDbLog::LogLevel_t lvl = FairDbLog::GetLevelCode(level);
    if ( lvl < 0 ) return false;
    s->SetLogLevel(lvl);
    return true;
}

const char* FairDbLogUtil::GetLogLevel(const char* stream)
{
    FairDbLogStream *s = FairDbLogService::Instance()->GetStream(stream);
    if (!s) return FairDbLog::LevelAsString(FairDbLog::kInfo); 
    FairDbLog::LogLevel_t lvl = s->GetLogLevel();
    if ( lvl < 0 ) return FairDbLog::LevelAsString(FairDbLog::kInfo); 
    return FairDbLog::LevelAsString(lvl);
}
