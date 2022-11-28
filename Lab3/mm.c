/*
 * 
 * 
 * Implementation of explicit free list based on the implementation
 * from book. Block of memory always contains header and footer
 * with address of the next block and information if it is allocated.
 * If the block is free after header there is information about 
 * location of previous and next free block in the explicit free list.
 *
 * Malloc -> first tries to find fit in free list using first fit algorithm.
 * When he finds block from free list, removes it and uses it for allocation.
 * If no sufficient free block is found, it requests more memory to the heap.
 *
 * Free -> Frees the block, and adds it to explicit free list.
 * Before that, it tries to coalesce the block with its surroundings.
 *
 * Realloc -> at the moment of writing this, I couldn't figure out 
 * better solution, so I have just used malloc + free operations.
 * 
 * 
 * Coalesce -> when coalescing it is important to keep track of 
 * blocks in explicit free list. If I am joining with free block,
 * there is a need to remove that block from the list and afterwards
 * add new free block to the list.
 *
 * Find fit -> going through beggining of the free list till the end,
 * until it finds enough space. If not returns NULL;
 *
 * Explicit free list -> Two way linked list of free blocks. Adding to the 
 * list is always to the begging of the list. At the end of the list, there  
 * is a prologue header of the heap, that is always allocated. This helps
 * fit finding algorithm to better check, when there are no more free blocks
 * in the list.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "jpir",
    /* First member's full name */
    "Jan Piroutek",
    /* First member's email address */
    "jpir@itu.dk",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* Basic constants and macros */
#define WSIZE 4
/* Word and header/footer size (bytes) */
#define DSIZE 8
/* Double word size (bytes) */
#define CHUNKSIZE (1<<12) /* Extend heap by this amount (bytes) */

#define MAX(x, y) ((x) > (y)? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x07)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((void *)(bp) - WSIZE)
#define FTRP(bp) ((void *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((void *)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp) ((void *)(bp) - GET_SIZE((void *)(bp) - DSIZE))

// jumping through free list
#define NEXT_FREE_PTR(bp) (*(void **)(bp + WSIZE))
#define PREV_FREE_PTR(bp) (*(void **)(bp))

static void* find_fit(size_t size);
static void place(void* bp, size_t size);
static void* coalesce(void *bp);
static void* extend_heap(size_t words);

// pointer to start of heap list
static void* heap_listp = 0;

// pointer to explicit free list
static void* exp_free_listp = 0;

static void add_to_free_list(void* bp);
static void delete_from_free_list(void * bp);

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
	/* Create the initial empty heap */
	// 8 * WSIZE because in explicit free list I need more space for pointers to prev and next free blocks
	// implicit list is enough with 4 * WSIZE
	if ((heap_listp = mem_sbrk(8*WSIZE)) == (void *)-1)
		return -1;
	PUT(heap_listp, 0);
	/* Alignment padding */
	PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1)); /* Prologue header */
	PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1)); /* Prologue footer */
	PUT(heap_listp + (3*WSIZE), PACK(0, 1));
	/* Epilogue header */
	heap_listp += (2*WSIZE);

	// pointer to explicit free list
	exp_free_listp = heap_listp;
	
	/* Extend the empty heap with a free block of CHUNKSIZE bytes */
	if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
		return -1;

	
	return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
	size_t asize;
	/* Adjusted block size */
	size_t extendsize; /* Amount to extend heap if no fit */
	void *bp;

	/* Ignore spurious requests */
	if (size == 0)
		return NULL;
	/* Adjust block size to include overhead and alignment reqs. */
	if (size <= DSIZE)
		asize = 2*DSIZE;
	else
		asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);

	/* Search the free list for a fit */
	if ((bp = find_fit(asize)) != NULL) {
		place(bp, asize);
		return bp;
	}

	/* No fit found. Get more memory and place the block */
	extendsize = MAX(asize,CHUNKSIZE);
	if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
		return NULL;

	place(bp, asize);

	return (void *)bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
	size_t size;

	if (ptr == NULL) {
		return;
	}
	// get size of current block
	size = GET_SIZE(HDRP(ptr));

	// set header and footer of currrent block as free 0
	PUT(HDRP(ptr), PACK(size, 0));
	PUT(FTRP(ptr), PACK(size, 0));
	// merge with other blocks
	coalesce(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *newptr;
    size_t old_size, new_size;
    // variables for merging with next block
    size_t is_next_alloc, next_blk_size, total_size;

    // edge cases for realloc
    if ((int)size < 0) {
	    return NULL;
    }

    if (size == 0) {
	    mm_free(ptr);
	    return NULL;
    }

    if (ptr == NULL) {
	    return mm_malloc(size);
    }
	
    
    old_size = GET_SIZE(HDRP(ptr));
    /*
    newptr = mm_malloc(size);
    if (newptr == NULL) {
	    return NULL;
    }

    if(size < old_size) { 
	    old_size = size;
    }
    memcpy(newptr, ptr, old_size);
    mm_free(ptr);
    return newptr;
    */
    /*
     *try for effective realloc
     * works except for case when user is trying to shrink space
     * then it fails first on not keeping original data and second realloc fails on seg fault
     * */
    new_size = size + 2 * DSIZE;

    if (new_size < old_size) {
	    // can't figure this part out     
	    /*
	    if (size <= DSIZE) {
		    size = DSIZE;
	    } else {
		    size=DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);
	    } 
	    if (old_size - size > 2 * DSIZE) {
	    	
		    // put new size to header and set footer of new block
		    PUT(HDRP(ptr), PACK(size, 1));
		    PUT(FTRP(ptr), PACK(size, 1));
		    newptr = ptr;
		    ptr = NEXT_BLKP(newptr);

		    // update data in newly created block (basically splitting memory block)
		    PUT(HDRP(ptr), PACK(old_size - size, 0));
		    PUT(FTRP(ptr), PACK(old_size - size, 0));
		    // free newly created block and add to free list
		    add_to_free_list(ptr);
		    coalesce(ptr);
		    return newptr;
	    } else {

		    return ptr;
	    }
	    */
	    
	   return ptr;
    } else if (new_size > old_size){
	    // true if next block is allocated
	    is_next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
	    // size of the next block and total size of free space
	    next_blk_size = GET_SIZE(HDRP(NEXT_BLKP(ptr)));
	    total_size = old_size + next_blk_size;	

	    if(!is_next_alloc && total_size>= new_size){
		    // if merged with next block (that has to be free) would have enough space 
		    
		    // next block won't be free anymore
		    delete_from_free_list(NEXT_BLKP(ptr));	
		    // update header and footer of this block
		    PUT(HDRP(ptr),PACK(total_size,1));
		    PUT(FTRP(ptr),PACK(total_size,1));
		    return ptr;
	    } else {
		    // allocate space for new needed memorry
		    newptr = mm_malloc(new_size);
		    if (newptr == NULL)
		      return NULL;
		    // set header of new block
		    place(newptr, new_size);
		    // copy the data to a new place
		    memcpy(newptr, ptr, old_size);
		    // free the previous pointer
		    mm_free(ptr);
		    
		    return newptr;
	    }
    } else {
	    return ptr;
    }
}

// find place for memory of size 'size'
static void* find_fit(size_t size) {
	// first fit algorithm for implicit free list
	void *bp;
	//printf("ptr %x", bp);
	for (bp = exp_free_listp; GET_ALLOC(HDRP(bp)) == 0; bp = NEXT_FREE_PTR(bp)) {
		//printf("ptr %x", bp);
		if (size <= GET_SIZE(HDRP(bp))) {
			return bp;
		}
	}
	//printf("ptr %x", bp);
	return NULL;
}

// set header and footer of newly allocated block
static void place(void* bp, size_t size) {
	// get size of while block
	size_t csize = GET_SIZE(HDRP(bp));

	// if the size of block is bigger than requested size
	// and after allocation there will still be enough space for header and footer (and payload)
	// split the block
	if ((csize - size) >= (2*DSIZE)) {
		// put info to block header that blocks size is 'size' and 1 for allocated
		// same for footer
		PUT(HDRP(bp), PACK(size, 1));
		PUT(FTRP(bp), PACK(size, 1));
		delete_from_free_list(bp);
		// move pointer to end of needed space for allocation
		bp = NEXT_BLKP(bp);
		// here the block splits 
		// put info to start of new block about its size (csize - size) and info that it's free 0
		PUT(HDRP(bp), PACK(csize-size, 0));
		PUT(FTRP(bp), PACK(csize-size, 0));
		//add_to_free_list(bp);
		coalesce(bp);
	} else {
		// just allocate whole block
		PUT(HDRP(bp), PACK(csize, 1));
		PUT(FTRP(bp), PACK(csize, 1));
		delete_from_free_list(bp);
	}
}

// merge free block with its surroundings
static void *coalesce(void *bp)
{
	//printf("ptr %x\n", bp);
	// is previous block allocated or is bp the first block in the heap
	size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp))) || PREV_BLKP(bp) == bp;
	// is next block allocated
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	// size of current block
	size_t size = GET_SIZE(HDRP(bp));

	// 4 cases for surrounding blocks
	if (prev_alloc && next_alloc) {
		/* Case 1 */
		// everything around block is allocated
		// can't be merged with anything
		add_to_free_list(bp);
		return bp;
	}
	else if (prev_alloc && !next_alloc) {
		/* Case 2 */
		// previous block is allocated and next block is free
		// merge this block with next block
		
		
		// increase total size with size of next block
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		// delete next block from free list so I can add merged block later
		delete_from_free_list(NEXT_BLKP(bp));

		// put info to blocks header about new size and that it's free
		// also into footer
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));
	}
	else if (!prev_alloc && next_alloc) {
		/* Case 3 */
		// previous block is free, but next block is allocated
		// merge this block with previous block
		
		// increase total size with size of previous block
		size += GET_SIZE(HDRP(PREV_BLKP(bp)));
		// delete previous block from free list so I can add merged block later
		delete_from_free_list(PREV_BLKP(bp));

		// put info to footer of this block about size and free 0
		PUT(FTRP(bp), PACK(size, 0));
		// put info to header of previous block
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		// set pointer to previous block
		bp = PREV_BLKP(bp);
	}
	else {
		/* Case 4 */
		// both previous and next block are free
		// have to merge them all together
		
		// total size is sum of size of this block + previous block + next block
		size += GET_SIZE(HDRP(PREV_BLKP(bp))) +
			GET_SIZE(FTRP(NEXT_BLKP(bp)));

		// delete both previous and next block from free list so merged can be added later
		delete_from_free_list(PREV_BLKP(bp));
		delete_from_free_list(NEXT_BLKP(bp));
		// put info about size and free to header of previous block
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		// put info about size and free to footer of next block
		PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
		// set pointer to previous block
		bp = PREV_BLKP(bp);
	}
	// add merged block
	add_to_free_list(bp);
	return bp;
}

// extending heap
static void *extend_heap(size_t words)
{
	void *bp;
	size_t size;

	/* Allocate an even number of words to maintain alignment */
	size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
	if ((long)(bp = mem_sbrk(size)) == -1)
		return NULL;

	// Initialize free block header/footer and the epilogue header
	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));
	// last block is of size 0
	PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));
	// merge if last block was free
	return coalesce(bp);
}

// put bp to beginning of the free list
static void add_to_free_list(void* bp) {
	NEXT_FREE_PTR(bp) = exp_free_listp;
	PREV_FREE_PTR(exp_free_listp) = bp;
	PREV_FREE_PTR(bp) = NULL;
	exp_free_listp = bp;
}

// delete block from free list
static void delete_from_free_list(void * bp) {
	if (PREV_FREE_PTR(bp) == NULL) {
		// bp points to beginnig of free list
		exp_free_listp = NEXT_FREE_PTR(bp);
	} else {
		//void *p = PREV_FREE_PTR(bp);
		//void *n = NEXT_FREE_PTR(bp);
		NEXT_FREE_PTR(PREV_FREE_PTR(bp)) = NEXT_FREE_PTR(bp);
	}
	PREV_FREE_PTR(NEXT_FREE_PTR(bp)) = PREV_FREE_PTR(bp);
}

