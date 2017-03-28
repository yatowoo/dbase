#ifndef CBMSTSWTQAIVCHARTVIEW_H
#define CBMSTSWTQAIVCHARTVIEW_H

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

class CbmStsWtQaIvTableChartView: public Wt::WContainerWidget
{
public:
  CbmStsWtQaIvTableChartView(Wt::WContainerWidget *parent);
};

class CbmStsWtQaIvChartView: public Wt::WContainerWidget
{
public:
  CbmStsWtQaIvChartView(int sid, Wt::WContainerWidget *parent);
  void initIvContainer(int sid, int rid=0);

 private: 
 bool fInit;
 string fName; 
 Int_t fUID;
 Int_t fN;         
 Double_t* fI; //[fN]
 Double_t* fV; //[fN]
     
};

#endif // CBMSTSWTQAIVCHARTVIEW
