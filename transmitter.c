#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>
#include <string.h>

#define NUM_MICRO 25000
#define SYNC_TIME 50000

void transmitChar(char c);
int probe(char* adrs);
void sync();

int main(int argc, char** argv) {
  // Get string
  char* transmitString;
  transmitString = argv[1];
  char currChar;
  int i = 0;
  i = 0;
  
  // Initial synchronization
  sync();

  // Signal start of transmission
  transmitChar(0xff);
  printf("Start signal transmitted.\n");
  sync();
  while ((currChar = transmitString[i]) != 0) {
    transmitChar(currChar);
    i++;
  }
  // Signal end of transmission
  transmitChar(0x00);
  return 0;
}

// Code for transmitting a single character
void transmitChar(char c) {
  bool bit;
  struct timeval  newTime, oldTime;
  for (int j = 7; j >= 0; j--) {
    sync();
    gettimeofday(&oldTime, NULL);
    bit = (c & (1 << j)) >> j;
    if (bit) {
      do {
      // Seemingly useless code designed to ensure continuous cache hits
	int m = 5;
	m++;
	gettimeofday(&newTime, NULL);
      } while ((newTime.tv_sec - oldTime.tv_sec) * 1000000
	       + (newTime.tv_usec - oldTime.tv_usec) < NUM_MICRO);
    }
    else {
      usleep(NUM_MICRO);
    }
  }
}

// Currently unused, but here in case
/*int probe(char* adrs) {
  volatile unsigned long time;
  asm __volatile__ (
     " mfence                \n"
     " lfence                \n"
     " rdtsc                 \n"
     " lfence                \n"
     " movl %%eax, %%esi     \n"
     " movl (%1), %%eax      \n"
     " lfence                \n"
     " rdtsc                 \n"
     " subl %%esi, %%eax     \n"
     " clflush 0(%1)         \n"
     : "=a" (time)
     : "c" (adrs)
     : "%esi", "%edx");
  return time;
}*/

// Synchronize receiver and transmitter using global clock
void sync() {
  struct timeval syncer;
  do {
    gettimeofday(&syncer, NULL);
  } while ((syncer.tv_sec * 1000000 + syncer.tv_usec) % SYNC_TIME > 14000);
}
