/*
 *  Задание #6
 *  Автор: Олег Уткин
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "map.h"

/*
 * Обрабатывает входные аргументы
 * и выполняет соответствующие операции с БД
 */
int process(COUNTRY ** map);
void undecorate_name(char * name);

int main(int argc, char * argv[])
{
    COUNTRY ** map = map_load();

    int err = process(map);
    if (err != 0) {
        return 1;
    }

    /* Удаление списка из динамической памяти */
    map_clear(map);

    return 0;
}

int process(COUNTRY ** map)
{
    printf("> ");
    char operator[8];
    while (scanf("%7s", operator)) {
        if (!strcmp(operator, "add")) {
            char name[128];
            int population = 0;
            int area = 0;
            if (scanf("%s %d %d", name, &population, &area) != 3) {
                return 1;
            }
            undecorate_name(name);

            map_add(map, name, population, area);
        } else if (!strcmp(operator, "delete")) {
            char name[128];
            if (scanf("%127s", name) != 1) {
                return 1;
            }
            undecorate_name(name);

            map_delete(map, name);
        } else if (!strcmp(operator, "dump")) {
            map_dump(map);
        } else if (!strcmp(operator, "view")) {
            char name[128];
            if (scanf("%127s", name) != 1) {
                return 1;
            }
            undecorate_name(name);

            COUNTRY * country = map_find(map, name);
            if (country != NULL) {
               print_country(country);
            } else {
                printf("not found\n");
            }
        } else if (!strcmp(operator, "save")) {
            map_save(map);
        } else if (!strcmp(operator, "quit")) {
            return 0;
        } else {
            printf("invalid command: '%s'\n", operator);
        }

        printf("> ");
    }

    return 0;
}

void undecorate_name(char * name)
{
    int cnt;
    while (name[cnt]) {
        if (name[cnt] == '_')
            name[cnt] = ' ';
        cnt++;
    }
}
