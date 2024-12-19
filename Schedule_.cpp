#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

typedef struct {
    int day, month, year;
}date;
//name, hard, time,done, when_done, description, deadline
typedef struct {
    char name[20];
    date deadline, done_date;
    int hard, time, done;
    char description[500];
}task;
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int correct_int(const char* prompt) {
    int num;
    char term;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d%c", &num, &term) != 2 || term != '\n') {
            printf("ERROR: Input integer.\n");
            clear_input_buffer();
        }
        else {
            return num;
        }
    }
}


void correct_str(const char* prompt, char* str, int max_length) {
    while (1) {
        printf("%s", prompt);
        if (fgets(str, max_length, stdin) == NULL) {
            printf("Line read error.\n");
        }
        else {
            str[strcspn(str, "\n")] = 0;
            if (strlen(str) > 0) {
                return;
            }
            else {
                printf("ERROR: Input non-empty line.\n");
            }
        }
    }
}

date correct_date(const char* promt) {
    while (1) {
        int day, month, year;
        char c1, c2;
        printf("%s", promt);
        if ((scanf("%d%c%d%c%d", &day, &c1, &month, &c2, &year) == 5) && (day > 0) && (month > 0) && (year > 0) && (day < 32) && (month < 13) && ((c1 == '.') || (c1 == '\n')) && ((c2 == '.') || (c2 == '\n'))) {
            date a{ day,month,year };
            return a;
        }
        printf("Plese input correct format of date 'dd.mm.yyyy'\n");
        clear_input_buffer();
    }
}
int are_books_available(int count) {
    if (count == 0) {
        printf("There are no books. Add books before using this function.\n");
        return 0;
    }
    return 1;
}

task add_task() {
    task new_task;
    correct_str("Input name of task: ", new_task.name, 20);
    new_task.deadline = correct_date("Input date of deadline: ");
    while (1) {
        printf("Input complexity of your task from 1 to 10: ");
        new_task.hard = correct_int("");
        if ((new_task.hard >= 0) && (new_task.hard <= 20))break;
    }
    while (1) {
        printf("Input time that you need to do this task in minutes: ");
        new_task.time = correct_int("");
        if (new_task.time >= 0) break;
    }
    while (1) {
        printf("Input '0' if task isn't done yet else input '1': ");
        new_task.hard = correct_int("");
        if ((new_task.hard >= 0) && (new_task.hard <= 1))break;
    }
    correct_str("Input description for this task: ", new_task.description, 500);
    return new_task;
}

int main() {
    task a;
    printf("HI\n");	
    a = add_task();
}