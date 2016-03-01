#ifndef CBMSTSQAWTREEVIEW_H
#define CBMSTSQAWTREEVIEW_H


#include "FairDbWtTreeView.h"
#include "CbmStsQaWParTreeTable.h"
#include "CbmStsQaWParTreeTableNode.h"
#include <Wt/WCssDecorationStyle>
#include <string>

using namespace std;


class  CbmStsQaWTreeView : public FairDbWtTreeView
{

public:
  CbmStsQaWTreeView(WContainerWidget *parent = 0);
  virtual ~CbmStsQaWTreeView();

   // Widgets
  virtual FairDbWtParTreeTable* TreeTableView();
  virtual void TreeTableChanged();
  virtual void InspectParameter();
  virtual void Show_Iv(int sid=0);
  virtual void Show_Geo(vector<int>& sid);
  virtual void Show_Owner(vector<int>& sid);
  virtual void Show_Results(vector<int>& sid);
  virtual void Show_ChannelMap(int sid=0);


  void CreateParView(WContainerWidget* parent
                    , CbmStsQaWParTreeTableNode* tNode
                    , string pName);


};

#endif //CBMSTSQAWTREEVIEW_H
