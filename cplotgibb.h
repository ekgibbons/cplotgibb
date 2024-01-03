/**
 * @file      cplotgibb.h
 * @brief     Header for functions to write to tikz
 * @date      Fri Nov 10 15:24:37 2023
 * @author    Eric Gibbons, Weber State University
 * @copyright BSD-3-Clause
 *
 * This module will read C arrays and write them to formatted tikz figure files.
 * The syntax is largely modeled after the behavior seen in matplotlib.
 */

#ifndef CPLOTGIBB_H
#define CPLOTGIBB_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct plot_data
{
    int length;
    char type[100];
    char color[20];
    char legend[50];
    struct plot_data *next;
    double data[0];
} plot_data;

typedef struct plt
{
    char filename[100];
    char type[10];
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    unsigned char grid;
    double width;
    double height;
    char xlabel[100];
    char ylabel[100];
    char legend_position[20];
    plot_data *data;
} plt;

plt *plt_figure(char *filename)
{
    plt *figure = (plt *)malloc(sizeof(*figure));
    strcpy(figure->filename, filename);
    strcpy(figure->type, "standard");
    strcpy(figure->xlabel, "");
    strcpy(figure->ylabel, "");
    strcpy(figure->legend_position, "");
    figure->xmin = 0.;
    figure->xmax = 0.;
    figure->ymin = 0.;
    figure->ymax = 0.;
    figure->data = NULL;

    return figure;
}

void plt_axes_type(plt *figure, char *type)
{
    strcpy(figure->type, type);
}

void plt_xlim(plt *figure, double xmin, double xmax)
{
    figure->xmin = xmin;
    figure->xmax = xmax;
}

void plt_ylim(plt *figure, double ymin, double ymax)
{
    figure->ymin = ymin;
    figure->ymax = ymax;
}

void plt_dims(plt *figure, double width, double height)
{
    figure->width = width;
    figure->height = height;
}

void plt_grid(plt *figure)
{
    figure->grid = 1;
}

void plt_xlabel(plt *figure, char *xlabel)
{
    strcpy(figure->xlabel, xlabel);
}

void plt_ylabel(plt *figure, char *ylabel)
{
    strcpy(figure->ylabel, ylabel);
}

void plt_legend_pos(plt *figure, char *position)
{
    strcpy(figure->legend_position, position);
}

void plt_plot(plt *figure, double *x, double *y, int data_len, char *color,
              char *legend_entry)
{
    plot_data *data_in = (plot_data *)malloc(
        sizeof(*data_in) + sizeof(double) * (size_t)(2 * data_len));
    data_in->length = data_len;
    strcpy(data_in->type, "plot");
    strcpy(data_in->color, color);
    strcpy(data_in->legend, legend_entry);
    data_in->next = NULL;

    for (int i = 0; i < data_len; i++)
    {
        data_in->data[2 * i] = x[i];
        data_in->data[2 * i + 1] = y[i];
    }

    if (figure->data == NULL)
    {
        figure->data = data_in;
    }
    else
    {
        plot_data *current = figure->data;

        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = data_in;
    }
}

void plt_stem(plt *figure, double *x, double *y, int data_len, char *color,
              char *legend_entry)
{
    plot_data *data_in = (plot_data *)malloc(
        sizeof(*data_in) + sizeof(double) * (size_t)(2 * data_len));
    data_in->length = data_len;
    strcpy(data_in->type, "stem");
    strcpy(data_in->color, color);
    strcpy(data_in->legend, legend_entry);
    data_in->next = NULL;

    for (int i = 0; i < data_len; i++)
    {
        data_in->data[2 * i] = x[i];
        data_in->data[2 * i + 1] = y[i];
    }

    if (figure->data == NULL)
    {
        figure->data = data_in;
    }
    else
    {
        plot_data *current = figure->data;

        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = data_in;
    }
}

void add_plot(FILE *fp, plot_data *data_in)
{

    fprintf(fp, "\\addplot [\n");

    if (strcmp(data_in->color, "") != 0)
    {
        fprintf(fp, "color=%s,\n", data_in->color);
    }

    /* "mark=%s, " */
    /* "mark options={scale=1.5}, " */
    fprintf(fp, "line width=1pt] coordinates {\n");

    // Write x and y values to the plot
    for (int i = 0; i < data_in->length; ++i)
    {
        fprintf(fp, "    (%f,%f)\n", data_in->data[2 * i],
                data_in->data[2 * i + 1]);
    }

    fprintf(fp, "};\n");
    if (strcmp(data_in->legend, "") != 0)
    {
        fprintf(fp, "\\addlegendentry{%s}\n", data_in->legend);
    }
}

void add_stem(FILE *fp, plot_data *data_in)
{

    fprintf(
        fp,
        "\\addplot +[ycomb, %s, thick, mark options={fill}] coordinates {\n",
        data_in->color);

    // Write x and y values to the plot
    for (int i = 0; i < data_in->length; ++i)
    {
        fprintf(fp, "    (%f,%f)\n", data_in->data[2 * i],
                data_in->data[2 * i + 1]);
    }

    fprintf(fp, "};\n");
    if (strcmp(data_in->legend, "") != 0)
    {
        fprintf(fp, "\\addlegendentry{%s}\n", data_in->legend);
    }
}

void write_data(FILE *fp, plot_data *data_in)
{
    plot_data *current = data_in;

    while (current != NULL)
    {
        if (strcmp(current->type, "plot") == 0)
        {
            add_plot(fp, current);
        }
        else if (strcmp(current->type, "stem") == 0)
        {
            add_stem(fp, current);
        }
        else
        {
            printf("ERROR:  %s invalid plot type!\n", current->type);
            exit(1);
        }
        current = current->next;
    }
}

void clean_up(plt *figure)
{
    plot_data *current = figure->data;
    plot_data *previous;

    while (current != NULL)
    {
        previous = current;
        current = current->next;
        free(previous);
        previous = NULL;
    }

    free(figure);
    figure = NULL;
}

void plt_save_fig(plt *figure)
{
    FILE *fp = fopen(figure->filename, "w");
    fprintf(fp, "\\begin{tikzpicture}\n");
    fprintf(fp, "\\begin{axis}\n");
    fprintf(fp, "[\n");

    // Fix label locations for "center" case
    if (strcmp(figure->type, "center") == 0)
    {
        fprintf(fp, "axis lines=center,\n"
                    "axis x line = middle,\n"
                    "every axis x label/.style={\n"
                    "at={(ticklabel* cs:1.0)},\n"
                    "anchor=west,\n"
                    "},\n"
                    "axis y line = left,\n"
                    "every axis y label/.style={\n"
                    "at={(ticklabel* cs:1.0)},\n"
                    "anchor=south,\n"
                    "},");
    }
    else if (strcmp(figure->type, "standard") == 0)
    {
    }
    else
    {
        printf("ERROR: please set axis type  'standard' or "
               "'center'\n");
        exit(1);
    }

    if ((figure->xmin != 0.) || (figure->xmax != 0.))
    {
        fprintf(fp, "xmin = %f, xmax = %f,\n", figure->xmin, figure->xmax);
    }

    if ((figure->ymin != 0.) || (figure->ymax != 0.))
    {
        fprintf(fp, "ymin = %f, ymax = %f,\n", figure->ymin, figure->ymax);
    }

    if (figure->grid == 1)
    {
        fprintf(fp, "grid=major,\n");
    }

    if (figure->width != 0.)
    {
        fprintf(fp, "width=%f cm,\n", figure->width);
    }

    if (figure->height != 0.)
    {
        fprintf(fp, "height=%f cm,\n", figure->height);
    }

    if (strcmp(figure->xlabel, "") != 0)
    {
        fprintf(fp, "xlabel=%s,\n", figure->xlabel);
    }

    if (strcmp(figure->ylabel, "") != 0)
    {
        fprintf(fp, "ylabel=%s,\n", figure->ylabel);
    }

    if (strcmp(figure->legend_position, "") != 0)
    {
        fprintf(fp, "legend pos=%s,\n", figure->legend_position);
    }

    fprintf(fp, "]\n");

    write_data(fp, figure->data);

    fprintf(fp, "\\end{axis}\n");
    fprintf(fp, "\\end{tikzpicture}\n");

    // Free memory
    clean_up(figure);

    // Close the file
    fclose(fp);
}

#endif /* CPLOTGIBB_H */
