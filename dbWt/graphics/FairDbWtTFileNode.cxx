#include "FairDbWtTFileNode.h"
#include "FairDbWtFileTreeTable.h"
#include <Wt/WIconPair>
#include <Wt/WStringUtil>
#include <Wt/WText>
#include <Wt/WTree>

#include "TFile.h"
#include "TKey.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <time.h>



using namespace Wt;
namespace fs = ::boost::filesystem;


const char *FairDbWtTFileNode::FileSelectionMimeType
  = "application/x-computers-selection";




FairDbWtTFileNode::FairDbWtTFileNode(const string& name)
 : WTreeTableNode(name, createIcon(name))
 , fName(name)
 , fTName("")
 , fIsLeaf(false)
 {
   cout << "-I- FairDbWTFileNode created with name:" << name << endl;
   label()->setTextFormat(PlainText);

   label()->setDraggable(FileSelectionMimeType,(WWidget*) labelIcon()->icon1(), true, this);

   // connect evt handling
   expanded().connect(this, &FairDbWtTFileNode::isExpanded);
   collapsed().connect(this, &FairDbWtTFileNode::isCollapsed);

 }



FairDbWtTFileNode::FairDbWtTFileNode(const string& name, const string& c_name,
                                     const string& tname, bool leaf)
 : WTreeTableNode(name, createIcon(c_name))
 , fName(name)
 , fTName(tname)
 , fIsLeaf(leaf)
 {
   cout << "-I- FairDbWTFileNode created with name:" << name << " : "
   << c_name << endl;
   label()->setTextFormat(PlainText);
   label()->setDraggable(FileSelectionMimeType,(WWidget*) labelIcon()->icon1(), true, this);

   // connect evt handling
   expanded().connect(this, &FairDbWtTFileNode::isExpanded);
   collapsed().connect(this, &FairDbWtTFileNode::isCollapsed);

 }


FairDbWtTFileNode::FairDbWtTFileNode(const boost::filesystem::path& path)
#if BOOST_FILESYSTEM_VERSION < 3
#ifndef WT_NO_STD_WSTRING
: WTreeTableNode(Wt::widen(path.leaf()), createIcon(path)),
#else
: WTreeTableNode(path.leaf(), createIcon(path)),
#endif
#else
: WTreeTableNode(path.leaf().string(), createIcon(path)),
#endif
path_(path)
, fName("")
, fTName("")
, fIsLeaf(false)
{
  cout << "-I- FairDbWtTFileNode created with path name:" << path.leaf().string() << endl;
  label()->setTextFormat(PlainText);
  label()->setDraggable(FileSelectionMimeType,(WWidget*) labelIcon()->icon1(), true);

  if (boost::filesystem::exists(path)) {
    if (!boost::filesystem::is_directory(path)) {
      int fsize = (int)boost::filesystem::file_size(path);
    } else
    setSelectable(false);

    std::time_t t = boost::filesystem::last_write_time(path);
    struct tm ttm;
    #if WIN32
    ttm=*localtime(&t);
    #else
    localtime_r(&t, &ttm);
    #endif
    char c[100];
    strftime(c, 100, "%b %d %Y", &ttm);
  }

  // connect evt handling
  expanded().connect(this, &FairDbWtTFileNode::isExpanded);
  collapsed().connect(this, &FairDbWtTFileNode::isCollapsed);
  selected().connect(this, &FairDbWtTFileNode::isSelected);
}


WIconPair *FairDbWtTFileNode::createIcon(const string& name)
{
  cout << "-I- FairDbWtTFileNode createIcon with name: " << name << endl;
  std::string th1("TH1");
  std::string tgraf("TGraph");
  std::string tcanvas("TCanvas");
  std::string ttree("TTree");
  std::string tntuple("TNtuple");
  std::string tleaf("TLeaf");

  if (name.find(th1) != string::npos) {
    return new WIconPair("icons/h1_t.png","icons/h1_t.png", false);
  }else if (name.find(tgraf)   != string::npos) {
    return new WIconPair("icons/h1_t.png","icons/h1_t.png", false);
  }else if (name.find(ttree)   != string::npos) {
    return new WIconPair("icons/tree_s.png","icons/tree_s.png", false);
  }else if (name.find(tntuple)   != string::npos) {
    return new WIconPair("icons/tree_t.png","icons/tree_t.png", false);
  }else if (name.find(tleaf)   != string::npos) {
    return new WIconPair("icons/leaf_t.png","icons/leaf_t.png", false);
  }else if (name.find(tcanvas) != string::npos) {
    return new WIconPair("icons/document.png",
    "icons/yellow-folder-open.png", false);
  }
  // default?
  return new WIconPair("icons/h1_t.png","icons/h1_t.png", false);
}

WIconPair *FairDbWtTFileNode::createIcon(const boost::filesystem::path& path)
{
  if (boost::filesystem::exists(path)
      && boost::filesystem::is_directory(path))
    return new WIconPair("icons/yellow-folder-closed.png",
			 "icons/yellow-folder-open.png", false);
  else
    return new WIconPair("icons/document.png",
			 "icons/yellow-folder-open.png", false);
}

void FairDbWtTFileNode::populate()
{
  // Here will the node analysed in terms of Root Objects
  std::cout << "-I- FairDbWtTFileNode populate:  path:"
            <<  path_.string() << " : " << "filename:" << path_.filename() << std::endl;

  // Does path actually exist?
  if (fs::exists(path_))
  {
    // is p a regular file?
    if (fs::is_regular_file(path_))
    cout << path_ << " size is " << fs::file_size(path_) << '\n';
    // is p a directory?
    else if (fs::is_directory(path_))
    cout << path_ << "is a directory\n";
    else
    cout << path_ << "exists, but is neither a regular file nor a directory\n";
  }
  else
    cout << path_ << "does not exist .. look up file\n";


}

void FairDbWtTFileNode::isSelected(){
  cout << "-I- FairDbWtTFileNode is selected  ! fname:" << path_.string() << endl;
  // Get the tree
  WTree* aTree = tree();

  for (aWTreeNodeSet::const_iterator i = aTree->selectedNodes().begin();
  i != aTree->selectedNodes().end(); ++i){

    WTreeNode *aNode=(*i);
    // Get label
    const string pName= aNode->label()->text().toUTF8();
    // DB Check if Single-Composite node
    if (aNode->displayedChildCount()>0 ) continue;

    cout << "-I- FairDbWtTFileNode: selected node"
    <<  aNode->label()->text().value()
    << " fullpath: "  << aNode->label()->text().narrow() << endl;

  }

}


void FairDbWtTFileNode::isExpanded(){
 cout << "-I- FairDbWtTFileNode is expanded  ! fname:" << path_.string() << endl;
}

void FairDbWtTFileNode::isCollapsed(){
 cout << "-I- FairDbWtTFileNode is collapsed  ! fname:" << path_.string() << endl;
}

bool FairDbWtTFileNode::expandable()
{
  if (!populated()) {
    return boost::filesystem::is_directory(path_);
  } else
    return WTreeTableNode::expandable();
}
