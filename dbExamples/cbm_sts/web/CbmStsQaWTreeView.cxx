#include "CbmStsQaWTreeView.h"
#include "CbmStsQaIvChartView.h"

CbmStsQaWTreeView::CbmStsQaWTreeView(WContainerWidget *parent)
 : FairDbWtTreeView(parent)
{
    cout << "-------------------- CbmStsTreeView ctor  -------- " << endl;   
    createLayout(parent); 

}

CbmStsQaWTreeView::~CbmStsQaWTreeView(){
 //
  cout << "~CbmStsQaWTreeView" << endl;
}


FairDbWtParTreeTable* CbmStsQaWTreeView::TreeTableView(){

  cout << "-------------------- CbmSts Tree Table View  -------- " << endl;

  CbmStsQaWParTreeTable *treeTable = new CbmStsQaWParTreeTable();
  treeTable->resize(300, 1400);
  //treeTable->resize(500, WLength::Auto);
  treeTable->tree()->setSelectionMode(ExtendedSelection);
  treeTable->treeRoot()->setNodeVisible(false);
  treeTable->treeRoot()->setChildCountPolicy(WTreeNode::Enabled);
  treeTable->tree()->itemSelectionChanged()
                    .connect(this, &FairDbWtTreeView::TreeTableChanged); 
  
  setTreeTable(treeTable); 
  return treeTable; 

}

void CbmStsQaWTreeView::InspectParameter() {

  // Get the tree
  WTree* aTree = fFileTreeTable->tree(); 
  // Get Tree Node
  CbmStsQaWParTreeTableNode* tNode = (CbmStsQaWParTreeTableNode*) fFileTreeTable->treeRoot();
  
  for (WTreeNodeSet::const_iterator i = aTree->selectedNodes().begin(); 
                                  i != aTree->selectedNodes().end(); ++i){
     WTreeNode *aNode=(*i);

     cout << "-I Server: CbmStsQaWTreeView::InspectParameter() Node:" 
     <<  aNode->label()->text().value() << endl;     
 
     string pName= aNode->label()->text().toUTF8();
     
     CreateParView(this,tNode,pName);  

  }
}


void CbmStsQaWTreeView::TreeTableChanged() {

  // Get the tree
  WTree* aTree = fFileTreeTable->tree(); 
  // Get Tree Node
  CbmStsQaWParTreeTableNode* tNode = (CbmStsQaWParTreeTableNode*) fFileTreeTable->treeRoot();
  
  for (WTreeNodeSet::const_iterator i = aTree->selectedNodes().begin(); 
                                  i != aTree->selectedNodes().end(); ++i){
     WTreeNode *aNode=(*i);
     string pName= aNode->label()->text().toUTF8();
     cout << "-I Server: CbmStsQaWTreeView::Inspect parameter Name:" 
          <<  aNode->label()->text().value() << endl; 
     WTable * mTable = tNode->createMetaTable(this, pName);
     WTable * cTable = tNode->createConnTable(this, pName);

     if (mTable){
         fWObject_Info->clear();
         fWObject_Info->addWidget(mTable); 
     }

     if (cTable){
         fConnections->clear();
         fConnections->addWidget(cTable); 
     }

     
  }//!(nodes)

}


 void CbmStsQaWTreeView::CreateParView( WContainerWidget* parent
                                      , CbmStsQaWParTreeTableNode* tNode
                                      , string pName)
 {
  // This code create a View from parameter container
  // according to the sts Qa parameter type 

   const char* st = pName.c_str(); 
         cout << "-E  CreateParView Table: " << st << endl;

   switch (str2int(st))
   {
    {
    case str2int("cbmstsdbqasensorpar"):     
      WContainerWidget *envelop  = tNode->createDataTableView(parent, pName);
 
     if (envelop){ 
        WMenuItem *tab = fTabWidget_top->addTab(envelop, pName);
        int index = fTabWidget_top->indexOf(envelop);
        fTabWidget_top->setCurrentIndex(index);
        tab->setCloseable(false);
     } 
    break;
    }
    {
    case str2int("cbmstsdbqaivpar"):
         WContainerWidget *envelop = new WContainerWidget();
       Wt::WTable* table = tNode->createDataTable(envelop,pName);
       envelop->decorationStyle().setBackgroundColor(Wt::white);
       envelop->setPadding(8); 
       envelop->setOverflow(OverflowAuto); 
       envelop->addWidget(table);  

       if (envelop){ 
         WMenuItem *tab = fTabWidget_top->addTab(envelop, pName);
         int index = fTabWidget_top->indexOf(envelop);
         fTabWidget_top->setCurrentIndex(index);
         tab->setCloseable(false);
        }
     break;    
    }
    {
     default:
       cout << "-E unknown parameter table: " << st << endl;
       return;
       break;
    }

    }//!switch
}

 void CbmStsQaWTreeView::Show_Iv(int sid){

  cout << "-I- CbmStsQaWTreeView::Show_Iv -----  " << endl;
  CbmStsQaIvChartView *iv_view = new CbmStsQaIvChartView(sid, this);
  WMenuItem *item_down = fTabWidget_down->addTab(iv_view,WString("I-V Sensor ID: {1}").arg(sid));
  int index = fTabWidget_down->indexOf(iv_view);              
  fTabWidget_down->setCurrentIndex(index);
  item_down->setCloseable(false);

}


