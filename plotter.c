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

    double x_dt[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    double y_dt[11];

    for (int i = 0; i < 11; i++)
    {
        y_dt[i] = pow(0.75, x_dt[i]);
    }

    int n = sizeof(x) / sizeof(x[0]); // Size of the arrays

    plt *fig1 = plt_figure("out.pdf");

    plt_plot(fig1, x, y1, n, "teal", "$y_1(t)$");
    plt_plot(fig1, x, y2, n, "orange", "$y_2(t)$");
    plt_stem(fig1, x_dt, y_dt, 11, "red", "$y_d[n]$");

    plt_axes_type(fig1, "center");
    plt_xlim(fig1, 0, 10);
    plt_ylim(fig1, -1.1, 1.1);
    plt_grid(fig1);
    plt_dims(fig1, 12, 8); // width = 12cm, height = 8cm
    plt_xlabel(fig1, "$t$");
    plt_ylabel(fig1, "$y(t)$");

    plt_save_fig(fig1);

    return 0;
}
