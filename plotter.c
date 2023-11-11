#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "cplotgibb.h"

void damped_sin(double *y, double *x, int N)
{
    for (int i = 0; i < N; i++)
    {
        y[i] = exp(-x[i] / 8) * sin(2 * x[i]);
    }
}

void crit_damped(double *y, double *x, int N)
{
    for (int i = 0; i < N; i++)
    {
        y[i] = x[i] * exp(-x[i] / 2);
    }
}

int main()
{
    // Sample input arrays x and y
    double x[1000];
    double y1[1000];
    double y2[1000];

    x[0] = 0;
    for (int i = 1; i < 1000; i++)
    {
        x[i] = x[i - 1] + 0.01;
    }

    damped_sin(y1, x, 1000);
    crit_damped(y2, x, 1000);

    int n = sizeof(x) / sizeof(x[0]); // Size of the arrays

    FILE *fp = tikz_figure("out.tikz");
    tikz_define_axis(fp, 5, "center", 0., 10., -1., 1.);
    tikz_plot(fp, x, y1, n, "teal", "$y_1(t)$");
    tikz_plot(fp, x, y2, n, "orange", "$y_2(t)$");
    tikz_grid(fp);
    tikz_dims(fp, 12, 8); // width = 12cm, height = 8cm
    tikz_xlabel(fp, "$t$");
    tikz_ylabel(fp, "$y(t)$");
    tikz_legend(fp, "north east");
    tikz_plotter_close(fp);

    return 0;
}
