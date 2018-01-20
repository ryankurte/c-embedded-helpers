
set(MEMQUEUE_SRC ${CMAKE_CURRENT_LIST_DIR}/memqueue.c)
set(TEST_SRC ${TEST_SRC} ${CMAKE_CURRENT_LIST_DIR}/memqueue_test.cpp)

add_library(cem-memqueue ${MEMQUEUE_SRC})

set(CEM_LIBS ${CEM_LIBS} cem-memqueue)
