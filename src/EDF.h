#include <Arduino.h>
#include <esp_timer.h>

#define TASK_NUMBER 4

typedef struct
{
    int period;
    int duration;
    int deadline;
    int priority;
    void (*task_function)(void); // ponteiro para a função
} task_t;

esp_timer_handle_t timer_handle;

task_t tasks[TASK_NUMBER];
int num_tasks = 0;
int index_;
int exec_index = -1;

void addTask(int period,
             int duration,
             int deadline,
             int priority, void (*_function)(void))
{

    task_t task = {
        .period = period,
        .duration = duration,
        .deadline = deadline,
        .priority = priority,
        .task_function = _function};

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
    exec_index = index_;

    esp_timer_start_once(timer_handle, tasks[index_].deadline + tasks[index_].duration);
}

void start_EDF()
{

    esp_timer_create_args_t timer_config = {
        .callback = timer_isr,
        .arg = NULL,
        .name = "my_timer"};
    esp_timer_create(&timer_config, &timer_handle);

    // Inicia a primeira execução no primeiro deadline
    index_ = find_min_deadline_index(tasks, num_tasks);
    task_t minTask = tasks[index_];

    esp_timer_start_once(timer_handle, tasks[index_].deadline + tasks[index_].duration);
}

void update_all_deadlines(int duration)
{
    for (int i = 0; i < num_tasks; i++)
    {
        tasks[i].deadline -= duration;
    }
}

void execFunctionAndUpdateDeadlines()
{
    uint64_t start = esp_timer_get_time();

    tasks[exec_index].task_function();

    uint64_t real_duration = esp_timer_get_time() - start;
    tasks[exec_index].deadline = tasks[exec_index].period + 2 * real_duration - tasks[exec_index].duration;

    update_all_deadlines(real_duration);
}