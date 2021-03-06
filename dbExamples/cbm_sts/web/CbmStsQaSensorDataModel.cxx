#include "CbmStsQaSensorDataModel.h"
#include "CbmStsDbQaPar.h"

#include <iostream>
using namespace std;


WString CbmStsQaSensorDataModel::dateDisplayFormat(WString::fromUTF8("MMM dd, yyyy"));
WString CbmStsQaSensorDataModel::dateEditFormat(WString::fromUTF8("dd-MM-yyyy"));


CbmStsQaSensorDataModel::CbmStsQaSensorDataModel(int rows, int columns, WObject *parent)
    : Wt::WAbstractTableModel(parent),
      rows_(rows),
      columns_(columns)
{ 
  fConnections = FairDbTableInterfaceStore::Instance().fConnectionPool;  
  fillheader();
}



int CbmStsQaSensorDataModel::rowCount(const Wt::WModelIndex& parent) const
{
    if (!parent.isValid())
      return rows_;
    else
      return 0;
}

int CbmStsQaSensorDataModel::columnCount(const Wt::WModelIndex& parent ) const
{
    if (!parent.isValid())
      return columns_;
    else
      return 0;
}

boost::any CbmStsQaSensorDataModel::data(const Wt::WModelIndex& index, int role) const
{

 // Here i had a problem with Wt::asString() that
 // i solved by using ths std streams to convert.
 // the point is that Wt::asString uses internally snprintf?
 // which gives a crazy  rounding ...

    std::ostringstream s;
    boost::any aV = fDContents[index.row()][index.column()];

    switch (role) {
    case Wt::DisplayRole:
      {       
        if (aV.type() == typeid(int)){
         //cout << "-I- NUMBER INT: " << boost::any_cast<int> (fDContents[index.row()][index.column()]) << endl;
         s.str("");
         s << boost::any_cast<int> (fDContents[index.row()][index.column()]);
         return Wt::WString(s.str());
        }
        else if (aV.type() == typeid(float)){
         //cout << "-I- NUMBER FLOAT: " << boost::any_cast<float> (fDContents[index.row()][index.column()]) << endl;
         s.str("");
         s << boost::any_cast<float> (fDContents[index.row()][index.column()]);
         return Wt::WString(s.str());
        }
        else if (aV.type() == typeid(string)){
         //cout << "-I- NUMBER STRING: " << boost::any_cast<string> (fDContents[index.row()][index.column()]) << endl;
         return Wt::WString(boost::any_cast<string>(fDContents[index.row()][index.column()]));
        }
        else if (aV.type() == typeid(double)){
         //cout << "-I- NUMBER DOUBLE: " << boost::any_cast<double> (fDContents[index.row()][index.column()]) << endl;
         s.str("");
         s << boost::any_cast<double> (fDContents[index.row()][index.column()]);
         return Wt::WString(s.str());
        }
       break; 
      }
     case Wt::EditRole:
      {
        return aV;
        break;
      }
    default:
      return boost::any();
      break;
   }//!role
 return boost::any(); 
}

boost::any CbmStsQaSensorDataModel::headerData(int section, Wt::Orientation orientation, int role) const
{
    if (orientation == Wt::Horizontal) {
      switch (role) {
      case Wt::DisplayRole:
      return Wt::WString(fHContents[0][section]);
      default:
      return boost::any();
      }
    } else
      return boost::any();
}


void CbmStsQaSensorDataModel::fillheader()
{
 vector<string> header_input {  "Unique_id"
                              , "Sensor_type"
                              , "Batch_id" 
                              , "Wafer_id" 
                              , "Sensor_name"
                              , "Manufacturer"              
                              , "Production_date"
                              , "Current_owner" 
                              , "Current_location"  
                              , "V_fd" 
                              , "I_150V_20C"
                              , "I_250V_20C"
                              , "Defect_pside"
                              , "Defect_nside"
                              , "Quality_grade"
                              , "Problem"
                              , "Passed"
                              , "Comments"};   

 //cout << "-I header input: " << header_input[0] << endl;  
 fHContents.push_back(header_input);
 //cout << "-I header input: " << fContents[0][0] << endl;  

}

void CbmStsQaSensorDataModel::fillrows(int rid)
{
 //fill the rows to the model 
auto pQaSensorPar = 
      FairDbWtParFactory::Instance().InitContainer<CbmStsDbQaSensorPar>();  
// if rid = 0 take the last version
 if (rid == 0) { 
  FairRunIdGenerator runID;
  UInt_t runId =  runID.generateId();
    pQaSensorPar->fill(runId); 
 }else{
    pQaSensorPar->fill(rid); 
 } 

 // Now get all rows from parameter 
  FairDbReader<CbmStsDbQaSensorPar>* pReader = pQaSensorPar->GetParamReader();
  int numRows = pReader->GetNumRows();
  // Update the n_rows in model
  rows_ = numRows;
  for (int i = 0; i < numRows; ++i) 
    {
      CbmStsDbQaSensorPar* cgd = (CbmStsDbQaSensorPar*) pReader->GetRow(i);
      if (!cgd) { continue; }
       int compId = cgd->GetCompId(); 
       int uID = cgd->GetUID();
       string sensor_type = cgd->GetSensorType(); 
       string batch_id = cgd->GetBatchId(); 
       int wafer_id = cgd->GetWaferId(); 
       string sensor_name = cgd->GetSensorName(); 
       string manufacturer = cgd->GetManufacturer(); 
       string production_date = cgd->GetProductionDate(); 
       string current_owner = cgd->GetOwner(); 
       string current_location = cgd->GetLocation(); 
       float v_fd =  cgd->GetV_fd(); 
       float i_150V_20C = cgd->GetI_150V_20C(); 
       float i_250V_20C = cgd->GetI_250V_20C(); 
       int sdefect_pside = cgd->GetSdefect_pside(); 
       int sdefect_nside = cgd->GetSdefect_nside(); 
       int quality_grade = cgd->GetQuality_grade(); 
       string problem = cgd->GetProblem(); 
       string passed = cgd->GetPassed(); 
       string comments =  cgd->GetComment();  
     
       vector< boost::any >  v_row {
                                   boost::any(uID)
                                  ,boost::any(sensor_type)
                                  ,boost::any(batch_id)
                                  ,boost::any(wafer_id)
                                  ,boost::any(sensor_name)
                                  ,boost::any(manufacturer)
                                  ,boost::any(production_date)
                                  ,boost::any(current_owner)
                                  ,boost::any(current_location)
                                  ,boost::any(v_fd)
                                  ,boost::any(i_150V_20C)
                                  ,boost::any(i_250V_20C)
                                  ,boost::any(sdefect_pside)
                                  ,boost::any(sdefect_nside)
                                  ,boost::any(quality_grade)
                                  ,boost::any(problem)
                                  ,boost::any(passed)
                                  ,boost::any(comments)
                                 };
       fDContents.push_back(v_row);
 
    }//! for

}

   
