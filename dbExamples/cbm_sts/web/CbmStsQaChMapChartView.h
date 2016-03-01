#ifndef CBMSTSQACHMAPCHARTVIEW_H
#define CBMSTSQACHMAPCHARTVIEW_H

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

class CbmStsQaChMapTableChartView: public Wt::WContainerWidget
{
public:
  CbmStsQaChMapTableChartView(Wt::WContainerWidget *parent);
};

class CbmStsQaChMapChartView: public Wt::WContainerWidget
{
public:
  CbmStsQaChMapChartView(int sid, Wt::WContainerWidget *parent);
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

#endif // CBMSTSQACHMAPCHARTVIEW
