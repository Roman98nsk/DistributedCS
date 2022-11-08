#include <time.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct options{
    int r;
    int t;
    int id;
};

struct m{
    int t;
    int r;
};

//void ffdh(int T, int weight_line, struct options tasks[], int n, int m);
//void nfdh(int T, int weight_line, struct options tasks[], int n, int m, int general_size[]);

int main() {
    int r_r = 0, t_t = 0, n = 0, m = 0, T = 0, weight_line = 0, alghoritm, counting = 0, same_size = 0;

    printf("Введите количество ЭМ (n): ");
    scanf("%d", &n);

    //итоговая упаковка
    int general_size[n];

    //массив одинаковых задач по полю r
    int same[same_size];
    
    printf("Введите количество задач (m): ");
    scanf("%d", &m);

    struct options tasks[m];

    srand(time(NULL));
    for (int i = 0; i < m; i++) {
        if (m < 5 || m > 5000) {
            printf("Недопустимое значение для количества задач.\n");
            return 0;
        } else {
            r_r = 1 + rand() % n;
            tasks[i].r = r_r;

            t_t = 1 + rand() % 100;
            tasks[i].t = t_t;

            tasks[i].id = i;
        }
    }
    printf("\n");

    int output[m];

    int max = tasks -> r;
    for (int i = 0; i < m; i++) {
        if (tasks[i].r > max) {
            max = tasks[i].r;
        }
    }

    int count[max + 1];
    
    for (int i = 0; i <= max; ++i) {
        count[i] = 0;
    }

    for (int i = 0; i < m; i++) {
        count[tasks[i].r]++;
    }

    for (int i = 1; i <= max; i++) {
        count[i] += count[i - 1];
    }

    for (int i = m - 1; i >= 0; i--) {
        output[count[tasks[i].r] - 1] = tasks[i].r;
        count[tasks[i].r]--;
    }

    for (int i = 0; i < m; i++) {
        tasks[i].r = output[i];
    }

    for (int i = 0; i < m; i++) {
        printf("i = %d, n = %d, m = %d, r = %d, t = %d\n", tasks[i].id, n, m, tasks[i].r, tasks[i].t);
    }

    /*for (int i = 0; i < m; i++) {
        if (tasks[i].r == tasks[i + 1].r) {
            if (same[counting - 1] == tasks[i].r) {
                same[counting] = tasks[i + 1].r;
                i++;
            } else {
                same_size += 2;
                same[counting] = tasks[i].r;
                same[counting + 1] = tasks[i + 1].r;
                counting += 2;
                printf("%d %d ", same[counting], same[counting + 1]);
            }
        }
    }*/

    for (int i = 0; i < m - 1; i++) {
        for (int j = i + 1; j < m; j++) {
            if (tasks[i].r == tasks[j].r) {

                    same_size += 2;
                    same[counting] = tasks[i].r;
                    same[counting + 1] = tasks[j].r;
                    counting += 2;
                    printf("%d %d \n", same[counting], same[counting + 1]);
                }
            }
        }
    

return 0;
}
    /*printf("Введите название алгоритма упаковки: ");
    scanf("%d", &alghoritm);

    switch(alghoritm) {
        case 1:
            ffdh(T, weight_line, tasks, n, m);
            break;

        case 2:
            nfdh(T, weight_line, tasks, n, m, general_size);
            break;
    }

    return 0;
}

void ffdh(int T, int weight_line, struct options tasks[], int n, int m) {
    T = tasks[0].t; //поместили первый элемент
    weight_line = n - tasks[0].r; //посчитали остаток ширины

    for (int i = 1; i < m; i++) {
        if (tasks[i].r <= weight_line) {// если ширина прямоугольника меньше остатка, то помещаем еще прямоугольник
            weight_line -= tasks[i].r;
        } else { //а если нет, то организуем новый уровень
            T += tasks[i].t;
            weight_line = n - tasks[i].r;
        }
    }
    printf("\n(FFDH)Высота упаковки при n = %d равна %d\n", n, T);
    printf("(FFDH)Ширина упаковки при n = %d равна %d\n", n, weight_line);
}

void nfdh(int T, int weight_line, struct options tasks[], int n, int m, int general_size[]) {
    int allocate[m];
 
    memset(allocate, -1, sizeof(allocate));
	
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) { 
            if (general_size[j] >= tasks[i].r) {
                allocate[i] = j;
                general_size[j] -= tasks[i].r;
                break;
            }
        }
    }
    printf("\n(NFDH)Высота упаковки при n = %d равна %d\n", n, T);
    printf("(NFDH)Ширина упаковки при n = %d равна %d\n", n, weight_line);
}*/