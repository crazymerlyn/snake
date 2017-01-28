#include "snake.h"

#include <curses.h>

int main(void)
{
    initscr();
    timeout(100);
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    WINDOW* play_window = newwin(0, 0, 2, 0);
    WINDOW* score_window = newwin(1, 0, 0, 0);
    WINDOW* separator_window = newwin(1, 0, 1, 0);


    int maxx, maxy;
    getmaxyx(play_window, maxy, maxx);
    
    Field* field = create_field(maxx, maxy);
    grow_snake(field->snake, make_pos(2, 2));
    grow_snake(field->snake, make_pos(2, 3));
    
    for (int i = 0; i < 6; ++i) {
        add_food_random(field, FOOD_NORMAL);
    }
    
    int highscore = 20;
    int score = 2;

    while (true) {
        int ch = getch();
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
            default:
                break;
        }

        move_snake_on_field(field);
        score = length_snake(field->snake);

        if (field->snake_collided == COLLISION_FOOD) {
            add_food_random(field, FOOD_NORMAL);
        } else if (field->snake_collided == COLLISION_SELF) {
            break;
        }
        werase(score_window);
        werase(separator_window);
        werase(play_window);

        wprintw(score_window, "Score: %5d\tHigh Score: %5d", score, highscore);
        
        for(int i = 0; i < maxx; ++i) {
            mvwaddch(separator_window, 0, i, '=');
        }

        for (PosList* it = field->snake->head; it != NULL; it = it->next) {
            mvwaddch(play_window, it->y, it->x, '#');
        }

        for (int i = 0; i < field->n_foods; ++i) {
            mvwaddch(play_window, field->foods[i].pos.y, 
                    field->foods[i].pos.x, ACS_DIAMOND);
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
    return 0;
}
