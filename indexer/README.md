# CS50 Lab 5
## TSE Indexer README
## Erich Woo - CS50 Spring 2020

### Contents

    - `indexer.c` module, building and writing the inverted-index data structure
ls.
    - `indextext.c` module, loading and writing the inverted-index data structure

    - `testing.sh` for integration testing of `indexer.c` and `indextest.c`

### Compilation

To compile, simply `make`. 

To clean, simply `make clean`

To test, simply `make test`.

### Assumptions

*indexer* assumes:
   - pageDirectory has files named 1, 2, 3, …, without gaps.
   - The content of files in pageDirectory follow the format as specified in crawler/DESIGN.md
   - pageDirectory is crawler-generated; in other words, pageDirectory MUST contain a readable '.crawler' file.
   
*indextest* assumes:
   - The content of the index file follows the format specified in DESIGN.md
