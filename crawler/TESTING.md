# CS50 Lab 4
## TESTING.md for crawler.c in testing.sh
## Erich Woo - CS50 Spring 2020

### Description

Integration testing for crawler.c was done in testing.sh shell script.
Unit testing was not done.
Testing output dumped in testing.out
Ensure $UNITTEST in Makefile is uncommented for testing mode
Reminder that crawler program usage is

```c
./crawler seedURL pageDirectory maxDepth
```

### Testing

#### Error Cases

1. Incorrect number of arguments
2. External seedURL `www.google.com`
3. Non-existent seedURL `gotcha`
4. Valid server but Non-existent page
5. Non-existent pageDirectory
6. Unwritable pageDirectory
7. Negative maxDepth

#### Standard Cases

1. Crawling `http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html` with maxDepth 0-6
2. Crawling the same site as above but with different seedURL `http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/H.html` at maxDepth 5
3. Crawling Wikipedia playground `http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/` with maxDepth 0-2

### Notes

A pageDirectory `tse-output` is created in the current directory for use in standard testing. A nonwritable pageDirectory `unwritable` is created in the current directory for use in error testing.

For each Standard Test Case, after successful crawling, the first 4 lines of every written file in `tse-output` is outputted. I could've printed all lines, but I felt that would be excessive and could blow up the output. The first 2 lines should contain URL and Depth. For these seedURLs, the 3rd line should be the opening `<html>` tag and the 4th line should be the full `<title>blah</title>` tag, which is hopefully sufficient for identifying correct file writing of the full HTML. The Wikipedia playground crawling grabs the first 4 lines and the 10th line. 3rd is `<!DOCTYPE html>` header, 4th is `<html blah>`, and the 10th is the identifier title tag `<title>blah</title>`.

If full HTML printing is desired, UNCOMMENT lines 74, 91, and 109, and COMMENT lines 69-73, 86-90, and 104-108.

The resulting `testing.out` file is very long solely because of the Wikipedia playground depth=2 crawl, taking up >90% of the file. Scroll to the top for most of all of the file contents.

When dumping out written files, the prints are not guarenteed to be in increasing numerical order, but definitely in increasing alphanumeric order; ie 1, 14, 150, 2, 23...

### Assumptions

For Standard Test Cases, assuming the format of HTML so I can print a fixed set of lines from the writtien directory.

Assumes `common` directory is fully compiled by calling `make` in the `common` directory.