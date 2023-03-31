/**********************************************************************\
 Wellesley CS 240 Malloc: mm.c starter code
 Completed code by Veronica Lin (yl102) & Becky Chen (bc103)
 Starter code by Ben Wood, 2015-2016.
 Additional support files use CSAPP materials.
\**********************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include "memlib.h"
#include "mm.h"
/* HEAP BLOCK LAYOUT ************************************************\
  The heap is divided into contiguous blocks.  Blocks are flexibly
  sized, subject to ALIGNMENT (2 words).  The initial word of a block
  is treated as a header status word at all times.  Free blocks use
  the last word as a footer to store SIZE ONLY (not tags).  The
  required header and footer words for free blocks fix the minimum
  block size at 2 words.  While allocated, the header is the only
  metadata needed in a block.
  Payloads must begin at addresses that are multiples of ALIGNMENT.
  Blocks begin at addresses A where A mod ALIGNMENT = (ALIGNMENT / 2)
                             ... lower addresses up
           HEAP BLOCK
     +--------------------+  <-- Start of block
     | header (size|tags) |
  ---+--------------------+  <-- Start of payload (when allocated)
   P |                    |      Address is multiple of ALIGNMENT
   A |                    | 
   Y |        ...         | 
   L |                    | 
   O |                    | 
   A +--------------------+
   D | footer (size)      |
  ---+--------------------+  <-- End of block, start following block.
                             ... higher addresses down
 
\*********************************************************************/
static const int NEXT_FIT_POLICY = 1; //this is 1 when we use implicit free list next fit, and we use first fit if it's 0
word_t* last_allocated_block = 0; // a global variable to keep track of the address of the last allocated block
/* Type for words: word_t. */
typedef unsigned long word_t;
/* Size of a word on this architecture.
  All pointers and size_t values are one word in size. */
#define WORD_SIZE (sizeof(word_t))
/* Payloads returned by mm_malloc must be aligned to 2 words, as in
  the standard malloc implementation. */
#define ALIGNMENT ((size_t)(2*WORD_SIZE))
/* Minimum block size: header and footer. */
#define MIN_BLOCK_SIZE (ALIGNMENT)
/* POINTER ARITHMETIC ************************************************** \
  PADD and PSUB support easier unscaled pointer arithmetic.
  Casting the pointer argument to char* makes pointer arithmetic
  effectively unscaled, since sizeof(char) == 1.
\**********************************************************************/
/* Perform unscaled pointer addition. */
static word_t* PADD(void* address, long distance) {
 return ((word_t*)((char*)(address) + (distance)));
}
/* Perform unscaled pointer subtraction. */
static word_t* PSUB(void* address, long distance) {
 return ((word_t*)((char*)(address) - (distance)));
}
/* STATUS WORDS *******************************************************\
  Status words, used in block headers, store a block's size (measured
  in bytes) and two 1-bit tags indicating the allocated/free status
  of this block and the preceding block in memory order.
  Since ALIGNMENT >= 8 (8 on 32-bit machines, 16 on 64-bit machines),
  all valid block sizes are at leasts multiples of 8 and the
  low-order 3 bits of valid block sizes are always 000 (0000 for
  64-bit machines).  We use these status word bits as tag bits.
            STATUS WORD
     +-----------------------+
     | . . . | 3 | 2 | 1 | 0 |
     +-----------------------+
  Bits ...-4:        <size>
  Bits 3-2:          <unused>
  Bit  1 (2^1 == 2): PRED_USED_BIT tag    [For coalescing]
  Bit  0 (2^0 == 1): USED_BIT tag
  The size and two tags can be extracted separately by masking.  Use
  the provided functions status_size, status_pred, status_used.
  Block footers store block size only, not tags.
\**********************************************************************/
/* status_size(x) extracts the block size information from a status word, x,
  masking off the other status bits. */
#define SIZE_MASK (~(ALIGNMENT - 1))
static word_t status_size(word_t status_word) {
 return status_word & SIZE_MASK;
}
/* status_pred(x) extracts the predecessor status bit from a status
  word, x, masking off the other status bits. */
#define PRED_USED_BIT 2
static word_t status_pred(word_t status_word) {
 return status_word & PRED_USED_BIT;
}
/* status_used(x) extracts the allocation status bit from a status
  word, x, masking off the other status bits. */
#define USED_BIT 1
static word_t status_used(word_t status_word) {
 return status_word & USED_BIT;
}
/* make_status(s,p,u) makes a new status word by extracting the block
  size information from status word s, the predecessor status bit
  from word p, and the allocation status bit from word u.
 
  WARNING: to set the predecessor or used bits explicitly, pass
  PRED_USED_BIT or USED_BIT, not 1.
*/
static word_t make_status(word_t size, word_t pred_used, word_t used) {
 return (size & SIZE_MASK) | (pred_used & PRED_USED_BIT) | (used & USED_BIT);
}
/* HEAP LAYOUT ********************************************************\
  The heap starts at an address that is a multiple of page size.  Due
  to payload alignment requirements, blocks begin at addresses that
  are (ALIGNMENT/2 == 1 word) off from aligned.  Thus, the initial
  heap block starts with the 2nd heap word and the initial heap word
  cannot be part of any well-aligned block.
                             ... lower addresses up
             HEAP       
    +--------------------+   <-- HEAP_BASE, HEAP_HEADER_ADDR
    |                    | 
    +--------------------+   <-- ORIGIN_BLOCK_ADDR
  B | header (size|tags) |
  L +--------------------+   <-- payload aligned @ 2 words
  O | payload            |
  C | ...                |
  K |                    |
    +--------------------+
    |                    |
    |   ... blocks ...   |
    |                    |
    +--------------------+   <-- HEAP_FOOTER_ADDR
    |  heap footer word  |     
    +--------------------+   <-- HEAP_BOUND
                             ... higher addresses down
\**********************************************************************/
/* Base address of the heap. */
#define HEAP_BASE ((word_t*)mem_heap_lo())
/* Bound address of the heap (first address after the heap. */
#define HEAP_BOUND ((word_t*)PADD(mem_heap_hi(), 1))
/* Address of the heap header word. */
#define HEAP_HEADER_ADDR ((word_t**)HEAP_BASE)
/* Address of the first block in the heap (in memory order). */
#define ORIGIN_BLOCK_ADDR ((word_t*)PADD(HEAP_BASE, WORD_SIZE))
/* Address of the heap footer word. */
#define HEAP_FOOTER_ADDR ((word_t*)PSUB(HEAP_BOUND, WORD_SIZE))
/* CHECKED ACCESSES ***************************************************\
  The functions LOAD, PLOAD, STORE, and PSTORE can be used in place
  of pointer operations in your code to help detect errors early and
  avoid casting and pointer noise.  In addition to loading and
  storing data words (LOAD/STORE) and pointer words (PLOAD/PSTORE) in
  memory, these functions verify that all pointers used, stored, and
  loaded point within the heap and are word-aligned.  The helper
  function check_address can be used to perform these assertions
  elsewhere as well.
\**********************************************************************/
/* Assert that address is well-formed address of some word within
  heap. */
static bool check_address(void* address) {
 return
   // Address must be in heap bounds.
   ((void*)HEAP_BASE <= address && address < (void*)HEAP_BOUND)
   // Address must be word-aligned.
   && (((word_t)address & (WORD_SIZE - 1)) == 0);
}
/* LOAD(a) loads a word from memory at address a. */
static word_t LOAD(word_t* address) {
 assert(check_address(address)
        && "LOAD must load from a word-aligned address within the heap");
 return *((word_t*)(address));
}
/* LOAD(a) loads a pointer word from memory at address a. */
static word_t* PLOAD(word_t** address) {
 assert(check_address(address)
        && "PLOAD must load from a word-aligned address within the heap");
 word_t* ptr = *((word_t**)(address));
 assert((!ptr || check_address(ptr))
        && "PLOAD must return a word-aligned address within the heap");
 return ptr;
}
/* STORE(a,w) stores word w into memory at address a. */
static void STORE(word_t* address, word_t word) {
 assert(check_address(address)
        && "STORE must store to a word aligned address within the heap");
 *((word_t*)address) = word;
}
/* STORE(a,w) stores pointer word w into memory at address a. */
static void PSTORE(word_t** address, void* ptr) {
 assert((!ptr || check_address(ptr))
        && "PSTORE must store a word-aligned address within the heap");
 assert(check_address(address)
        && "PSTORE must store to a word aligned address within the heap");
 *((word_t**)address) = ptr;
}
/* BLOCK FUNCTIONS ****************************************************\
  The functions block_{get,set}_header are provided for easy
  manipulation of block headers.  You may wish to add similar
  functions for accessing footers.  The functions block_succ and
  block_pred compute the addresses of a block's successor and
  predecessor blocks in memory order, respectively.
  If implementing explicit free lists, additional functions
  block_{get,set}_{next,prev} should be added here to support easy
  manipulation of blocks as doubly-linked list nodes.
  For next-fit or explicit free lists, you should add functions to
  get and set the free list head pointer, stored in the (otherwise
  empty) first word of the heap.
\**********************************************************************/
/* Get the header word of the given block. */
static word_t block_get_header(word_t* block) {
 return LOAD(block);
}
/* Set the header word of the given block. */
static void block_set_header(word_t* block, word_t header) {
 STORE(block, header);
}
/* Given a block address, calculate the address of the block's
  successor, using this block's header. */
static word_t* block_succ(word_t* block) {
 // Get this block's size from its header and add to its address.
 return PADD(block, status_size(LOAD(block)));
}
/* Given a block address, calculate the address of the block's
  predecessor, assuming its predecessor is free. */
static word_t* block_pred(word_t* block) {
 // Predecessor must be free.
 assert(!status_pred(LOAD(block))
        && "predecessor must be free");
 // Get predecessor size from predecessor footer and subtract from
 // this block's address.
 word_t footer = LOAD(PSUB(block, WORD_SIZE));
 // Footers must hold sizes.
 assert(status_size(footer) == footer
        && "footer must hold size only, no status bits");
 return PSUB(block, footer);
}
/* ALLOCATOR INTERNALS ************************************************\
  The function check_heap is a basic heap consistency checker useful
  for debugging.  The functions {extend_heap, search, allocate,
  coalesce} encapsulate key heap maintenance operations within the
  allocator.  These functions will need to be updated as you
  implement more sophisticated allocation schemes.
\**********************************************************************/
/** Report a heap error.  Helper for check_heap. */
static void heap_error(unsigned long threshold, unsigned long* count) {
 (*count)++;
 if (threshold < *count) {
   fprintf(stderr, "\n[!] MALFORMED HEAP\n\n");
   assert(false && "heap error");
 }
}
/* Check the heap.  Iterate through blocks as implicit free list.
  ADD CODE to check for consistency.  */
static void check_heap(unsigned long fail_threshold) {
 // print to stderr so output is displayed immediately, not buffered
 unsigned long errors = 0;
 word_t expect = PRED_USED_BIT;
 // For each block in the heap...
 word_t* block= ORIGIN_BLOCK_ADDR;
 while (1) {
   word_t header = LOAD(block);
   // print out common block attributes...
   fprintf(stderr, "%p: %lu %s %s\t",
           (void *)block,
           status_size(header),
           status_pred(header) ? "PA" : "PF",
           status_used(header) ? "A" : "F");
   // Check consistency of PRED_USED_BIT.
   if (status_pred(LOAD(block)) != expect) {
     fprintf(stderr, "  [BAD PRED TAG]");
     heap_error(fail_threshold, &errors);
   }
   // Mark the predecessor bit expected in the next block header.
   expect = status_used(LOAD(block)) ? PRED_USED_BIT : 0;
   // Check allocated/free/footer status.
   if (status_size(header) == 0) {
     // Misplaced or malformed heap footer.
     if (block == HEAP_FOOTER_ADDR && status_used(header)) {
       fprintf(stderr, "  <-- HEAP FOOTER\n\n");
       return;
     } else if (status_used(header)) {
       fprintf(stderr,
               "  [PREMATURE HEAP FOOTER, should appear at %p]",
               HEAP_FOOTER_ADDR);
     } else if (block == HEAP_FOOTER_ADDR) {
       fprintf(stderr, "  [MALFORMED HEAP FOOTER]");
     } else {
       fprintf(stderr,
               "  [ZERO-SIZED BLOCK OR MALFORMED HEAP FOOTER, should not appear until %p]",
               HEAP_FOOTER_ADDR);
     }
     // Definitely exit.
     heap_error(errors, &errors);
     assert(false && "heap error");
   } else if (!status_used(header)) {
     size_t footer = LOAD(PADD(block, status_size(header) - WORD_SIZE));
     if (footer != status_size(header)) {
       fprintf(stderr, "  [BAD BLOCK FOOTER %ld expected %ld]",
               footer, status_size(header));
       heap_error(fail_threshold, &errors);
     }
   }
   // Check the size.
   word_t* succ_addr = PADD(block, status_size(header));
   // Is size large enough to wrap around?
   if (succ_addr < block) {
     fprintf(stderr, "\t  [BAD SIZE]");
     heap_error(errors, &errors);
     assert(false && "heap error");
   }
   // Is size too big to fit in the heap?
   if (HEAP_FOOTER_ADDR < succ_addr) {
     fprintf(stderr,
             "\t  [BAD SIZE: passes heap footer %p]",
             HEAP_FOOTER_ADDR);
     heap_error(errors, &errors);
     assert(false && "heap error");
   }
   fprintf(stderr, "\n");
   // Go to next block
   block = block_succ(block);
 }
 // Unreachable
 assert(false && "unreachable");
}
/* Find a free block of the requested ideal_block_size in the free list.  Returns
  NULL if no free block is large enough. */
static void* search(size_t ideal_block_size) {
 // ideal_block_size must be a legal block size.
 assert(ideal_block_size >= MIN_BLOCK_SIZE
        && "ideal block size must be no smaller than minimum block size");
 assert((ideal_block_size & (~SIZE_MASK)) == 0
        && "ideal block size must be 16-byte aligned");

 if (NEXT_FIT_POLICY == 1){
 word_t* block = last_allocated_block; //start searching from the last allocated block
 do {
   // Get block header.
   word_t header = LOAD(block);  //load header information from block
   // If the block is unused and big enough, we return the block
   if (!status_used(header) && ideal_block_size <= status_size(header)) {
     return block;
   }
   /* If the block does not meet our requirement, that is, the block is either allocated or
   * the block is not at least as big as the ideal_block_size, we go to the next block in heap
   */
   else  {
     block = PADD(block,status_size(header));
   }
 } while (block != HEAP_FOOTER_ADDR); //we stop if we reach the heap footer
 
 //if the program keeps running till here, it means that there isn't a suitable block
 //between the last allocated block and the heap footer, and now we need to search from
 //the beginning till we reach the last allocated block 
 block = ORIGIN_BLOCK_ADDR; //start searching from the beginning of the heap
 do {
   // Get block header.
   word_t header = LOAD(block);  //load header information from block
   // If the block is unused and big enough, we return the block
   if (!status_used(header) && ideal_block_size <= status_size(header)) {
     return block;
   }
   /* If the block does not meet our requirement, that is, the block is either allocated or
   * the block is not at least as big as the ideal_block_size, we go to the next block in heap
   */
   else  {
     block = PADD(block,status_size(header));
   }
 } while (block != last_allocated_block); //we stop if we reach the last allocated block
 }

 //else clause: if we're using first fit 
 else{
  /* in order to find this block, we are going to search the whole heap. This search starts
   * from ORIGIN_BLOCK_ADDR all the way to the end till we reach HEAP_FOOTER_ADDR. (A do-while logic)
   */
    word_t* block = ORIGIN_BLOCK_ADDR;
    do {
      // Get block header.
      word_t header = LOAD(block);  //load header information from block
      // If the block is unused and big enough, we return the block
      if (!status_used(header) && ideal_block_size <= status_size(header)) {
        return block;
      }
      /* If the block does not meet our requirement, that is, the block is either allocated or
      * the block is not at least as big as the ideal_block_size, we go to the next block in heap
      */
      else  {
        block = PADD(block,status_size(header));
      }
    } while (block != HEAP_FOOTER_ADDR); //we stop if we reach the heap footer
 }
 // Return NULL if no suitable block found.
 return NULL;
}

/* Make all heap updates to mark the given free block allocated and
  split if reasonbale to target the given ideal block size. */
static void allocate(word_t* block, size_t ideal_block_size) {
 //load header information from block
 word_t header = LOAD(block);
 //load footer information from block,
 word_t* footer = PSUB(block_succ(block),WORD_SIZE);
 // Get the size of the block to be allocated.
 size_t block_size = status_size(header);
   //check if curr block is large enough for the ideal size
   //Case 1: if the block size is bigger than the ideal_block_size (We need to split in this case)
 if (block_size > ideal_block_size){
     /* We need to split the block if our block size is bigger than the
     * ideal block size
     */
   //get the size of the split successor block
   word_t splitSucc_size = block_size - ideal_block_size;
   
   // Set the USED_BIT in the header, leaving other header bits untouched.
   STORE(block, make_status(ideal_block_size, header, USED_BIT));
   //get information about the successor block
   word_t* successor = PADD(block, ideal_block_size);
   //get the header of the successor by using LOAD()
   word_t successor_header = LOAD(successor);
  
   // Set the PRED_USED_BIT in the successor block's header, leaving
   // other header bits untouched.
   STORE(successor,make_status(splitSucc_size, PRED_USED_BIT, 0));
   //store the size of the successor block into the footer
   STORE(footer,splitSucc_size);
 
   //Case 2: if the block size is the same as the ideal_block_size (No splitting in this case)
 } else if (block_size == ideal_block_size){
  
   // Set the USED_BIT in the header, leaving other header bits untouched.
   STORE(block, make_status(ideal_block_size, header, USED_BIT));
   //get information about the successor block
   word_t* successor = PADD(block, ideal_block_size);
   //get the header of the successor by using LOAD()
   word_t successor_header = LOAD(successor);
   // Set the PRED_USED_BIT in the successor block's header, leaving
   // other header bits untouched.
   STORE(successor,make_status(successor_header, PRED_USED_BIT, successor_header));
   //store ideal_block_size into the footer because ideal_block_size == block_size
   STORE(footer,ideal_block_size);
 }
}
/* Coalesce this block with its memory-order predecessor or successor
  where possible. */
static void coalesce(word_t* block) {
 // Coalesce should be called on free, non-empty blocks only.
 word_t header = LOAD(block);
 assert(!status_used(header)
        && "coalesce accepts only free blocks");
 assert(0 < status_size(header)
        && "coalesce accepts only nonempty blocks");
 
 word_t prevUse = status_pred(header); //extract the status bit of the predecessor
 word_t* prevHeader = 0; //declare and initialize a pointer to the header of the predecessor
 word_t prevHeadInfo = 0; //declare and initialize a variable which stores information about the head of the predecessor
 word_t prevSize = 0; //declare and initialize the size of the predecessor
  // if the predecessor is unused, update information the variables prevHeader, prevHeadInfo, and prevSize stores
 if (prevUse==0){
   prevHeader = block_pred(block);
   prevHeadInfo = LOAD(prevHeader);
   prevSize = status_size(prevHeadInfo);
 }
 //initializing variables...
 word_t* footer = PSUB(block_succ(block),WORD_SIZE); //a pointer that points to the footer of the current block
 word_t currSize = status_size(header); //get the size of the current block
 word_t* succHeader = block_succ(block); //a pointer that points to the header of the successor
 word_t succHeadInfo = LOAD(succHeader); //load info of the header of the successor into succHeadInfo
 word_t succSize = status_size(succHeadInfo); //get the size of the successor
 word_t succUse = status_used(succHeadInfo); //get the status bit of the successor (whether it's used)
 word_t* succFooter = PSUB(block_succ(succHeader),WORD_SIZE); //a pointer that points to the footer of the successor
 //Case 1: if the predecessor is free and the successor is allocated
 if (prevUse == 0 && succUse != 0){
   word_t totalSize = prevSize + currSize; //coalesce with the predecessor, update the total block size
   STORE(prevHeader,make_status(totalSize,prevHeadInfo,0)); //update the block size info in the header of the predecessor
   STORE(footer,totalSize); //store the updated block size into the footer of the current block
 
 //Case 2: if the predecessor is used and the successor is free
 } else if (prevUse != 0 && succUse == 0){
   word_t totalSize = currSize + succSize; //coalesce with the successor, update the total block size
   STORE(block,make_status(totalSize,header,0)); //update the block size info in the header of the current block
   STORE(succFooter,totalSize); //store the updated block size into the footer of the successor
 
 //Case 3: if both the predecessor and the successor are free
 } else if (prevUse == 0 && succUse == 0){
   word_t totalSize = prevSize + currSize + succSize; //coalesce with both the predecessor and the successor and update the total block size
   STORE(prevHeader,make_status(totalSize,prevHeadInfo,0)); //update the block size info in the header of the predecessor
   STORE(succFooter,totalSize); //store the updated block size into the footer of the successor
 }
}
/* Attempt to grow the heap by at least req_size additional bytes.
  Return 1 if success, 0 if ran out of memory. */
static int extend_heap(size_t req_size) {
 // Record the current heap footer address, which will become a new
 // block.
 word_t* block = HEAP_FOOTER_ADDR;
 // Align the required size to page alignment in total_size.
 size_t pagesize = mem_pagesize();
 size_t num_pages = (req_size + pagesize - 1) / pagesize;
 size_t total_size = num_pages * pagesize;
 // Attempt to grow the heap by total_size.
 void* mem_sbrk_result = mem_sbrk(total_size);
 if ((long)mem_sbrk_result == -1) {
   return 0;
 }
 // Initialize the new space as a block starting at the old heap footer.
 // Set header, retaining predecessor status bit.
 STORE(block, make_status(total_size, LOAD(block), 0));
 // Initialize block footer: block size.
 STORE(PADD(block, total_size - WORD_SIZE), total_size);
 // Initialize the new heap footer.
 STORE(PADD(block, total_size), make_status(0, 0, USED_BIT));
 // Coalesce the new block with the former final heap block if possible.
 coalesce(block);
 // Success.
 return 1;
}
/* ALLOCATOR INTERFACE ************************************************\
  The functions {mm_init, mm_malloc, mm_free} constitute the
  interface to the memory allocator.
\**********************************************************************/
/* Initialize the memory allocator and heap. */
int mm_init() {
 // Initial heap size is one page.
 void* mem_sbrk_result = mem_sbrk(mem_pagesize());
 if ((long)mem_sbrk_result == -1) {
   fprintf(stderr, "       in mm_init, returning\n");
   return 1;
 }
 // Size of block = heap size - header word - footer word
 size_t block_size = ((char*)HEAP_FOOTER_ADDR) - ((char*)ORIGIN_BLOCK_ADDR);
 // Block header: indicate that predecessor is used to avoid
 // coalescing beyond start of heap.
 STORE(ORIGIN_BLOCK_ADDR, make_status(block_size, PRED_USED_BIT, 0));
 // Block footer: store only size.
 STORE(PADD(ORIGIN_BLOCK_ADDR, block_size - WORD_SIZE), block_size);
 // Heap footer: masquerade as 0-size allocated block.
 STORE(HEAP_FOOTER_ADDR, make_status(0, 0, USED_BIT));
 return 0;
}
/* Allocate a block of size bytes and return a pointer to its payload. */
void* mm_malloc(size_t payload_size) {
 // Empty payloads not allowed.
 if (payload_size == 0) {
   return NULL;
 }
 // Minimum block size to support the requested payload.
 // Best case: payload + header.
 size_t ideal_block_size = payload_size + WORD_SIZE;
 if (ideal_block_size <= MIN_BLOCK_SIZE) {
   // Block must be at least minimum size.
   ideal_block_size = MIN_BLOCK_SIZE;
 } else {
   // Block size must be aligned. Round up as needed.
   ideal_block_size = ALIGNMENT * ((ideal_block_size + ALIGNMENT - 1) / ALIGNMENT);
 }
 // Search for a suitable free block.
 word_t* block = search(ideal_block_size);
 // If none found, extend the heap and search again.
 if (block == NULL) {
   if (extend_heap(ideal_block_size)) {
     block = search(ideal_block_size);
     assert(block != NULL
            && "search must yield a (non-NULL) block in this case");
   } else {
     // Ran out of memory.
     return NULL;
   }
 }
 // Allocate the selected free block, targeting the ideal size.
 allocate(block, ideal_block_size);
 // A good place for heap consistency checking.
 // Disable this for performance evaluation.
 //printf("calling free")
 //check_heap(0);
 
 // Return the block's payload address.
 return PADD(block, WORD_SIZE);
}
/* Free the allocated-but-not-yet-freed block whose payload is
  referenced by payload_pointer. */
void mm_free(void* payload_pointer) {
// Change tags of block header, footer = size, update succ block header’s pred-used to 0
// Coalesce if there are other free blocks next to it
 word_t* block = PSUB(payload_pointer,WORD_SIZE); //get block, block pointer is one WORD_SIZE away from the payload_pointer
 word_t blockHeader = block_get_header(block);   //get block header
 word_t blockSize = status_size(blockHeader); //get block size
 word_t* blockSucc = block_succ(block); //a pointer that points to the successor of the current block
 word_t blockSuccHeader = block_get_header(blockSucc); //get the header of the successor
 word_t* footer = PSUB(blockSucc,WORD_SIZE); //a pointer that points to the footer of the current block
 //set the header of the current block (set the allocation status bit to 0)
 block_set_header(block,make_status(blockHeader,blockHeader,0));
  STORE(footer,blockSize); //put size into footer
 //set the header of the successor block （set the predecessor status bit of the successor to be 0)
 block_set_header(blockSucc,make_status(blockSuccHeader,0,blockSuccHeader));
 // Read mm_malloc and especially allocate(...) carefully to see what
 // metadata are changed for an allocation.  This is a good hint for
 // what metadata need to be changed when freeing.
 // A good place for heap consistency checking.
 // Disable this for performance evaluation.
 //printf("calling free(%p)\n",block);
 //check_heap(0);
 coalesce(block); //call coalesce on the current block after
 //check_heap(0);
}
 
 


