//
// Created by maximus on 31.12.23.
//
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include "prj_types/String_struct.h"
#include "terminal_funcs.h"
#include "random/statistical_random.h"
#include "colors.h"
#include "loging/log.h"
#include <pthread.h>
#include <time.h>
#include <errno.h>

#include <termios.h>
#define background_fill color_to_rgb_background(20, 20, 20);
/* msleep(): Sleep for the requested number of milliseconds. */
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do
    {
        res = nanosleep(&ts, &ts);
    }
    while (res && errno == EINTR);

    return res;
}

struct winsize get_window_size()
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w;
}
//struct uint_pair{
//    uint x,y:3;
//};

string generate_snow_string(float snow_chance, int size)
{
    string_fast sf = string_fast_create_new(size);
    for (int I = 0; I < size; ++I)
    {
        float chance = rand_plain;
        if (chance < snow_chance)
        {
            char *snowflakes = "**..F";
            string_fast_add_char(&sf, snowflakes[randint(0, strlen(snowflakes))]);
        }
        else
        {
            string_fast_add_char(&sf, ' ');
        }
    }
    return sf.string_part;
}

string_array generate_snow(float snow_chance)
{
    struct winsize w = get_window_size();
    string_array array = string_array_create();
    for (int i = 0; i < w.ws_row; ++i)
    {
        string_array_push(&array, generate_snow_string(snow_chance, w.ws_col));
    }
    write_log(INFO, "snow size = %d", array.size);
    return array;
}

void render_snow(string_array *snow, float snow_chance, int num, int even)
{
    terminal_invisible_cursor;
    for (int i = 0; i < snow->size; ++i)
    {
        printf("%s\n", snow->elements[i].line);
        write_log(DEBUG, "Snow showed %d", i);
    }
    write_log(DEBUG, "Snow showed");
    if (num % even == 0)
    {
        string poped = string_array_pop(snow);
        struct winsize w = get_window_size();
        string new_line = generate_snow_string(snow_chance, w.ws_col);
        write_log(DEBUG, "New line created");
        free_string(poped);
        write_log(DEBUG, "Line fried");
        string_array_push_to_start(snow, new_line);
        write_log(INFO, "snow size = %d", snow->size);
        write_log(DEBUG, "Line added");
    }
//    terminal_visible_cursor;
}

void render_labels(string_array labels, uint frame_no, uint start_pos, struct winsize size)
{
    terminal_invisible_cursor;
    for (uint i = 0; i < labels.size; i++, start_pos++)
    {
        if (start_pos > size.ws_row)
        {
            break;
        }
        string str = string_array_get_element(&labels, i);
        uint horizontal_start = str.len > size.ws_col ? 0 : (size.ws_col - str.len) / 2;
        terminal_goto_xy(horizontal_start, start_pos);
        //color_to_rgb_background(0, 0, 80);
        background_fill
        color_to_rgb_foreground(255, 255, 0);
        for (uint current_char = 0; horizontal_start < size.ws_col && current_char < str.len && current_char < frame_no;
             horizontal_start++, current_char++)
        {
            if (current_char == frame_no - 1)
            {
                color_to_rgb_background(255, 255, 0);
                color_to_rgb_foreground(0, 0, 0);
            }
            printf("%c", *str.line++);
        }
    }
    color_to_default();
//    terminal_visible_cursor;
}
void render_NY_tree(struct winsize size, uint start, uint height, uint *toy_type)
{

    for (uint start_horizontal = size.ws_col / 2, width = 1, toy_array_pos = 0; height--;
         start++, start_horizontal--, width += 2)
    {
        terminal_goto_xy(start_horizontal, start);
        for (uint i = width; i; i--, toy_array_pos++)
        {
            switch (toy_type[toy_array_pos])
            {
                case 0:;
                    int lightness = randint(0, 50), add_green = randint(0, 50);
                    color_to_rgb_background(lightness, 120 + add_green + lightness, lightness);
                    break;
                case 1:
                    color_to_rgb_background(255, 255, 0);
                    break;
                case 2:
                    color_to_rgb_background(255, 0, 255);
                    break;
                case 3:
                    color_to_rgb_background(0, 0, 255);
                    break;
                case 4:
                    color_to_rgb_background(255, 0, 0);
                    break;
            }
            putchar(' ');
        }
    }
    uint start_horizontal = size.ws_col / 2 - 1;
    start;
    color_to_rgb_background(70, 50, 30);
    for (int i = 0; i < 2; i++)
    {
        terminal_goto_xy(start_horizontal, start++);
        for (int i2 = 0; i2 < 3; i2++)
        {
            putchar(' ');
        }
    }
}
uint *gen_toy_type(uint height)
{
    uint *result = malloc(sizeof(uint) * height * height);
    for (uint i = 0; i < height * height; i++)
    {
        result[i] = !randint(0, 5) ? randint(1, 5) : 0;
    }
    return result;
}
void fill_screen(struct winsize size)
{
    background_fill
    terminal_goto_xy(0, 0);
    for (int i = 0; i < size.ws_row; i++)
    {
        for (int j = 0; j < size.ws_col; j++)
        {
            printf(" ");
        }
    }
}

int main()
{
    terminal_invisible_cursor;
    init_logger(INFO, "log.txt");
    write_log(INFO, "Program start");

    float snow_chance = 0.05;
    uint height = 15;
    uint *toys = gen_toy_type(height);
    string_array snow = generate_snow(snow_chance);

    string_array arr = string_array_create();
    string_array_push_charp(&arr, "Happy New Year!");
    string_array_push_charp(&arr, "");
    string_array_push_charp(&arr, "BFO wishes you all to:");
    string_array_push_charp(&arr, "write code like crazy");
    string_array_push_charp(&arr, "learn something new every minute");
    string_array_push_charp(&arr, "learn more math, because math is the best science");
    string_array_push_charp(&arr, "take part in scientific progress");
    string_array_push_charp(&arr, "survive");
    srandom(time(NULL));
    terminal_erase_display;
    write_log(INFO, "Init succes");
    for (int I = 0; I < 100; ++I)
    {
//        terminal_erase_screen;

        struct winsize size = get_window_size();
        fill_screen(size);
        write_log(INFO, "Screen filed %d", I);
        render_snow(&snow, snow_chance, I, 2);
        write_log(INFO, "Snow rendered %d", I);
        fflush(stdout);
        render_NY_tree(size, 12, height, toys);
        render_labels(arr, I, 32, size);
        //printf("C");
        fflush(stdout);
        msleep(100);
    }
    free_string_array(&snow);
    free_string_array(&arr);
    write_log(INFO, "Program end");
    free(toys);
    struct winsize size = get_window_size();
    terminal_goto_xy(0, size.ws_col);
    terminal_visible_cursor;
    return !"! HAPPY NEW YEAR !";
}