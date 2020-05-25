#!/bin/bash

# Script Name: testing.sh
# Author: Erich Woo
# Date: 23 May 2020
# Testing for querier.c
#
# usage: bash -v testing.sh

# define variables
dir=~cs50/data/tse-output
unreadabledir=unreadabledir
notcrawlerdir=notcrawlerdir
unreadablefile=unreadablefile
d=dartmouth
c=college

# setup
make clean
make

mkdir $unreadabledir
mkdir $notcrawlerdir
touch $unreadablefile
chmod u-r $unreadabledir
chmod u-r $unreadablefile

###################### Error Cases ########################

echo -e "\n------------- Error Cases -------------"

############# Args Validation ###############
# incorrect number of arguments
echo -e "\n******* Testing incorrect number (1) of arguments *******"
./querier $dir/letters-depth-0 

# invalid pageDirectory
echo -e "\n****** Testing invalid pageDirectory *******"
./querier invalid/ $dir/letters-index-0

# unreadable pageDirectory
echo -e "\n****** Testing unreadable pageDirectory ******"
./querier $unreadabledir $dir/letters-index-0

# not a .crawler created pageDirectory
echo -e "\n****** Testing not-crawler-created pageDirectory ******"
./querier $notcrawlerdir $dir/letters-index-0

# invalid indexFilename path
echo -e "\n****** Testing invalid indexFilename path ******"
./querier $dir/letters-depth-0 invalid/letters-index-0

# unreadable indexFilename
echo -e "\n****** Testing unreadable indexFilename ******"
./querier $dir/letters-depth-0 $unreadablefile

############# Query Validation #################

# create testfile of error cases for piping into stdin
echo -e "\n********* Testing erroneous queries **********"
echo -e "Examples: leading/trailing 'and/or', adjacent 'and/or's, nonalphabetic characters"
echo -e "and\nor\nand or\nand $d\nor $d\n$d and\n$d or\n$d and or $c\n$d and and $c\n$d or and $c\n$d or or $c\n$d $c!\n$d $c 250\nBackus-Naur Form" > testfile
echo -e "\n~~~~~~~~~ Contents of file to pipe into stdin for querier ~~~~~~~~~"
cat testfile
echo -e "\n~~~~~~~~~~~~~~~~ Output ~~~~~~~~~~~~~~~~"
./querier $dir/wikipedia-depth-1 $dir/wikipedia-index-1 < testfile

############## Standard Cases ###############
echo -e "\n-------------- Standard Cases ------------"

############# Cases not testable by Fuzztesting ##########
echo -e "\n~~~~~~~~~~~~~~~ Cases untestable by Fuzztesting ~~~~~~~~~~~~~"

# create testfile for piping into stdin
# some tests taken from cs50 website

# Testing empty query
echo -e "\n********** Testing empty query input ***********"
echo -e "             " > testfile
echo -e "\n~~~~~~~~~ Contents of file to pipe into stdin for querier ~~~~~~~~~"
cat testfile
echo -e "\n~~~~~~~~~~~~~~~~ Output ~~~~~~~~~~~~~~~~"
./querier $dir/wikipedia-depth-1 $dir/wikipedia-index-1 < testfile

# Testing no documents matching
echo -e "\n********* Testing No-Matching-Documents ***********"
echo -e "a bunch of words that definitely flabbergastinglycreatedword will not have a match for sureyduryrandomness" > testfile
echo -e "\n~~~~~~~~~ Contents of file to pipe into stdin for querier ~~~~~~~~~"
cat testfile
echo -e "\n~~~~~~~~~~~~~~~~ Output ~~~~~~~~~~~~~~~~"
./querier $dir/wikipedia-depth-1 $dir/wikipedia-index-1 < testfile

# variable output same expected output
echo -e "\n***** Testing variable output with same expected output ******"
echo -e "Examples: capitalization, implicit 'and', extra whitespace"
echo -e "Dartmouth College\n$d $c\n$d and $c\n      $d      AND     College      " > testfile
echo -e "\n~~~~~~~~~ Contents of file to pipe into stdin for querier ~~~~~~~~~"
cat testfile
echo -e "\n~~~~~~~~~~~~~~~~ Output ~~~~~~~~~~~~~~~~"
./querier $dir/wikipedia-depth-1 $dir/wikipedia-index-1 < testfile

# Regression Testing with expected output from cs50 website
echo -e "\n***** Regression Testing with expected output from Requirements Spec *****"
echo -e "querying 'dartmouth or computer science or programming or unix' and comparing with output from REQUIREMENTS.md (lines 143-152) for wikipedia-depth-1 (I suspect)"

echo -e "dartmouth or computer science or programming or unix" > testfile
./querier $dir/wikipedia-depth-1 $dir/wikipedia-index-1 < testfile > actual

sed -n "143,152p" REQUIREMENTS.md > expected
echo -e "\n~~~~~~~~~~ Results of 'diff' command ~~~~~~~~~~~"
diff -s actual expected
echo -e "\nMy URL (first or the two) is actually correct, and expected output is wrong!!!! I checked $dir/wikipedia-depth-1/1 and the URL is actually (via 'head -1' command):"
head -1 $dir/wikipedia-depth-1/1
echo -e "\nAnd the '11d10' is because of the newline in my output, but for the expected output from REQUIREMENTS.md, I cut off at the line of dashes (since there was more error tests that would mess up). I didn't include the other error tests from REQUIREMENTS.md because of the ['Query?' prompt when stdin is a tty/keyboard] problem, as well as different implementation methods of error-checking leading to slightly different output."

echo -e "~~~~~~~~~~~ End of Manual Standard-Case Testing ~~~~~~~~~~~~~"

############ Fuzztesting ##############
echo -e "\n~~~~~~~~~~~~~~~~~ Fuzztesting ~~~~~~~~~~~~~~~~~~~~"

# Fuzztesting on letters-index-3
echo -e "\n***** Fuzztesting with same files & seed(0) as lecture notes *****"
echo -e "Can't perform Regression testing since link is unaccessible from ~cs50, so just visually compare it!"
echo -e "\n~~~~~~~~~~~~~~~~~~~ Output ~~~~~~~~~~~~~~~~~~~~"
./fuzzquery $dir/letters-index-3 10 0 | ./querier $dir/letters-depth-3 $dir/letters-index-3

# Fuzztesting on wikipedia-index-1
echo -e "\n****** Fuzztesting on wikipedia-index-1 **********"
echo -e "\n~~~~~~~~~~~~~~~~~~~ Output ~~~~~~~~~~~~~~~~~~~~"
./fuzzquery $dir/wikipedia-index-1 10 0 | ./querier $dir/wikipedia-depth-1 $dir/wikipedia-index-1

echo -e "\n---------------- Testing Complete -----------------"

########### Cleanup #############
rm -rf $unreadabledir $notcrawlerdir $unreadablefile testfile actual expected
