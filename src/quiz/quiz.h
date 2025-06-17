#ifndef QUIZ_H
#define QUIZ_H

#define MAX_DIFFICULTY 10 
#define MAX_QUESTIONS 100

#include <time.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct quiz_question {
    char *question_text;
    char *options[4];
    int correct_option_index;
    int difficulty;
    struct quiz_question *next;
} QuizQuestion;

 void initialize_quiz();
 void add_question_to_file(const char *filename, const char *text, uint8_t difficulty, const char *options[], uint8_t correct_index);
 void edit_question_in_file(const char *filename, int question_number, const char *newText, uint8_t new_difficulty, const char *newOptions[], uint8_t newCorrect_index);
 void save_questions_to_file(const char *filename);
 void cleanup_quiz();
 void load_questions(const char *filename, char options[4][100], char* text, int* correct_option, int* difficulty, bool* found, int desired_difficulty);
 void print_questions(const char *filename, bool print_answers, bool print_difficulty);
 void load_random_question(const char *filename, char options[4][100], char *text, int *correct_option, int *difficulty);extern void interactive_edit_question(const char *filename);
 void interactive_edit_question(const char *filename);

#endif