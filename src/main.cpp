#include <EDF.h>

void executeA()
{
  Serial.println("run A");
  delay(200);
}

void executeB()
{
  delay(100);
  Serial.println("run B");
}

void setup()
{
  delay(2000);
  addTask(50000, 200, 50, "001", 1, &executeA);
  addTask(25000, 100, 40, "002", 1, &executeB);
  Serial.begin(115200);
  esp_timer_create_args_t timer_config = {
      .callback = timer_isr,
      .arg = NULL,
      .name = "my_timer"};
  esp_timer_create(&timer_config, &timer_handle);

  // Inicia a primeira execução no primeiro deadline
  index_ = find_min_deadline_index(tasks, num_tasks);
  task_t minTask = tasks[index_];

  esp_timer_start_once(timer_handle, millis() - tasks[index_].deadline + tasks[index_].duration);
}

void loop()
{
  int i = find_task_by_code(minTaskCode);
  if (i != -1)
  {
    int start = millis();
    Serial.println(minTaskCode);

    tasks[i].task_function();

    int real_duration = millis() - start;

    tasks[i].deadline += tasks[i].period + 2 * real_duration - tasks[i].duration;

    update_all_deadlines(real_duration);

    strcpy(minTaskCode, "000");
  };
  // put your main code here, to run repeatedly:
}
