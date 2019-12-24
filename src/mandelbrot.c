#include <ncurses.h>
#include <math.h>
#include <complex.h>
#include <unistd.h>

#include "banner.h"

#define SENSITIVITY 0.25

const int RESOLUTION  = 80; // Base resolution (crispness)
int MAX_ITER          = RESOLUTION; // ignore
double ZOOM           = 0.75;
double OFFSET_X       = -0.75;
double OFFSET_Y       = 0;

void draw(int width, int height, double drawtime);
int mandelbrot(complex c, int range);
double map(double value, double min, double max, double start, double end);

int main(void)
{
    initscr(); // Initialise ncurses
    start_color(); // Enable colors
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // Screen colors
    init_pair(2, COLOR_BLACK, COLOR_WHITE); // Status bar colors
    curs_set(0); // Hide cursor

    // Set screen colors
    bkgd(COLOR_PAIR(1));

    // Print banner
    printw(banner);
    refresh();
    getch();

    // Get width / height
    int width, height;
    getmaxyx(stdscr, height, width);

    // Create status bar
    WINDOW *statusbar = newwin(1, width, 0, 0);
    wbkgd(statusbar, COLOR_PAIR(2));

    // Status bar info
    int render = 0;

    // Kinda ugly but works
    werase(statusbar);
    wprintw(statusbar, " Render #%d: [x: %.8f, y: %.8f], ZOOM = %.2fx, MAX_ITER = %d",
            render, OFFSET_X, OFFSET_Y, ZOOM, MAX_ITER);
    draw(width, height, 0);
    refresh();
    wrefresh(statusbar);
    
    for (;;) 
    {
        // Listen for keypresses
        char key = getc(stdin);
        switch (key)
        {
            case 'q':
                endwin(); // End curses mode
                return 0;
            case 'w':
                OFFSET_Y -= 1 / ZOOM * SENSITIVITY;
                break;
            case 'a':
                OFFSET_X -= 1 / ZOOM * SENSITIVITY;
                break;
            case 's':
                OFFSET_Y += 1 / ZOOM * SENSITIVITY;
                break;
            case 'd':
                OFFSET_X += 1 / ZOOM * SENSITIVITY;
                break;
            case 'e':
                ZOOM -= ZOOM * SENSITIVITY;
                if (ZOOM < 0) ZOOM = 1;
                break;
            case 'r':
                ZOOM += ZOOM * SENSITIVITY;
                break;               
            default:
                continue;
        }

        // Dinamically increase resolution based on ZOOM
        // Comment out to disable
        MAX_ITER = round(log10(ZOOM) * RESOLUTION) + 1;
        if (MAX_ITER < RESOLUTION ) MAX_ITER = RESOLUTION;

        // Update status bar
        werase(statusbar);
        wprintw(statusbar, " Render #%d: [x: %.8f, y: %.8f], ZOOM = %.2fx, MAX_ITER = %d",
                render, OFFSET_X, OFFSET_Y, ZOOM, MAX_ITER);
        render++;

        // Draw fractal
        draw(width, height, 0);

        // Refresh windows
        refresh();
        wrefresh(statusbar);
    }

    return 0;
}

void draw(int width, int height, double drawtime)
{
    char *intensity = " .,-+#";

    // y = 1 for statusbar
    for (int y = 1; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // Get complex number based on current "pixel"
            double cx = map(x, 0, width,  -1, 1);
            double cy = map(y, 0, height, -1, 1);
            complex c = (cx / ZOOM + OFFSET_X) + (cy / ZOOM + OFFSET_Y) * I;
            int m = mandelbrot(c, MAX_ITER);

            // Map fractal into char array and print
            double val = map(m, 0, MAX_ITER, 0, 5);
            mvaddch(y, x, intensity[(int)round(val)]);
        }
    }

    return;
}

int mandelbrot(complex c, int range)
{
    complex z = 0;
    int i;
    for (i = 0; i < range && cabs(z) <= 2; ++i)
    {
        z = z * z + c;
    }

    return i;
}

double map(double value, double min, double max, double start, double end)
{
    return ((value / (max - min) * (end - start)) + start);
}