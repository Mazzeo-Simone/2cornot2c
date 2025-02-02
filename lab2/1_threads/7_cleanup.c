/***********************************************************************
* Code listing from "Advanced Linux Programming," by CodeSourcery LLC  *
* Copyright (C) 2001 by New Riders Publishing                          *
* See COPYRIGHT for license information.                               *
***********************************************************************/
#include <stdio.h>
#include <malloc.h>
#include <pthread.h>

/* Allocate a temporary buffer.  */

void* allocate_buffer (size_t size)
{
  return malloc (size);
}

/* Deallocate a temporary buffer.  */

void deallocate_buffer (void* buffer)
{
  printf("Called cleanup handler function\n");
  free (buffer);
}

void* do_some_work ()
{
  /* Allocate a temporary buffer.  */
  void* temp_buffer = allocate_buffer (1024);
  /* Register a cleanup handler for this buffer, to deallocate it in
     case the thread exits or is cancelled.  */
  pthread_cleanup_push (deallocate_buffer, temp_buffer);

  /* Do some work here that might call pthread_exit or might be
     cancelled...  */
  pthread_exit(0);
  /* Unregister the cleanup handler.  Since we pass a non-zero value,
     this actually performs the cleanup by calling
     deallocate_buffer.  */
  pthread_cleanup_pop (1);
  
  return NULL;
}

int main(void){
  pthread_t allocator_thread;
  pthread_create(&allocator_thread, NULL, do_some_work, NULL);
  pthread_join(allocator_thread, NULL);
  return 0;

}
