#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define HASH_SIZE 243
#define STATE_SIZE (HASH_SIZE * 3) //729
#define NUMBER_OF_ROUNDS 9

static int F[] = { 0, -1, 1, 0, 1, -1, -1, 1, 0 };

static int INDICES[STATE_SIZE] = { 0 }; //729 * 4 bytes
static int state[STATE_SIZE] = { 0 }; //729 * 4 bytes
static int leftPart[STATE_SIZE] = { 0 };
static int rightPart[STATE_SIZE] = { 0 };

static int f(int a, int b) {
  return F[(a + 1) * 3 + (b + 1)];
}

static void transform() {

  int index = 0;
  int indexNext = 0;
  int round = 0;
  int i = 0;

  for (round = NUMBER_OF_ROUNDS; round-- > 0; ) {

    for (i = 0; i < STATE_SIZE; i++) {
      int a, b;

      indexNext = INDICES[i];
      a = state[index];
      b = state[indexNext];
      
      index = indexNext;
      leftPart[i] = f(a, b);
      rightPart[i] = f(b, a);
    }

    for (i = 0; i < STATE_SIZE; i++) {

      indexNext = INDICES[i];
      state[i] = f(leftPart[index], rightPart[indexNext]);
      index = indexNext;
    }
  }
}

void init() {
  int index = 0;
  int i = 0;

  for (i = 0; i < STATE_SIZE; i++) {

    index += (index <= STATE_SIZE / 2 ? STATE_SIZE / 2 : (STATE_SIZE / 2 - STATE_SIZE));
    INDICES[i] = index;
  }
}

void reset() {
  int i = 0;

  for (i = 0; i < STATE_SIZE; i++) {

    state[i] = 0;
  }
}

void absorb(int input[], int offset, int size) {

  do {

    memcpy(state, input + offset, size < HASH_SIZE ? size : HASH_SIZE);

    transform();

    offset += HASH_SIZE;
    size -= HASH_SIZE;

  } while (size > 0);
}

void main()
{
  long count = 0;
  double totalDiff = 0;
  time_t start, end;

  init();
  
  fprintf(stderr, "Starting SaM profiling\r\n");
  time(&start);

  while(1) {
    transform();
    count++;
    time(&end);

    totalDiff = difftime(end, start);
    if (totalDiff >= 1.0) break;
  }
  
  fprintf(stderr, "Total %ld transforms per 1 second.\r\n", count);
  getchar();
}