/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef FAIRDBGENERICPARSET_H
#define FAIRDBGENERICPARSET_H

#include "FairDbLogService.h"
#include "FairDbExceptionLog.h"       
#include "FairDbConnectionPool.h"
#include "FairDbStatement.h"
#include "FairDbParSet.h"
#include "FairDbReader.h"
#include "FairDbWriter.h"

#include "FairDbUtilType.h"



template <typename T>
class FairDbGenericParSet : public FairDbParSet
{
  public:
  
    FairDbGenericParSet();
    FairDbGenericParSet(FairDbDetector::Detector_t detid, DataType::DataType_t dataid, 
                        const char* name, const char* title, const char* context, Bool_t ownership=kFALSE);
    FairDbGenericParSet(const FairDbGenericParSet& from);
    FairDbGenericParSet& operator=(const FairDbGenericParSet&);
    virtual ~FairDbGenericParSet();

    // Data Composition 
    void SetCompId(Int_t x)  {SetComboNo(x); fCompId=x;}
    Int_t GetCompId() const {return fCompId;}
    
    virtual FairDbObjTableMap* CreateObjTableMap() const {
      return new T();
    }
    
    // Create the FairDb tables (data + metatdata) in DB
    void CreateDbTable(Int_t db_entry);
     
    // RuntimeDb IO 
    void clear();
    void fill(UInt_t rid=0);
    void store(UInt_t rid=0);

    // Validity frame definition
    virtual ValCondition GetContext(UInt_t rid) {
      return ValCondition(fDet_id,
                          fData_id,
                          ValTimeStamp(rid));
    }
    
    void SetTableName(string name ) {
      fTableName = name ;
    }
    
    // Generate a table name 
    string GetTableName() {
      if (fTableName.empty()){
        T a;
        string table_name = FairDbUtilType::GetTypeAsString(a) ;
        if (table_name.empty()) {
          DBLOG("FairDbLog",FairDbLog::kFatal)
            << "Cannot create Table Name type_id (class) ="
            << typeid(T).name() << endl;
          exit(1);
        }
        return table_name;
      } else {    
        return fTableName;
      }
    }
    
    // SQL-IO Meta-Class Getters
    FairDbReader<T>* GetParamReader();
    FairDbWriter<T>* GetParamWriter();
    FairDbWriter<T>* ActivateWriter(Int_t rid);
    
  protected:
    // Meta Data added in ctor
    FairDbDetector::Detector_t fDet_id; //!
    DataType::DataType_t fData_id; //!
    //
    string fTableName;
    // Data aggregation id
    Int_t fCompId;
    // Parameter Container SQL Writer Meta-Class
    FairDbWriter<T>* fParam_Writer; //!
    // Parameter Container SQL Writer Meta-Class
    FairDbReader<T>* fParam_Reader; //!
    // Connection Pool
    FairDbConnectionPool* fMultConn;  //!
    
    ClassDefT(FairDbGenericParSet<T>,0)   // FairDbGenericParSet.
};
ClassDefT2(FairDbGenericParSet,T)

template<class T>
inline  FairDbReader<T>* FairDbGenericParSet<T>::GetParamReader()
{
  if (fParam_Reader) { return fParam_Reader; }
  else {
    fParam_Reader = new  FairDbReader<T>();
    return fParam_Reader;
  }
}
template<class T>
inline FairDbWriter<T>* FairDbGenericParSet<T>::GetParamWriter()
{
  if (fParam_Writer) { return fParam_Writer; }
  else {
    fParam_Writer = new  FairDbWriter<T>();
    return fParam_Writer;
  }
}





#endif  // FAIRDBGENERICPARSET_H
