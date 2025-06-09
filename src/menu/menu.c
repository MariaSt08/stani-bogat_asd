#include "menu.h"
#include "../quiz/quiz.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define OPTION_COUNT 5
#define QUESTION_COUNT 10

void interactive_edit_question(const char *filename);

int is_empty_string(const char *str) {
    return str == NULL || str[0] == '\0' || str[0] == '\n';
}

void info_questions(char *question_text, char **options, int *correct_index, int *difficulty) {
    printf("Vuvedete tekst na vuprosa: ");
    if (fgets(question_text, 256, stdin) == NULL || is_empty_string(question_text)) {
        printf("Greshka: Tekstut na vuprosa ne moje da bude prazen.\n");
        return;
    }
    question_text[strcspn(question_text, "\n")] = '\0';

    for (int i = 0; i < 4; i++) {
        char option[256];
        printf("Vuvedete otgovor %d: ", i + 1);
        if (fgets(option, sizeof(option), stdin) == NULL || is_empty_string(option)) {
            printf("Greshka: Otgovor %d ne moje da bude prazen.\n", i + 1);
            i--;
            continue;
        }
        option[strcspn(option, "\n")] = '\0';
        options[i] = strdup(option);
    }

    printf("Vuvedete nomera na pravilniya otgovor (1-4): ");
    if (scanf("%d", correct_index) != 1 || *correct_index < 1 || *correct_index > 4) {
        printf("Greshka: Nomerut na pravilniya otgovor trqbva da bude mejdu 1 i 4.\n");
        while (getchar() != '\n');
        return;
    }
    *correct_index -= 1; // Превръщаме от 1-4 в 0-3

    printf("Vuvedete trudnostta na vuprosa (1-10): ");
    if (scanf("%d", difficulty) != 1 || *difficulty < 1 || *difficulty > 10) {
        printf("Greshka: Trudnostta trqbva da bude mejdu 1 i 10.\n");
        while (getchar() != '\n');
        return;
    }

    while (getchar() != '\n');
}

void show_question(int otg, int *options, int joker5050_used, int jokerObadise_used, int jokerPublika_used, char *vupros, char option[4][100], int difficulty) {
    system("clear");
    system("clear");

    printf("=================================\n");
    if (difficulty <= 2)
        printf("(Kategoriya: Lesen)\n");
    else if (difficulty <= 5)
        printf("(Kategoriya: Sreden)\n");
    else
        printf("(Kategoriya: Truden)\n");
    printf("Vupros: %s\n", vupros);

    for (int i = 0; i < 4; i++) {
        if (options[i]) {
            printf("%d. Otgovor %c: %s\n", i + 1, 'A' + i, option[i]);
        } else {
            printf("%d. ---\n", i + 1);
        }
    }
    printf("=================================\n");
    printf("5. Jokeri\n");
    printf("Dostupni Jokeri: ");
    if (!joker5050_used) printf("50/50 ");
    if (!jokerObadise_used) printf("Obadise ");
    if (!jokerPublika_used) printf("Publika ");
    printf("\n");
}

void joker5050(int correct, int *options) {
    int removed = 0;
    while (removed < 2) {
        int r = rand() % 4;
        if (r != correct && options[r]) {
            options[r] = 0;
            removed++;
        }
    }
    printf("50/50 joker izpolzvan!\n");
    getchar();
}

void jokerObadise(int correct, int *options, int difficulty) {
    int probability = (difficulty <= 3) ? 80 : (difficulty <= 6 ? 60 : 30);
    int chance = rand() % 100;
    char available_options[4];
    int count = 0;
    for (int i = 0; i < 4; i++) if (options[i]) available_options[count++] = 'A' + i;
    char friend_answer = (chance < probability) ? 'A' + correct : available_options[rand() % count];
    printf("Prijatelqt vi smqta che verniqt otgovor e: %c\n", friend_answer);
    getchar();
}

void jokerPublika(int correct, int *options, int difficulty) {
    int probability = (difficulty <= 3) ? 80 : (difficulty <= 6 ? 60 : 30);
    int votes[4] = {0};
    int total_votes = 100;
    if (rand() % 100 < probability)
        votes[correct] = rand() % 51 + 50;
    else
        votes[correct] = rand() % 25;
    total_votes -= votes[correct];
    for (int i = 0; i < 4; i++) if (options[i] && i != correct) votes[i] = rand() % total_votes, total_votes -= votes[i];
    votes[correct] += total_votes;
    printf("Publikata glasuva taka:\nA: %d%%\nB: %d%%\nC: %d%%\nD: %d%%\n", votes[0], votes[1], votes[2], votes[3]);
    getchar();
}

// останалата част от кода остава непроменена


int menu() {
    int izbor;
    do {
        system("clear");
        printf("***************************\n");
        printf("*       STANI BOGAT       *\n");
        printf("***************************\n");
        printf("1. Zapochni igra\n");
        printf("2. Pravila na igrata\n");
        printf("3. Dobavqne na nov vupros\n");
        printf("4. Redaktirane na vupros\n");
        printf("5. Izhod\n");
        printf("Izberete opcija: ");
        if (scanf("%d", &izbor) != 1) {
            printf("Nevaliden vhod.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (izbor) {
            case 1: {
                int joker5050_used = 0, jokerObadise_used = 0, jokerPublika_used = 0;
                int question_count = 10, current_question = 0;
                srand(time(NULL));
                while (current_question < question_count) {
                    char options[4][100];
                    char text[256];
                    int correct_option, difficulty;
                    bool found = false;
                    load_questions("quiz_questions.txt", options, text, &correct_option, &difficulty, &found, current_question);
                    if (!found) {
                        printf("Nqma poveche vuprosi.\n");
                        break;
                    }
                    int otg;
                    int active_options[4] = {1, 1, 1, 1};
                    do {
                        show_question(otg, active_options, joker5050_used, jokerObadise_used, jokerPublika_used, text, options, difficulty);
                        printf("Izberi otgovor (1-4) ili 5 za jokeri: ");
                        scanf("%d", &otg);
                        while (getchar() != '\n');
                        if (otg >= 1 && otg <= 4 && active_options[otg - 1]) {
                            if (otg - 1 == correct_option) {
                                printf("Pravilen otgovor!\n");
                                current_question++;
                            } else {
                                printf("Greshen otgovor!\nKray na igrata.\n");
                                return 0;
                            }
                            printf("Natisni Enter za prodyljenie...\n");
                            getchar();
                            break;
                        } else if (otg == 5) {
                            int j;
                            do {
                                printf("Izberi joker:\n");
                                if (!joker5050_used) printf("1. 50/50\n");
                                if (!jokerObadise_used) printf("2. Obadise\n");
                                if (!jokerPublika_used) printf("3. Publika\n");
                                printf("4. Nazad\n");
                                scanf("%d", &j);
                                while (getchar() != '\n');
                                if (j == 1 && !joker5050_used) {
                                    joker5050(correct_option, active_options);
                                    joker5050_used = 1;
                                    show_question(otg, active_options, joker5050_used, jokerObadise_used, jokerPublika_used, text, options, difficulty);
                                } else if (j == 2 && !jokerObadise_used) {
                                    jokerObadise(correct_option, active_options, difficulty);
                                    jokerObadise_used = 1;
                                    show_question(otg, active_options, joker5050_used, jokerObadise_used, jokerPublika_used, text, options, difficulty);
                                } else if (j == 3 && !jokerPublika_used) {
                                    jokerPublika(correct_option, active_options, difficulty);
                                    jokerPublika_used = 1;
                                    show_question(otg, active_options, joker5050_used, jokerObadise_used, jokerPublika_used, text, options, difficulty);
                                }
                            } while (j != 4);
                        } else {
                            printf("Nevaliden izbor.\n");
                        }
                    } while (1);
                }
                printf("Pozdravleniq! Premina vsichki vuprosi!\n");
                break;
            }
            case 2:
                printf("\nPravila:\n- 10 vyprosa\n- 3 jokera\n- edin praven otgovor\n");
                printf("Natisni Enter za prodyljenie...\n");
                getchar();
                break;
            case 3: {
                char question_text[256];
                int difficulty, correct_index;
                char *options[4];
                info_questions(question_text, options, &correct_index, &difficulty);
                add_question_to_file("quiz_questions.txt", question_text, difficulty, options, correct_index);
                printf("Vuprosyt e dobaven.\n");
                getchar();
                break;
            }
            case 4: {
                interactive_edit_question("quiz_questions.txt");
                printf("Natisni Enter za prodyljenie...\n");
                getchar();
                break;
            }
            case 5:
                printf("Izhod.\n");
                return 0;
            default:
                printf("Nevalidna opciq.\n");
        }
    } while (1);
    return 0;
}