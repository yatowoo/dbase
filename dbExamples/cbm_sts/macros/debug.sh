#!/bin/sh -
#
# Shell script for debug on student@CBMPC041.gsi.de
#

CURRENT_DIR=$PWD
cd $FAIRROOT_BUILD
make install -j8
. $FAIRROOTPATH/bin/FairRootConfig.sh
cd $CURRENT_DIR
rm -v *.db
root -l $1
unset CURRENT_DIR
