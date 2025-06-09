#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>


#include "menu/menu.h"
#include "quiz/quiz.h"



int main()
{  
    initialize_quiz();
    print_questions("quiz_questions.txt", true, true);
    const char *newoptions[] = {"1", "2", "3", "4"};

    const char *options1[] = {"Barcelona", "Paris", "Madrid ", "Milano"};
    add_question_to_file("quiz_questions.txt", "Koq e stolicana na Ispaniq ?", 0, options1, 2);

    const char *option1[] = {"Berlin", "Tokyo", "Lisbon", "Havana"};
    add_question_to_file("quiz_questions.txt", "Koq e stolicata na Kuba?", 0, option1, 3);

    const char *option2[] = {"Vodorod", "Kislorod", "Vaglerod", "Azot"};
    add_question_to_file("quiz_questions.txt", "Koy e nay-lekiyat element v periodichnata tablica?", 1, option2, 0);

    const char *option3[] = {"Hristofor Kolumb", "Vasko da Gama", "Fernando Magelan", "Dzheyms Kuk"};
    add_question_to_file("quiz_questions.txt", "Koy e otkril Amerika prez 1492 godina?", 2, option3, 0);

    const char *option4[] = {"3.14", "2.17", "1.61", "4.20"};
    add_question_to_file("quiz_questions.txt", "Kakva e priblizitelnata stoynost na chisloto pi?", 3, option4, 0);

    const char *option5[] = {"Slance", "Yupiter", "Zemya", "Mars"};
    add_question_to_file("quiz_questions.txt", "Koe e nay-golyamoto tyalo v Slanchevata sistema?", 4, option5, 0);

    const char *option6[] = {"Heminguey", "Shekspir", "Gyote", "Bayron"};
    add_question_to_file("quiz_questions.txt", "Koy e avtorat na piesata 'Hamlet'?", 5, option6, 1);

    const char *option7[] = {"6", "7", "8", "9"};
    add_question_to_file("quiz_questions.txt", "Kolko kraka ima payak?", 6, option7, 2);

    const char *option8[] = {"Python", "C++", "HTML", "Java"};
    add_question_to_file("quiz_questions.txt", "Koy ot izbroenite e markirasht ezik, a ne programen?", 7, option8, 2);

    const char *option9[] = {"Norvegiya", "Finlandiya", "Shveciya", "Daniya"};
    add_question_to_file("quiz_questions.txt", "Koya strana e izvestna sas severnoto siyanie i fiordite?", 8, option9, 0);

    const char *option10[] = {"1989", "1991", "2001", "1999"};
    add_question_to_file("quiz_questions.txt", "Koga se razpada SSR?", 9, option10, 1);

    menu();

    cleanup_quiz();

    return 0;
} 
