#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>

//#define PROBE_ADDR mappedAddr + (0x1380 & ~0x3f)
#define PROBE_ADDR mappedAddr + 0x1380
#define NUM_MICRO 25000
#define SYNC_TIME 50000
//#define DECISION_THRESHOLD 300000

int probe(char* address);
void sync();
void printBytes(char transmittedString[]);

int main(int argc, char **argv) {
  // v[1] points to the file name
  if (argc < 2) {
    perror("ERROR: File name not given.\n");
    exit(1);
  }
  int fileDescriptor = open(argv[1], O_RDONLY); // open file; read only
  if (fileDescriptor < 0) {
    perror("ERROR: Failed to open file.\n");
    exit(-1);
  }
  // get the size of the file
  struct stat stats;
  fstat(fileDescriptor, &stats);
  int size = stats.st_size;
  char *mappedAddr = (char *)mmap(NULL, size, PROT_READ, MAP_PRIVATE,
			   fileDescriptor, 0);
  // Create clock
  struct timeval oldTime, newTime;
  int transmit1;
  // Remove noise
  probe(PROBE_ADDR);
  int threshold = 171;
  char transmittedChar = 0;
  char transmittedString[256] = {'a'};
  int charNum = 0;
  int numHits;
  int numMisses;
  int i = 0;
  int startSignal = 0;
  // Get start signal
  gettimeofday(&oldTime, NULL);
  while (!startSignal) {
    usleep(1000);
    numHits = 0;
    numMisses = 0;
    do {
      transmit1 = probe(PROBE_ADDR);
      numHits += (transmit1 < threshold);
      numMisses += (transmit1 >= threshold);
      gettimeofday(&newTime, NULL);
    } while ((newTime.tv_sec - oldTime.tv_sec) * 1000000 +
	     (newTime.tv_usec - oldTime.tv_usec) < NUM_MICRO);
    //gettimeofday(&oldTime, NULL);
    sync();
    gettimeofday(&oldTime, NULL);
    //printf("numHits, numMisses: %d, %d\n", numHits, numMisses);
    transmittedChar = transmittedChar << 1;
    if (numHits > numMisses)
      transmittedChar = transmittedChar | 0x1;
    if (transmittedChar == 0xffffffff) startSignal = 1;
    // printf("numHits, numMisses: %d, %d\n", numHits, numMisses);
    //   printf("transmittedChar %x\n", transmittedChar);
  }
  printf("Start signal received.\n");
  sync();
  gettimeofday(&oldTime, NULL);
  i = 0;
  transmittedChar = 0;
  do {
    i = 0;
    while (i < 8) {
      usleep(1000);
      numHits = 0;
      numMisses = 0;
      do {
	transmit1 = probe(PROBE_ADDR);
	numHits += (transmit1 < threshold);
	numMisses += (transmit1 >= threshold);
	gettimeofday(&newTime, NULL);
      } while ((newTime.tv_sec - oldTime.tv_sec) * 1000000 +
	     (newTime.tv_usec - oldTime.tv_usec) < NUM_MICRO);
      sync();
      gettimeofday(&oldTime, NULL);
      // printf("numHits, numMisses: %d, %d\n", numHits, numMisses);
      transmittedChar = transmittedChar << 1;
      if (numHits > numMisses)
	transmittedChar = transmittedChar | 0x1;
      i++;
      // printf("numHits, numMisses: %d, %d\n", numHits, numMisses);
    }
    //printf("transmittedChar: %c\n", transmittedChar);
    transmittedString[charNum] = transmittedChar;
    charNum++;
  } while (transmittedChar != 0);
   printf("Transmitted string:\n%s\n", transmittedString);
   printBytes(transmittedString);
   return 0;
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
  usleep(25);
  return time;
}

void sync() {
  struct timeval syncer;
  do {
    gettimeofday(&syncer, NULL);
  } while ((syncer.tv_sec * 1000000 + syncer.tv_usec) % SYNC_TIME > 14000);
}

void printBytes(char transmittedString []) {
  printf("Bytes transmitted:\n");
  int i = 0;
  while (transmittedString[i] != '\0') {
    printf("0x%d ", transmittedString[i]);
    i++;
  }
  printf("0x%d\n", (unsigned char)transmittedString[i]);
}
