/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
{
  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* fRun= new FairRunAna();
  fRun->SetInputFile("dummysim.root");
  fRun->SetOutputFile("dummyana.root");

  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();

  FairParRootFileIo* parInput1 = new FairParRootFileIo();
  parInput1->open("par.root","READ");// or RECREATE etc. like in TFile from root
  rtdb->setFirstInput(parInput1);

  // Set the SQL IO as second input
  FairParTSQLIo* inp = new FairParTSQLIo();
  inp->open();
  rtdb->setSecondInput(inp);

  // -----   Tasks to run   -------------------------------------------------
  fRun->AddTask(new FairDbTutAccessRtdbTask());

  // -----   Intialise and run   --------------------------------------------
  fRun->Init();

  rtdb->print();

  fRun->Run();

}
