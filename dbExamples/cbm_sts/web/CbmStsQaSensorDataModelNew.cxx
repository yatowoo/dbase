#include "CbmStsQaSensorDataModelNew.h"
#include "CbmStsDbQaNewPar.h"

#include <iostream>
using namespace std;


WString CbmStsQaSensorDataModelNew::dateDisplayFormat(WString::fromUTF8("MMM dd, yyyy"));
WString CbmStsQaSensorDataModelNew::dateEditFormat(WString::fromUTF8("dd-MM-yyyy"));


CbmStsQaSensorDataModelNew::CbmStsQaSensorDataModelNew(int rows, int columns, WObject *parent)
    : Wt::WAbstractTableModel(parent),
      rows_(rows),
      columns_(columns)
{
  fConnections = FairDbTableInterfaceStore::Instance().fConnectionPool;  
  fillheader();
}


int CbmStsQaSensorDataModelNew::rowCount(const Wt::WModelIndex& parent) const
{
    if (!parent.isValid())
      return rows_;
    else
      return 0;
}

int CbmStsQaSensorDataModelNew::columnCount(const Wt::WModelIndex& parent ) const
{
    if (!parent.isValid())
      return columns_;
    else
      return 0;
}

boost::any CbmStsQaSensorDataModelNew::data(const Wt::WModelIndex& index, int role) const
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

boost::any CbmStsQaSensorDataModelNew::headerData(int section, Wt::Orientation orientation, int role) const
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


void CbmStsQaSensorDataModelNew::fillheader()
{
 vector<string> header_input {  "S_id"
                              , "S_type"
                              , "Batch_id"
                              , "Wafer_id"
                              , "Reticle"
                              , "Vendor"
                              , "Processing"
                              , "Height"
                              , "Width"
                              , "Strips/Side"
                              , "Year"
                              , "Owner"
                              , "Location"
                              , "V_fd"
                              , "I_150V_20C"
                              , "I_250V_20C"
                              , "Defect_p"
                              , "Defect_n"
                              , "Qa_grade"
                              , "Problem"
                              , "Passed"
                              , "OpticalPassed"
                              , "Comments"};

 //cout << "-I header input: " << header_input[0] << endl;
 fHContents.push_back(header_input);
 //cout << "-I header input: " << fContents[0][0] << endl;

}

void CbmStsQaSensorDataModelNew::fillrows(int rid)
{
 //fill the rows to the model
auto pQaSensorPar =
      FairDbWtParFactory::Instance().InitContainer<CbmStsDbQaSensorNewPar>();
// if rid = 0 take the last version
 if (rid == 0) {
  FairRunIdGenerator runID;
  UInt_t runId =  runID.generateId();
    pQaSensorPar->fill(runId);
 }else{
    pQaSensorPar->fill(rid);
 }

 // Now get all rows from parameter
  FairDbReader<CbmStsDbQaSensorNewPar>* pReader = pQaSensorPar->GetParamReader();
  int numRows = pReader->GetNumRows();
  // Update the n_rows in model
  rows_ = numRows;
  for (int i = 0; i < numRows; ++i)
    {
      CbmStsDbQaSensorNewPar* cgd = (CbmStsDbQaSensorNewPar*) pReader->GetRow(i);
      if (!cgd) { continue; }
      int compId = cgd->GetCompId();
      int uID = cgd->GetUID();
      string sensor_type = cgd->GetSensorType();
      string batch_id = cgd->GetBatchId();
      int wafer_id = cgd->GetWaferId();
      string reticle_name = cgd->GetReticleName();
      string vendor_name = cgd->GetVendorName();
      string processing = cgd->GetProcessing();
      double height = cgd->GetHeight();
      double width = cgd->GetWidth();
      int stripsPerSide = cgd->GetStripsPerSide();
      int year = cgd->GetYear();
      string current_owner = cgd->GetOwner();
      string current_location = cgd->GetLocation();
      double v_fd =  cgd->GetV_fd();
      double i_150V_20C = cgd->GetI_150V_20C();
      double i_250V_20C = cgd->GetI_250V_20C();
      int sdefect_pside = cgd->GetSdefect_pside();
      int sdefect_nside = cgd->GetSdefect_nside();
      int quality_grade = cgd->GetQuality_grade();
      int problem = cgd->GetProblem();
      int passed = cgd->GetPassed();
      int optical_check = cgd->GetOpticalCheck();
      string comments =  cgd->GetComment();

       vector< boost::any >  v_row {
                                   boost::any(uID)
                                  ,boost::any(sensor_type)
                                  ,boost::any(batch_id)
                                  ,boost::any(wafer_id)
                                  ,boost::any(reticle_name)
                                  ,boost::any(vendor_name)
                                  ,boost::any(processing)
                                  ,boost::any(height)
                                  ,boost::any(width)
                                  ,boost::any(stripsPerSide)
                                  ,boost::any(year)
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
                                  ,boost::any(optical_check)
                                  ,boost::any(comments)
                                 };
       fDContents.push_back(v_row);
    }//! for
}
