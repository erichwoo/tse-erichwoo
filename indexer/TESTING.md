# CS50 Lab 5
## TSE Indexer Testing Spec
## Erich Woo - 14 May 2020

### Testing

`indexer.c` and `indextest.c` are tested with bash script `testing.sh`. Testing inputs and outputs are as follows:

#### *indexer* Testing

Reminder of *indexer* usage:

```bash
./indexer pageDirectory indexFilename
```

* Error cases - all should print error statements and end the specific command
  * incorrect number (1) of arguments
  * invalid `pageDirectory`
    * inputted a non-existent pageDirectory
  * unreadable `pageDirectory`
    * created a directory and turned its read permission off using `chmod`
  * invalid `indexFilename` path
    * inputted a non-existent indexFilename path
  * unwriteable `indexFilename` path bc of unwriteable parent directory
    * created a directory and turned its write permission off using `chmod`
    * tacked on a filename to the unwriteable directory and used that as input
  * unwriteable `indexFilename` path bc of unwriteable file
    * created a file and turned its write permossion off using `chmod`, inputted that
* Standard cases
  * Letters
    * created a test directory and sorted-test directory
      * output files of *indexer* will go in these two directories
      * as the name suggests, test directory contains raw output of *indexer* while sorted-test directory contains sorted output of *indexer* using `indexsort.awk`
    * pageDirectory = ~cs50/data/tse-output/data/letters-depth-i, where i = 0-5
    * indexFilename = testdir/letters-index-depth-i, where i = 0-5
    * created a sorted-data directory
      * contains sorted versions of ~cs50/data/tse-output/data/letters-index-i using `indexsort.awk`
    * used `indexsort.awk` to sort output
    * compared sorted data files and sorted output files
    * used `diff` command to see results (should say file 1 and 2 identical)
  * ToScrape
    * identical to Letters testing, but...
    * pageDirectory = ~cs50/data/tse-output/data/toscrape-depth-i, where i = 0-2
    * indexFilename = testdir/letters-index-depth-i, where i = 0-2
    * output sorted, then compared with sorted ~cs50/data/tse-output/data/toscrape-index-i
 
#### *indextest* Testing

Reminder of *indextest* usage:

```bash
./indextest oldIndexFilename newIndexFilename
```

* Error cases - all should print error statements and end the specific command
  * incorrect number (1) of arguments
  * invalid `oldIndexFilename`
    * inputted a non-existent oldIndexFilename
  * unreadable `oldIndexFilename`
    * turned read permissions off for an existing file in test directory, inputting that file
    * made sure to turn read permissions back on for the file for later testing.
  * invalid `indexFilename` path
    * inputted a non-existent indexFilename path
  * unwriteable `newIndexFilename` path bc of unwriteable parent directory
    * created a directory and turned its write permission off using `chmod`
    * tacked on a filename to the unwriteable directory and used that as input
  * unwriteable `newIndexFilename` path bc of unwriteable file
    * created a file and turned its write permossion off using `chmod`, inputted that
* Standard cases
  * Letters
    * same as *indexer* test but,
    * oldIndexFilename = sortedtestdir/letters-index-i, where i = 0-5
      * this is the sorted output of *indexer* test above
    * newIndexFilename = testdir/letters-test-i, where i = 0-5
    * newIndexFilename output is sorted and placed in sorted-test directory
    * compared sorted oldIndexFilename files and sorted output newIndexFilename files
    * used `diff` command to see results (should say file 1 and 2 identical)
  * ToScrape
    * identical to Letters testing, but...
    * oldIndexFilename = sortedtestdir/toscrape-index-i, where i = 0-2
    * newIndexFilename = testdir/toscrape-test-i, where i = 0-2
    * output sorted, then compared with oldIndexFilename

#### Cleanup

At the end of testing, all created directoriesare deleted. To leave those files available for review after testing, simply remove '$testdir', '$sorteddir', and '$sortedtestdir' from the `rm` command.