/**
 * A simple fifo memory queue (or ring buffer) in c.
 * This replaces a memory pool and queue combination and \should be\ "thread safe" for single producer/consumer 
 * with atomic writes of size_t.
 * This is because the head and tail indexes are only written by the producer and consumer respectively.
 * Note that empty is head==tail, thus only QUEUE_SIZE-1 entries may be used.
 * 
 * Copyright 2018 Ryan Kurte
 */

#include "memqueue/memqueue.h"

#include <stdlib.h>
#include <string.h>

int mem_queue_init(mem_queue_t *queue,size_t entry_size, uint8_t* data, size_t data_len) {
    queue->tail = 0;
    queue->head = 0;

    queue->entry_size = entry_size;

    // Check that the data array can contain an integer number of entries
    if ((data_len == 0) || (data_len % entry_size != 0)) {
        return MEMQUEUE_ERR_LENGTH;
    }

    // Check that data array is not null
    if (data == NULL) {
        return MEMQUEUE_ERR_NULL;
    }

    // Set element count from entry_size and data_len
    queue->size = data_len / entry_size;

    // Store data array info
    queue->data = data;
    queue->data_len = data_len;
    
    return MEMQUEUE_OK;
}

int mem_queue_read(mem_queue_t *queue, void* data) {
    // Check for null data pointer
    if (data == NULL) {
        return MEMQUEUE_ERR_NULL;
    }

    // Check if data exists
    if (queue->tail == queue->head) {
        return MEMQUEUE_OK;
    }

    // Fetch current handle
    void* handle = &queue->data[queue->tail * queue->entry_size];

    // Copy data
    memcpy(data, handle, queue->entry_size);
    
    // Increment tail (following) pointer
    queue->tail = (queue->tail + 1) % queue->size;

    return MEMQUEUE_READ_OK;
}

int mem_queue_write(mem_queue_t *queue, void* data) {
    // Check for null data pointer
    if (data == NULL) {
        return MEMQUEUE_ERR_NULL;
    }

    // Check there is space in the queue
    if (((queue->head + 1) % queue->size) == queue->tail) {
        return MEMQUEUE_ERR_FULL;
    }

    // Copy data into queue
    void* handle = &queue->data[queue->head * queue->entry_size];
    memcpy(handle, data, queue->entry_size);

    // Update head (leading) pointer
    queue->head = (queue->head + 1) % queue->size;

    return MEMQUEUE_OK;
}
