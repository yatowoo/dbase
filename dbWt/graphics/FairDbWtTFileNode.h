#ifndef FAIRDBTFILENODE
#define FAIRDBTFILENODE

#include <Wt/WTreeTableNode>
#include <map>
#include <set>
#include <boost/filesystem/path.hpp>

using namespace std;

class FairDbWtTFileNode : public Wt::WTreeTableNode
{
public:
  FairDbWtTFileNode(const string& name);
  FairDbWtTFileNode(const string& name, const string& c_name,
                    const string& tname="", bool leaf=false);
  FairDbWtTFileNode(const boost::filesystem::path& path);

  void setPath(boost::filesystem::path p){path_= p;}

  const boost::filesystem::path& getPath(){return path_;}
  const string& getFileName(){return fName;}
  const string& getTreeName(){return fTName;}
  bool isLeaf() {return fIsLeaf;}

  // Mime type definition
  static const char *FileSelectionMimeType;

private:
  boost::filesystem::path path_;
  const string fName;
  const string fTName;
  bool fIsLeaf;

  virtual void populate();
  virtual bool expandable();
  virtual void isExpanded();
  virtual void isCollapsed();
  virtual void isSelected();

  typedef set<WTreeNode*> aWTreeNodeSet;
  static Wt::WIconPair *createIcon(const boost::filesystem::path& path);
  static Wt::WIconPair *createIcon(const string& cname);

};

#endif // FAIRDBWTFILENODE
