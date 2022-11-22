#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

#define N 1024

#define MAX_TIME 100

typedef struct
{
    uint32_t id;
    uint32_t h;
    uint32_t w;
} task_t;

typedef struct
{
    task_t task;
    uint32_t start_time;
    uint32_t end_time;
    char node_range[12];
} task_schedule_t;

typedef struct
{
    task_schedule_t *schedules;
    uint32_t task_count;
    uint32_t time_max;
} schedules_t;

typedef struct level_t
{
    uint32_t time;
    uint32_t cnt_free;
    struct level_t *next;
} level_t;

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

double disp_mat (double eps){
    double summand1 = 0, summand2 = 0;
    double mat_wait = 0, dispersion = 0;
    summand1 += eps * eps;
    summand2 += eps;
    summand1 /= 10;
    summand2 /= 10;
    mat_wait = summand2;
    printf("Мат. ожидание = %10lf\n", mat_wait);
    summand2 *= summand2;
    dispersion = summand1 - summand2;
    return dispersion;
}

void sort_step_one(schedules_t *sch)
{
    if (!sch || !sch->schedules || sch->task_count <= 0)
    {
        return;
    }
    double factor = 1.247;
    int step = sch->task_count - 1;
    task_schedule_t temp;
    while (step >= 1)
    {
        for (int i = 0; i < sch->task_count - step; i++)
        {
            if (sch->schedules[i].task.h < sch->schedules[i + step].task.h)
            {
                temp = sch->schedules[i];
                sch->schedules[i] = sch->schedules[i + step];
                sch->schedules[i + step] = temp;
            }
        }
        step /= factor;
    }
}

void sort_step_two(schedules_t *sch)
{
    if (!sch || !sch->schedules || sch->task_count <= 0)
    {
        return;
    }
    double factor = 1.247;
    int step = sch->task_count - 1;
    task_schedule_t temp;
    while (step >= 1)
    {
        for (int i = 0; i < sch->task_count - step; i++)
        {
            if (sch->schedules[i].task.h == sch->schedules[i + step].task.h &&
                sch->schedules[i].task.w < sch->schedules[i + step].task.w)
            {
                temp = sch->schedules[i];
                sch->schedules[i] = sch->schedules[i + step];
                sch->schedules[i + step] = temp;
            }
        }
        step /= factor;
    }
}

void NFDH_algorithm(schedules_t *sch)
{
    if (!sch || !sch->schedules || sch->task_count <= 0)
    {
        return;
    }
    int t_level = 0;
    int cnt_free = 0;
    sort_step_one(sch);
    sort_step_two(sch);
    for (int i = 0; i < sch->task_count; i++)
    {
        if (sch->schedules[i].task.w > cnt_free)
        {
            t_level = sch->time_max;
            sch->time_max += sch->schedules[i].task.h;
            cnt_free = N;
        }
        int start_rang = N - cnt_free;
        cnt_free -= sch->schedules[i].task.w;
        int end_rang = N - cnt_free - 1;
        sch->schedules[i].start_time = t_level;
        sch->schedules[i].end_time = t_level + sch->schedules[i].task.h;
        sprintf(sch->schedules[i].node_range, "%d-%d", start_rang, end_rang);
    }
}

void FFDH_algorithm(schedules_t *sch)
{
    if (!sch || !sch->schedules || sch->task_count <= 0)
    {
        return;
    }
    level_t *levels = malloc(sizeof(level_t));
    level_t *current_level;
    levels->time = 0;
    levels->cnt_free = N;
    levels->next = NULL;
    sort_step_one(sch);
    sort_step_two(sch);
    sch->time_max = sch->schedules[0].task.h;
    for (int i = 0; i < sch->task_count; i++)
    {
        current_level = levels;
        while (sch->schedules[i].task.w > current_level->cnt_free)
        {
            if (current_level->next)
            {
                current_level = current_level->next;
            }
            else
            {
                current_level->next = malloc(sizeof(level_t));
                current_level = current_level->next;
                current_level->time = sch->time_max;
                current_level->cnt_free = N;
                current_level->next = NULL;
                sch->time_max += sch->schedules[i].task.h;
            }
        }
        int start_rang = N - current_level->cnt_free;
        current_level->cnt_free -= sch->schedules[i].task.w;
        int end_rang = N - current_level->cnt_free - 1;
        sch->schedules[i].start_time = current_level->time;
        sch->schedules[i].end_time = current_level->time + sch->schedules[i].task.h;
        sprintf(sch->schedules[i].node_range, "%d-%d", start_rang, end_rang);
    }
    while (levels)
    {
        current_level = levels;
        levels = levels->next;
        free(current_level);
    }
}

int main(int argc, char *argv[]){
    FILE *file1 = fopen(argv[1], "w");
    FILE *file2 = fopen(argv[2], "w");
    fprintf(file1, "%10s\t%10s\t%10s\t%10s\t%10s\n", "cnt_tasks", "t_max", "t_low_bnd", "eps", "alg_time");
    fprintf(file2, "%10s\t%10s\t%10s\t%10s\t%10s\n", "cnt_tasks", "t_max", "t_low_bnd", "eps", "alg_time");
    
    int id_alg;
    double deviantion;
    double dispersion = 0;

    printf("1 - NFDH\n2 - FFDH\n");
    printf("Выберите алгоритм: ");
    scanf("%d", &id_alg);
    printf("\nВы ввели: %d\n", id_alg);

    for (int num_tasks = 500; num_tasks <= 5000; num_tasks += 500){
            double alg_time;
            double deviantion;
            schedules_t sch = {.schedules = NULL, .task_count = 0, .time_max = 0};
            sch.task_count = num_tasks;
            sch.schedules = malloc(sizeof(task_schedule_t) * num_tasks);
            memset(sch.schedules, 0, sizeof(task_schedule_t) * num_tasks);
            double t_low_bound = 0;     
            double eps;
            srand(time(NULL));
            for (int j = 0; j < num_tasks; j++)
            {
                sch.schedules[j].task.h = rand() % MAX_TIME + 1;
                sch.schedules[j].task.w = rand() % N + 1;
                sch.schedules[j].task.id = j + 1;
            }
            alg_time = wtime();
            switch (id_alg)
            {
            case 1:
                NFDH_algorithm(&sch);
                break;
            case 2:
                FFDH_algorithm(&sch);
                break;
            }
            alg_time = wtime() - alg_time;
            for (int j = 0; j < num_tasks; j++)
            {
                // T` = 1/n * sum(rj*tj)
                t_low_bound += sch.schedules[j].task.w * sch.schedules[j].task.h;
            }
            t_low_bound /= N;
            // e = (T(S) - T`)/T`
            eps = (sch.time_max - t_low_bound) / t_low_bound;
            
            switch (id_alg)
            {
            case 1:
                fprintf(file1, "%10d\t", num_tasks);
                fprintf(file1, "%10d\t", sch.time_max);
                fprintf(file1, "%10.0lf\t", t_low_bound);
                fprintf(file1, "%10lf\t", eps);
                fprintf(file1, "%10lf\n", alg_time);  

                printf("\nTasks: %d\n", num_tasks);                            
    
                dispersion = disp_mat(eps);
                printf("Дисперсия = %10lf\n", dispersion);

                deviantion = sqrt(dispersion);
                printf("Среднеквадратическое отклонение е = %.10f\n\n", deviantion);  
                break;   
                
            case 2:
                fprintf(file2, "%10d\t", num_tasks);
                fprintf(file2, "%10d\t", sch.time_max);
                fprintf(file2, "%10.0lf\t", t_low_bound);
                fprintf(file2, "%10lf\t", eps);
                fprintf(file2, "%10lf\n", alg_time);
                
                 printf("\nTasks: %d\n", num_tasks);                            
    
                dispersion = disp_mat(eps);
                printf("Дисперсия = %10lf\n", dispersion);

                deviantion = sqrt(dispersion);
                printf("Среднеквадратическое отклонение е = %.10f\n\n", deviantion); 

                break;
            } 
            free(sch.schedules);

    }

    fclose(file1);
    fclose(file2);
    return 0;
}