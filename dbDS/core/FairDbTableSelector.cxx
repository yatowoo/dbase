#include "FairDbMemIoStatusId.h"

namespace db_mio {

std::string statusId::ToString() const {
  if (fMessage == "") {
    return "SUCCESS";
  } else {
    char tmp[30];
    const char* type;
    switch (GetCode()) {
      case kSUCCESS:
        type = "SUCCESS: ";
        break;
      case kNOTFOUND:
        type = "NOT FOUND: ";
        break;
      case kINVALID:
        type = "INVALID ARG: ";
        break;
      case kIOERROR:
        type = "IO ERROR: ";
        break;
      default:
        snprintf(tmp, sizeof(tmp), "UNKNOWN CODE(%d): ",
                 static_cast<int>(GetCode()));
        type = tmp;
        break;
    }
    std::string result(type);
    result.append(fMessage);
    return result;
  }
}

};
