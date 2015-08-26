#ifndef CBMSTSQAIVCHARTVIEW_H
#define CBMSTSQAIVCHARTVIEW_H

#include <Wt/WContainerWidget>

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


#include <string>
#include <iostream>

namespace Wt {
  class WAbstractItemModel;

  namespace Ext {
    class TableView;
  }
}

class CbmStsQaIvTableChartView: public Wt::WContainerWidget
{
public:
  CbmStsQaIvTableChartView(Wt::WContainerWidget *parent);
};

class CbmStsQaIvChartView: public Wt::WContainerWidget
{
public:
  CbmStsQaIvChartView(int sid, Wt::WContainerWidget *parent);
  void initIvContainer(int sid, int rid=0);

 private: 
 bool fInit;
 string fName; 
 Int_t fUID;
 Int_t fN;         
 Double_t* fI; //[fN]
 Double_t* fV; //[fN]
     
};

#endif // CBMSTSQAIVCHARTVIEW
