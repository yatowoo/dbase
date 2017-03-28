#ifndef CBMSTSWTQACHMAPCHARTVIEW_H
#define CBMSTSWTQACHMAPCHARTVIEW_H

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

#define NCHMAX 1024

namespace Wt {
  class WAbstractItemModel;

  namespace Ext {
    class TableView;
  }
}

class CbmStsWtQaChMapTableChartView: public Wt::WContainerWidget
{
public:
  CbmStsWtQaChMapTableChartView(Wt::WContainerWidget *parent);
};

class CbmStsWtQaChMapChartView: public Wt::WContainerWidget
{
public:
  CbmStsWtQaChMapChartView(int sid, Wt::WContainerWidget *parent);
  void initChMapContainer(int sid, int rid=0);

private:
 bool fInit;
 string fName;
 Int_t fUID;
 Int_t  fAC_Cap_Ok_pside[NCHMAX];
 Int_t  fAC_Cap_Ok_nside[NCHMAX];
 Double_t  fAC_Cap_Value_pside[NCHMAX];
 Double_t  fAC_Cap_Value_nside[NCHMAX];
 Double_t  fI_Strip_pside[NCHMAX];
 Double_t  fI_Strip_nside[NCHMAX];
};

#endif // CBMSTSWTQACHMAPCHARTVIEW
