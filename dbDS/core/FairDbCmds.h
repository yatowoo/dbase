#ifndef FAIRDBLOGFILE_H
#define FAIRDBLOGFILE_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

#include <memory>
#include <string>
#include <string.h>

#include <TObject.h>

namespace db_admin {
  
  class FairDbLogFile : public TObject {
  public:
    FairDbLogFile() {} 
    virtual ~FairDbLogFile() {}
  protected:
 
  };
  
}

#endif // FAIRDBLOGFILE
