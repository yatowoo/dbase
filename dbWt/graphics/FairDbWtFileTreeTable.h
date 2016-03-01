#ifndef FAIRDBWTFILETREETABLE
#define FAIRDBWTFILETREETABLE

#include <Wt/WTreeTable>

#include <boost/filesystem/path.hpp>
#include <map>

using namespace std;
namespace fs = ::boost::filesystem;


class FairDbWtFileTreeTable : public Wt::WTreeTable
{
public:
  FairDbWtFileTreeTable(const boost::filesystem::path& path,
		Wt::WContainerWidget *parent = 0);

 const string& getFullPathName( const string& filename)
 {return fMapPaths[filename].string();}

 const fs::path& getFullPath( const string& filename)
 {return fMapPaths[filename];}

void  registerFile(const string& fname, const fs::path& a_path)
 {fMapPaths[fname] = a_path;}

private:
  fs::path fPath;
  map<string, fs::path > fMapPaths;
};

#endif // FAIRDBWTFILETREETABLE
