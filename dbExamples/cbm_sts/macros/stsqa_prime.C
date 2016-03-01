Int_t  stsqa_prime()
{

  // Generate a unique RunID
  FairRunIdGenerator runID;
  UInt_t runId =  runID.generateId();

  // Create the Runtime Database ( parameter manager class )
  FairRuntimeDb* db = FairRuntimeDb::instance();

  // Create in memory the relevant container
  CbmStsDbQaPar* stsPar = (CbmStsDbQaNewPar*)(db->getContainer("CbmStsDbQaPar"));

  // Set the SQL based IO as second input
  FairParTSQLIo* input_db = new FairParTSQLIo();  
  input_db->SetVerbosity(1);

  // Set Global SeqNo ( Replication Global Index Base )
  //inp2->SetGlobalSeqNoIn();

  // Shutdown Mode ( True, False )
  input_db->SetShutdown(kTRUE);


  // Open first input
  input_db->open();
  db->setFirstInput(input_db);

  // Set the output=input
  db->setOutput(input_db); 

  cout << "-I- STS QA New Paremeters Priming:  Opening database + Meta Data done via TSQLIO .. " << endl;
 

  // First import Csv Model file
  stsPar->ImportFromCsvFile("../data/sensor-table.csv");

  // Read the I:V tables in data dir.
  stsPar->ImportIvFilesFromDir();
  
  // Tell RuntimeDb Container has changed
  stsPar->setChanged(kTRUE); 

  cout << "  Sensor  # " <<  stsPar->GetNumSensors() 
       << "  IvMeasu # " <<  stsPar->GetNumIvMeas() << endl; 

  
  // Write just added container
  db->addRun(runId,-1);
  db->writeContainer(stsPar, db->getCurrentRun());
  
  // FairRuntimeDB deletion
  if (db) delete db;
  return 0;
}
