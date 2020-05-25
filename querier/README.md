# CS50 Lab 6
## TSE Querier README
## Erich Woo - CS50 Spring 2020

### Contents

    - `querier.c` module, parsing user query and ranking results based on index
    - `testing.sh` for integration testing of `querier.c`

### Compilation

To compile, simply `make`

To clean, simply `make clean`

To test, simply `make test`

### Assumptions

    - pageDirectory has files named 1, 2, 3, …, without gaps.
    - The content of files in pageDirectory follow the format as specified in crawler/DESIGN.md
    - The content of the index file follows the format specified in DESIGN.md

### Notes

I implemented the `querier` with "full functionality" for grading

I printed this note in the output of `testing.sh`, but I'll reiterate. There are 2 registered differences using 'diff' in my Regression Testing part of my `testing.sh`.

  1. different URL for doc 1, in which case the example output from Requirements Spec is actually wrong on...
  2. An extra space at the end of my output, just as a consequence of not doing the full test that the Requirements spec did. The rest of Requirements spec test had messy output that wouldn't look clean in 'diff' command for testing-logistics reasons (explained in my output of `testing.sh`.
