#include <ncurses.h>
#include <math.h>
#include <complex.h>
#include <unistd.h>

#include "banner.h"

int RESOLUTION  = 120;
double ZOOM     = 1.1;
double OFFSET_X = -1.4075920618131836;
double OFFSET_Y = -0.12763681635104013;

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

    // Draw loop
    for (;;) 
    {
        draw(width, height, 0);
        ZOOM += 0.25 * ZOOM;

        // Update status bar
        werase(statusbar);
        wprintw(statusbar, " Render #%d", render);
        wrefresh(statusbar);
        render++;

        usleep(150000);
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
            int m = mandelbrot(c, RESOLUTION);

            // Fit output into char array and print
            double val = map(m, 0, RESOLUTION, 0, 5);
            mvaddch(y, x, intensity[(int)round(val)]);
        }
    }

    refresh();

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