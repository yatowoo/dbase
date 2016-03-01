#include "FairDbWtFileTreeTableNode.h"

#include "FairDbWtTFileNode.h"
#include "FairDbWtFileTreeTable.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <time.h>

#include <Wt/WIconPair>
#include <Wt/WStringUtil>
#include <Wt/WText>

using namespace Wt;
namespace fs = ::boost::filesystem;

FairDbWtFileTreeTableNode::FairDbWtFileTreeTableNode(const boost::filesystem::path& path)
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
{
  cout << "-I- FairDbWtFileTreeTableNode created with name:" << path.leaf().string() << endl;
  label()->setTextFormat(PlainText);

  if (boost::filesystem::exists(path)) {
    if (!boost::filesystem::is_directory(path)) {
      int fsize = (int)boost::filesystem::file_size(path);
      // DB comment size
      //setColumnWidget(1, new WText(boost::lexical_cast<std::string>(fsize)));
      //columnWidget(1)->setStyleClass("fsize");
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

    // Db Comment also times
    //setColumnWidget(2, new WText(c));
    //columnWidget(2)->setStyleClass("date");

  }
}

WIconPair *FairDbWtFileTreeTableNode::createIcon(const boost::filesystem::path& path)
{
  if (boost::filesystem::exists(path)
      && boost::filesystem::is_directory(path))
    return new WIconPair("icons/yellow-folder-closed.png",
			 "icons/yellow-folder-open.png", false);
  else
    return new WIconPair("icons/document.png",
			 "icons/yellow-folder-open.png", false);
}

void FairDbWtFileTreeTableNode::populate()
{
std::cout << "-I- populate get: " << path_ << ":" << path_.string() << std::endl;

  if (boost::filesystem::is_directory(path_)) {
    std::set<boost::filesystem::path> paths;
    boost::filesystem::directory_iterator end_itr;

    for (boost::filesystem::directory_iterator i(path_); i != end_itr; ++i)
    {
      try {
        // Here create a filter for Root filesystem_error
        //std::cout << "-I- select only  file" << *i << std::endl;
        if(fs::is_regular_file(*i) && i->path().extension() == ".root"){
          std::cout << "-I- select only root file" << *i << ":" << i->path().filename() << std::endl;
          //paths.insert(*i);
          const string r_filename = i->path().filename().string();
          addChildNode( new FairDbWtTFileNode(i->path()));
        // Store the Contents of a File
        FairDbWtFileTreeTable* c_table = static_cast<FairDbWtFileTreeTable*> ( table() );
        c_table->registerFile(i->path().filename().string(), i->path());

        }else if (fs::is_directory(*i)){
          // still put back the directory for recursion
          paths.insert(*i);
        }
      } catch (boost::filesystem::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
      }
    }

    for (std::set<boost::filesystem::path>::iterator i = paths.begin();
    i != paths.end(); ++i)
    try {
      addChildNode(new FairDbWtFileTreeTableNode(*i));
    } catch (boost::filesystem::filesystem_error& e) {
      std::cerr << e.what() << std::endl;
    }
  }
}

bool FairDbWtFileTreeTableNode::expandable()
{
  if (!populated()) {
    return boost::filesystem::is_directory(path_);
  } else
    return WTreeTableNode::expandable();
}
