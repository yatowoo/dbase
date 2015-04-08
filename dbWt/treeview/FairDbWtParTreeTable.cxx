#include "FairDbWtParTreeTable.h"
#include "FairDbWtParTreeTableNode.h"

#include <Wt/WText>

using namespace Wt;

FairDbWtParTreeTable::FairDbWtParTreeTable(WContainerWidget *parent)
  : WTreeTable(parent)
{
  //addColumn("Created", 80);
  addColumn("Created", 110);
  addColumn("Last Modified", 110);

  //header(1)->setStyleClass("fsize");
  header(1)->setStyleClass("date");
  header(2)->setStyleClass("date");

  WString topName="ParamList";
  setTreeRoot(new FairDbWtParTreeTableNode(0,topName, 1), "FairDb Contents");

  treeRoot()->setImagePack("icons/");
  treeRoot()->expand();
}
