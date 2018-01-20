#include "gtest/gtest.h"

#include <time.h>
#include <stdlib.h>

#include "memqueue/memqueue.h"

#define QUEUE_SIZE      16
#define ENTRY_SIZE      sizeof(uint32_t)

class MemqueueTest: public::testing::Test {
public:
    virtual void SetUp()
    {
        uint8_t* data = (uint8_t*) malloc(ENTRY_SIZE * QUEUE_SIZE);
        ASSERT_TRUE(data != NULL);

        int res = mem_queue_init(&queue, ENTRY_SIZE, data, ENTRY_SIZE * QUEUE_SIZE);
        EXPECT_EQ(res, 0);
    }

    virtual void TearDown()
    {
        free(queue.data);
    }

    mem_queue_t queue;
};

TEST_F(MemqueueTest, InitErrorSizeMismatch)
{
    mem_queue_t q;
    uint8_t data[ENTRY_SIZE * QUEUE_SIZE ];
    int res = mem_queue_init(&q, ENTRY_SIZE, data, ENTRY_SIZE * QUEUE_SIZE + 1);
    EXPECT_EQ(res, MEMQUEUE_ERR_LENGTH);
}

TEST_F(MemqueueTest, InitErrorNoData)
{
    mem_queue_t q;
    int res = mem_queue_init(&q, ENTRY_SIZE, NULL, ENTRY_SIZE * QUEUE_SIZE);
    EXPECT_EQ(res, MEMQUEUE_ERR_NULL);
}

TEST_F(MemqueueTest, initState)
{
    uint32_t j;
    int res = mem_queue_read(&this->queue, (void*) &j);
    EXPECT_EQ(res, MEMQUEUE_OK);
}

TEST_F(MemqueueTest, nullData)
{
    int res;
    
    res = mem_queue_read(&this->queue, (void*) NULL);
    EXPECT_EQ(res, MEMQUEUE_ERR_NULL);

    res = mem_queue_write(&this->queue, (void*) NULL);
    EXPECT_EQ(res, MEMQUEUE_ERR_NULL);
}

TEST_F(MemqueueTest, fillEmpty)
{
    int res;
    uint32_t i, j;

    // Fill the queue
    for(i=0; i<QUEUE_SIZE-1; i++) {
        res = mem_queue_write(&this->queue, (void*) &i);
        EXPECT_EQ(res, MEMQUEUE_OK);
    }

    // Check for fill error
    res = mem_queue_write(&this->queue, (void*) &j);
    EXPECT_EQ(res, MEMQUEUE_ERR_FULL);

    // Empty the queue
    for(i=0; i<QUEUE_SIZE-1; i++) {
        res = mem_queue_read(&this->queue, (void*) &j);
        EXPECT_EQ(res, MEMQUEUE_READ_OK);
        EXPECT_EQ(j, i);
    }

    // Check queue returns empty state
    res = mem_queue_write(&this->queue, (void*) &j);
    EXPECT_EQ(res, MEMQUEUE_OK);
}

TEST_F(MemqueueTest, interleaveWriteRead)
{
    int res;
    uint32_t i, j;

    // Half fill the queue
    for(i=0; i<QUEUE_SIZE/2; i++) {
        res = mem_queue_write(&this->queue, (void*) &i);
        EXPECT_EQ(res, MEMQUEUE_OK);
    }

    // Write/Read interleaved
    for(i=0; i<QUEUE_SIZE*2; i++) {
        j = (i + QUEUE_SIZE / 2);
        res = mem_queue_write(&this->queue, (void*) &j);
        EXPECT_EQ(res, MEMQUEUE_OK);

        res = mem_queue_read(&this->queue, (void*) &j);
        EXPECT_EQ(res, MEMQUEUE_READ_OK);
    }

    // Empty the queue
    for(i=0; i<QUEUE_SIZE/2; i++) {
        res = mem_queue_read(&this->queue, (void*) &j);
        EXPECT_EQ(res, MEMQUEUE_READ_OK);
    }

    // Check queue returns empty state
    res = mem_queue_write(&this->queue, (void*) &j);
    EXPECT_EQ(res, MEMQUEUE_OK);
}

TEST_F(MemqueueTest, interleaveReadWrite)
{
    int res;
    uint32_t i, j;

    // Half fill the queue
    for(i=0; i<QUEUE_SIZE/2; i++) {
        res = mem_queue_write(&this->queue, (void*) &i);
        EXPECT_EQ(res, MEMQUEUE_OK);
    }

    // Write/Read interleaved
    for(i=0; i<QUEUE_SIZE*2; i++) {
        res = mem_queue_read(&this->queue, (void*) &j);
        EXPECT_EQ(res, MEMQUEUE_READ_OK);

        j = (i + QUEUE_SIZE / 2);
        res = mem_queue_write(&this->queue, (void*) &j);
        EXPECT_EQ(res, MEMQUEUE_OK);
    }

    // Empty the queue
    for(i=0; i<QUEUE_SIZE/2; i++) {
        res = mem_queue_read(&this->queue, (void*) &j);
        EXPECT_EQ(res, MEMQUEUE_READ_OK);
    }

    // Check queue returns empty state
    res = mem_queue_write(&this->queue, (void*) &j);
    EXPECT_EQ(res, MEMQUEUE_OK);
}
