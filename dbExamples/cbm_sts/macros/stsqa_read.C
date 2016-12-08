Int_t  stsqa_read()
{

  // Benchmark priming to DB
  TStopwatch t; 
  t.Start(); 
  
  // Create a Runtime Database singleton.
  FairRuntimeDb* db = FairRuntimeDb::instance();

  // Set the SQL IO as first input
  FairParTSQLIo* inp = new FairParTSQLIo();
  
  // Verbosity level
  inp->SetVerbosity(0);

  inp->open();
  db->setFirstInput(inp);

  // Create in memory the relevant container
  CbmStsDbQa* stsQa = (CbmStsDbQa*)(db->getContainer("StsDbQa"));

  // Create a dummy runID using date in UTC from which
  // corresponding  parameters will be initialised

  ValTimeStamp tStamp;
  UInt_t runId = tStamp.GetSec();
  cout << "-I- looking for parameters at runID# " << runId << endl;
  cout << "-I- corresponding  time in runID (UTC) " << tStamp.Format("iso") << endl;

  // Use the generated RunID to initialised the parameter
  // using the SQL-based IO input
  db->initContainers(runId);

  // Get the container after initialisation
  CbmStsDbQa* stsQa = (CbmStsDbQa*)(db->getContainer("StsDbQa"));

  cout << endl;
  cout << "-I- Reading STS Qa data from SQL Database: \n" << endl;
  cout << endl;

  stsQa->Print();

  cout << endl;

  t.Stop();
  t.Print();
  
  if (db) delete db;
  exit(0);
  return 0;
}
