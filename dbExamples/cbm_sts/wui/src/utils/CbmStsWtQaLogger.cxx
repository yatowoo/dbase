
#include "CbmStsWtQaLogger.h"

std::string GetModule(Module mod)
{
  switch (mod)
    {
    case Module::MAIN:	        return "MAIN";      
    case Module::MODELS:	return "MODELS";
    case Module::UTILS:		return "UTILS";
    case Module::WCHARTS:	return "WCHARTS";
    case Module::WDIALOGS:	return "WDIALOGS";
    case Module::WLAYOUTS:	return "WLAYOUTS";
    case Module::WVIEWS:	return "WVIEWS";
    case Module::DB:	        return "DB";            
    }
  return "";
}

std::string GetSeverity(Severity sev)
{
  switch (sev)
    {
    case Severity::FATAL:		return "fatal";
    case Severity::ERROR:		return "error";
    case Severity::WARNING:		return "warning";
    case Severity::INFO:		return "info";
    case Severity::DEBUG:		return "debug";
    }
  return "";
}

