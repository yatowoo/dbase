Int_t  stsqan_prime()
{

  // Generate a unique RunID
  FairRunIdGenerator runID;
  UInt_t runId =  runID.generateId();

  // Create the Runtime Database ( parameter manager class )
  FairRuntimeDb* db = FairRuntimeDb::instance();

  // Create in memory the relevant container
  CbmStsDbQaNewPar* stsPar = (CbmStsDbQaNewPar*)(db->getContainer("StsQaPar"));

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
  // Sensor summary
  stsPar->ImportFromCsvFile("../data/new/summary.csv");
  // Add Channel Map
  stsPar->ImportFromCsvFile("../data/new/channel_map.csv");
  // Add Geometry  
  stsPar->ImportFromCsvFile("../data/new/geometry.csv");
  // Add Ownership
  stsPar->ImportFromCsvFile("../data/new/ownership.csv");
  // Add Results
  stsPar->ImportFromCsvFile("../data/new/qa_results.csv"); 

  // Read the I:V tables in the data directory
  stsPar->ImportIvFilesFromDir();
  
  // Tell RuntimeDb Container has changed
  stsPar->setChanged(kTRUE); 

  cout << "  Sensors  # " <<  stsPar->GetNumSensors() 
       << "  IvMeasu # " <<  stsPar->GetNumIvMeas() 
       << "  ChannelMap # " <<  stsPar->GetNumSChannelMap()
       << "  Geometry # " <<  stsPar->GetNumSGeometry()
       << "  Ownership # " <<  stsPar->GetNumSOwnership()
       << "  Results # " <<  stsPar->GetNumSResults()
       << endl; 

  
  // Write just added container
  db->addRun(runId,-1);
  db->writeContainer(stsPar, db->getCurrentRun());
  
  // FairRuntimeDB deletion
  if (db) delete db;
  return 0;
}
