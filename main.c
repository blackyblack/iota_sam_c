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

static int f(int a, int b) {
  return F[(a + 1) * 3 + (b + 1)];
}

static void transform() {

  int index = 0;
  int leftPart[STATE_SIZE] = { 0 };
  int rightPart[STATE_SIZE] = { 0 };

  for (int round = NUMBER_OF_ROUNDS; round-- > 0; ) {

    for (int i = 0; i < STATE_SIZE; i++) {

      int a, b;
      leftPart[i] = f(a = state[index], b = state[index = INDICES[i]]);
      rightPart[i] = f(b, a);
    }

    for (int i = 0; i < STATE_SIZE; i++) {

      state[i] = f(leftPart[index], rightPart[index = INDICES[i]]);
    }
  }
}

void init() {
  int index = 0;
  for (int i = 0; i < STATE_SIZE; i++) {

    INDICES[i] = index += (index <= STATE_SIZE / 2 ? STATE_SIZE / 2 : (STATE_SIZE / 2 - STATE_SIZE));
  }
}

void reset() {

  for (int i = 0; i < STATE_SIZE; i++) {

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
  init();
  
  fprintf(stderr, "Starting SaM profiling\r\n");
  time_t start, end;
  time(&start);

  long count = 0;
  double totalDiff = 0;

  while(1) {
    transform();
    count++;
    time(&end);

    double dif = difftime(end, start);
    totalDiff += dif;

    if (totalDiff >= 1.0) break;

    time(&start);
  }
  
  fprintf(stderr, "Total %ld transforms per 1 second.\r\n", count);
  getchar();
}