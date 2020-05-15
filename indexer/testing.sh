#!/bin/bash
# Script Name: testing.sh
# Author: Erich Woo
# Date: 14 May 2020
# Testing for indexer.c and indextest.c
#
# usage: bash -v testing.sh

# define variables
dir=~cs50/data/tse-output
testdir=output
sorteddir=sorteddata
sortedtestdir=sortedoutput
unreadabledir=unreadabledir
unwriteabledir=unwriteabledir
unwriteablefile=unwriteablefile
echo -e "Directory of crawler output comes from: $dir\n"

# setup
mkdir $testdir
mkdir $sorteddir
mkdir $sortedtestdir
mkdir $unreadabledir
mkdir $unwriteabledir
touch $unwriteablefile
chmod u-r $unreadabledir
chmod u-w $unwriteabledir
chmod u-w $unwriteablefile
echo -e "index and indextest files will be written to '$testdir/'. Sorted index and indextest files will be written to '$sortedtestdir/'. unreadable directory '$unreadabledir/' created. unwriteable directory '$unwriteabledir/' created. unwriteable file '$unwriteablefile' created.\n"

################# INDEXER.C ###################
echo -e "\n~~~~~~~~~~~~~~~~~~TESTING FOR INDEXER.C STARTING~~~~~~~~~~~~~~~~~~\n"

################# Error Cases #################
echo -e "\n------------- Error Cases -------------"

# incorrect number of arguments
echo -e "\n******* Testing incorrect number (1) of arguments *******"
./indexer $dir/letters-depth-5 

# invalid pageDirectory
echo -e "\n****** Testing invalid pageDirectory *******"
./indexer invalid/ $testdir/letters-index-0

# unreadable pageDirectory
echo -e "\n****** Testing unreadable pageDirectory ******"
./indexer $unreadabledir $testdir/letters-index-0

# invalid indexFilename path
echo -e "\n****** Testing invalid indexFilename path ******"
./indexer $dir/letters-depth-0 invalid/letters-index-0

# unwriteable indexFilename path bc of directory
echo -e "\n***** Testing unwriteable indexFilename path 1 *****"
./indexer $dir/letters-depth-0 $unwriteabledir/letters-index-0

# unwriteable indexFilename path bc of file
echo -e "\n***** Testing unwriteable indexFilename path 2 *****"
./indexer $dir/letters-depth-0 $unwriteablefile

############### Standard Cases ################
echo -e "\n--------- Standard Cases ---------"

# testing on letters-depth 0-5 (6 is identical to 5)
echo -e "\n\n^^^^^^^^^^^ Testing on 'letters' directory ^^^^^^^^^^^^"
for i in 0 1 2 3 4 5
do
    echo -e "\n********** Indexing on letters-depth-$i ***********"
    ./indexer $dir/letters-depth-$i $testdir/letters-index-$i
    echo -e "sorting '$dir/letters-index-$i' into .sorted file in '$sorteddir/'"
    gawk -f indexsort.awk $dir/letters-index-$i > $sorteddir/letters-index-$i.sorted
    echo -e "sorting '$testdir/letters-index-$i' into .sorted file in '$sortedtestdir/'"
    gawk -f indexsort.awk $testdir/letters-index-$i > $sortedtestdir/letters-index-$i.sorted
    echo -e "comparing '$sorteddir/letters-index-$i.sorted' with '$sortedtestdir/letters-index-$i.sorted'..."
    echo -e "----------- Output of 'diff' command -------------"
    diff -s $sorteddir/letters-index-$i.sorted $sortedtestdir/letters-index-$i.sorted
done

# testing on toscrape-depth 0-2
echo -e "\n\n^^^^^^^^^^^ Testing on 'toscrape' directory ^^^^^^^^^^^^"
for i in 0 1 2
do
    echo -e "\n********** Indexing on toscrape-depth-$i ***********"
    ./indexer $dir/toscrape-depth-$i $testdir/toscrape-index-$i
    echo -e "sorting '$dir/toscrape-index-$i' into .sorted file in '$sorteddir/'"
    gawk -f indexsort.awk $dir/toscrape-index-$i > $sorteddir/toscrape-index-$i.sorted
    echo -e "sorting '$testdir/toscrape-index-$i' into .sorted file in '$sortedtestdir/'"
    gawk -f indexsort.awk $testdir/toscrape-index-$i > $sortedtestdir/toscrape-index-$i.sorted
    echo -e "comparing '$sorteddir/toscrape-index-$i.sorted' with '$sortedtestdir/toscrape-index-$i.sorted'..."
    echo -e "------------ Output of 'diff' command -------------"
    diff -s $sorteddir/toscrape-index-$i.sorted $sortedtestdir/toscrape-index-$i.sorted
done

echo -e "\n~~~~~~~~~~~~~~~~~TESTING FOR INDEXER.C COMPLETE~~~~~~~~~~~~~~~~~~\n"

################# INDEXTEST.C ###################
echo -e "\n~~~~~~~~~~~~~~~~~TESTING FOR INDEXTEST.C STARTING ~~~~~~~~~~~~~~~~~~~\n"

################# Error Cases #################
echo -e "\n------------- Error Cases -------------"

# incorrect number of arguments
echo -e "\n******* Testing incorrect number (1) of arguments *******"
./indextest $testdir/letters-index-0

# non-existent oldIndexFilename
echo -e "\n****** Testing non-existent oldIndexFilename *******"
./indextest invalid/letters-index-0 $testdir/letters-test-0

# unreadable oldIndexFilename
echo -e "\n****** Testing unreadable oldIndexFilename ******"
echo -e "changing $testdir/letters-index-0 to unreadable..."
chmod u-r $testdir/letters-index-0
./indextest $testdir/letters-index-0 $testdir/letters-test-0
echo -e "\nchanging $testdir/letters-index-0 back to readable..."
chmod u+r $testdir/letters-index-0

# invalid newIndexFilename path
echo -e "\n****** Testing invalid newIndexFilename path ******"
./indextest $testdir/letters-index-0 invalid/letters-test-0

# unwriteable newIndexFilename path bc of directory
echo -e "\n***** Testing unwriteable newIndexFilename path 1 *****"
./indextest $testdir/letters-index-0 $unwriteabledir/letters-test-0

# unwriteable newIndexFilename path bc of file
echo -e "\n***** Testing unwriteable newIndexFilename path 2 *****"
./indextest $testdir/letters-index-0 $unwriteablefile

############### Standard Cases ################
echo -e "\n--------- Standard Cases ---------"

# testing on letters-index 0-5
echo -e "\n\n^^^^^^^^^^^ Testing on 'letters' index ^^^^^^^^^^^^"
for i in 0 1 2 3 4 5
do
    echo -e "\n********** IndexTest on letters-index-$i  ***********"
    ./indextest $sortedtestdir/letters-index-$i.sorted $testdir/letters-test-$i
    echo -e "sorting $testdir/letters-test-$i into .sorted file in '$sortedtestdir/'"
    gawk -f indexsort.awk $testdir/letters-test-$i > $sortedtestdir/letters-test-$i.sorted
    echo -e "comparing '$sortedtestdir/letters-index-$i.sorted' with '$sortedtestdir/letters-test-$i.sorted'..."
    echo -e "-------------- Output of 'diff' command ------------------"
    diff -s $sortedtestdir/letters-index-$i.sorted $sortedtestdir/letters-test-$i.sorted
done

# testing on toscrape-index 0-5
echo -e "\n\n^^^^^^^^^^^ Testing on 'toscrape' index ^^^^^^^^^^^^"
for i in 0 1 2
do
    echo -e "\n********** IndexTest on toscrape-index-$i  ***********"
    ./indextest $sortedtestdir/toscrape-index-$i.sorted $testdir/toscrape-test-$i
    echo -e "sorting $testdir/toscrape-test-$i into .sorted file in '$sortedtestdir/'"
    gawk -f indexsort.awk $testdir/toscrape-test-$i > $sortedtestdir/toscrape-test-$i.sorted
    echo -e "comparing '$sortedtestdir/toscrape-index-$i.sorted' with '$sortedtestdir/toscrape-test-$i.sorted'..."
    echo -e "-------------- Output of 'diff' command ------------------"
    diff -s $sortedtestdir/toscrape-index-$i.sorted $sortedtestdir/toscrape-test-$i.sorted
done

echo -e "\n~~~~~~~~~~~~~~~~~~TESTING FOR INDEXTEST.C COMPLETE~~~~~~~~~~~~~~~~~\n"

################## Cleanup ####################

rm -rf $testdir $unreadabledir $unwriteabledir $sorteddir $sortedtestdir $unwriteablefile
