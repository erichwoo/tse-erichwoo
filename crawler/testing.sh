#!/bin/bash

# Script Name: testing.sh
# Author: Erich Woo
# Date: 7 May 2020
# Testing for crawler.c
#
# usage: bash -v testing.sh

#Define variables
seedURL=http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
externalURL=www.google.com
nonexistentURL=gotcha
echo -e "seedURL is: '$seedURL'\nexternalURL is: '$externalURL'\nand nonexistentURL is: '$nonexistentURL'\n"

#setup crawler directory for testing
echo -e "setting up crawler directory for testing..."
make clean
make

#create directories for testing
echo -e "\ncreating tse-output/ and unwritable/ and changing permissions for unwritable/..."
mkdir tse-output
mkdir unwritable
chmod u-w unwritable

############## Error cases ###############
echo -e "\n----------Error Cases Begin----------"

# incorrect number of arguments
echo -e "\n*********** Testing incorrect number of arguments (2) **********"
./crawler $seedURL tse-output

# external url
echo -e "\n*********** Testing external URL '$externalURL' ************"
./crawler $externalURL tse-output 0

# nonexistent url
echo -e "\n*********** Testing nonexistent URL '$nonexistentURL' ***********"
./crawler $nonexistentURL tse-output 0

# valid server but nonexistent page
echo -e "\n*** Testing valid server but nonexistent page '$seedURL/bogus.html' ****"
./crawler $seedURL/bogus.html tse-output 0

# not a directory
echo -e "\n******** Testing a non-existent directory 'test' *********"
./crawler $seedURL test 0

# unwritable directory
echo -e "\n******* Testing unwritable directory 'unwritable' ********"
./crawler $seedURL unwritable 0

# negative maxDepth
echo -e "\n******* Testing a negative maxDepth (-1) ********"
./crawler $seedURL tse-output -1

############### Standard cases ################

echo -e "\n-------Standard Cases Begin---------\n"
echo -e "NOTE: ONLY FIRST 4 LINES OF WRITTEN FILES WILL BE PRINTED\n"

# crawling depth 0-6; any depth 5+ duplicates urls
for i in 0 1 2 3 4 5 6
do
    echo -e "\n*********** Testing at depth $i *************"
    ./crawler $seedURL tse-output $i
    echo -e "\ntse-output directory contents are: \n"
    for j in tse-output/*
    do
	echo -e "\n==> $j <=="
	sed -n "1,4p" $j
    done
    #cat tse-output/* #uncomment and comment above line if desiring full HTML print
    rm -rf tse-output #clean and remake tse-output for use again so
    mkdir tse-output #no confusion about which files are new or old    
done

# different seed page on the same site
# also testing with pageDir/ vs just pageDir from before
seedURL=http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/H.html

echo -e "\n******** Testing different seed page on same site at depth 5 ********"
./crawler $seedURL tse-output 5 # adds same files as previous 2 runs above
echo -e "\ntse-output directory contents are: \n"
for j in tse-output/*
do
    echo -e "\n==> $j <=="
    sed -n "1,4p" $j
done
#cat tse-output/* #uncomment and comment above line if desiring full HTML print
rm -rf tse-output
mkdir tse-output

# Crawl Wikipedia depth 0, 1, 2
echo -e "\nTesting Wikipedia playground..."
seedURL=http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/

for i in 0 1 2
do
echo -e "\n*********** Testing at depth $i *************"
./crawler $seedURL tse-output $i
echo -e "\ntse-output directory contents are: \n"
for j in tse-output/*
do
    echo -e "\n==> $j <=="
    sed -n "1,4p; 10p;" $j
done
#cat tse-output/* #uncomment and comment above line if desiring full HTML print
rm -rf tse-output
mkdir tse-output
done

############### cleanup ####################

rm -rf unwritable/

echo -e "\nTESTING COMPLETE!\n"
