#include "quiz.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static QuizQuestion *head = NULL;

void initialize_quiz() {
    head = NULL;
}

void add_question_to_file(const char *filename, const char *text, uint8_t difficulty, const char *options[], uint8_t correct_index) {
    QuizQuestion *new_question = (QuizQuestion *)malloc(sizeof(QuizQuestion));
    if (!new_question) {
        perror("Memory allocation failed");
        return;
    }

    new_question->question_text = strdup(text);

    for (int i = 0; i < 4; i++) {
        new_question->options[i] = strdup(options[i]);
    }

    new_question->correct_option_index = correct_index;
    new_question->difficulty = difficulty;
    new_question->next = head;
    head = new_question;

    FILE *file = fopen(filename, "a");
    if (!file) {
        perror("Cannot open file for writing");
        return;
    }

    fprintf(file, "%s\n", text);
    for (int i = 0; i < 4; i++) {
        fprintf(file, "%s\n", options[i]);
    }
    fprintf(file, "%d\n", correct_index);
    fprintf(file, "%d\n", difficulty);
    fclose(file);
}

void edit_question_in_file(const char *filename, int question_number, const char *newText, uint8_t new_difficulty, const char *newOptions[], uint8_t newCorrect_index) {
    FILE *file = fopen(filename, "r");
    if (!file) return;

    FILE *temp_file = fopen("temp.txt", "w");
    if (!temp_file) {
        fclose(file);
        return;
    }

    char buffer[256];
    int line_count = 0;
    int current_question = 1;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (current_question == question_number) {
            if (line_count == 0) {
                fprintf(temp_file, "%s\n", newText);
            } else if (line_count <= 4) {
                fprintf(temp_file, "%s\n", newOptions[line_count - 1]);
            } else if (line_count == 5) {
                fprintf(temp_file, "%d\n", newCorrect_index);
            } else if (line_count == 6) {
                fprintf(temp_file, "%d\n", new_difficulty);
            }
        } else {
            fputs(buffer, temp_file);
        }

        line_count++;
        if (line_count == 7) {
            line_count = 0;
            current_question++;
        }
    }

    fclose(file);
    fclose(temp_file);
    remove(filename);
    rename("temp.txt", filename);
}

void cleanup_quiz() {
    QuizQuestion *current = head;
    while (current) {
        QuizQuestion *next = current->next;
        free(current->question_text);
        for (int i = 0; i < 4; i++) {
            free(current->options[i]);
        }
        free(current);
        current = next;
    }
    head = NULL;
}

void save_questions_to_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) return;

    QuizQuestion *current = head;
    while (current) {
        fprintf(file, "%s\n", current->question_text);
        for (int i = 0; i < 4; i++) {
            fprintf(file, "%s\n", current->options[i]);
        }
        fprintf(file, "%d\n", current->correct_option_index);
        fprintf(file, "%d\n", current->difficulty);
        current = current->next;
    }

    fclose(file);
}

void load_questions(const char *filename, char options[4][100], char *text, int *correct_option, int *difficulty, bool *found, int desired_difficulty) {
    FILE *file = fopen(filename, "r");
    if (!file) return;

    char buffer[256];
    int questions_with_desired_difficulty = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        strcpy(text, buffer);
        text[strcspn(text, "\n")] = '\0';

        for (int i = 0; i < 4; i++) {
            fgets(buffer, sizeof(buffer), file);
            strcpy(options[i], buffer);
            options[i][strcspn(options[i], "\n")] = '\0';
        }

        fgets(buffer, sizeof(buffer), file);
        *correct_option = atoi(buffer);
        fgets(buffer, sizeof(buffer), file);
        *difficulty = atoi(buffer);

        if (*difficulty == desired_difficulty) {
            questions_with_desired_difficulty++;
        }
    }

    if (questions_with_desired_difficulty == 0) {
        fclose(file);
        *found = false;
        return;
    }

    srand(time(NULL));
    int target = rand() % questions_with_desired_difficulty;
    fseek(file, 0, SEEK_SET);

    int count = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        strcpy(text, buffer);
        text[strcspn(text, "\n")] = '\0';

        for (int i = 0; i < 4; i++) {
            fgets(buffer, sizeof(buffer), file);
            strcpy(options[i], buffer);
            options[i][strcspn(options[i], "\n")] = '\0';
        }

        fgets(buffer, sizeof(buffer), file);
        *correct_option = atoi(buffer);
        fgets(buffer, sizeof(buffer), file);
        *difficulty = atoi(buffer);

        if (*difficulty == desired_difficulty) {
            if (count == target) {
                *found = true;
                break;
            }
            count++;
        }
    }

    fclose(file);
}

void print_questions(const char *filename, bool print_answers, bool print_difficulty) {
    FILE *file = fopen(filename, "r");
    if (!file) return;

    char buffer[256];
    int index = 1;
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        printf("Vupros %d: %s\n", index++, buffer);

        for (int i = 0; i < 4; i++) {
            fgets(buffer, sizeof(buffer), file);
            buffer[strcspn(buffer, "\n")] = '\0';
            printf("  Otgovor %d: %s\n", i + 1, buffer);
        }

        fgets(buffer, sizeof(buffer), file);
        int correct = atoi(buffer);
        if (print_answers) printf("  Pravilen otgovor: %d\n", correct);

        if (print_difficulty) {
            fgets(buffer, sizeof(buffer), file);
            int difficulty = atoi(buffer);
            printf("  Trudnost: %d\n", difficulty);
        }
    }

    fclose(file);
}

void load_random_question(const char *filename, char options[4][100], char *text, int *correct_option, int *difficulty) {
    FILE *file = fopen(filename, "r");
    if (!file) return;

    char buffer[256];
    int count = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        fgets(buffer, sizeof(buffer), file);
        fgets(buffer, sizeof(buffer), file);
        fgets(buffer, sizeof(buffer), file);
        fgets(buffer, sizeof(buffer), file);
        fgets(buffer, sizeof(buffer), file);
        count++;
    }

    srand(time(NULL));
    int target = rand() % count;
    fseek(file, 0, SEEK_SET);
    count = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        strcpy(text, buffer);
        text[strcspn(text, "\n")] = '\0';

        for (int i = 0; i < 4; i++) {
            fgets(buffer, sizeof(buffer), file);
            strcpy(options[i], buffer);
            options[i][strcspn(options[i], "\n")] = '\0';
        }

        fgets(buffer, sizeof(buffer), file);
        *correct_option = atoi(buffer);
        fgets(buffer, sizeof(buffer), file);
        *difficulty = atoi(buffer);

        if (count == target) break;
        count++;
    }

    fclose(file);
}

void interactive_edit_question(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Ne moje da se otvori faila.\n");
        return;
    }

    char buffer[256];
    char questions[100][256];
    int question_count = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        strcpy(questions[question_count], buffer);
        for (int i = 0; i < 6; i++) fgets(buffer, sizeof(buffer), file);
        question_count++;
    }
    fclose(file);

    if (question_count == 0) {
        printf("Nqma dostupni vuprosi.\n");
        return;
    }

    printf("\n--- Nalichni vuprosi ---\n");
    for (int i = 0; i < question_count; i++) {
        printf("%d. %s\n", i + 1, questions[i]);
    }

    int choice;
    printf("Izberi nomer na vupros za redakciq: ");
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > question_count) {
        printf("Nevaliden izbor.\n");
        return;
    }

    file = fopen(filename, "r");
    if (!file) return;

    int current_question = 1;
    char text[256];
    char options[4][100];
    int correct_index;
    int difficulty;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (current_question == choice) {
            strcpy(text, buffer);
            text[strcspn(text, "\n")] = '\0';

            for (int i = 0; i < 4; i++) {
                fgets(buffer, sizeof(buffer), file);
                strcpy(options[i], buffer);
                options[i][strcspn(options[i], "\n")] = '\0';
            }

            fgets(buffer, sizeof(buffer), file);
            correct_index = atoi(buffer);
            fgets(buffer, sizeof(buffer), file);
            difficulty = atoi(buffer);
            break;
        } else {
            for (int i = 0; i < 6; i++) fgets(buffer, sizeof(buffer), file);
            current_question++;
        }
    }
    fclose(file);

    printf("\n--- Tekusht vupros ---\n");
    printf("Tekst: %s\n", text);
    for (int i = 0; i < 4; i++) {
        printf("Otgovor %d: %s\n", i + 1, options[i]);
    }
    printf("Pravilen otgovor: %d\n", correct_index);
    printf("Trudnost: %d\n", difficulty);

    printf("\nKakvo iskash da redaktirash?\n");
    printf("1. Tekst na vuprosa\n2. Opredelen otgovor\n3. Pravilen otgovor\n4. Trudnost\n5. Vsichko\n6. Izlizane\n");
    printf("Izbor: ");
    int edit_choice;
    scanf("%d", &edit_choice);
    getchar();

    switch (edit_choice) {
        case 1:
            printf("Star tekst: %s\nNov (Enter za bez promqna): ", text);
            fgets(buffer, sizeof(buffer), stdin);
            if (buffer[0] != '\n') {
                buffer[strcspn(buffer, "\n")] = '\0';
                strcpy(text, buffer);
            }
            break;

        case 2: {
            int ans_index;
            printf("Koi otgovor iskash da redaktirash (1-4): ");
            scanf("%d", &ans_index);
            getchar();
            if (ans_index >= 1 && ans_index <= 4) {
                printf("Star otgovor %d: %s\nNov (Enter za bez promqna): ", ans_index, options[ans_index - 1]);
                fgets(buffer, sizeof(buffer), stdin);
                if (buffer[0] != '\n') {
                    buffer[strcspn(buffer, "\n")] = '\0';
                    strcpy(options[ans_index - 1], buffer);
                }
            } else {
                printf("Nevaliden otgovor.\n");
            }
            break;
        }

        case 3:
            printf("Nov praven otgovor (1-4): ");
            scanf("%d", &correct_index);
            getchar();
            break;

        case 4:
            printf("Nova trudnost (1-10): ");
            scanf("%d", &difficulty);
            getchar();
            break;

        case 5:
            printf("Nov tekst: ");
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = '\0';
            for (int i = 0; i < 4; i++) {
                printf("Nov otgovor %d: ", i + 1);
                fgets(options[i], sizeof(options[i]), stdin);
                options[i][strcspn(options[i], "\n")] = '\0';
            }
            printf("Nov praven otgovor: ");
            scanf("%d", &correct_index);
            printf("Nova trudnost: ");
            scanf("%d", &difficulty);
            getchar();
            break;

        case 6:
            printf("Izlizane bez promqna.\n");
            return;
    }

    const char *newOptions[4] = { options[0], options[1], options[2], options[3] };
    edit_question_in_file(filename, choice, text, difficulty, newOptions, correct_index);
    printf("Redakciqta e zavurshena.\n");
}
