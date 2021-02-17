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

  transmitChar(0xff); // signal that transmission will start
  printf("Start signal transmitted.\n");
  sync();
  while ((currChar = transmitString[i]) != 0) {
    transmitChar(currChar);
    i++;
  }
  transmitChar(0x00);
  transmitChar(0x00);
  return 0;
}

void transmitChar(char c) {
  bool bit;
  struct timeval  newTime, oldTime;
  // gettimeofday(&oldTime, NULL);
  for (int j = 7; j >= 0; j--) {
    sync();
    gettimeofday(&oldTime, NULL);
    bit = (c & (1 << j)) >> j;
    if (bit) {
      do {
	int m = 5;
	m++;
	// dummy code to ensure sufficient separation between instructions
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m - 7;
	m = m - 13;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m - 7;
	m = m - 13;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m - 7;
	m = m - 13;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m - 7;
	m = m - 13;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m + 8;
	m = m - 7;
	m = m - 13;
	m = m - 7;
	m = m - 13;
	gettimeofday(&newTime, NULL);
      } while ((newTime.tv_sec - oldTime.tv_sec) * 1000000
	       + (newTime.tv_usec - oldTime.tv_usec) < NUM_MICRO);
      //printf("Transmitted 1\n");
    }
    else {
      usleep(NUM_MICRO);
      // do {
      //clock_gettime(CLOCK_REALTIME, &newTime);
      //} while (newTime.tv_nsec - oldTime.tv_nsec < 100000);
      //  printf("Transmitted 0\n");
    }
    //sync();
  }
}

int probe(char* adrs) {
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
}

void sync() {
  struct timeval syncer;
  do {
    gettimeofday(&syncer, NULL);
  } while ((syncer.tv_sec * 1000000 + syncer.tv_usec) % SYNC_TIME > 14000);
}
