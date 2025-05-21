#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <alchemy/task.h>
#include <alchemy/queue.h>
#include <stdint.h>

#define QUEUE_NAME "mq_queue"
#define MAX_MSG_SIZE 1024
#define MAX_MSG_COUNT 10
#define MAX_THREAD_COUNT 100

RT_TASK pi_task[MAX_THREAD_COUNT];
RT_QUEUE mq[MAX_THREAD_COUNT];

int pointsPerTask;

float MonteCarlo(int points) {
  int k = 0;
  float x, y;
  for (int j = 1; j <= points; j++) {
    x = (float)rand() / RAND_MAX;
    y = (float)rand() / RAND_MAX;
    if (x*x + y*y <= 1) {
      k++;
    }
  }

  return 4.0 * k / (float)points;
}

void task(void *arg) {
  char task_name[32];
  RT_QUEUE q;
  float pi;

  sprintf(task_name, "%s-%d", QUEUE_NAME, (int)(uintptr_t)arg);
  rt_queue_bind(&q, task_name, TM_INFINITE);

  srand(time(NULL) + (uintptr_t)arg);
  pi = MonteCarlo(pointsPerTask);

  rt_queue_write(&q, &pi, sizeof(float), Q_NORMAL);
  rt_queue_unbind(&q);
}

int main() {
  int threads = 100;
  float sum = 0.0;

  pointsPerTask = 100;

  for (int i = 0; i < threads; ++i) {
    char task_name[32];
    sprintf(task_name, "%s-%d", QUEUE_NAME, i);
    rt_queue_create(&mq[i], task_name, sizeof(float), MAX_MSG_COUNT, Q_FIFO);
    rt_task_create(&pi_task[i], task_name, 0, 50, 0);
    rt_task_start(&pi_task[i], &task, (void *)(uintptr_t)i);
  }

  for (int i = 0; i < threads; ++i) {
    float pi;
    rt_queue_read(&mq[i], &pi, sizeof(float), TM_INFINITE);
    sum += pi;
    rt_queue_delete(&mq[i]);
    rt_task_delete(&pi_task[i]);
  }

  double pi = sum / threads;
  printf("Pi: %.5f\n", pi);

  return 0;
}

