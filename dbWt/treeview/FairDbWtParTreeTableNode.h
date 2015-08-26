#ifndef FAIRDBWTPARTREETABLENODE_H
#define FAIRDBWTPARTREETABLENODE_H

#include <map> 
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>


#include <WContainerWidget>
#include <WTable>
#include <WTableView>
#include <WTreeTableNode>
#include <WString>
#include <WIconPair>
#include <WStringUtil>
#include <WText>
#include <WPushButton>


#include "FairDbObjTableMap.h"
#include "FairDbConnectionPool.h"
#include "FairDbConnection.h"
#include "FairDbStatement.h"
#include "TSQLServer.h"
#include "TSQLStatement.h"
#include "FairDbTableInterfaceStore.h"
#include "FairRunIdGenerator.h"
#include "FairDbValRecord.h"
#include "ValTimeStamp.h"
#include "ValInterval.h"

using namespace std;
using namespace Wt;



constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1)*33) ^ str[h];
}


class FairDbWtParTreeTableNode : public Wt::WTreeTableNode
{

 public:
 
    struct TableColumn {
     string t_name;
     string t_ctype;
     string t_sqltype;

     TableColumn(const string& tname, 
                 const string& ctype,
                 const string& sqltype)
                 : t_name(tname),
                   t_ctype(ctype),
                   t_sqltype(sqltype)
                 {};
  };

  FairDbWtParTreeTableNode(FairDbObjTableMap* aPar,
                           WString& pName, bool isDir);


 virtual WTable* createDataTable(WContainerWidget* aParent,string pName); 
 //virtual WTableView* createDataTableView(WContainerWidget* aParent,string pName); 
 virtual WContainerWidget* createDataTableView(WContainerWidget* aParent,string pName); 
 virtual WTable* createValTable(WContainerWidget* aParent,string pName); 
 virtual WTable* createMetaTable(WContainerWidget* aParent,string pName); 
 virtual WTable* createConnTable(WContainerWidget* aParent,string pName); 

 virtual void fillTableRows(WTable* table,string pName); 
 virtual void createTree();
 
 // <DB> dirty trick to map the par. names
 // and table names ... need to be checked.
 virtual void initParameter(string pname);

protected:

  bool expandable();
  void split(const std::string& str, std::vector<std::string>& v); 

  static Wt::WIconPair *createIcon(bool isDir);

protected:
  map<string , string> creationDate;
  map<string , string> modifiedDate;
  map<string , FairDbObjTableMap*> fMapObjects;

  FairDbConnectionPool* fConnections;  

  
};

#endif // FAIRDBWTPARTREETABLENODE_H
