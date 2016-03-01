#ifndef CBMSTSQASENSOROWNERDATAMODEL_H
#define CBMSTSQASENSOROWNERDATAMODEL_H

#include <WAbstractTableModel>
#include <WStandardItem>
#include <WString>


#include <Wt/WComboBox>
#include <Wt/WContainerWidget>
#include <Wt/WDatePicker>
#include <Wt/WDateValidator>
#include <Wt/WDialog>
#include <Wt/WEnvironment>
#include <Wt/WIntValidator>
#include <Wt/WItemDelegate>
#include <Wt/WLabel>
#include <Wt/WLineEdit>
#include <Wt/WMessageBox>
#include <Wt/WPushButton>
#include <Wt/WRegExpValidator>
#include <Wt/WGridLayout>
#include <Wt/WPopupMenu>
#include <Wt/WSortFilterProxyModel>
#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <Wt/WTableView>
#include <Wt/WTreeView>
#include <Wt/WText>
#include <Wt/WVBoxLayout>

//FairDB headers
#include "FairDbConnectionPool.h"
#include "FairDbConnection.h"
#include "FairDbWtParFactory.h"
#include "FairDbParFactory.h"
#include "FairDbStatement.h"
#include "FairRunIdGenerator.h"
#include "FairDbValRecord.h"
#include "TSQLServer.h"
#include "TSQLStatement.h"
#include "ValTimeStamp.h"
#include "ValInterval.h"


#include <vector>
#include <string>

using namespace std;
using namespace Wt;



class CbmStsQaSensorOwnerDataModel : public Wt::WAbstractTableModel
{
public:
  CbmStsQaSensorOwnerDataModel(int rows, int columns=12, Wt::WObject *parent = 0);

  virtual int rowCount(const Wt::WModelIndex& parent = Wt::WModelIndex()) const;
  virtual int columnCount(const Wt::WModelIndex& parent = Wt::WModelIndex()) const;
  virtual boost::any data(const Wt::WModelIndex& index,
                          int role = Wt::DisplayRole) const;

  virtual boost::any headerData(int section,
        Wt::Orientation orientation = Wt::Horizontal,
        int role = Wt::DisplayRole) const;

  // Define a  Mime type
  virtual std::string mimeType() const {
     const char* amim = "application/x-computers-selection";
     return amim;
   }


  void fillheader();
  void fillrows(int rid=0);

  void setSids(vector<int>& sid){fSids=sid;}
  bool is_contained(int id){
    if (std::find(fSids.begin(), fSids.end(), id) != fSids.end()){
      return true;
    }else{
      return false;
    }
  }
  // Date display format.
  static WString dateDisplayFormat;
  // Date edit format.
  static WString dateEditFormat;


private:
  int rows_, columns_;

  // Internal Data Structure
  typedef vector < string > c_row;
  typedef vector < c_row > str_matrix;

  typedef vector < boost::any > b_row;
  typedef vector < b_row > any_matrix;

  str_matrix fHContents;
  any_matrix fDContents;

  //Sids
  vector<int> fSids;


  // FairDb Connector
  FairDbConnectionPool *fConnections;
};



#endif
