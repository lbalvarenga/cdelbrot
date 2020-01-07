#include <ncurses.h>
#include <math.h>
#include <complex.h>
#include <time.h> // For benchmarking

#include "banner.h"

typedef struct _FRACTAL_SETTINGS_
{    
    int resolution;
    int max_iter;
    unsigned int width;
    unsigned int height;
    unsigned int iteration;
    double sensitivity;
    double zoom;
    double offset_x;
    double offset_y;
} fractal_settings;

// try this: [x: -0.74815226, y: -0.07151054]

void draw_fractal(fractal_settings settings);
void draw_screen(fractal_settings *settings, WINDOW *statusbar);
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

    // Set fractal settings
    fractal_settings settings;
    settings.resolution  = 80;
    settings.max_iter    = settings.resolution;
    getmaxyx(stdscr, settings.height, settings.width);
    settings.iteration   = 0;
    settings.sensitivity = 0.25;
    settings.zoom        = 0.75;
    settings.offset_x    = -0.75;
    settings.offset_y    =  0.00;

    // Create status bar
    WINDOW *statusbar = newwin(1, settings.width, 0, 0);
    wbkgd(statusbar, COLOR_PAIR(2));

    // Initial draw
    draw_screen(&settings, statusbar);

    while (true) 
    {
        // Listen for keypresses
        char key = getc(stdin);
        switch (key)
        {
            case 'q':
                endwin(); // End curses mode
                return 0;
            case 'w':
                settings.offset_y -= 1 / settings.zoom * settings.sensitivity;
                break;
            case 'a':
                settings.offset_x -= 1 / settings.zoom * settings.sensitivity;
                break;
            case 's':
                settings.offset_y += 1 / settings.zoom * settings.sensitivity;
                break;
            case 'd':
                settings.offset_x += 1 / settings.zoom * settings.sensitivity;
                break;
            case 'e':
                settings.zoom -= settings.zoom * settings.sensitivity;
                if (settings.zoom < 0) settings.zoom = 1;
                break;
            case 'r':
                settings.zoom += settings.zoom * settings.sensitivity;
                break;               
            default:
                continue;
        }

        draw_screen(&settings, statusbar);

    }

    return 0;
}

void draw_fractal(fractal_settings settings)
{
    char *intensity = " .-+#";

    // y = 1 for statusbar
    for (int y = 1; y < settings.height; ++y)
    {
        for (int x = 0; x < settings.width; ++x)
        {
            // Get complex number based on current "pixel"
            double cx = map(x, 0, settings.width,  -1, 1);
            double cy = map(y, 0, settings.height, -1, 1);
            complex c = (cx / settings.zoom + settings.offset_x) + (cy / settings.zoom + settings.offset_y) * I;
            int m = mandelbrot(c, settings.max_iter);

            // Map fractal into char array and print
            double val = map(m, 0, settings.max_iter, 0, 4);
            mvaddch(y, x, intensity[(int)val]);
        }
    }

    return;
}

void draw_screen(fractal_settings *settings, WINDOW *statusbar)
{
    // Dinamically increase resolution based on settings->zoom
    // Comment out to disable
    settings->max_iter = round(log10(settings->zoom) * settings->resolution);
    if (settings->max_iter < settings->resolution )
    {
        settings->max_iter = settings->resolution;
    }

    // Draw fractal
    clock_t clk = clock();

    draw_fractal(*settings);

    clk = clock() - clk;
    double exec_time = (double)clk / CLOCKS_PER_SEC;

    // Update status bar
    settings->iteration++;
    werase(statusbar);
    wprintw(statusbar, "(%.6fs) [#%d]: [x: %.8f, y: %.8f], ZOOM = %.2fx, MAX_ITER = %d",
            exec_time,
            settings->iteration,
            settings->offset_x,
            settings->offset_y,
            settings->zoom,
            settings->max_iter);

    // Refresh windows
    refresh();
    wrefresh(statusbar);
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
