Int_t stsqa_prime(){

  // Benchmark priming to DB
  TStopwatch t; 
  t.Start(); 

  
  TString InputDir = gSystem->Getenv("PWD");
  InputDir+= "/../batch_data/c4-before-dicing/";
  TString InputFiles[] =    
   {
     "batch-overview-331140.csv",
     "batch-overview-351135.csv",
     "batch-overview-351139.csv",
     "batch-overview-351141.csv",
     "batch-overview-351142.csv",
     "batch-overview-351152.csv",
   }


  
  // Generate a unique RunID (corresponding to execute time)
  FairRunIdGenerator runID;
  UInt_t runId =  runID.generateId();

  // Create the Runtime Database ( parameter manager class )
  FairRuntimeDb* db = FairRuntimeDb::instance();

  // Create in memory the relevant container
  CbmStsDbQa* stsQa = (CbmStsDbQa*)(db->getContainer("StsDbQa"));

  // Set the SQL based IO as second input
  FairParTSQLIo* input_db = new FairParTSQLIo();  
  input_db->SetVerbosity(0);

  // Set Global SeqNo ( Replication Global Index Base )
  //inp2->SetGlobalSeqNoIn();

  // Shutdown Mode ( True, False )
  input_db->SetShutdown(kTRUE);

  // Open first input
  input_db->open();
  db->setFirstInput(input_db);

  // Set the output=input
  db->setOutput(input_db); 


  // Sensor - Overview 
  for (Int_t i=0; i<6; i++ ){
    TString InputFile = InputDir + InputFiles[i];
    cout << "-I- processing file: " << InputFile << endl;

    // Import and Generate UID for Qa components 
    stsQa->GenerateSUIDFromCsvFile(InputFile.Data());

    // Import Sensors data
    stsQa->ImportFromCsvFile(InputFile.Data());

  }

  // Sensor - iv
  stsQa->ImportIvFilesFromDir(InputDir.Data());

  // Trigger change 
  stsQa->setChanged(kTRUE);

  // Write just added container
  db->addRun(runId,-1);
  db->writeContainer(stsQa, db->getCurrentRun());
  
  // FairRuntimeDB deletion
  if (db) delete db;

  t.Stop();
  t.Print();
  
  return 0;
}
 
