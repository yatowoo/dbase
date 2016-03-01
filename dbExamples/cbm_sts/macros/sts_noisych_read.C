Int_t  sts_noisych_read()
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
  CbmStsDbSInfoPar* stsPar = (CbmStsDbSInfoPar*)(db->getContainer("StsDbSInfoPar"));

  // Create a dummy runID using date in UTC from which
  // corresponding  parameters will be initialised

  ValTimeStamp tStamp(2016,03,10,08,00,00);
  UInt_t runId = tStamp.GetSec();
  cout << "-I- looking for parameters at runID# " << runId << endl;
  cout << "-I- corresponding  time in runID (UTC) " << tStamp.Format("iso") << endl;

  // Use the generated RunID to initialised the parameter
  // using the SQL-based IO input
  db->initContainers(runId);

  // Get back the container after initialisation
  CbmStsDbSInfoPar* stsPar = (CbmStsDbSInfoPar*)(db->getContainer("StsDbSInfoPar"));

  cout << endl;
  cout << "-I- Reading Sts Noisy Channel from SQL Database: \n" << endl;
  cout << endl;

  stsPar->Print();

  cout << endl;
 
  if (db) delete db;
  return 0;
}
