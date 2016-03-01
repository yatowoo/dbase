#include "CbmStsQaWParTreeTableNode.h"
#include "CbmStsDbQaPar.h"
#include "CbmStsDbQaNewPar.h"
#include "CbmStsQaSensorDataModel.h"
#include "CbmStsQaSensorTableView.h"
#include "CbmStsQaSensorTableViewNew.h"

#include <WContainerWidget>
#include <WStandardItemModel>
#include <WStandardItem>
#include <WDoubleSpinBox>
#include <WLabel>
#include <WText>


#include "FairDbWtParFactory.h"
#include <iostream>

using namespace std;
using namespace Wt;


CbmStsQaWParTreeTableNode::CbmStsQaWParTreeTableNode(FairDbObjTableMap *aPar,
                                                     WString& pName, bool isDir)
  : FairDbWtParTreeTableNode(aPar, pName, isDir)
{
}


CbmStsQaWParTreeTableNode::~CbmStsQaWParTreeTableNode()
{
  cout << "~CbmStsQaWParTreeTableNode" << endl;
}


void CbmStsQaWParTreeTableNode::createTree()
{
 cout << "-----------------------------  createTree() called " << endl;

 bool fail=false;
 TString global="Logs";
 TString par[5]={"QaSts","Tof","Trd","Mvd","Rich"};

 auto_ptr<FairDbStatement> stmtDb(fConnections->CreateStatement(0));
  if ( ! stmtDb.get() ) {
    cout << "Cannot get a statement for cascade entry 0.  \n"
         << "Please check the ENV_TSQL_* environment.  Quitting ... " << endl;
  }

  // Create the tree with Table Names from FairDB
  TSQLStatement* stmt_init = stmtDb->ExecuteQuery("select * from FAIRDBGLOBALSEQNO;");

  if ( stmt_init ) {

     //Main Folders
     // <DB> Commented out
     // needs specific treatment
     //
     //WTreeTableNode *gNode = new WTreeTableNode(global.Data(),createIcon(true));
     //gNode->setSelectable(false);
     //addChildNode(gNode);

     WTreeTableNode *pNode = new WTreeTableNode(par[0].Data(),createIcon(true));
      addChildNode(pNode);


     while ( stmt_init->NextResultRow() ) {

     TString tableName = stmt_init->GetString(0);
     TString branchName=tableName;
     branchName.Remove(0,10);

      int lastUsedSeqNo = stmt_init->GetInt(1);
       cout << " ------- tableName : " << tableName <<  " BranchName: " << branchName << endl;

      /* Comment this for now
         Since FairDB internal table
         needs specific treatment

      if (tableName.Contains("FAIRDBLOGENTRY")) {
          tableName.ToLower();
          WTreeTableNode *aNode = new WTreeTableNode(tableName.Data(),createIcon(false));
          aNode->setSelectable(true);
          aNode->setInteractive(true);
          gNode->addChildNode(aNode);
          break;
      }
      */
      if  ( tableName != "*" && tableName != "FAIRDBLOGENTRY") {
          cout << "tablename:" << tableName << "seqno:" << lastUsedSeqNo << endl;
          tableName.ToLower();
          // <DB> for now do not show the IV table
          // Since it will be linked to the Sensor table
          if (!tableName.Contains("stsqasensorpar")) continue;
          WTreeTableNode *tNode = new WTreeTableNode(tableName.Data(),createIcon(false));
          tNode->setSelectable(true);
          tNode->setInteractive(true);
          pNode->addChildNode(tNode);
        }
     }
  }

}



WTable* CbmStsQaWParTreeTableNode::createDataTable(WContainerWidget* aParent, string pName)
{
// Get the Meta Data
  FairDbWtParFactory::parInfo p_Info = FairDbWtParFactory::Instance().GetParInfo(pName);
  FairDbTableMetaData t_meta = p_Info.fTableInterface->GetMetaData();

  cout << "-------I ----- Table name " << t_meta.TableName()
         << " nCols:  " << t_meta.NumCols() << endl;

   int n_cols = t_meta.NumCols();
   vector<TableColumn> tcol;

  WTable* table = new WTable(aParent);
  table->setStyleClass("CSS-example");
  table->addStyleClass("table form-inline");
  table->addStyleClass("table-bordered");
  table->addStyleClass("table-condensed");
  table->addStyleClass("table-striped");
  table->addStyleClass("table-hoved");

  table->setHeaderCount(1);
  table->setWidth(Wt::WLength("100%"));

  //table->elementAt(0, 0)->addWidget(new Wt::WText("Parameter:"));
  //table->elementAt(0, 1)->addWidget(new Wt::WText(pName));
  //table->rowAt(0)->setStyleClass("info");

  // Table Header
  // for (unsigned int i=1;i<=t_meta.NumCols();i++){
  //  table->elementAt(0, i)->addWidget(new Wt::WText(t_meta.ColName(i)));
  //
  // }

  fillTableRows(table,pName);

  return table;
}

WContainerWidget* CbmStsQaWParTreeTableNode::createDataTableView(WContainerWidget* aParent,string pName)
{
  const char* st = pName.c_str();
  int new_version = 0;

  switch (str2int(st))
  {
    {
      case str2int("stsqasensorpar"):
      new_version = 1;
      break;
    }
    break;
  }

  cout << "-I CbmStsQaWParTreeTableNode:: create a STS Qa Sensor Table View with version = "
       << new_version << endl;;

  if (new_version == 1)
  return new CbmStsQaSensorTableViewNew(aParent,pName);
  else
  return new CbmStsQaSensorTableView(aParent,pName);
}


WTable* CbmStsQaWParTreeTableNode::createConnTable(WContainerWidget* aParent, string pName)
{

  cout << "-I- connections status table: "  << endl;

  vector<string> db_name;
  vector<string> url_name;
  vector<string> user_name;
  vector<string> status;
  vector<string> server_prod;
  vector<string> server_info;

  int ndb = 0;

  if(fConnections){
   ndb = fConnections->GetNumDb();
   if (ndb == 0 ) return NULL;

   for (int i=0; i<ndb;i++){
    FairDbConnection* pConn = fConnections->GetConnection(i);

    if (!pConn) continue;
     db_name.push_back(pConn->GetDbName());
     url_name.push_back(pConn->GetUrl());
     user_name.push_back(pConn->GetUser());
     status.push_back(fConnections->GetStatusAsString(i));
     TSQLServer* pServ = pConn->GetServer();
     server_prod.push_back(pServ->GetDBMS());
     server_info.push_back(pServ->ServerInfo());
   }// !for()
  }//!fConnections

  Wt::WTable *table = new Wt::WTable();
  table->setStyleClass("CSS-example");
  table->setHeaderCount(1);
  table->setWidth(Wt::WLength("100%"));


 int row=-1;

 for (int i = 0; i < ndb; ++i) {

   string conn_entry = "Connection# ";
   conn_entry += std::to_string( i );

   table->elementAt(++row, 0)->addWidget(new Wt::WText("FairDB"));
   table->elementAt(row, 1)
            ->addWidget(new Wt::WText(conn_entry));

   table->rowAt(row)->setStyleClass("info");


   table->elementAt(++row, 0)
            ->addWidget(new Wt::WText("db_name"));

   table->elementAt(row, 1)
            ->addWidget(new Wt::WText(db_name[i]));

   table->elementAt(++row, 0)
            ->addWidget(new Wt::WText("db_prod"));

   table->elementAt(row, 1)
            ->addWidget(new Wt::WText(server_prod[i]));


   table->elementAt(++row, 0)
            ->addWidget(new Wt::WText("db_info"));

   table->elementAt(row, 1)
            ->addWidget(new Wt::WText(server_info[i]));


   table->elementAt(++row, 0)
            ->addWidget(new Wt::WText("db_url"));

   table->elementAt(row, 1)
            ->addWidget(new Wt::WText(url_name[i]));

   table->elementAt(++row, 0)
            ->addWidget(new Wt::WText("db_user"));

   table->elementAt(row, 1)
            ->addWidget(new Wt::WText(user_name[i]));


  table->elementAt(++row, 0)
            ->addWidget(new Wt::WText("connection status:"));

  table->elementAt(row, 1)
            ->addWidget(new Wt::WText(status[i]));

  }

  return table;
}

WTable* CbmStsQaWParTreeTableNode::createValTable(WContainerWidget* aParent, string pName)
{
 return NULL;
}


WTable* CbmStsQaWParTreeTableNode::createMetaTable(WContainerWidget* aParent, string pName)
{
  // First initialise the container if not done
  // already
  initParameter(pName);


  // Get the Meta Data
  FairDbWtParFactory::parInfo p_Info = FairDbWtParFactory::Instance().GetParInfo(pName);
  FairDbTableMetaData t_meta = p_Info.fTableInterface->GetMetaData();

  cout << "-------I ----- Table name " << t_meta.TableName()
         << " nCols:  " << t_meta.NumCols() << endl;

   int n_cols = t_meta.NumCols();
   vector<TableColumn> tcol;

   for (unsigned int i=1;i<=t_meta.NumCols();i++){
      FairDbFieldType cType = t_meta.ColFieldType(i);
      cout << " at column: " << i <<  " Colname: "
           << t_meta.ColName(i) <<  ": " << cType.AsString()
           << " : " << cType.AsSQLString() << endl;

      tcol.push_back(TableColumn(t_meta.ColName(i), cType.AsString(), cType.AsSQLString()));
   }

  //Create the table
  Wt::WTable *table = new Wt::WTable();
  table->setStyleClass("CSS-example");
  table->setHeaderCount(3);
  table->setWidth(Wt::WLength("100%"));


  table->elementAt(0, 0)->addWidget(new Wt::WText("Table:"));
  table->elementAt(0, 1)->addWidget(new Wt::WText(t_meta.TableName()));
  table->elementAt(0, 2)->addWidget(new Wt::WText(""));


  table->elementAt(1, 0)->addWidget(new Wt::WText(" "));
  table->elementAt(1, 1)->addWidget(new Wt::WText(" "));
  table->elementAt(1, 2)->addWidget(new Wt::WText(" "));


  table->elementAt(2, 0)->addWidget(new Wt::WText("Columns"));
  table->elementAt(2, 1)->addWidget(new Wt::WText("C Type"));
  table->elementAt(2, 2)->addWidget(new Wt::WText("SQL Type"));

  for (unsigned i = 0; i < n_cols; ++i) {
       TableColumn& aCol = tcol[i];
       int row = i + 3;

       table->elementAt(row, 0)
            ->addWidget(new Wt::WText(aCol.t_name));

       table->elementAt(row, 1)
            ->addWidget(new Wt::WText(aCol.t_ctype));

       table->elementAt(row, 2)
            ->addWidget(new Wt::WText(aCol.t_sqltype));
   }

// Set Some Style
   // <DB> condensing doesnt work for any reason?
   //table->toggleStyleClass("table-condensed", true);
   table->rowAt(0)->setStyleClass("info");

 return table;
}



void CbmStsQaWParTreeTableNode::initParameter(string pName)
{
 const char* st = pName.c_str();

 switch (str2int(st))
 {
   {
     case str2int("cbmstsdbqapar"):
     auto  pQaPar = FairDbWtParFactory::Instance().InitContainer<CbmStsDbQaPar>();
     if(pQaPar)
     {
       FairDbReader<CbmStsDbQaSensorPar>* r_Sensor = pQaPar->GetReaderSensor();
       FairDbWtParFactory::Instance().RegisterReader<CbmStsDbQaSensorPar>(r_Sensor);
     }
     cout <<"-I- CbmStsDbQaPar is initialised " << endl;
     break;
   }
   {
     case str2int("cbmstsdbqasensorpar"):
     auto pQaSensorPar =
     FairDbWtParFactory::Instance().InitContainer<CbmStsDbQaSensorPar>();
     if(pQaSensorPar)
     {
       FairDbReader<CbmStsDbQaSensorPar>* r_Sensor = pQaSensorPar->GetParamReader();
       FairDbWtParFactory::Instance().RegisterReader<CbmStsDbQaSensorPar>(r_Sensor);
     }
     cout <<"-I- CbmStsDbQaSensorPar is initialised " << endl;
     break;
   }
   {
     case str2int("cbmstsdbqaivpar"):
     auto pQaIvPar =
     FairDbWtParFactory::Instance().InitContainer<CbmStsDbQaIvPar>();
     if(pQaIvPar)
     {
       FairDbReader<CbmStsDbQaIvPar>* r_Sensor = pQaIvPar->GetParamReader();
       FairDbWtParFactory::Instance().RegisterReader<CbmStsDbQaIvPar>(r_Sensor);
     }
     cout <<"-I- CbmStsDbQaIvPar is initialised " << endl;
     break;
   }
    {
      case str2int("stsqasensorpar"):
      auto pQaSensorPar_n =
      FairDbWtParFactory::Instance().InitContainer<CbmStsDbQaSensorNewPar>();
      if(pQaSensorPar_n)
      {
        FairDbReader<CbmStsDbQaSensorNewPar>* r_Sensor = pQaSensorPar_n->GetParamReader();
        FairDbWtParFactory::Instance().RegisterReader<CbmStsDbQaSensorNewPar>(r_Sensor);
      }
      cout <<"-I- CbmStsDbQaSensorNewPar is initialised " << endl;
      break;
    }

 } //! switch

}


void CbmStsQaWParTreeTableNode::fillTableRows(WTable* table, string pName)
{

 const char* st = pName.c_str();

 FairRunIdGenerator runID;
 UInt_t runId =  runID.generateId();
 std::ostringstream s;

 switch (str2int(st))
 {

  { // Sensors
  case str2int("cbmstsdbqasensorpar"):
    auto pQaSensorPar =
         FairDbWtParFactory::Instance().InitContainer<CbmStsDbQaSensorPar>();
    pQaSensorPar->fill(runId);
    // Now get all rows from parameter
    FairDbReader<CbmStsDbQaSensorPar>* pReader = pQaSensorPar->GetParamReader();
    int numRows = pReader->GetNumRows();


    table->elementAt(0, 0)->addWidget(new Wt::WText("Unique_id"));
    table->elementAt(0, 1)->addWidget(new Wt::WText("Sensor_type"));
    table->elementAt(0, 2)->addWidget(new Wt::WText("Batch_id"));
    table->elementAt(0, 3)->addWidget(new Wt::WText("Wafer_id"));
    table->elementAt(0, 4)->addWidget(new Wt::WText("Sensor_name"));
    table->elementAt(0, 5)->addWidget(new Wt::WText("Manufacturer"));
    table->elementAt(0, 6)->addWidget(new Wt::WText("Production_date"));
    table->elementAt(0, 7)->addWidget(new Wt::WText("Current_owner"));
    table->elementAt(0, 8)->addWidget(new Wt::WText("Current_location"));
    table->elementAt(0, 9)->addWidget(new Wt::WText("V_fd"));
    table->elementAt(0, 10)->addWidget(new Wt::WText("I_150V_20C"));
    table->elementAt(0, 11)->addWidget(new Wt::WText("I_250V_20C"));
    table->elementAt(0, 12)->addWidget(new Wt::WText("Defect_pside"));
    table->elementAt(0, 13)->addWidget(new Wt::WText("Defect_nside"));
    table->elementAt(0, 14)->addWidget(new Wt::WText("Quality_grade"));
    table->elementAt(0, 15)->addWidget(new Wt::WText("Problem"));
    table->elementAt(0, 16)->addWidget(new Wt::WText("Passed"));
    table->elementAt(0, 17)->addWidget(new Wt::WText("Comments"));


    table->rowAt(0)->setStyleClass("info");


    for (int i = 0; i < numRows; ++i)
     {
      CbmStsDbQaSensorPar* cgd = (CbmStsDbQaSensorPar*) pReader->GetRow(i);
      if (!cgd) { continue; }
       int compId = cgd->GetCompId();
       int uID = cgd->GetUID();
       string sensor_type = cgd->GetSensorType();
       string batch_id = cgd->GetBatchId();
       int wafer_id = cgd->GetWaferId();
       string sensor_name = cgd->GetSensorName();
       string manufacturer = cgd->GetManufacturer();
       string production_date = cgd->GetProductionDate();
       string current_owner = cgd->GetOwner();
       string current_location = cgd->GetLocation();
       double v_fd =  cgd->GetV_fd();
       double i_150V_20C = cgd->GetI_150V_20C();
       double i_250V_20C = cgd->GetI_250V_20C();
       int sdefect_pside = cgd->GetSdefect_pside();
       int sdefect_nside = cgd->GetSdefect_nside();
       int quality_grade = cgd->GetQuality_grade();
       string problem = cgd->GetProblem();
       string passed = cgd->GetPassed();
       string comments =  cgd->GetComment();


       cout << "-I- fillTableRows row = " << i << " uid " << uID << " sensor_type " << sensor_type
       <<  " bacth_id " << manufacturer << endl;
       int rowid = i+1;
       s.str("");
       s << uID;
       table->elementAt(rowid, 0)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << sensor_type;
       table->elementAt(rowid, 1)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << batch_id;
       table->elementAt(rowid, 2)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << wafer_id;
       table->elementAt(rowid, 3)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << sensor_name;
       table->elementAt(rowid, 4)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << manufacturer;
       table->elementAt(rowid, 5)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << production_date;
       table->elementAt(rowid, 6)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << current_owner;
       table->elementAt(rowid, 7)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << current_location;
       table->elementAt(rowid, 8)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << v_fd;
       table->elementAt(rowid, 9)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << i_150V_20C;
       table->elementAt(rowid, 10)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << i_250V_20C;
       table->elementAt(rowid, 11)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << sdefect_pside;
       table->elementAt(rowid, 12)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << sdefect_nside;
       table->elementAt(rowid, 13)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << quality_grade;
       table->elementAt(rowid, 14)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << problem;
       table->elementAt(rowid, 15)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << passed;
       table->elementAt(rowid, 16)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << comments;
       table->elementAt(rowid, 17)->addWidget(new Wt::WText(s.str()));
       s.str("");

     }

  break;
  }

  { //IV measureaments
  case str2int("cbmstsdbqaivpar"):
   // here i will try to use the WTableView

    auto pQaIvPar =
        FairDbWtParFactory::Instance().InitContainer<CbmStsDbQaIvPar>();
    pQaIvPar->fill(runId);

   if(pQaIvPar)
    {
       int uid = pQaIvPar->GetUID();
       s << uid;
       table->elementAt(1, 0)->addWidget(new Wt::WText("Unique_Id:"));
       table->elementAt(1, 1)->addWidget(new Wt::WText(s.str()));
       s.str("");
       int n = pQaIvPar->GetN();
       s << n;
       table->elementAt(2, 0)->addWidget(new Wt::WText("N_meas:"));
       table->elementAt(2, 1)->addWidget(new Wt::WText(s.str()));
       s.str("");

        // I array
       double* i_meas = pQaIvPar->GetI();
       Wt::WContainerWidget *icontainer = new Wt::WContainerWidget();
       icontainer->addStyleClass("form-group");

       s << "Meas. nb [0 - " << n-1 << "]?";
       Wt::WLabel *ilabel = new Wt::WLabel(s.str(), icontainer);
       Wt::WDoubleSpinBox *isb = new Wt::WDoubleSpinBox(icontainer);
       isb->setRange(0,n-1);
       isb->setValue(0);
       isb->setDecimals(0);
       isb->setSingleStep(1.);
       ilabel->setBuddy(isb);

       WText *iout = new WText("", icontainer);
       iout->addStyleClass("help-block");

       isb->changed().connect(std::bind([=] () {
         if (isb->validate() == Wt::WValidator::Valid) {
           iout->setText(Wt::WString::fromUTF8("I_measured (muA) :  {1}")
           .arg(i_meas[(int)(isb->value())]));
         } else {
        iout->setText(Wt::WString::fromUTF8("Invalid I meas. nb value!"));
       }
     }));

     table->elementAt(3, 0)->addWidget(new Wt::WText("I (muA):"));
     table->elementAt(3, 1)->addWidget( icontainer);


      // V Array
      double* v_meas = pQaIvPar->GetV();
      Wt::WContainerWidget *vcontainer = new Wt::WContainerWidget();
      vcontainer->addStyleClass("form-group");
      s.str("");
      s << "Meas. nb [0 - " << n-1 << "]?";
      Wt::WLabel *vlabel = new Wt::WLabel(s.str(), vcontainer);
      Wt::WDoubleSpinBox *vsb = new Wt::WDoubleSpinBox(vcontainer);
      vsb->setRange(0,n-1);
      vsb->setValue(0);
      vsb->setDecimals(0);
      vsb->setSingleStep(1.);
      vlabel->setBuddy(vsb);

      WText *vout = new WText("", vcontainer);
      vout->addStyleClass("help-block");

      vsb->changed().connect(std::bind([=] () {
         if (vsb->validate() == Wt::WValidator::Valid) {
           vout->setText(Wt::WString::fromUTF8("V_measured (V) :  {1}")
           .arg(v_meas[(int)(vsb->value())]));
         } else {
        vout->setText(Wt::WString::fromUTF8("Invalid V meas. nb value!"));
       }
     }));

     table->elementAt(4, 0)->addWidget(new Wt::WText("V (Volt):"));
     table->elementAt(4, 1)->addWidget( vcontainer);

    }
    break;
   }

   { // Sensors
     case str2int("stsqasensorpar"):
     auto pQaSensorNewPar =
     FairDbWtParFactory::Instance().InitContainer<CbmStsDbQaSensorNewPar>();
     pQaSensorNewPar->fill(runId);
     // Now get all rows from parameter
     FairDbReader<CbmStsDbQaSensorNewPar>* pReader = pQaSensorNewPar->GetParamReader();
     int numRows = pReader->GetNumRows();


     table->elementAt(0, 0)->addWidget(new Wt::WText("Unique_id"));
     table->elementAt(0, 1)->addWidget(new Wt::WText("Sensor_type"));
     table->elementAt(0, 2)->addWidget(new Wt::WText("Batch_id"));
     table->elementAt(0, 3)->addWidget(new Wt::WText("Wafer_id"));
     table->elementAt(0, 4)->addWidget(new Wt::WText("Reticle_name"));
     table->elementAt(0, 5)->addWidget(new Wt::WText("Vendor"));
     table->elementAt(0, 6)->addWidget(new Wt::WText("Processing"));
     table->elementAt(0, 7)->addWidget(new Wt::WText("Height"));
     table->elementAt(0, 8)->addWidget(new Wt::WText("Width"));
     table->elementAt(0, 9)->addWidget(new Wt::WText("StripsPerSide"));
     table->elementAt(0, 10)->addWidget(new Wt::WText("Year"));
     table->elementAt(0, 11)->addWidget(new Wt::WText("Owner"));
     table->elementAt(0, 12)->addWidget(new Wt::WText("Location"));
     table->elementAt(0, 13)->addWidget(new Wt::WText("V_fd"));
     table->elementAt(0, 14)->addWidget(new Wt::WText("I_150V_20C"));
     table->elementAt(0, 15)->addWidget(new Wt::WText("I_250V_20C"));
     table->elementAt(0, 16)->addWidget(new Wt::WText("Defect_pside"));
     table->elementAt(0, 17)->addWidget(new Wt::WText("Defect_nside"));
     table->elementAt(0, 18)->addWidget(new Wt::WText("Quality_grade"));
     table->elementAt(0, 19)->addWidget(new Wt::WText("Problem"));
     table->elementAt(0, 20)->addWidget(new Wt::WText("Passed"));
     table->elementAt(0, 21)->addWidget(new Wt::WText("OpticalCheck"));
     table->elementAt(0, 22)->addWidget(new Wt::WText("Comments"));


     table->rowAt(0)->setStyleClass("info");


     for (int i = 0; i < numRows; ++i)
     {
       CbmStsDbQaSensorNewPar* cgd = (CbmStsDbQaSensorNewPar*) pReader->GetRow(i);
       if (!cgd) { continue; }
       int compId = cgd->GetCompId();
       int uID = cgd->GetUID();
       string sensor_type = cgd->GetSensorType();
       string batch_id = cgd->GetBatchId();
       int wafer_id = cgd->GetWaferId();
       string reticle_name = cgd->GetReticleName();
       string vendor_name = cgd->GetVendorName();
       string processing = cgd->GetProcessing();
       double height = cgd->GetHeight();
       double width = cgd->GetWidth();
       int stripsPerSide = cgd->GetStripsPerSide();
       int year = cgd->GetYear();
       string current_owner = cgd->GetOwner();
       string current_location = cgd->GetLocation();
       double v_fd =  cgd->GetV_fd();
       double i_150V_20C = cgd->GetI_150V_20C();
       double i_250V_20C = cgd->GetI_250V_20C();
       int sdefect_pside = cgd->GetSdefect_pside();
       int sdefect_nside = cgd->GetSdefect_nside();
       int quality_grade = cgd->GetQuality_grade();
       int problem = cgd->GetProblem();
       int passed = cgd->GetPassed();
       int optical_check = cgd->GetOpticalCheck();
       string comments =  cgd->GetComment();

       cout << "-I- fillTableRows row = " << i << " uid " << uID << " sensor_type " << sensor_type
       <<  " bacth_id " << batch_id << endl;
       int rowid = i+1;
       s.str("");
       s << uID;
       table->elementAt(rowid, 0)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << sensor_type;
       table->elementAt(rowid, 1)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << batch_id;
       table->elementAt(rowid, 2)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << wafer_id;
       table->elementAt(rowid, 3)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << reticle_name;
       table->elementAt(rowid, 4)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << vendor_name;
       table->elementAt(rowid, 5)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << processing;
       table->elementAt(rowid, 6)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << height;
       table->elementAt(rowid, 7)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << width;
       table->elementAt(rowid, 8)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << stripsPerSide;
       table->elementAt(rowid, 9)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << year;
       table->elementAt(rowid, 10)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << current_owner;
       table->elementAt(rowid, 11)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << current_location;
       table->elementAt(rowid, 12)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << v_fd;
       table->elementAt(rowid, 13)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << i_150V_20C;
       table->elementAt(rowid, 14)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << i_250V_20C;
       table->elementAt(rowid, 15)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << sdefect_pside;
       table->elementAt(rowid, 16)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << sdefect_nside;
       table->elementAt(rowid, 17)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << quality_grade;
       table->elementAt(rowid, 18)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << problem;
       table->elementAt(rowid, 19)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << passed;
       table->elementAt(rowid, 20)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << optical_check;
       table->elementAt(rowid, 21)->addWidget(new Wt::WText(s.str()));
       s.str("");
       s << comments;
       table->elementAt(rowid, 22)->addWidget(new Wt::WText(s.str()));
       s.str("");
     }
   break;
  }//! switch
 cout <<"-I- Table filled with rows from parameter " <<  pName << endl;
 }
}


 ///////////////////////////////////////////////////////////////////////////////////////////////////

// auto pQaPar =
 //  FairDbWtParFactory::Instance().InitContainer("CbmStsDbQaPar");
 // initContainer("CbmStsDbQaPar");
 // initContainer("CbmStsDbQaSensorPar");


 /*
 // Init the main  Parameter Manager  using date of the process
 // (last modified will then be taken)
 auto pQaPar =
   FairDbWtParFactory::Instance().InitContainer<CbmStsDbQaPar>();

 // Main Qa Parameter initialised and register the relevant reader ptrs
  if(pQaPar)
    {
       FairDbReader<CbmStsDbQaSensorPar>* r_Sensor = pQaPar->GetReaderSensor();
       FairDbWtParFactory::Instance().RegisterReader<CbmStsDbQaSensorPar>(r_Sensor);
    }

   auto pSensorReader =  FairDbWtParFactory::Instance().GetReader<CbmStsDbQaSensorPar>();
   if (pSensorReader){
     cout << " ------- FOUND Sensor reader !!!!!! " << endl;
   }

   const string creation =
   FairDbWtParFactory::Instance().GetCreationDate<CbmStsDbQaSensorPar>();
   cout << "--- I created at: " << creation << endl;

   const string modif =
   FairDbWtParFactory::Instance().GetModifiedDate<CbmStsDbQaSensorPar>();
   cout << "--- I modified at: " << modif << endl;

   const string endd =
   FairDbWtParFactory::Instance().GetEndDate<CbmStsDbQaSensorPar>();
   cout << "--- I end at: " << endd << endl;

*/

   /*
WTableView* CbmStsQaWParTreeTableNode::createDataTableView(WContainerWidget* aParent, string pName){

FairDbWtParFactory::parInfo p_Info = FairDbWtParFactory::Instance().GetParInfo(pName);
FairDbTableMetaData t_meta = p_Info.fTableInterface->GetMetaData();

  cout << "-------I ----- Table View name " << t_meta.TableName()
         << " nCols:  " << t_meta.NumCols() << endl;

   int n_cols = t_meta.NumCols();

Wt::WTableView *tableView = new Wt::WTableView(aParent);

CbmStsQaSensorDataModel *t_model =
         new CbmStsQaSensorDataModel(100, n_cols-3, tableView);

// Fill the model with data
t_model->fillrows();

tableView->setModel(t_model);
tableView->setColumnResizeEnabled(false);
tableView->setColumnAlignment(0, Wt::AlignCenter);
tableView->setHeaderAlignment(0, Wt::AlignCenter);

// treat first column as 'fixed' row headers
// <DB> check me : problem with event handling ??
//  date: 8.07.2015
//tableView->setRowHeaderCount(1);

tableView->setSortingEnabled(true);
tableView->setAlternatingRowColors(true);
tableView->setRowHeight(28);
tableView->setHeaderHeight(28);
tableView->setSelectionMode(Wt::ExtendedSelection);

tableView->setDragEnabled(true);
tableView->sortByColumn(1, AscendingOrder);

//tableView->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);
tableView->resize(900, 800);

tableView->clicked().connect(this, &CbmStsQaWParTreeTableNode::editFile);

tableView->doubleClicked().connect(
           boost::bind(&CbmStsQaWParTreeTableNode::editFiled, this, _1, t_model));

return tableView;
}
*/
