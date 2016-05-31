#ifndef FAIRDBMEMIOBASE_H
#define FAIRDBMEMIOBASE_H

#include "FairDbMemIoStatusCode.h"

namespace db_mio {

class FairDbMemIoBase {
 public:
  FairDbMemIoBase() {};
  virtual ~FairDbMemIoBase() {};
  virtual statusCode Get(FairDbIoHandle* key, FairDbIoHandle** value) = 0;
  virtual statusCode Put(FairDbIoHandle *key, FairDbIoHandle *data) = 0;
  virtual statusCode Remove(FairDbIoHandle *key) = 0;
  virtual statusCode PutData(FairDbIoHandle *key, FairDbIoHandle *data, 

  uint64_t offset, uint64_t size) = 0;

  virtual statusCode Open() = 0;
  virtual void Close() = 0;
};

} // namespace db_mio

#endif // !(FAIRDBMEMIOBASE)
