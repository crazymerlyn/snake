#include "../src/snake.h"

#test test_pos
    Pos pos = make_pos(2, 3);
    ck_assert_int_eq(pos.x, 2);
    ck_assert_int_eq(pos.y, 3);


#test test_create_snake
    Snake* snake = create_snake();
    ck_assert_ptr_eq(snake->head , NULL);
    free_snake(snake);


#test test_grow_snake
    Snake* snake = create_snake();
    grow_snake(snake, make_pos(2, 2));
    ck_assert_ptr_ne(snake->head, NULL);
    ck_assert_int_eq(snake->head->x, 2);
    ck_assert_int_eq(snake->head->y, 2);
    grow_snake(snake, make_pos(2, 3));
    ck_assert_int_eq(snake->head->y, 3);
    ck_assert_int_eq(snake->head->next->y, 2);
    free_snake(snake);


#test test_length_snake
    Snake* snake = create_snake();
    
    ck_assert_int_eq(length_snake(snake), 0);
    
    grow_snake(snake, make_pos(2, 2));
    ck_assert_int_eq(length_snake(snake), 1);

    grow_snake(snake, make_pos(2, 3));
    ck_assert_int_eq(length_snake(snake), 2);

    for (int i = 0; i < 100; ++i) {
        grow_snake(snake, make_pos(5, i));
    }
    ck_assert_int_eq(length_snake(snake), 102);

    free_snake(snake);


#test test_move_snake
    Snake* snake = create_snake();
    int res = move_snake(snake, make_pos(2, 2));
    ck_assert_int_eq(res, -1);
    ck_assert_ptr_eq(snake->head, NULL);

    grow_snake(snake, make_pos(2, 2));

    res = move_snake(snake, make_pos(2, 3));
    ck_assert_int_eq(res, 0);
    ck_assert_int_eq(snake->head->x, 2);
    ck_assert_int_eq(snake->head->y, 3);
    ck_assert_ptr_eq(snake->head->next, NULL);
    grow_snake(snake, make_pos(2, 4));

    move_snake(snake, make_pos(2, 5));
    ck_assert_ptr_ne(snake->head->next, NULL);
    ck_assert_int_eq(snake->head->next->x, 2);
    ck_assert_int_eq(snake->head->next->y, 4);
    ck_assert_ptr_eq(snake->head->next->next, NULL);

    free_snake(snake);

#test test_create_field
    Field* field = create_field(4, 5);
    ck_assert_int_eq(field->width, 4);
    ck_assert_int_eq(field->height, 5);
    ck_assert_int_eq(length_snake(field->snake), 0);
    ck_assert_int_eq(field->snake_dir, RIGHT);
    ck_assert_int_eq(field->snake_collided, COLLISION_NONE);
    ck_assert_int_eq(field->n_foods, 0);
    free_field(field);

#test test_add_food
    Field* field = create_field(2, 2);
    
    int res = add_food(field, make_pos(1, 1), FOOD_NORMAL);
    ck_assert_int_eq(field->n_foods, 1);
    ck_assert_int_eq(field->foods->pos.x, 1);
    ck_assert_int_eq(field->foods->pos.y, 1);
    ck_assert_int_eq(res, 0);
    
    add_food(field, make_pos(1, 0), FOOD_NORMAL);
    ck_assert_int_eq(field->n_foods, 2);

    res = add_food(field, make_pos(1, 0), FOOD_NORMAL);
    ck_assert_int_eq(field->n_foods, 2);
    ck_assert_int_eq(res, -1);
    
    free_field(field);


#test test_remove_food
    Field* field = create_field(2, 2);
    add_food(field, make_pos(1, 1), FOOD_NORMAL);
    add_food(field, make_pos(1, 0), FOOD_NORMAL);

    remove_food(field, make_pos(1, 1));
    ck_assert_int_eq(field->n_foods, 1);
    ck_assert_int_eq(field->foods[0].pos.x, 1);
    ck_assert_int_eq(field->foods[0].pos.y, 0);

    remove_food(field, make_pos(1, 0));
    ck_assert_int_eq(field->n_foods, 0);
    free_field(field);


#test test_change_snake_dir
    Field* field = create_field(4, 4);
    
    Dir newdir = change_snake_dir(field, UP);
    ck_assert_int_eq(newdir, UP);
    ck_assert_int_eq(field->snake_dir, UP);

    newdir = change_snake_dir(field, DOWN);
    ck_assert_int_eq(newdir, UP);
    ck_assert_int_eq(field->snake_dir, UP);

    change_snake_dir(field, LEFT);
    ck_assert_int_eq(field->snake_dir, LEFT);

    free_field(field);


#test test_is_food
    Field* field = create_field(10, 10);
    add_food(field, make_pos(2, 2), FOOD_NORMAL);
    ck_assert_int_eq(is_food(field, make_pos(2, 3)), false);
    ck_assert_int_eq(is_food(field, make_pos(2, 2)), true);

    add_food(field, make_pos(2, 3), FOOD_NORMAL);
    ck_assert_int_eq(is_food(field, make_pos(2, 3)), true);

    add_food(field, make_pos(2, 4), FOOD_NORMAL);
    remove_food(field, make_pos(2, 3));
    ck_assert_int_eq(is_food(field, make_pos(2, 3)), false);

    free_field(field);



#test test_move_snake_on_field
    Field* field = create_field(10, 10);
    add_food(field, make_pos(1, 1), FOOD_NORMAL);
    add_food(field, make_pos(1, 2), FOOD_NORMAL);
    add_food(field, make_pos(2, 2), FOOD_NORMAL);

    grow_snake(field->snake, make_pos(0, 0));
    grow_snake(field->snake, make_pos(0, 1));

    move_snake_on_field(field);
    ck_assert_int_eq(length_snake(field->snake), 3);
    ck_assert_int_eq(field->snake->head->x, 1);
    ck_assert_int_eq(field->snake_collided, COLLISION_FOOD);
    
    change_snake_dir(field, DOWN);
    move_snake_on_field(field);
    ck_assert_int_eq(length_snake(field->snake), 4);
    ck_assert_int_eq(field->snake->head->y, 2);

    change_snake_dir(field, RIGHT);
    move_snake_on_field(field);
    ck_assert_int_eq(length_snake(field->snake), 5);
    ck_assert_int_eq(field->snake->head->x, 2);

    change_snake_dir(field, UP);
    move_snake_on_field(field);
    ck_assert_int_eq(field->snake->head->y, 1);
    move_snake_on_field(field);
    ck_assert_int_eq(field->snake->head->y, 0);
    move_snake_on_field(field);
    ck_assert_int_eq(field->snake->head->y, field->height-1);
    
    free_field(field);


