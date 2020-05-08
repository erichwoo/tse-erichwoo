# CS50 Lab 4
## README.md file for crawler.c
## Erich Woo, CS50 Spring 2020

### crawler

A *crawler* crawls a website and retrieves webpages starting with a specified URL. It parses the initial webpage, extracts any embedded URLs and retrieves those pages, and crawls the pages found at those URLs, but limits itself to maxDepth hops from the seed URL and to URLs that are ‘internal’ to the designated CS50 server. When the crawler process is complete, the indexing of the collected documents can begin.

Reference: https://www.cs.dartmouth.edu/~cs50/Labs/Lab4/DESIGN.html

### Usage

Run crawler using the following command line arguments:

```c
./crawler seedURL pageDirectory maxDepth
```

The crawler requires 3 arguments, a valid and internal seedURL, an existing and writable pageDirectory, and a non-negative maxDepth.

### Assumptions

No assumptions beyond those that are clear from the spec.

### Compilation

To compile, simply `make`.

To test, simply `make test`.

To clean, simply `make clean`.
