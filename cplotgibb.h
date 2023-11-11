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

/**
 * @brief Open the .tikz file to write and start writing
 *
 * @param[in] filename file name
 * @return file pointer
 */
FILE *tikz_figure(char *filename)
{
    FILE *fp = fopen(filename, "w");
    fprintf(fp, "\\begin{tikzpicture}\n");

    return fp;
}

/**
 * @brief Finish writing the file and close it
 *
 * @param[in] fp file pointer
 */
void tikz_plotter_close(FILE *fp)
{
    fprintf(fp, "\\end{axis}\n");
    fprintf(fp, "\\end{tikzpicture}\n");

    // Close the file
    fclose(fp);
}

/**
 * @brief Define basic axes
 *
 * @param[inout] fp file pointer
 * @param[in] type axis type ("standard" or "center") where "standard" are the
 *                 axes normally seen in matplotlib and "center" is a more
 *                 traditional cartesian axisDescription
 * @param[in] count the number of additional inputs 0 to 4
 */
void tikz_define_axis(FILE *fp, int count, ...)
{
    // type ("center" | "standard")
    char type[10];

    // start parsing additional arguments
    va_list args;
    va_start(args, count);

    // if not specified, default to "standard"
    if (count == 0)
    {
        strcpy(type, "standard");
    }
    else
    {
        strcpy(type, va_arg(args, char *));
    }

    // Write the plot data
    fprintf(fp, "\\begin{axis}[\n");

    // Fix label locations for "center" case
    if (strcmp(type, "center") == 0)
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
    else if (strcmp(type, "standard") == 0)
    {
    }
    else
    {
        printf("ERROR: please set axis type  'standard' or 'center'\n");
        exit(1);
    }

    // Parse xlim and ylim

    // At least xlim values were entered
    if ((count == 3) || (count == 5))
    {
        double xmin = va_arg(args, double);
        double xmax = va_arg(args, double);

        fprintf(fp, "xmin = %f, xmax = %f,\n", xmin, xmax);

        // ylim values were ALSO added
        if (count == 5)
        {
            double ymin = va_arg(args, double);
            double ymax = va_arg(args, double);

            fprintf(fp, "ymin = %f, ymax = %f,\n", ymin, ymax);
        }
    }
    else if ((count != 0) && ((count % 2) == 0))
    {
        printf("ERROR: please specify the type, xmin, xmax, ymin, and ymax "
               "values\n");
        exit(1);
    }
    fprintf(fp, "]\n");
    va_end(args);
}

/**
 * @brief Adds plot line
 *
 * @param[inout] fp Description
 * @param[out] x Description
 * @param[out] y Description
 * @param[in] data_len The length of data
 * @param[out] color Description
 * @param[out] legend_entry legend label
 */
void tikz_plot(FILE *fp, double *x, double *y, int data_len, char *color,
               char *legend_entry)
{

    fprintf(fp,
            "\\addplot [color=%s, "
            /* "mark=%s, " */
            /* "mark options={scale=1.5}, " */
            "line width=1pt] coordinates {\n",
            color);

    // Write x and y values to the plot
    for (int i = 0; i < data_len; ++i)
    {
        fprintf(fp, "    (%f,%f)\n", x[i], y[i]);
    }

    fprintf(fp, "};\n");
    if (legend_entry != NULL)
    {
        fprintf(fp, "\\addlegendentry{%s}\n", legend_entry);
    }
}

/**
 * @brief Turn on grid lines
 *
 * @param[inout] fp file pointer
 */
void tikz_grid(FILE *fp)
{
    fprintf(fp, "\\pgfplotsset{grid=major}\n");
}

/**
 * @brief Set figure dimensions
 *
 * @param[inout] fp file pointer
 * @param[in] width width in centimeters
 * @param[in] height height in centimeters
 */
void tikz_dims(FILE *fp, double width, double height)
{
    fprintf(fp, "\\pgfplotsset{width = %f cm, height = %f cm}\n", width,
            height);
}

/**
 * @brief Add x-label
 *
 * @param[inout] fp file pointer
 * @param[out] xlabel text for x-axis label
 */
void tikz_xlabel(FILE *fp, char *xlabel)
{
    fprintf(fp, "\\pgfplotsset{xlabel=%s}\n", xlabel);
}

/**
 * @brief Add y-label
 *
 * @param[inout] fp file pointer
 * @param[out] ylabel text for y-axis label
 */
void tikz_ylabel(FILE *fp, char *ylabel)
{
    fprintf(fp, "\\pgfplotsset{ylabel=%s}\n", ylabel);
}

/**
 * @brief Set legend location
 *
 * @param[inout] fp file pointer
 * @param[out] position location ("north east" | "south east" | "south west" |
 *                      "north west")
 */
void tikz_legend(FILE *fp, char *position)
{
    fprintf(fp, "\\pgfplotsset{legend pos=%s}\n", position);
}

#endif /* CPLOTGIBB_H */
