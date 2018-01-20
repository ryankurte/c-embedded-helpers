/**
 * A simple fifo memory queue (or ring buffer) in c.
 * This replaces a memory pool and queue combination and \should be\ "thread safe" for single producer/consumer 
 * with atomic writes of size_t.
 * This is because the head and tail indexes are only written by the producer and consumer respectively.
 * Note that empty is head==tail, thus only QUEUE_SIZE-1 entries may be used.
 * 
 * Copyright 2018 Ryan Kurte
 */

#ifndef MEM_QUEUE_H
#define MEM_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>

#define MEMQUEUE_OK              0 //!< No error occured (but no action either)
#define MEMQUEUE_READ_OK         1 //!< Memqueue read succeeded (data is valid)
#define MEMQUEUE_ERR_FULL       -1 //!< Memqueue is full
#define MEMQUEUE_ERR_LENGTH     -2 //!< Provided data array does not fit an integer number of elements
#define MEMQUEUE_ERR_NULL       -3 //!< Provided data was NULL

// mem_queue_t is a memory queue for use where a queue + memory pool combination would be used
typedef struct {
    size_t head;        //!< Head (leading) index
    size_t tail;        //!< Tail (following) index
    size_t size;        //!< Size (in entries) of queue
    size_t entry_size;  //!< Size of entries in bytes
    size_t data_len;    //!< Length of data storage in bytes
    uint8_t *data;        //!< Data storage array
} mem_queue_t;

// Initialse a memory queue with a provided entry size and data storage array
int mem_queue_init(mem_queue_t *queue, size_t entry_size, uint8_t* data, size_t data_len);

// Read an entry of entry_size from the memory queue into the provided handle
int mem_queue_read(mem_queue_t *queue, void* handle);

// Write an element of entry_size to the memory queue from the provided handle
int mem_queue_write(mem_queue_t *queue, void* handle);

#ifdef __cplusplus
}
#endif

#endif
