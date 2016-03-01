#include "CbmStsQaSensorResultsDataModel.h"
#include "CbmStsDbQaResultsPar.h"

#include <iostream>
using namespace std;


WString CbmStsQaSensorResultsDataModel::dateDisplayFormat(WString::fromUTF8("MMM dd, yyyy"));
WString CbmStsQaSensorResultsDataModel::dateEditFormat(WString::fromUTF8("dd-MM-yyyy"));


CbmStsQaSensorResultsDataModel::CbmStsQaSensorResultsDataModel(int rows, int columns, WObject *parent)
    : Wt::WAbstractTableModel(parent),
      rows_(rows),
      columns_(columns)
{
   fConnections = FairDbParFactory::Instance().GetConnections();

   fillheader();

}



int CbmStsQaSensorResultsDataModel::rowCount(const Wt::WModelIndex& parent) const
{
    if (!parent.isValid())
      return rows_;
    else
      return 0;
}

int CbmStsQaSensorResultsDataModel::columnCount(const Wt::WModelIndex& parent ) const
{
    if (!parent.isValid())
      return columns_;
    else
      return 0;
}

boost::any CbmStsQaSensorResultsDataModel::data(const Wt::WModelIndex& index, int role) const
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

boost::any CbmStsQaSensorResultsDataModel::headerData(int section, Wt::Orientation orientation, int role) const
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


void CbmStsQaSensorResultsDataModel::fillheader()
{
 vector<string> header_input {  "S_id"
                              , "Type"
                              , "Wafer_id"
                              , "V_fd"
                              , "I_150V_20C"
                              , "I_150V_20C"
                              , "Defect_p"
                              , "Defect_n"
                              , "Qa_grade"
                              , "Problem"
                              , "Qa_passed"
                              , "Optical_passed"
                              , "Comments"
                            };

 //cout << "-I header input: " << header_input[0] << endl;
 fHContents.push_back(header_input);
 //cout << "-I header input: " << fContents[0][0] << endl;

}

void CbmStsQaSensorResultsDataModel::fillrows(int rid)
{
 //fill the rows to the model
auto pQaSensorPar =
      FairDbWtParFactory::Instance().InitContainer<CbmStsDbQaResultsPar>();
// if rid = 0 take the last version
 if (rid == 0) {
  FairRunIdGenerator runID;
  UInt_t runId =  runID.generateId();
    pQaSensorPar->fill(runId);
 }else{
    pQaSensorPar->fill(rid);
 }

 // Now get all rows from parameter
  FairDbReader<CbmStsDbQaResultsPar>* pReader = pQaSensorPar->GetParamReader();
  int numRows = pReader->GetNumRows();
  // Update the n_rows in model
  rows_ = 0;
  for (int i = 0; i < numRows; ++i)
    {
      CbmStsDbQaResultsPar* cgd = (CbmStsDbQaResultsPar*) pReader->GetRow(i);
      if (!cgd) { continue; }
       int compId = cgd->GetCompId();
       int uID = cgd->GetUID();
       string type = cgd->GetType();
       int wafer_id = cgd->GetWaferId();
       double vfd   = cgd->GetV_fd();
       double i_150 = cgd->GetI_150V_20C();
       double i_250 = cgd->GetI_250V_20C();
       int def_p    =  cgd->GetSdefect_pside();
       int def_n    =  cgd->GetSdefect_nside();
       int qa_grade =  cgd->GetQuality_grade();
       int prob     =  cgd->GetProblem();
       int qa_passed = cgd->GetQaPassed();
       int optical   = cgd->GetOpticalCheckPassed();
       string comment = cgd->GetComment();


       vector< boost::any >  v_row {
                                   boost::any(uID)
                                  ,boost::any(type)
                                  ,boost::any(wafer_id)
                                  ,boost::any(vfd)
                                  ,boost::any(i_150)
                                  ,boost::any(i_250)
                                  ,boost::any(def_p)
                                  ,boost::any(def_n)
                                  ,boost::any(qa_grade)
                                  ,boost::any(prob)
                                  ,boost::any(qa_passed)
                                  ,boost::any(optical)
                                  ,boost::any(comment)
                                 };

    if (is_contained(uID)){
       fDContents.push_back(v_row);
       rows_++;
     }
    }//! for
}
