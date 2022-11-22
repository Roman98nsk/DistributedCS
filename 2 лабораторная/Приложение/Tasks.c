#include <time.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct options{
    int r;
    int t;
    int id;
};

typedef struct package{
    int *ID;
    int R;
    int Teta;
    
} Stack_t;

//void ffdh(int T, int weight_line, struct options tasks[], int n, int m);
//void nfdh(int T, int weight_line, struct options tasks[], int n, int m, int general_size[]);

int main() {
    int r_r = 0, t_t = 0, n = 0, m = 0, T = 0, weight_line = 0, alghoritm, currentElement = 0, maxT = 0, count_package = 1, r;

    printf("Введите количество ЭМ (n): ");
    scanf("%d", &n);

    int general_size[n];
    
    printf("Введите количеисво задач (m): ");
    scanf("%d", &m);

    struct options tasks[m];

    srand(time(NULL));
    for (int i = 0; i < m; i++) {
        if (m < 5 || m > 5000) {
            printf("Неверное значение для количества задач.\n");
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
    printf("\n");

    //Количество укрупненных задач
    for (int i = 1; i < m; ++i) {
        for (r = 0; r < i && tasks[i].r != tasks[r].r; ++r)
            ;
        count_package += (r == i);
    }
    printf("\nКоличество пакетов = %d\n", count_package);

    //Поиск максимального t
    maxT = tasks[0].t;
    for (int i = 0; i < m; i++) {
        if (tasks[i].t > maxT) {
            maxT = tasks[i].t;
        }
    }
    printf("t максимальное = %d\n", maxT);

    struct package bigOption[count_package];

    for (int i = 0; i < count_package; i++) {
        bigOption[i].Teta = 10 * maxT;
    }
    printf("Ɵ = %d\n\n", bigOption[0].Teta);



    //Поиск повторов по полю r
    /*for (int i = 0; i < m; i++) {
		currentElement = tasks[i].r;
		bool Equals = false;

		for (int r = 0; r < m; ++r) {
			if (i == r) {
				continue;
			}
            if (currentElement != tasks[r].r && i != r) {
				Equals = true;
			}
		    if (currentElement == tasks[r].r && i != r) {
				Equals = true;
			}
        }

		if (Equals) {
            printf("tasks[%d].r = %d\n\n", i, currentElement);
        }
    }*/
    /*
    int repeat = 0;
    int numarr = 0;
    int bigoption = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            if (tasks[numarr].r == tasks[j].r) {
                repeat++;
                bigOption[bigoption].R = tasks[numarr].r;
                
            }
        }
        printf("%d повторилось %d раз\n", tasks[numarr].r, repeat);
        printf("Ширина R пакета [%d] = %d,\tвысота Ɵ пакета [%d] = %d\n\n", bigoption, bigOption[bigoption].R, bigoption, bigOption[0].Teta);
        repeat = 0;
        numarr++;
        bigoption++;
    }*/

    return 0;
}

Stack_t* createStack() {
    Stack_t *out = NULL;

    out = malloc (sizeof(Stack_t));
    if (out == NULL) {
        exit(-1);
    }

    out -> size = 10;
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