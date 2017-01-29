#ifndef INCLUDE_SNAKE_H
#define INCLUDE_SNAKE_H

#include <stdbool.h>

typedef struct Pos {
    int x;
    int y;
} Pos;

typedef struct PosList {
    int x;
    int y;
    struct PosList* next;
} PosList;

typedef enum Dir {
    UP = -10,
    DOWN = 10,
    LEFT = -1,
    RIGHT = 1
} Dir;

typedef struct Snake {
    PosList* head;
} Snake;

typedef enum FoodType {
    FOOD_NORMAL,
    FOOD_BONUS
} FoodType;

typedef struct Food {
    Pos pos;
    FoodType type;
} Food;

typedef enum CollisionState {
    COLLISION_NONE,
    COLLISION_SELF,
    COLLISION_FOOD
} CollisionState;

typedef struct Field {
    Snake* snake;
    Dir snake_dir;
    CollisionState snake_collided;
    Food* foods;
    int n_foods;
    int width;
    int height;
} Field;


Pos make_pos(int x, int y);
Dir opposite_dir(Dir dir);
Snake* create_snake();
int length_snake(Snake* snake);
void grow_snake(Snake* snake, Pos pos);
int move_snake(Snake* snake, Pos pos);

void free_snake(Snake* snake);

Field* create_field(int width, int height);
int add_food(Field* field, Pos pos, FoodType type);
void add_food_random(Field* field, FoodType type);
bool is_food(Field* field, Pos pos);
bool is_snake(Field* field, Pos pos);
int remove_food(Field* field, Pos pos);
Dir change_snake_dir(Field* field, Dir newdir);
void move_snake_on_field(Field* field);
void free_field(Field* field);

#endif
