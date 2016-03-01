#ifndef FAIRDBFILETREETABLENODE
#define FAIRDBFILETREETABLENODE

#include <Wt/WTreeTableNode>
#include <map>
#include <boost/filesystem/path.hpp>

using namespace std;

class FairDbWtFileTreeTableNode : public Wt::WTreeTableNode
{
public:
  FairDbWtFileTreeTableNode(const boost::filesystem::path& path);

private:
  boost::filesystem::path path_;
  virtual void populate();
  virtual bool expandable();
  static Wt::WIconPair *createIcon(const boost::filesystem::path& path);

};

#endif // FAIRDBWTFILETREETABLENODE
