#include "CbmStsQaSensorOwnerDataModel.h"
#include "CbmStsDbQaOwnershipPar.h"

#include <iostream>
using namespace std;


WString CbmStsQaSensorOwnerDataModel::dateDisplayFormat(WString::fromUTF8("MMM dd, yyyy"));
WString CbmStsQaSensorOwnerDataModel::dateEditFormat(WString::fromUTF8("dd-MM-yyyy"));


CbmStsQaSensorOwnerDataModel::CbmStsQaSensorOwnerDataModel(int rows, int columns, WObject *parent)
    : Wt::WAbstractTableModel(parent),
      rows_(rows),
      columns_(columns)
{
   fConnections = FairDbTableInterfaceStore::Instance().fConnectionPool;  
   fillheader();
}



int CbmStsQaSensorOwnerDataModel::rowCount(const Wt::WModelIndex& parent) const
{
    if (!parent.isValid())
      return rows_;
    else
      return 0;
}

int CbmStsQaSensorOwnerDataModel::columnCount(const Wt::WModelIndex& parent ) const
{
    if (!parent.isValid())
      return columns_;
    else
      return 0;
}

boost::any CbmStsQaSensorOwnerDataModel::data(const Wt::WModelIndex& index, int role) const
{

 // Here i had a problem with Wt::asString() that
 // i solved by using ths std streams to convert.
 // the point is that Wt::asString uses internally snprintf?
 // which gives a crazy  rounding ...

    std::ostringstream s;
    //cout << "-I data row Column" << index.row() << ":" << index.column() << endl;
    //cout << "-I data fDContents" <<  DContents[index.row()][index.column()] << endl;
    boost::any aV = fDContents[index.row()][index.column()];
    //cout << "-I data fetched in FDCOntents" << endl;

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

boost::any CbmStsQaSensorOwnerDataModel::headerData(int section, Wt::Orientation orientation, int role) const
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


void CbmStsQaSensorOwnerDataModel::fillheader()
{
 vector<string> header_input {  "S_id"
                              , "Vendor"
                              , "Type"
                              , "Wafer_id"
                              , "Reticle_name"
                              , "Year"
                              , "Owner"
                              , "Location"};

 //cout << "-I header input: " << header_input[0] << endl;
 fHContents.push_back(header_input);
 //cout << "-I header input: " << fContents[0][0] << endl;

}

void CbmStsQaSensorOwnerDataModel::fillrows(int rid)
{
 //fill the rows to the model
auto pQaSensorPar =
      FairDbWtParFactory::Instance().InitContainer<CbmStsDbQaOwnershipPar>();
// if rid = 0 take the last version
 if (rid == 0) {
  FairRunIdGenerator runID;
  UInt_t runId =  runID.generateId();
    pQaSensorPar->fill(runId);
 }else{
    pQaSensorPar->fill(rid);
 }
 
 // Now get all rows from parameter
  FairDbReader<CbmStsDbQaOwnershipPar>* pReader = pQaSensorPar->GetParamReader();
  int numRows = pReader->GetNumRows();
  // Update the n_rows in model
  rows_ = 0;
  for (int i = 0; i < numRows; ++i)
    {
      CbmStsDbQaOwnershipPar* cgd = (CbmStsDbQaOwnershipPar*) pReader->GetRow(i);
      if (!cgd) { continue; }
       int compId = cgd->GetCompId();
       int uID = cgd->GetUID();

       string vendor = cgd->GetVendor();
       string type   = cgd->GetType();
       int wafer_id = cgd->GetWaferId();
       string reticle_name = cgd->GetReticleName();
       int year = cgd->GetYear();
       string owner = cgd->GetOwner();
       string location = cgd->GetLocation();

       vector< boost::any >  v_row {
                                   boost::any(uID)
                                  ,boost::any(vendor)
                                  ,boost::any(type)
                                  ,boost::any(wafer_id)
                                  ,boost::any(reticle_name)
                                  ,boost::any(year)
                                  ,boost::any(owner)
                                  ,boost::any(location)
                                 };

    if (is_contained(uID)){
       fDContents.push_back(v_row);
       rows_++;
     }
    }//! for
}
