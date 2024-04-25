#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100

typedef struct {
    char task[256];
    int completed;
} Task;

// タスク一覧を表示する関数
void displayTasks(Task tasks[], int count) {
    printf("========== ToDoリスト ==========\n");
    for (int i = 0; i < count; ++i) {
        printf("%d. [%d] %s\n", i + 1, tasks[i].completed, tasks[i].task);
    }
    printf("==============================\n");
    printf("0ならば未完、1ならば完了です\n");
}

// タスクを追加する関数
void addTask(Task tasks[], int* count, const char* newTask) {
    if (*count < MAX_TASKS) {
        strcpy_s(tasks[*count].task, newTask);
        tasks[*count].completed = 0;
        (*count)++;
    }
    else {
        printf("タスクの上限に達しました。\n");
    }
}

// タスクを編集する関数
void editTask(Task tasks[], int count) {
    int num;
    displayTasks(tasks, count);
    printf("編集するタスクの番号を入力してください: ");
    scanf_s("%d", &num);
    if (num < 1 || num > count) {
        printf("無効な番号です。\n");
        return;
    }
    printf("新しいタスク内容を入力してください: ");
    scanf_s("%s", tasks[num - 1].task, 255);
}

// タスクを削除する関数
void deleteTask(Task tasks[], int* count) {
    int num;
    displayTasks(tasks, *count);
    printf("削除するタスクの番号を入力してください: ");
    scanf_s("%d", &num);
    if (num < 1 || num > *count) {
        printf("無効な番号です。\n");
        return;
    }
    for (int i = num - 1; i < *count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }
    (*count)--;
}

// タスクをファイルに保存する関数
void saveTasks(Task tasks[], int count) {
    FILE* file;
    fopen_s(&file, "todo_list.txt", "w");
    if (file == NULL) {
        printf("ファイルを開けませんでした。\n");
        return;
    }
    for (int i = 0; i < count; ++i) {
        fprintf(file, "%d %s\n", tasks[i].completed, tasks[i].task);
    }
    fclose(file);
    printf("タスクを保存しました。\n");
}

// タスクをファイルから読み込む関数
void loadTasks(Task tasks[], int* count) {
    FILE* file;
    fopen_s(&file, "todo_list.txt", "r");
    if (file == NULL) {
        printf("ファイルを開けませんでした。\n");
        return;
    }
    *count = 0;
    while (fscanf_s(file, "%d %[^\n]", &tasks[*count].completed, tasks[*count].task, sizeof(tasks[*count].completed), sizeof(tasks[*count].task)) != EOF) {
        // ファイルに保存されている完了フラグが0または1以外の場合、エラーメッセージを表示してスキップ
        if (tasks[*count].completed != 0 && tasks[*count].completed != 1) {
            printf("無効な完了フラグ: %d\n", tasks[*count].completed);
            continue;
        }
        (*count)++;
    }
    fclose(file);
    printf("タスクを読み込みました。\n");
}


// タスク完了処理を行う関数
void doneTask(Task tasks[], int count) {
    int num;
    displayTasks(tasks, count);
    printf("編集するタスクの番号を入力してください: ");
    scanf_s("%d", &num);
    if (num < 1 || num > count) {
        printf("無効な番号です。\n");
        return;
    }
    else if (tasks[num - 1].completed == 0) {
        tasks[num - 1].completed = 1;
        printf("タスクを完了しました。\n");
    }
    else {
        tasks[num - 1].completed = 0;
        printf("タスクを未完にしました。\n");
    }
}
    

int main() {
    Task todo_list[MAX_TASKS];
    int todo_list_count = 0;
    char command;
    loadTasks(todo_list, &todo_list_count);

    while (1) {
        printf("A: 登録, B: 編集, C: 削除, D: 保存, E: 読み込み, F: 一覧, G: 完了\n");
        printf("処理を選択してください: ");
        scanf_s(" %c", &command);

        switch (command) {
        case 'A': {
            char newTask[256];
            printf("タスクを登録してください: ");
            scanf_s(" %[^\n]", newTask, 255);
            addTask(todo_list, &todo_list_count, newTask);
            break;
        }
        case 'B':
            editTask(todo_list, todo_list_count);
            break;
        case 'C':
            deleteTask(todo_list, &todo_list_count);
            break;
        case 'D':
            saveTasks(todo_list, todo_list_count);
            break;
        case 'E':
            loadTasks(todo_list, &todo_list_count);
            break;
        case 'F':
            displayTasks(todo_list, todo_list_count);
            break;
        case 'G':
            doneTask(todo_list, todo_list_count);
            break;
        default:
            printf("無効なコマンドです。\n");
        }
    }

    return 0;
}
