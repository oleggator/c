/*
 *  Задание #6
 *  Автор: Олег Уткин
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

/*
 * Обрабатывает входные аргументы
 * и выполняет соответствующие операции с БД
 */
int process(COUNTRY ** list, int argc, char * argv[]);

int main(int argc, char * argv[])
{
    if (argc < 2 || argc > 5) {
        return 1;
    }
    
    COUNTRY * list = load();

    int err = process(&list, argc, argv);

    /* Удаление списка из динамической памяти */
    clear(list);

    return err;
}

int process(COUNTRY ** list, int argc, char * argv[]) {
    const char* operator = argv[1];
    if (!strcmp(operator, "add")) {
        if (argc != 5) {
            printf("wrong command 'add' format\n");
            return 1;
        }

        char * name = argv[2];
        int population = atoi(argv[3]);
        int area = atoi(argv[4]);
        int err = add(list, name, population, area);
        if (err != 0) {
            printf("key '%s' addition error\n", name);
            return 1;
        }

        save(*list);
    } else if (!strcmp(operator, "count")) {
        if (argc != 3) {
            printf("wrong command 'count' format\n");
            return 1;
        }

        printf("rows count: %d", count(*list));
    } else if (!strcmp(operator, "delete")) {
        if (argc != 3) {
            printf("wrong command 'delete' format\n");
            return 1;
        }

        char * name = argv[2];
        COUNTRY * country = find(*list, name);
        if (country == NULL) {
            printf("key '%s' wasn't found\n", name);
            return 1;
        }

        delete(list, country);
        save(*list);
    } else if (!strcmp(operator, "dump")) {
        if (argc != 3) {
            printf("wrong command 'dump' format\n");
            return 1;
        }

        const char * flag = argv[2];
        int err = 0;
        if (!strcmp(flag, "-a")) {
            err = sort_by_area(list);
        } else if (!strcmp(flag, "-n")) {
            err = sort_by_name(list);
        } else if (!strcmp(flag, "-p")) {
            err = sort_by_population(list);
        } else {
            printf("wrong command 'dump' format: invalid flag '%s'\n", flag);
            return 1;
        }

        if (err != 0) {
            printf("sorting error\n");
            return 1;
        }

        dump(*list);
    } else if (!strcmp(operator, "view")) {
        if (argc != 3) {
            printf("wrong command 'view' format\n");
            return 1;
        }

        char * name = argv[2];
        COUNTRY * country = find(*list, name);
        if (country == NULL) {
            printf("key '%s' wasn't found\n", name);
            return 1;
        }

        print_country(country);
    } else {
        printf("invalid command: '%s'\n", operator);
        return 1;
    }

    return 0;
}
