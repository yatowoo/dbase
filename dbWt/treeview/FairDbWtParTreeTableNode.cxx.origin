#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <time.h>

#include <Wt/WIconPair>
#include <Wt/WStringUtil>
#include <Wt/WText>
#include <Wt/WPushButton>

#include "FairDbWtParTreeTableNode.h"
#include "FairDbStatement.h"
#include "TSQLStatement.h"
#include "FairDbConnectionPool.h"
#include "FairDbTableInterfaceStore.h"
#include "FairRunIdGenerator.h"
#include "FairDbValrecord.h"
#include "ValInterval.h"
#include "FairRunIdGenerator.h"



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
      
  // Connect FairDb via RTDB
  fRtdb = FairRuntimeDb::instance();

  // Set the SQL IO as first input
  fInput = new FairParTSQLIo();

  // Open Connection
  fInput->SetVerbosity(1);
  
  // Get the first connected DB instance
  fConnections = FairDbTableInterfaceStore::Instance().fConnectionPool;

  // Create the tree
  createTree();
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
          initContainer(tableName.Data());
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
        WTreeTableNode *cNode = new WTreeTableNode(buf,createIcon(true));
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


void FairDbWtParTreeTableNode::initContainer(string pName){

 //Init at Now() !
  FairRunIdGenerator runID;
  UInt_t runId =  runID.generateId();

 if (pName == "fairdbtutpar"){
  FairDbTutPar* p1 = new FairDbTutPar("FairDbTutPar","FairDbTutPar","FairDbTest"); 
    p1->SetVersion(0);
    p1->SetComboNo(-1);
    p1->SetDbEntry(0);
    p1->SetLogTitle(pName);

  if (p1) {

      p1->fill(runId);

      // register object
      fMapObjects[pName]=p1;

      const FairDbValRecord *p1_Rec = p1->GetParamReader()->GetValidityRec();
      ValInterval val_p1 = p1->GetParamReader()->GetValidityRec()->GetValInterval();
      ValTimeStamp p1_cDate  = p1_Rec->GetCreationDate();
      ValTimeStamp p1_update = val_p1.GetTimeStart(); 
      string a = p1_cDate.AsString("s");
      string b = p1_update.AsString("s");
      vector<string> va, vb;
      split(a,va);
      split(b,vb);
      creationDate[pName]=va[0];
      modifiedDate[pName]=vb[0];

     
      cout << "Container " << pName << "  is initialised with validity: " << creationDate[pName] 
           <<  " : " << modifiedDate[pName] << endl;

      p1->Print();
    } 
 }else if(pName == "fairdbtutparbin"){
  FairDbTutParBin* p2 = new FairDbTutParBin("FairDbTutParBin","FairDbTutParBin","FairDbTest");  
    p2->SetVersion(0);
    p2->SetComboNo(-1);
    p2->SetDbEntry(0);
    p2->SetLogTitle(pName);

  if(p2) {

    p2->fill(runId);

    // register objects
    fMapObjects[pName]=p2;

    const FairDbValRecord *p2_Rec = p2->GetParamReader()->GetValidityRec();
    ValInterval val_p2 = p2->GetParamReader()->GetValidityRec()->GetValInterval();
    ValTimeStamp p2_cDate  = p2_Rec->GetCreationDate();
    ValTimeStamp p2_update = val_p2.GetTimeStart(); 
    string a = p2_cDate.AsString("s");
    string b = p2_update.AsString("s");
    vector<string> va, vb;
    split(a,va);
    split(b,vb);
    creationDate[pName]=va[0];
    modifiedDate[pName]=vb[0];
    cout << "Container " << pName << "  is initialised with validity: " <<  creationDate[pName] 
         <<  " : " << modifiedDate[pName]  << endl;

    p2->Print();

    } 
 }
}

WTable* FairDbWtParTreeTableNode::createTable(WContainerWidget* aParent, string pName)
{
   WTable* table = NULL; 
   cout << "pName----------->" << pName << endl;   
   if(pName == "fairdbtutparbin"){
      
      FairDbTutParBin* p1 = (FairDbTutParBin*) fMapObjects[pName];
      std::ostringstream s;
      double topPitch   = p1->GetTopPitch();
      s << topPitch ;
      string s_topPitch = s.str();
      s.str("");

      double topAnchor  = p1->GetTopAnchor();
      s << topAnchor ;
      string s_topAnchor = s.str();
      s.str("");
      int    nrTopFE    = p1->GetNrTopFE();
      s << nrTopFE ;
      string s_nrTopFE = s.str();
      s.str("");
      string feType     = p1->GetFeType();
     
      int* i_arr;  
       i_arr = p1->GetMyIArray();
       for(int i=0;i<3;i++){
         
         if (i==2) s << i_arr[i]; 
         else s << i_arr[i] << ", ";      
       }
       string s_i_arr = s.str();
       s.str("");

      double* d_arr;  
       d_arr = p1->GetMyDArray();
       for(int i=0;i<10;i++){
         
         if (i==9) s << d_arr[i];
         else s << d_arr[i] << ", ";    
       }
       string s_d_arr = s.str();
       s.str("");
      
      table = new Wt::WTable(aParent);
      table->addStyleClass("table form-inline");
      table->addStyleClass("table-bordered");
      table->addStyleClass("table-condensed");
      table->addStyleClass("table-striped");
      table->addStyleClass("table-hoved");

      table->elementAt(0, 0)->addWidget(new Wt::WText("TopPitch"));
      table->elementAt(0, 1)->addWidget(new Wt::WText(s_topPitch.c_str()));
      table->elementAt(1, 0)->addWidget(new Wt::WText("TopAnchor"));
      table->elementAt(1, 1)->addWidget(new Wt::WText(s_topAnchor.c_str()));
      table->elementAt(2, 0)->addWidget(new Wt::WText("NrTopFE"));
      table->elementAt(2, 1)->addWidget(new Wt::WText(s_nrTopFE.c_str()));
      table->elementAt(3, 0)->addWidget(new Wt::WText("FeType"));
      table->elementAt(3, 1)->addWidget(new Wt::WText(feType.c_str()));
      table->elementAt(4, 0)->addWidget(new Wt::WText("myIArray"));
      table->elementAt(4, 1)->addWidget(new Wt::WText(s_i_arr.c_str()));
      table->elementAt(5, 0)->addWidget(new Wt::WText("myDArray"));
      table->elementAt(5, 1)->addWidget(new Wt::WText(s_d_arr.c_str()));
      table->elementAt(6, 0)->addWidget(new Wt::WText("ROOT::TF1"));
      Wt::WPushButton *ok = new Wt::WPushButton("Draw()");
      table->elementAt(6, 1)->addWidget(ok);


   }else if(pName == "fairdbtutpar"){
      
      FairDbTutPar* p1 = (FairDbTutPar*) fMapObjects[pName];
      std::ostringstream s;
      double topPitch   = p1->GetTopPitch();
      s << topPitch ;
      string s_topPitch = s.str();
      s.str("");

      double topAnchor  = p1->GetTopAnchor();
      s << topAnchor ;
      string s_topAnchor = s.str();
      s.str("");
      int    nrTopFE    = p1->GetNrTopFE();
      s << nrTopFE ;
      string s_nrTopFE = s.str();
      s.str("");
      string feType     = p1->GetFeType();
 
      table = new Wt::WTable(aParent);
      table->addStyleClass("table form-inline");
      table->addStyleClass("table-bordered");
      table->addStyleClass("table-condensed");
      table->addStyleClass("table-striped");
      table->addStyleClass("table-hoved");

      table->elementAt(0, 0)->addWidget(new Wt::WText("TopPitch"));
      table->elementAt(0, 1)->addWidget(new Wt::WText(s_topPitch.c_str()));
      table->elementAt(1, 0)->addWidget(new Wt::WText("TopAnchor"));
      table->elementAt(1, 1)->addWidget(new Wt::WText(s_topAnchor.c_str()));
      table->elementAt(2, 0)->addWidget(new Wt::WText("NrTopFE"));
      table->elementAt(2, 1)->addWidget(new Wt::WText(s_nrTopFE.c_str()));
      table->elementAt(3, 0)->addWidget(new Wt::WText("FeType"));
      table->elementAt(3, 1)->addWidget(new Wt::WText(feType.c_str()));

     
   }else if(pName == "fairdblogentry"){
 
   }


  return (table);
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

