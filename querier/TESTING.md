# CS50 Lab 6
## TSE Querier Testing Spec
## Erich Woo - 24 May 2020

### Testing

`querier.c` is tested with bash script `testing.sh`. The output has been piped to an output file `testing.out` for viewing. Testing inputs and outpus are as follows:

### *querier* Testing

Reminder of *querier* usage:

```bash
./querier pageDirectory indexFilename
```

* Error cases
  * in command-line, which all should print error statements and end the *querier* command
    * incorrect number (1) of arguments
    * invalid `pageDirectory` 
      * inputted a non-existent pageDirectory
    * unreadable `pageDirectory`
      * created and inputted a directory with read permission turned off using `chmod`
    * non-crawler-created `pageDirectory`
      * created and inputted a directory that wasn't derived from *crawler*
    * invalid `indexFilename` path
      * inputted a non-existent indexFilename path
    * unreadable `indexFilename` path
      * created and inputted a file with read permission turned off using `chmod`
  * in query, which all should print error statements and continue searching for another query; but should NOT print a clean query. Error statements should be specified to the type of error, and specified to the erroneous character/word entered
    * 'and'/'or' cannot be first
      * 'and'
      * 'or'
      * 'and or'
      * 'and dartmouth'
      * 'or dartmouth'
    * 'and'/'or' cannot be last
      * 'dartmouth and'
      * 'dartmouth or'
    * 'and'/'or' cannot be adjacent with another 'and'/'or'
      * 'dartmouth and or college'
      * 'dartmouth and and college'
      * 'dartmouth or and college'
      * 'dartmouth or or college'
    * non-alphabetic character
      * 'dartmouth college!'
      * 'dartmouth college250'
      * 'Backus-Naur Form'
* Standard cases
  * manually created cases not testable by Fuzztesting;
    * empty query - should print an empty query, should not print a result
    * query with no matches - should print "No documents match"
      * inputted a query consisting of bogus words that surely wouldn't have a match
    * variable input styles - should all print the same expected output
      * 'Dartmouth College' (Capitalization)
      * 'dartmouth college' (Lowercase)
      * 'dartmouth and college' (Explicit 'and')
      * '  dartmouth   AND     College' (Extra whitespace, and combination of the above)
    * regression testing using 'diff' command
      * Compared with the example in lines 143-152 of Requirements Spec, using the query 'dartmouth or computer science or programming or unix' from (a suspected) wikipedia-depth-1 and wikipedia-index-1
  * Fuzztesting
    * Comparing output from fuzztesting example on ~cs50/Lectures/querier (letters-index-3, 10 queries, random seed 0)
      * Note: Can't perform 'diff' regression testing like above, because this page is inaccessible from my linux account/server. So I just print the output, and the viewer can just open the page and check the differences (none!) by eye
    * with wikipedia-index-1, 10 queries, random seed 0
      * no way to check if these results are actually correct, besides manually looking through these pages and searching for the words queried      

### Cleanup

At the end of testing, all created directories and files are deleted. To leave the pertinent files (where testing actually happend) available for review after testing, simply comment out 'actual' and 'expected' (from Regression Testing) from the `rm` command.