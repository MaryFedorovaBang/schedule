#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>

typedef struct {
    int day, month, year;
}date;
//name, hard, time,done, when_done, description, deadline
typedef struct {
    char *name;
    date deadline, done_date;
    int hard, time, done;
    char *description;
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
        char str1[10];
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
int are_tasks_available(int count) {
    if (count == 0) {
        printf("There are no tasks. Add tasks before using this function.\n");
        return 0;
    }
    return 1;
}

void add_task(task* tasks, int* count, int capacity) {
    char new_name[20];
    date new_deadline;
    int new_hard, new_time, new_done;
    char new_description[500];

    correct_str("Input name of task: ", new_name, sizeof(new_name));
    new_deadline=correct_date("Input date of deadline: ");
    while (1){
        new_hard = correct_int("Input hard of task from 1 to 10: ");
        if ((new_hard > 0) && (new_hard <= 20))break;
    }
    while(1){
        new_time = correct_int("Input time(minutes) you need to do this task: ");
        if (new_time > 0)break;
    }
    while (1) {
        new_done = correct_int("Input '0' if task isn't done yet else print '1': ");
        if ((new_done == 1) || (new_done == 0))break;
    }
    
    correct_str("Input description for this task: ", new_description, sizeof(new_description));
    

    if (*count <= capacity) {
        task* t;
        t = tasks + *count;
        t->name = (char*)malloc(strlen(new_name) + 1);
        if (t->name == NULL) {
            printf("Memory allocation error!\n");
            return;
        }
        strcpy(t->name, new_name);
        t->deadline = new_deadline;
        t->hard = new_hard;
        t->time = new_time;
        t->done = new_done;
        t->description = (char*)malloc(strlen(new_description) + 1);
        if (t->description == NULL) {
            printf("Memory allocation error!\n");
            return;
        }
        strcpy(t->description, new_description);
        (*count)++;
        printf("Task was sucsessfully added.\n");
    }
    else {
        printf("Not enough space to add new task!\n");
    }
}



void save_data(task* tasks, int count, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("ERROR: couldn't open file for notes");
        return;
    }

    fwrite(&count, sizeof(int), 1, file);

    for (int i = 0; i < count; i++) {
        int name_length = strlen(tasks[i].name) + 1;
        fwrite(&name_length, sizeof(int), 1, file);
        fwrite(tasks[i].name, sizeof(char), name_length, file);

        fwrite(&tasks[i].deadline.day, sizeof(int), 1, file);
        fwrite(&tasks[i].deadline.month, sizeof(int), 1, file);
        fwrite(&tasks[i].deadline.year, sizeof(int), 1, file);

        fwrite(&tasks[i].hard, sizeof(int), 1, file);

        fwrite(&tasks[i].time, sizeof(int), 1, file);

        fwrite(&tasks[i].done, sizeof(int), 1, file);

        fwrite(&tasks[i].done_date.day, sizeof(int), 1, file);
        fwrite(&tasks[i].done_date.month, sizeof(int), 1, file);
        fwrite(&tasks[i].done_date.year, sizeof(int), 1, file);

        int description_length = strlen(tasks[i].description) + 1;
        fwrite(&description_length, sizeof(int), 1, file);
        fwrite(tasks[i].description, sizeof(char), description_length, file);
    }

    fclose(file);
    printf("Data was sucsessfully saved in %s\n", filename);
}

int load_data(task** tasks, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("ERROR: file is not availible for open");
        return 0;
    }

    int count;
    if (fread(&count, sizeof(int), 1, file) != 1) {
        if (feof(file)) {
            printf("File is empty.\n");
        }
        else {
            perror("ERROR: couldn't read number of notes");
        }
        fclose(file);
        return 0;
    }

    *tasks = (task*)malloc(count * sizeof(task));
    if (*tasks == NULL) {
        printf("Memory allocation error!\n");
        fclose(file);
        return 0;
    }

    for (int i = 0; i < count; i++) {

        int name_length;
        fread(&name_length, sizeof(int), 1, file);
        (*tasks)[i].name = (char*)malloc(name_length * sizeof(char));
        fread((*tasks)[i].name, sizeof(char), name_length, file);

        fread(&(*tasks)[i].deadline.day, sizeof(int), 1, file);
        fread(&(*tasks)[i].deadline.month, sizeof(int), 1, file);
        fread(&(*tasks)[i].deadline.year, sizeof(int), 1, file);

        fread(&(*tasks)[i].hard, sizeof(int), 1, file);

        fread(&(*tasks)[i].time, sizeof(int), 1, file);

        fread(&(*tasks)[i].done, sizeof(int), 1, file);

        fread(&(*tasks)[i].done_date.day, sizeof(int), 1, file);
        fread(&(*tasks)[i].done_date.month, sizeof(int), 1, file);
        fread(&(*tasks)[i].done_date.year, sizeof(int), 1, file);

        int description_length;
        fread(&description_length, sizeof(int), 1, file);
        (*tasks)[i].description = (char*)malloc(description_length * sizeof(char));
        fread((*tasks)[i].description, sizeof(char), description_length, file);
    }

    fclose(file);
    printf("Data was sucsessfully achivied from %s\n", filename);
    return count;
}
date get_current_date() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    date current_date;
    current_date.day = tm.tm_mday;
    current_date.month = tm.tm_mon + 1;
    current_date.year = tm.tm_year + 1900;
    return current_date;
}
int date_cmp(date deadline) {
    date today;
    today = get_current_date();
    if (deadline.year > today.year)return 1;
    else if (deadline.year < today.year)return 0;
    else if (deadline.month > today.month) return 1;
    else if (deadline.month < today.month)return 0;
    else if (deadline.day > today.day)return 1;
    else if (deadline.day < today.day)return 0;
    else return 2;
}
void print_header() {
    printf("| %-2s | %-20s | %-10s | %-2s | %-5s | %-1s |\n", " ", "Task", "Deadline", "Hard", "Time", "Done");
}
void print_task(task task1, int i) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (task1.done==1) {
        SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    }
    else if ((date_cmp(task1.deadline) == 0)|| (date_cmp(task1.deadline) == 2) ){
        SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
    }
    printf("| %-2d | %-20s | %-2d.%-2d.%-4d | %-4d | %-5d | %-4d |\n", i, task1.name, task1.deadline.day, task1.deadline.month, task1.deadline.year, task1.hard, task1.time, task1.done);
    SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
}
task* delete_task(task* tasks, int* count, int n, int del_i) {
    if (del_i > *count)return tasks;
    del_i--;
    for (int i = del_i; i < *count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }
    *count = *count - 1;
    if (are_tasks_available(*count)) {
        print_header();
        for (int i = 0; i < *count; i++) {
            print_task(tasks[i], i + 1);
        }
    }
    return tasks;
}


int main() {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    int n = 10;
    task* tasks = NULL;
    int count = 0;
    int choice;
    FILE* file;

    file = fopen("tasks.dat", "rb");
    if (file == NULL) {
        if (errno == ENOENT) {
            printf("File 'tasks.dat' would be build after save.\n");

            tasks = (task*)malloc(n * sizeof(task));
            if (tasks == NULL) {
                printf("Memory allocation error!\n");
                return 1;
            }
        }
        else {
            perror("ERROR: couldn't open file 'tasks.txt'");
            return 1;
        }
    }
    else {
        count = load_data(&tasks, "tasks.dat");
        if (count == 0) {
            printf("File is empty.\n");
            tasks = (task*)malloc(n * sizeof(task));
            if (tasks == NULL) {
                printf("Memory allocation error!\n");
                return 1;
            }

        }
        else
        {
            n = count * 2;
            tasks = (task*)realloc(tasks, n * sizeof(task));
            if (tasks == NULL) {
                printf("Memory allocation error!\n");
                return 1;
            }
        }

        fclose(file);
    }

    do {
        printf("\nChoose action: \n");
        printf(" 1 - add task\n");
        printf(" 2 - print tasks\n");
        printf(" 3 - delete task\n");
        printf(" 4 - exit\n");
        choice=correct_int("");
        switch (choice) {
        case 1:
            if (count == n) {
                n *= 2;
                tasks = (task*)realloc(tasks, n * sizeof(task));
                if (tasks == NULL) {
                    printf("Memory allocation error!\n");
                    return 1;
                }
            }
            add_task(tasks, &count, n);
            break;
        case 2:
            if (are_tasks_available(count)) {
                print_header();
                for (int i = 0; i < count; i++) {
                    print_task(tasks[i], i + 1);
                }
            }
            break;
        case 3:
            if (are_tasks_available(count)) {
                int del_i;
                del_i = correct_int("Choose task that you want to delete: ");
                /*char* na;
                date d;
                if (del_i == 1) {
                    d.day= tasks[1].deadline.day;
                    d.month = tasks[1].deadline.month;
                    d.year = tasks[1].deadline.year;
                    strcpy(na,tasks[1].name);
                }
                else {
                    d.day = tasks[0].deadline.day;
                    d.month = tasks[0].deadline.month;
                    d.year = tasks[0].deadline.year;
                    printf("1");
                    const char* t = tasks[0].name;
                    printf("2");
                    strcpy(na,t);
                    printf("3\n");
                }
                printf("%c %d.%d.%d\n", na, d.day, d.month, d.year);
                tasks=delete_task(tasks, &count, n, del_i);
                tasks[0].name = na;
                tasks[0].deadline = d;*/
                tasks = delete_task(tasks, &count, n, del_i);
                printf("%c %d.%d.%d\n", tasks[0].name, tasks[0].deadline.day, tasks[0].deadline.month, tasks[0].deadline.year);
            }
            break;

        case 4:
            save_data(tasks, count, "tasks.dat");
            printf("Exit\n");
            break;

        default:
            printf("Incorrect number of function.\n");
            break;
        }
    } while (choice != 4);
    for (int i = 0; i < count; i++) {
        free(tasks[i].name);
        free(tasks[i].description);
    }
    free(tasks);

}