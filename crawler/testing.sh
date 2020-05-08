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
nonexistantURL=gotcha
echo -e "seedURL is: '$seedURL'\nexternalURL is: '$externalURL'\nand nonexistantURL is: '$nonexistantURL'\n"

#setup crawler directory for testing
echo -e "setting up crawler directory for testing..."
make clean
make

#create directories for testing
echo -e "\ncreating tse-output/ and unwritable/ and changing permissions for unwritable/"
mkdir tse-output
mkdir unwritable
chmod u-w unwritable

############## Error cases ###############

# incorrect number of arguments
echo -e "\nTesting incorrect number of arguments..."
./crawler $seedURL tse-output

# external url
echo -e "\nTesting external URL..."
./crawler $externalURL tse-output 0

# nonexistant url
echo -e "\nTesting nonexistant URL..."
./crawler $nonexistantURL tse-output 0

# not a directory
echo -e "\nTesting a non-existent directory 'test'..."
./crawler $seedURL test 0

# unwritable directory
echo -e "\nTesting unwritable directory..."
./crawler $seedURL unwritable 0

# negative maxDepth
echo -e "\nTesting a negative maxDepth..."
./crawler $seedURL tse-output -1

############### Standard cases ################

 



############### cleanup ####################

rm -rf tse-output/ unwritable/
