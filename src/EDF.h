#include <Arduino.h>
#include <esp_timer.h>

#define TASK_NUMBER 4

typedef struct
{
    int period;
    int duration;
    int deadline;
    int priority;
    char taskcode[3];
    void (*task_function)(void); // ponteiro para a função
} task_t;

esp_timer_handle_t timer_handle;

task_t tasks[TASK_NUMBER];
int num_tasks = 0;
int index_;

char minTaskCode[4];

void addTask(int period,
             int duration,
             int deadline,
             const char *taskcode,
             int priority, void (*_function)(void))
{

    task_t task = {
        .period = period,
        .duration = duration,
        .deadline = deadline,
        .priority = priority,
        .task_function = _function};

    strcpy(task.taskcode, taskcode);
    tasks[num_tasks] = task;

    num_tasks++;
}

int find_min_deadline_index(task_t arr[], int size)
{
    int min_deadline = INT_MAX;
    int min_priority = INT_MAX;
    int min_index = -1;

    for (int i = 0; i < size; i++)
    {
        if (arr[i].deadline < min_deadline)
        {
            min_deadline = arr[i].deadline;
            min_priority = arr[i].priority;
            min_index = i;
        }
        else if (arr[i].deadline == min_deadline && arr[i].priority < min_priority)
        {
            min_priority = arr[i].priority;
            min_index = i;
        }
    }
    return min_index;
}

void IRAM_ATTR timer_isr(void *arg)
{
    index_ = find_min_deadline_index(tasks, num_tasks);
    strcpy(minTaskCode, tasks[index_].taskcode);
    Serial.print(tasks[index_].taskcode);
    Serial.print(" - ");
    Serial.println(tasks[index_].deadline);
    esp_timer_start_once(timer_handle, tasks[index_].deadline + tasks[index_].duration);
}

void start_EDF()
{
}

int find_task_by_code(char *taskcode)
{
    for (int i = 0; i < num_tasks; i++)
    {
        if (strcmp(tasks[i].taskcode, taskcode) == 0)
        {
            Serial.println(taskcode);

            return i;
        }
    }
    return -1; // Se não encontrar a tarefa com o código, retorna -1
}

void update_all_deadlines(int duration)
{
    for (int i = 0; i < num_tasks; i++)
    {
        tasks[i].deadline -= duration;
    }
}