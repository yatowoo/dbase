#include "FairDbWtFileTreeTable.h"
#include "FairDbWtFileTreeTableNode.h"

#include <Wt/WText>
#include <Wt/WTree>

using namespace Wt;

FairDbWtFileTreeTable::FairDbWtFileTreeTable(const boost::filesystem::path& path,
			     WContainerWidget *parent)
  : WTreeTable(parent)
{
  fPath = path;
  setTreeRoot(new FairDbWtFileTreeTableNode(path), "Contents:");
  treeRoot()->setImagePack("icons/");
  treeRoot()->expand();
	tree()->setSelectionMode(Wt::SingleSelection);
}
