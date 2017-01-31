#include "snake.h"

#include <curses.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_HIGH_SCORE 20


int main(void)
{
    const char* HIGH_SCORE_FILE = strcat(getenv("HOME"), "/.snake_highscore");
    initscr();
    start_color();
    timeout(100);
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    WINDOW* play_window = newwin(0, 0, 2, 0);
    WINDOW* score_window = newwin(1, 0, 0, 0);
    WINDOW* separator_window = newwin(1, 0, 1, 0);

    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);


    int maxx, maxy;
    getmaxyx(play_window, maxy, maxx);
    
    Field* field = create_field(maxx, maxy);
    grow_snake(field->snake, make_pos(2, 2));
    grow_snake(field->snake, make_pos(2, 3));
    
    for (int i = 0; i < 6; ++i) {
        add_food_random(field, FOOD_NORMAL);
    }
    
    int highscore;
    int score = 2;
    int score_attr = A_NORMAL;

    FILE* hf = fopen(HIGH_SCORE_FILE, "r");

    if (hf == NULL) highscore = DEFAULT_HIGH_SCORE;
    else {
        int err = fscanf(hf, "%d", &highscore);
        if (err == EOF) highscore = DEFAULT_HIGH_SCORE;
        fclose(hf);
    }

    while (true) {
        int ch = getch();
        bool quit = false;
        switch(ch) {
            case KEY_UP:
                change_snake_dir(field, UP);
                break;
            case KEY_DOWN:
                change_snake_dir(field, DOWN);
                break;
            case KEY_LEFT:
                change_snake_dir(field, LEFT);
                break;
            case KEY_RIGHT:
                change_snake_dir(field, RIGHT);
                break;
            case 'q':
            case 'Q':
                quit = true;
                break;
            default:
                break;
        }
        if (quit) break;

        move_snake_on_field(field);
        score = length_snake(field->snake);
        if (score > highscore) {
            highscore = score;
            score_attr = COLOR_PAIR(3) | A_BOLD;
        }
        

        if (field->snake_collided == COLLISION_FOOD) {
            add_food_random(field, FOOD_NORMAL);
        } else if (field->snake_collided == COLLISION_SELF) {
            break;
        }
        werase(score_window);
        werase(separator_window);
        werase(play_window);
        
        wattrset(score_window, A_NORMAL);
        wprintw(score_window, "Score: ");
        wattrset(score_window, score_attr);
        wprintw(score_window, "%5d", score);
        wattrset(score_window, A_NORMAL);
        wprintw(score_window, "\tHigh Score: ");
        wattrset(score_window, score_attr);
        wprintw(score_window, "%5d", highscore);
        
        for(int i = 0; i < maxx; ++i) {
            mvwaddch(separator_window, 0, i, '=');
        }

        for (PosList* it = field->snake->head; it != NULL; it = it->next) {
            mvwaddch(play_window, it->y, it->x, ACS_BLOCK | COLOR_PAIR(1));
        }

        for (int i = 0; i < field->n_foods; ++i) {
            mvwaddch(play_window, field->foods[i].pos.y, 
                    field->foods[i].pos.x, ACS_DIAMOND | COLOR_PAIR(2));
        }

        wrefresh(score_window);
        wrefresh(separator_window);
        wrefresh(play_window);
    }
    free_field(field);

    timeout(-1);
    char msg[] = "Game Over. Press any key to exit.";
    
    mvwprintw(play_window, maxy*4/5, (maxx - sizeof(msg))/2, msg);
    wrefresh(play_window);

    getch();
    endwin();

    hf = fopen(HIGH_SCORE_FILE, "w");
    if (hf != NULL) {
        fprintf(hf, "%d", highscore);
        fclose(hf);
    } else {
        printf("Error %s\n", HIGH_SCORE_FILE);
    }

    return 0;
}
