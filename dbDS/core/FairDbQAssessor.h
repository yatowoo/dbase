#ifndef FAIRDBIOHANDLEBASE_H
#define FAIRDBIOHANDLEBASE_H

namespace db_memio {

class FairDbIoHandleBase {
 public:
  FairDbIoHandleBase() {}
  virtual ~FairDbHandleIoBase() {}
  
  virtual char* Data() = 0;
  virtual uint64_t Size() = 0;
  virtual char* Data_as_const() const = 0;
  virtual uint64_t Size_as_const() const = 0;
  
  virtual bool BeginWith(const char *substr, int n) = 0;
  virtual Status Data_chunk(char **data, uint64_t *size) = 0;
  virtual std::string AsString() = 0;
  
  // Specs for binary compression 
  virtual bool Is_squashed() = 0;

  // Operators on IoHandle
  bool operator ==( FairDbIoHandleBase &ioHandle) const {
    //fprintf(stderr, "FairIoHandleBase operator==() -- left: %p %llu [%s] right: %p %llu [%s]\n", 
    //               data_, size_, std::string(data_, size_).c_str(), 
    // right.data_const(), right.size_const(), std::string(right.data_const(), right.size_const()).c_str());
    return (   Size_as_const() == ioHandle.Size_as_const()
            && memcmp(Data_as_const(), ioHandle.Data_as_const(), Size_as_const()) == 0);
  }

};

};

#endif // KINGDB_BYTE_ARRAY_BASE_H_