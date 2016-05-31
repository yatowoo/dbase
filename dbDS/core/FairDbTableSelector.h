#ifndef FAIRDBMEMIOSTATUSID_H
#define FAIRDBMEMIOSTATUSID_H

#include <string>

namespace db_memio {

class statusId {

 public:
  statusId() { fCode = kSUCCESS; fMessage = ""; }
  ~statusId() {}

  statusId(int code) {
    fCode = code;
  }

  statusId(int code, std::string message)
  {
    fCode_ = code;
    fMessage = message;
  }

  static statusId Success() { return statusCode(); }

  static statusId NotFound(const std::string& message) {
    return statusCode(kNOTFOUND, message);
  }

  static statusId Invalid(const std::string& message) {
    return statusCode(kINVALID, message);
  }

  static statusId IOError(const std::string& message) {
    return statusCode(kIOERROR, message);
  }

  bool IsSuccess()  const  { return  (GetCode() == kSUCCESS ); }
  bool IsNotFound() const  { return  (GetCode() == kNOTFOUND); }
  bool IsInvalid()  const  { return  (GetCode() == kINVALID ); }
  bool IsIOError()  const  { return  (GetCode() == kIOERROR ); }
  
  std::string ToString() const;

 private:
  int fCode;
  std::string fMessage;
 
  int code() const { return fCode; };

  enum errCode {
     kSUCCESS  = 0, 
     kNOTFOUND = 1,
     kINVALID  = 2,
     kIOERROR  = 3,
  };
};

}; // end namespace db_memio

#endif // !(FAIRDBMEMIOSTATUSID)