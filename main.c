#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "gfx.h"

#define SIZE 216
#define WIDTH 640
#define HEIGHT 480

struct Globals {
    char line1[10]; //global array to store current chosen word
    int stringLength; //string length of word
    char userInput; //global variable to store current character input by user
    int eventManager; // manage the flow of the program
};

int randLine(int x1); //return random value 0-216
int readLine(int lineNum, struct Globals *global); //open file and read the word from the line using random value
int usedLinecheck(struct Globals *global);

int menu(struct Globals *global);

int play(struct Globals *global);

int howtoPlay();

int fillRed(struct Globals *global); //fill red
int fillGreen(struct Globals *global); //fill green
int menuButton(int x, int y);

int main() {
    struct Globals global;
    gfx_open(WIDTH, HEIGHT, "Hangman");
    while (1) {
        global.eventManager = 0;

        menu(&global);

        if (global.eventManager == 2) {
            howtoPlay();
        } else if (global.eventManager == 1) {
            usedLinecheck(&global);
            play(&global);


        }
    }


    return 0;
}

int usedLinecheck(struct Globals *global) {
    srand(time(0));
    int lineUsed; //extract value of line chosen from readLine
    int usedArray[SIZE]; //array to store used line for comparison
    int i = 0, j, k;
    int repeat; //repeat loop
    do {
        repeat = 0;
        lineUsed = randLine(rand());
        if (i != 0 && lineUsed != usedArray[i - 1]) {
            usedArray[i] = lineUsed;
            i++;
        } else if (i == 0) {
            usedArray[i] = lineUsed;
            i++;
        } else {
            repeat = 1;
        }

    } while (repeat == 1); //change array space to store used linenum, making sure no repeating word

    for (j = i + 1; j <= SIZE; j++) {
        usedArray[j] = '\0'; //clear memory of spaced not used
    }

    readLine(lineUsed, global);
    global->line1[0] = 'a';
    global->line1[1] = 'f';
    global->line1[2] = 'r';
    global->line1[3] = 'a';
    global->line1[4] = 'i';
    global->line1[5] = 'd';
    global->stringLength = 6;
    printf("Word : %s \n", global->line1);

    printf("lineUsed = %d\nusedArray[%d] = %d\n", lineUsed, i - 1, usedArray[i - 1]); //print value for debugging


    return 0;
}

int menu(struct Globals *global) {
    global->eventManager = 0;
    int selected = 0;
    char c;
    int xpos;
    int ypos;
    int buttonPressed = 0;

    while (1) {
        gfx_clear_color(200, 200, 200);  // Set the background color to gray
        gfx_clear();
        gfx_color(0, 0, 0);  // Set the text color to black
        gfx_text("Hangman", WIDTH / 2 - 40, HEIGHT / 2 - 150, 1);
        gfx_rectangle(WIDTH / 2 - 60, HEIGHT / 2 - 15, 130, 30); // rectangle for the "Start"
        gfx_rectangle(WIDTH / 2 - 60, HEIGHT / 2 + 35, 130, 30); // rectangle for the "How to Play"
        gfx_rectangle(WIDTH / 2 - 60, HEIGHT / 2 + 85, 130, 30); // rectangle for the "Exit"
        gfx_text("Start", WIDTH / 2 - 40, HEIGHT / 2 - 10, 1);
        gfx_text("How to Play", WIDTH / 2 - 50, HEIGHT / 2 + 40, 1);
        gfx_text("Exit", WIDTH / 2 - 40, HEIGHT / 2 + 90, 1);
        gfx_flush();

        do {
            buttonPressed = 0;
            c = gfx_wait();
            xpos = gfx_xpos();
            ypos = gfx_ypos();

            if (xpos > 260 && xpos < 390 && ypos > 225 && ypos < 255) {
                buttonPressed = 1; //start
                gfx_clear();
            } else if (xpos > 260 && xpos < 390 && ypos > 275 && ypos < 300) {
                buttonPressed = 2; //howtoplay
                gfx_clear();
            } else if (xpos > 260 && xpos < 390 && ypos > 325 && ypos < 355) {
                buttonPressed = 3; //exit
                gfx_clear();
            }

            /*printf("Button pressed = %d", buttonPressed);
            printf("Mouse pos xpos is %d and ypos is %d \n", xpos,ypos);*/

        } while (buttonPressed == 0); //menu selection



        if (buttonPressed == 1) {
            printf("Starting game...\n");
            global->eventManager = 1;
            break;

        } else if (buttonPressed == 2) {
            printf("How to play...\n");
            global->eventManager = 2;
            break;
        } else if (buttonPressed == 3) {
            exit(0);
        }
    }


    return 0;
}

int howtoPlay() {

    gfx_clear_color(200, 200, 200);  // Set the background color to gray
    gfx_clear();
    gfx_color(0, 0, 0);  // Set the text color to black
    gfx_text("How to Play:", WIDTH / 2 - 50, HEIGHT / 2 - 170, 2);
    gfx_text("1) Guess the secret word by choosing letters using", WIDTH / 2 - 250, HEIGHT / 2 - 100, 1);
    gfx_text("mouse clicks.", WIDTH / 2 - 220, HEIGHT / 2 - 80, 1);
    gfx_text("2) You have 6 guesses before you lose the game.", WIDTH / 2 - 250, HEIGHT / 2 - 40, 1);
    gfx_text("3) Wrong Alphabet will become    .", WIDTH / 2 - 250, HEIGHT / 2 - 0, 1);
    gfx_color(170, 74, 68);
    gfx_text("red", WIDTH / 2 + 50, HEIGHT / 2 - 0, 1);
    gfx_color(100, 100, 100);
    gfx_text("-Left click anywhere on screen to return to the menu-", 50, HEIGHT / 2 + 100, 1);
    gfx_wait();

    return 0;
}

int readLine(int lineNum, struct Globals *global) {
    //char line1[10];
    FILE *fp; //file pointer
    int lineNumber = lineNum - 1;
    static const char filename[] = "word.txt";
    fp = fopen(filename, "r"); //open file to read
    int count = 0;
    if (fp != NULL) {
        char line[SIZE]; //maximum line size
        while (fgets(line, sizeof(line), fp) != NULL) //read a line
        {
            if (count == lineNumber && strlen(line) != 0) {
                if (line[strlen(line) - 1] == '\n') {
                    global->stringLength = (int)strlen(line) - 1;
                } else {
                    global->stringLength = (int)strlen(line);
                }
                strncpy(global->line1, line, global->stringLength);
                global->line1[global->stringLength] = '\0';
                printf("%s\nlength %d\n", global->line1, global->stringLength); //print value for debugging
                break;
            } else {
                count++;
            }
        }
        fclose(fp);
    } else {
        perror(strerror(errno));
        exit(0);
    }
    return 0;
}

int randLine(int x1) {
    return (int)(((float) x1 / (float)RAND_MAX) * SIZE);
}

int fillRed(struct Globals *global) {
    gfx_color(170, 74, 68);
    switch (global->userInput) {
        case 'a':
            gfx_fillrectangle(50, 150, 50, 50);
            break;
        case 'b':
            gfx_fillrectangle(100, 150, 50, 50);
            break;
        case 'c':
            gfx_fillrectangle(150, 150, 50, 50);
            break;
        case 'd':
            gfx_fillrectangle(200, 150, 50, 50);
            break;
        case 'e':
            gfx_fillrectangle(250, 150, 50, 50);
            break;
        case 'f':
            gfx_fillrectangle(50, 200, 50, 50);
            break;
        case 'g':
            gfx_fillrectangle(100, 200, 50, 50);
            break;
        case 'h':
            gfx_fillrectangle(150, 200, 50, 50);
            break;
        case 'i':
            gfx_fillrectangle(200, 200, 50, 50);
            break;
        case 'j':
            gfx_fillrectangle(250, 200, 50, 50);
            break;
        case 'k':
            gfx_fillrectangle(50, 250, 50, 50);
            break;
        case 'l':
            gfx_fillrectangle(100, 250, 50, 50);
            break;
        case 'm':
            gfx_fillrectangle(150, 250, 50, 50);
            break;
        case 'n':
            gfx_fillrectangle(200, 250, 50, 50);
            break;
        case 'o':
            gfx_fillrectangle(250, 250, 50, 50);
            break;
        case 'p':
            gfx_fillrectangle(50, 300, 50, 50);
            break;
        case 'q':
            gfx_fillrectangle(100, 300, 50, 50);
            break;
        case 'r':
            gfx_fillrectangle(150, 300, 50, 50);
            break;
        case 's':
            gfx_fillrectangle(200, 300, 50, 50);
            break;
        case 't':
            gfx_fillrectangle(250, 300, 50, 50);
            break;
        case 'u':
            gfx_fillrectangle(50, 350, 50, 50);
            break;
        case 'v':
            gfx_fillrectangle(100, 350, 50, 50);
            break;
        case 'w':
            gfx_fillrectangle(150, 350, 50, 50);
            break;
        case 'x':
            gfx_fillrectangle(200, 350, 50, 50);
            break;
        case 'y':
            gfx_fillrectangle(250, 350, 50, 50);
            break;
        case 'z':
            gfx_fillrectangle(150, 400, 50, 50);
            break;
        default:
            break;
    }
    return 0;
}

int fillGreen(struct Globals *global) {
    gfx_color(69, 75, 27);
    switch (global->userInput) {
        case 'a':
            gfx_fillrectangle(50, 150, 50, 50);
            break;
        case 'b':
            gfx_fillrectangle(100, 150, 50, 50);
            break;
        case 'c':
            gfx_fillrectangle(150, 150, 50, 50);
            break;
        case 'd':
            gfx_fillrectangle(200, 150, 50, 50);
            break;
        case 'e':
            gfx_fillrectangle(250, 150, 50, 50);
            break;
        case 'f':
            gfx_fillrectangle(50, 200, 50, 50);
            break;
        case 'g':
            gfx_fillrectangle(100, 200, 50, 50);
            break;
        case 'h':
            gfx_fillrectangle(150, 200, 50, 50);
            break;
        case 'i':
            gfx_fillrectangle(200, 200, 50, 50);
            break;
        case 'j':
            gfx_fillrectangle(250, 200, 50, 50);
            break;
        case 'k':
            gfx_fillrectangle(50, 250, 50, 50);
            break;
        case 'l':
            gfx_fillrectangle(100, 250, 50, 50);
            break;
        case 'm':
            gfx_fillrectangle(150, 250, 50, 50);
            break;
        case 'n':
            gfx_fillrectangle(200, 250, 50, 50);
            break;
        case 'o':
            gfx_fillrectangle(250, 250, 50, 50);
            break;
        case 'p':
            gfx_fillrectangle(50, 300, 50, 50);
            break;
        case 'q':
            gfx_fillrectangle(100, 300, 50, 50);
            break;
        case 'r':
            gfx_fillrectangle(150, 300, 50, 50);
            break;
        case 's':
            gfx_fillrectangle(200, 300, 50, 50);
            break;
        case 't':
            gfx_fillrectangle(250, 300, 50, 50);
            break;
        case 'u':
            gfx_fillrectangle(50, 350, 50, 50);
            break;
        case 'v':
            gfx_fillrectangle(100, 350, 50, 50);
            break;
        case 'w':
            gfx_fillrectangle(150, 350, 50, 50);
            break;
        case 'x':
            gfx_fillrectangle(200, 350, 50, 50);
            break;
        case 'y':
            gfx_fillrectangle(250, 350, 50, 50);
            break;
        case 'z':
            gfx_fillrectangle(150, 400, 50, 50);
            break;
        default:
            break;
    }
    return 0;
}

int play(struct Globals *global) {
    int width = 640;
    int height = 480;
    int selected = 0;
    char c;
    int xpos;
    int ypos;
    int buttonPressed = 0;
    int x, y, i, j, k, l, m, n, count, match;
    int wrongAttempt = 0;

    //while (1)
    //{
    gfx_clear_color(200, 200, 200);  // Set the background color to gray
    gfx_clear();

    gfx_color(100, 100, 100);
    gfx_rectangle(10, 10, 50, 20); //menu button
    gfx_text("Menu", 15, 12, 1);

    gfx_color(0, 0, 0);  // Set the text color to black

    char alphabet[] = "abcdefghijklmnopqrstuvwxy";
    int xPosForGuesses = 90;
    char guessedWord[15] = {'\0'};
    do {
        gfx_color(0, 0, 0);

        // TODO: change this to something nicer
        if (strncmp(global->line1, guessedWord, strlen(global->line1)) == 0) {
            gfx_text("CONGRATULATIONS!", 0, 0, 2);
            gfx_wait();
            break;
        }

        int startXPos = 400;
        for (int j = 0; j < strlen(global->line1); j++) {
            startXPos += 15;
            if (guessedWord[j] == '\0') {
                gfx_text("_", startXPos, 400, 2);
            } else {
                gfx_text(&guessedWord[j], startXPos, 400, 2);
            }
        }
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 5; j++) {
                gfx_rectangle(50 + (i * 50), 150 + (j * 50), 50, 50);
            }
        }

        gfx_rectangle(150, 400, 50, 50); //extra box for z
        count = 0;

        for (k = 0; k < 5; k++) {
            y = 15 + 150 + k * 50;
            for (l = 0; l < 5; l++) {
                x = 20 + 50 + l * 50;
                char c1[2] = {alphabet[count], '\0'};
                gfx_text(c1, x, y, 1);
                count++;
            }
        }

        gfx_text("z", 170, 415, 1);

        gfx_flush();

        buttonPressed = 0;
        c = gfx_wait();
        xpos = gfx_xpos();
        ypos = gfx_ypos();

        if (xpos > 50 && xpos < 100 && ypos > 150 && ypos < 200) {
            buttonPressed = 1; //A
            global->userInput = 'a';
        } else if (xpos > 100 && xpos < 150 && ypos > 150 && ypos < 200) {
            buttonPressed = 2; //B
            global->userInput = 'b';
        } else if (xpos > 150 && xpos < 200 && ypos > 150 && ypos < 200) {
            buttonPressed = 3; //C
            global->userInput = 'c';
        } else if (xpos > 200 && xpos < 250 && ypos > 150 && ypos < 200) {
            buttonPressed = 4; //D
            global->userInput = 'd';
        } else if (xpos > 250 && xpos < 300 && ypos > 150 && ypos < 200) {
            buttonPressed = 5; //E
            global->userInput = 'e';
        } else if (xpos > 50 && xpos < 100 && ypos > 200 && ypos < 250) {
            buttonPressed = 6; //F
            global->userInput = 'f';
        } else if (xpos > 100 && xpos < 150 && ypos > 200 && ypos < 250) {
            buttonPressed = 7; //G
            global->userInput = 'g';
        } else if (xpos > 150 && xpos < 200 && ypos > 200 && ypos < 250) {
            buttonPressed = 8; //H
            global->userInput = 'h';
        } else if (xpos > 200 && xpos < 250 && ypos > 200 && ypos < 250) {
            buttonPressed = 9; //I
            global->userInput = 'i';
        } else if (xpos > 250 && xpos < 300 && ypos > 200 && ypos < 250) {
            buttonPressed = 10; //J
            global->userInput = 'j';
        } else if (xpos > 50 && xpos < 100 && ypos > 250 && ypos < 300) {
            buttonPressed = 11; //K
            global->userInput = 'k';
        } else if (xpos > 100 && xpos < 150 && ypos > 250 && ypos < 300) {
            buttonPressed = 12; //L
            global->userInput = 'l';
        } else if (xpos > 150 && xpos < 200 && ypos > 250 && ypos < 300) {
            buttonPressed = 13; //M
            global->userInput = 'm';
        } else if (xpos > 200 && xpos < 250 && ypos > 250 && ypos < 300) {
            buttonPressed = 14; //N
            global->userInput = 'n';
        } else if (xpos > 250 && xpos < 300 && ypos > 250 && ypos < 300) {
            buttonPressed = 15; //O
            global->userInput = 'o';
        } else if (xpos > 50 && xpos < 100 && ypos > 300 && ypos < 350) {
            buttonPressed = 16; //P
            global->userInput = 'p';
        } else if (xpos > 100 && xpos < 150 && ypos > 300 && ypos < 350) {
            buttonPressed = 17; //Q
            global->userInput = 'q';
        } else if (xpos > 150 && xpos < 200 && ypos > 300 && ypos < 350) {
            buttonPressed = 18; //R
            global->userInput = 'r';
        } else if (xpos > 200 && xpos < 250 && ypos > 300 && ypos < 350) {
            buttonPressed = 19; //S
            global->userInput = 's';
        } else if (xpos > 250 && xpos < 300 && ypos > 300 && ypos < 350) {
            buttonPressed = 20; //T
            global->userInput = 't';
        } else if (xpos > 50 && xpos < 100 && ypos > 350 && ypos < 400) {
            buttonPressed = 21; //U
            global->userInput = 'u';
        } else if (xpos > 100 && xpos < 150 && ypos > 350 && ypos < 400) {
            buttonPressed = 22; //V
            global->userInput = 'v';
        } else if (xpos > 150 && xpos < 200 && ypos > 350 && ypos < 400) {
            buttonPressed = 23; //W
            global->userInput = 'w';
        } else if (xpos > 200 && xpos < 250 && ypos > 350 && ypos < 400) {
            buttonPressed = 24; //X
            global->userInput = 'x';
        } else if (xpos > 250 && xpos < 300 && ypos > 350 && ypos < 400) {
            buttonPressed = 25; //Y
            global->userInput = 'y';
        } else if (xpos > 150 && xpos < 200 && ypos > 400 && ypos < 450) {
            buttonPressed = 26; //Z
            global->userInput = 'z';
        } else if (xpos > 10 && xpos < 60 && ypos > 10 && ypos < 30) {
            break;
        }

        printf("Button pressed = %c\n", global->userInput);
        printf("Button pressed = %d\n", buttonPressed);
        printf("Mouse pos xpos is %d and ypos is %d \n", xpos, ypos);
        printf("stringlength = %d\n", global->stringLength);

        match = 0;

        for (m = 0; m < global->stringLength; m++) {
            if (global->userInput == global->line1[m]) {
                fillGreen(global);
                match = 1;
                guessedWord[m] = global->line1[m];
            }
        }

        if (match == 0) {
            fillRed(global);
            xPosForGuesses += 10;
            gfx_text("X", xPosForGuesses, 50, 1);
            wrongAttempt++;
        }
//        } else {
//            gfx_color(69, 75, 27);
//            gfx_text("X", xPosForGuesses, 50, 1);
//        }


//        if (wrongAttempt == 1)
//            gfx_text("X", 100, 50, 1);
//        else if (wrongAttempt == 2)
//            gfx_text("X", 110, 50, 1);
//        else if (wrongAttempt == 3)
//            gfx_text("X", 120, 50, 1);
//        else if (wrongAttempt == 4)
//            gfx_text("X", 130, 50, 1);
//        else if (wrongAttempt == 5)
//            gfx_text("X", 140, 50, 1);

        if (wrongAttempt > 5) {
            break;
        }
        gfx_color(0, 0, 0); //menu button

    } while (global->eventManager == 1);


    return 0;
}

//int menuButton(int x, int y) {
//    int xpos;
//    int ypos;
//
//    gfx_color(0, 0, 0);
//    gfx_rectangle(x, y, 130, 30);
//    gfx_text("Menu", x + (130 / 2), y + (30 / 2), 3);
//    gfx_wait();
//    if (xpos > x && xpos < x + 130 && ypos > y && ypos < y + 30) {
//        break;
//    }
//
//    return 0;
//}
