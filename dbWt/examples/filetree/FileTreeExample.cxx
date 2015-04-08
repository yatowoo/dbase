/*
 * Copyright (C) 2008 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WTree>
#include <Wt/WTreeTableNode>
 #include <WServer>

#include "FileTreeTable.h"

using namespace Wt;

WApplication *createApplication(const WEnvironment& env)
{
  WApplication *app = new WApplication(env);
  app->setTitle("File explorer example");
  app->useStyleSheet("filetree.css");

  FileTreeTable *treeTable = new FileTreeTable(".");
  treeTable->resize(500, 300);
  treeTable->tree()->setSelectionMode(ExtendedSelection);
  treeTable->treeRoot()->setNodeVisible(false);
  treeTable->treeRoot()->setChildCountPolicy(WTreeNode::Enabled);

  app->root()->addWidget(treeTable);

  return app;
}

int main(int argc, char **argv)
{

   const char * const v[9] =
  {
    argv[0],
    "--docroot", "/Users/denis/fairroot/fairbase/dbase/dbWt/docroot",
    "--approot", "/Users/denis/fairroot/fairbase/dbase/dbWt/approot",
    "--http-port", "5564",
    "--http-address", "127.0.0.1"
  };

    try {
      WServer server(9, const_cast<char**>(v), WTHTTP_CONFIGURATION);

      server.addEntryPoint(Application, createApplication);

      server.run();

    } catch (WServer::Exception& e) {
      std::cerr << e.what() << std::endl;

    } catch (std::exception &e) {
      std::cerr << "-E- exception: " << e.what() << std::endl;
    } 



  // return WRun(argc, argv, &createApplication);
}

