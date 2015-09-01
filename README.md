FairDB
========

The **FairDB** Database interface is on Object Relational Mapping library 
(ORM) which implements a time based version management for parameter handling.


##Installation

You need to first install the FairRoot framework and then follow the 
instructions at the 
[FairRoot github page.] (https://github.com/FairRootGroup/FairRoot)

##Getting started

Tutorials how to write/read parameter classes are provided in the
[FairRoot example] (https://github.com/FairRootGroup/FairRoot/tree/master/example) 
directory. 

* [Tutorial5] (https://github.com/FairRootGroup/FairRoot/tree/master/example/Tutorial5)
shows how to use the FairDB interface to read and write parameter to any supported
database system i.e MySQL, PosGreSQL and SQLite.
Two type of parameter classes are implemented with
simple data members [FairDbTutPar](https://github.com/FairRootGroup/FairRoot/blob/master/example/Tutorial5/src/FairDbTutPar.h) 
complex data members i.e ROOT object [FairDbTutParBin.](https://github.com/FairRootGroup/FairRoot/blob/master/example/Tutorial5/src/FairDbTutParBin.h)

In both cases, corresponding macro to read and write as well as a script. To setup the database to be used (dbconfig.sh) are available from the macro directory 

* [Tutorial6] (https://github.com/FairRootGroup/FairRoot/tree/master/example/Tutorial6)
  shows how to use the **zeroMQ** front end of **FairDB**.

## FairDB Web Interface
FairDB uses the powerful [Wt framework](htpp://www.webtoolkit.eu/wt) to allow the user to
quickly develop web application.
 
![Cbm Sts Web Interface](/doc/stsqaweb.png)
Format: ![Alt Text](url) 


