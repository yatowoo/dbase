#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <time.h>


#include "FairDbParFactory.h"
#include "FairDbWtParTreeTableNode.h"


using namespace Wt;
using namespace std;

FairDbWtParTreeTableNode::FairDbWtParTreeTableNode(FairDbObjTableMap *aPar,
                                                   WString& pName, bool isDir)
  : WTreeTableNode(pName, createIcon(isDir))
{
   label()->setTextFormat(PlainText);

     if (!isDir) {
      setColumnWidget(1, new WText("Created"));
      //columnWidget(1)->setStyleClass("fsize");
      columnWidget(1)->setStyleClass("date");
      setColumnWidget(2, new WText("Updated"));
      columnWidget(2)->setStyleClass("date"); 
     } else {
      //setSelectable(false);
    }
      

  // Get the first connected DB instance
  fConnections = FairDbParFactory::Instance().GetConnections();

}


FairDbWtParTreeTableNode::~FairDbWtParTreeTableNode()
{
  cout << "~FairDbWtParTreeTableNode" << endl;
}


WIconPair *FairDbWtParTreeTableNode::createIcon(bool isDir)
{
  if (isDir)
    return new WIconPair("icons/yellow-folder-closed.png",
			 "icons/yellow-folder-open.png", false);
  else
    return new WIconPair("icons/document.png",
			 "icons/yellow-folder-open.png", false);
}

void FairDbWtParTreeTableNode::createTree()
{

 bool fail=false;
 TString global="Logs";
 TString par[5]={"Sts","Tof","Trd","Mvd","Rich"};

 auto_ptr<FairDbStatement> stmtDb(fConnections->CreateStatement(0));
  if ( ! stmtDb.get() ) {
    cout << "Cannot get a statement for cascade entry 0.  \n"
         << "Please check the ENV_TSQL_* environment.  Quitting ... " << endl;
  }

  TSQLStatement* stmt_init = stmtDb->ExecuteQuery("select * from FAIRDBGLOBALSEQNO;");

  if ( stmt_init ) {
     WTreeTableNode *gNode = new WTreeTableNode(global.Data(),createIcon(true));
     //gNode->setSelectable(false);
     addChildNode(gNode);
      
     while ( stmt_init->NextResultRow() ) {
     TString tableName = stmt_init->GetString(0);
      int lastUsedSeqNo = stmt_init->GetInt(1);
       cout << " ------- tableName : " << tableName << endl;
      if (tableName.Contains("FAIRDBLOGENTRY")) {
          tableName.ToLower(); 
          WTreeTableNode *aNode = new WTreeTableNode(tableName.Data(),createIcon(false));
          aNode->setColumnWidget(1, new WText("2015-01-08"));
          aNode->columnWidget(1)->setStyleClass("date");
          aNode->setColumnWidget(2, new WText("2015-03-10"));
          aNode->columnWidget(2)->setStyleClass("date");
          aNode->setSelectable(true); 
          aNode->setInteractive(true); 
          gNode->addChildNode(aNode);
          break;  
      }
      
     }
   }
     

 // Loop artificially just for the  presentation
 for (int i=0;i<1;i++){  
 
  TSQLStatement* stmtTSQL = stmtDb->ExecuteQuery("select * from FAIRDBGLOBALSEQNO;");

  if ( stmtTSQL ) {

     WTreeTableNode *pNode = new WTreeTableNode(par[i].Data(),createIcon(true));
     //pNode->setSelectable(false);
     addChildNode(pNode);
     TString tableName="";
     int lastUsedSeqNo=0; 
     while ( stmtTSQL->NextResultRow() ) {
  
      TString tName = stmtTSQL->GetString(0);
      int lUsedSeqNo = stmtTSQL->GetInt(1);
       if (tName == tableName)  continue; 
       
          tableName = tName;
          lastUsedSeqNo = lUsedSeqNo;

       if  ( tableName != "*" && tableName != "FAIRDBLOGENTRY") {
          cout << "tablename:" << tableName << "seqno:" << lastUsedSeqNo << endl;
          tableName.ToLower(); 
          initParameter(tableName.Data());
          WTreeTableNode *tNode = new WTreeTableNode(tableName.Data(),createIcon(false));
          tNode->setColumnWidget(1, new WText(creationDate[tableName.Data()]));
          tNode->columnWidget(1)->setStyleClass("date");
          tNode->setColumnWidget(2, new WText(modifiedDate[tableName.Data()]));
          tNode->columnWidget(2)->setStyleClass("date");
          tNode->setSelectable(true); 
          tNode->setInteractive(true); 
          pNode->addChildNode(tNode); 
        }
      }
       
    }//! stmtTSQL
  }//! (i)

// example multiple nodes

TString mFolder[5]={"Sts_Cal","Sts_Align","Sts_Cable","Sts_mapping","Sts_Elex"};

WTreeTableNode *aNode;
for (int i=0;i<4;i++){  
  aNode = new WTreeTableNode(mFolder[i].Data(),createIcon(true));
  WTreeTableNode *bNode; 
   for(int j=1;j<10;j++){
       char buf[10];
       sprintf(buf,"Layer_%i",j);  
       bNode = new WTreeTableNode(buf,createIcon(true)); 
       WTreeTableNode *cNode;
       for(int k = 0;k<2;k++){
        //char buf[10];
        sprintf(buf,"Module_%i",k);  
        cNode = new WTreeTableNode(buf,createIcon(true));
        WTreeTableNode *tNode;
         for(int l = 0;l<6;l++){

          char date_c[100],date_u[100];
          sprintf(buf,"cal_par%i",l);  
          sprintf(date_c,"2015-01-%i%i",k,l);
          sprintf(date_u,"2015-02-%i%i",k,l);

          tNode = new WTreeTableNode(buf,createIcon(false));

          tNode->setColumnWidget(1, new WText(date_c));
          tNode->columnWidget(1)->setStyleClass("date");
          tNode->setColumnWidget(2, new WText(date_u));
          tNode->columnWidget(2)->setStyleClass("date");
          tNode->setSelectable(true); 
          tNode->setInteractive(true); 
     
          cNode->addChildNode(tNode);
         }
       bNode->addChildNode(cNode); 
       } 
    aNode->addChildNode(bNode); 
   }
 addChildNode(aNode); 
}

 
 
            
}

bool FairDbWtParTreeTableNode::expandable()
{
      return WTreeTableNode::expandable();
}


void FairDbWtParTreeTableNode::initParameter(string pName){
//
}

WTable* FairDbWtParTreeTableNode::createDataTable(WContainerWidget* aParent, string pName)
{
  return NULL;
}


WContainerWidget* FairDbWtParTreeTableNode::createDataTableView(WContainerWidget* aParent, string pName)
{
  return NULL;
}

/*
WTableView* FairDbWtParTreeTableNode::createDataTableView(WContainerWidget* aParent, string pName)
{
  return NULL;
}
*/

WTable* FairDbWtParTreeTableNode::createValTable(WContainerWidget* aParent, string pName)
{
  return NULL;
}


WTable* FairDbWtParTreeTableNode::createMetaTable(WContainerWidget* aParent, string pName)
{
  return NULL;
}

WTable* FairDbWtParTreeTableNode::createConnTable(WContainerWidget* aParent, string pName)
{
  return NULL;
}

void FairDbWtParTreeTableNode::fillTableRows(WTable* table,string pName) 
{
  ;
} 


void FairDbWtParTreeTableNode::split(const std::string& str, std::vector<std::string>& v) {
  std::stringstream ss(str);
  ss >> std::noskipws;
  std::string field;
  char ws_delim;
  while(1) {
    if( ss >> field )
      v.push_back(field);
    else if (ss.eof())
      break;
    else
      v.push_back(std::string());
    ss.clear();
    ss >> ws_delim;
  }
}






