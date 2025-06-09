#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>

#define OPTION_COUNT 5
#define QUESTION_COUNT 10

#ifndef _WIN32
#include <termios.h>
#include <unistd.h>

static int getch(void) {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}
#define GETCH() getch()
#else
#include <conio.h>
#define GETCH() getch()
#endif


 void option_menu(unsigned char c, int *otg, int count);
extern void info_questions(char *question_text, char **options, int *correct_index, int *difficulty);
 void joker5050(int correct, int *options);
 void jokerObadise(int correct, int *options, int difficulty);
 void jokerPublika(int correct, int *options, int difficulty);
 void show_question(int otg, int *options, int joker5050_used, int jokerObadise_used, int jokerPublika_used, char *vupros, char option[4][100], int difficulty);

int menu();

#endif 
