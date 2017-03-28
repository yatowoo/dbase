#ifndef CBMSTSWTQALOGGER_H
#define CBMSTSWTQALOGGER_H

#include <WServer>
#include <WApplication>
#include <WLogger>

#include <string>


enum class Severity
  {
    FATAL,
    ERROR,
    WARNING,
    INFO,
    DEBUG,
   };

enum class Module
  {
    MAIN,
    MODELS,
    UTILS,
    WCHARTS,
    WDIALOGS,
    WLAYOUTS,
    WVIEWS,
    DB,
  };

std::string GetModule(Module mod);
std::string GetSeverity(Severity sev);

#define QA_LOG(module, level)	Wt::log(GetSeverity(Severity::level))   \
  << Wt::WLogger::sep << "[" << GetModule(Module::module) << "]" <<  Wt::WLogger::sep

#endif
