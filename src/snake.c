#include <snake.h>

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

Pos make_pos(int x, int y) {
    Pos pos;
    pos.x = x;
    pos.y = y;
    return pos;
}

void free_snake(Snake* snake) {
    while(snake->head != NULL) {
        PosList* temp = snake->head->next;
        free(snake->head);
        snake->head = temp;
    }
    free(snake);
}

Snake* create_snake() {
    Snake* snake = (Snake*) malloc(sizeof(Snake));
    snake->head = NULL;
    return snake;
}

int length_snake(Snake* snake) {
    int length = 0;
    for(PosList* it = snake->head; it != NULL; it=it->next, length++);
    return length;
}

void grow_snake(Snake* snake, Pos pos) {
    PosList* newhead = (PosList*) malloc(sizeof(PosList));
    newhead->x = pos.x;
    newhead->y = pos.y;

    newhead->next = snake->head;
    snake->head = newhead;

    return;
}

int move_snake(Snake* snake, Pos pos) {
    if (snake->head == NULL) return -1;
    grow_snake(snake, pos);

    PosList* it;
    for(it = snake->head; it->next->next != NULL; it=it->next);
    free(it->next);
    it->next = NULL;

    return 0;
}


Field* create_field(int width, int height) {
    Field* field = (Field*) malloc(sizeof(Field));
    field->width = width;
    field->height = height;
    field->snake = create_snake();
    field->snake_dir = RIGHT;
    field->snake_collided = COLLISION_NONE;
    field->foods = NULL;
    field->n_foods = 0;
    return field;
}

int add_food(Field* field, Pos pos, FoodType type) {
    for (int i = 0; i < field->n_foods; ++i) {
        if (field->foods[i].pos.x == pos.x && field->foods[i].pos.y == pos.y) {
            return -1;
        }
    }

    field->n_foods += 1;
    field->foods = realloc(field->foods, field->n_foods * sizeof(Food));
    field->foods[field->n_foods - 1].pos = pos;
    field->foods[field->n_foods - 1].type = type;

    return 0;
}

void add_food_random(Field* field, FoodType type) {
    Pos new_food_pos;
    do {
        new_food_pos = (Pos) {rand() % field->width, rand() % field->height};
    } while (is_food(field, new_food_pos) || is_snake(field, new_food_pos));
    add_food(field, new_food_pos, type);
}

int remove_food(Field* field, Pos pos) {
    int found = -1;
    for (int i = 0; i < field->n_foods; ++i) {
        if (field->foods[i].pos.x == pos.x && field->foods[i].pos.y == pos.y) {
            found = i;
            break;
        }
    }

    if (found == -1) return -1;
    Food* newfoods = malloc(sizeof(Food) * (field->n_foods - 1));
    memcpy(newfoods, field->foods, sizeof(Food) * found);
    memcpy(newfoods+found, 
            (field->foods + found + 1), 
            sizeof(Food) * (field->n_foods - 1 - found));
    field->n_foods--;
    free(field->foods);
    field->foods = newfoods;

    return 0;
}

Dir change_snake_dir(Field* field, Dir newdir) {
    if (field->snake_dir == -newdir) {
        // Snake can't move back
        // preserve the original direction
        return field->snake_dir; 
    }

    field->snake_dir = newdir;

    return newdir;
}

bool is_food(Field* field, Pos pos) {
    for (int i = 0; i < field->n_foods; ++i) {
        if (field->foods[i].pos.x == pos.x && field->foods[i].pos.y == pos.y) {
            return true;
        }
    }
    return false;
}

bool is_snake(Field* field, Pos pos) {
    for (PosList* it = field->snake->head; it != NULL; it = it->next) {
        if (it->x == pos.x && it->y == pos.y) {
            return true;
        }
    }
    return false;
}

void move_snake_on_field(Field* field) {
    int xdiff = 0, ydiff = 0;
    switch(field->snake_dir) {
        case UP:
            ydiff = -1;
            break;
        case DOWN:
            ydiff = +1;
            break;
        case RIGHT:
            xdiff = 1;
            break;
        case LEFT:
            xdiff = -1;
            break;
    };

    int newx = (field->snake->head->x + xdiff) % field->width;
    int newy = (field->snake->head->y + ydiff) % field->height;
    if (newx < 0) newx += field->width;
    if (newy < 0) newy = newy + field->height;
    Pos newpos = {newx, newy};
    

    if (is_food(field, newpos)) {
        grow_snake(field->snake, newpos);
        field->snake_collided = COLLISION_FOOD;
        remove_food(field, newpos);
        return;
    } else if (is_snake(field, newpos)) {
        field->snake_collided = COLLISION_SELF;
        return;
    } else {
        move_snake(field->snake, newpos);
        field->snake_collided = COLLISION_NONE;
    }
}

void free_field(Field* field) {
    free_snake(field->snake);
    free(field->foods);
    free(field);
}


