# CS50 Lab 4
## IMPLEMENTATION.md for crawler.c
## Erich Woo, CS50 Spring 2020

### Implementation

A compiled crawler runs as follows:

1. Error checks command-line arguments
   1. Use dir_exists in pagedir.c for checking directory validity
   2. exit 1 on error
2. Initialize bag of webpages and hashtable of URLs
3. Create initial webpage from seedURL
4. Insert initial webpage into bag, seedURL into hashtable
5. while bag isn't empty,
   1. initialize an id counter = 1
   2. extract a webpage from the bag
   3. allocate space and assign characters for the filepath using pageDir and id
   4. if the filepath is valid and writable
      1. fetch the html for the webpage with pagefetcher
      	 1. track any error in stderr, but continue
      2. write the webpage contents to the filepath (generated in step 5.3) using pagesaver
      	 1. track any error in stderr, but continue
      3. increment id + 1
      4. if the webpage's depth < maxDepth
      	 1. initialize integer index to 0
	 2. extract the webpage's next URL, starting at index (5.4.4.1) using pagescanner
	 3. while the extracted URL isn't NULL
	    1. Normalize URL, check if its internal, and try inserting into hashtable
	       1. ignore if not internal or can't insert (duplicate URL)
	    2. Once inserted to hashtable, make a new webpage for URL at depth + 1
	       1. track any error in stderr, but continue
	    3. add new webpage to bag
	 4. repeat step 5.4.4.2 until NULL
6. delete and free remaining allocated space
7. exit 0
	    
	    