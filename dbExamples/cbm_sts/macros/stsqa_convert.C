Int_t  stsqa_convert()
{
  // Create a Runtime Database singleton.
  FairRuntimeDb* db = FairRuntimeDb::instance();

  // Set the SQL IO as first input
  FairParTSQLIo* inp = new FairParTSQLIo();
  
  // Verbosity level
  inp->SetVerbosity(1);

  inp->open();
  db->setFirstInput(inp);

  // Create in memory the relevant container
  CbmStsDbQa* stsPar = (CbmStsDbQa*)(db->getContainer("StsDbQa"));

  // Create a dummy runID using date in UTC from which
  // corresponding  parameters will be initialised

  ValTimeStamp tStamp;
  UInt_t runId = tStamp.GetSec();
  cout << "-I- looking for parameters at runID# " << runId << endl;
  cout << "-I- corresponding  time in runID (UTC) " << tStamp.Format("iso") << endl;

  // Use the generated RunID to initialised the parameter
  // using the SQL-based IO input
  db->initContainers(runId);

  // Get back the container after initialisation
  CbmStsDbQa* stsPar = (CbmStsDbQa*)(db->getContainer("StsDbQa"));

  // Dump the Parameters
  cout << endl;
  cout << "-I- Reading Parameter data from SQL Database: \n" << endl;
  cout << endl;

  stsPar->Print();
  cout << endl;

  // Convert in ROOT format 
  stsPar->setChanged();
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open("stsqa.root");
  db->setOutput(parOut);
  db->saveOutput();
  db->print();
  // ------------------------------------------------------------------------

  if (db) delete db;
  return 0;
}
