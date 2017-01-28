#include "snake.h"

#include <curses.h>

int main(void)
{
    initscr();
    timeout(100);
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    int maxx, maxy;
    getmaxyx(stdscr, maxy, maxx);
    Field* field = create_field(maxx, maxy);
    grow_snake(field->snake, make_pos(2, 2));
    grow_snake(field->snake, make_pos(2, 3));
    
    for (int i = 0; i < 6; ++i) {
        add_food_random(field, FOOD_NORMAL);
    }

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

        if (field->snake_collided == COLLISION_FOOD) {
            add_food_random(field, FOOD_NORMAL);
        } else if (field->snake_collided == COLLISION_SELF) {
            break;
        }
        erase();

        for (PosList* it = field->snake->head; it != NULL; it = it->next) {
            mvaddch(it->y, it->x, '#');
        }

        for (int i = 0; i < field->n_foods; ++i) {
            mvaddch(field->foods[i].pos.y, field->foods[i].pos.x, ACS_DIAMOND);
        }
    }
    free_field(field);

    timeout(0);
    getch();
    endwin();
    return 0;
}
