/**
 * @file cplotgibb.h
 * @brief Header file for functions to generate TikZ figures from C arrays.
 *
 * This header defines the interface for a module that reads C arrays and
 * writes them to formatted TikZ figure files. The module's syntax and
 * functionality are inspired by the behavior of matplotlib, providing
 * a convenient way to create publication-quality plots directly from C code.
 *
 * @details
 * The module supports various plotting functionalities, including:
 * - Writing data points to TikZ-compatible files.
 * - Configuring plot styles and options.
 * - Generating reusable and customizable TikZ figures.
 *
 * Example usage:
 * @code
 * // Example code snippet demonstrating usage
 * #include "cplotgibb.h"
 *
 *   plt *fig1 = plt_figure("out.pdf");
 *
 *   plt_plot(fig1, x, y1, n, "teal", "$y_1(t)$");
 *   plt_plot(fig1, x, y2, n, "orange", "$y_2(t)$");
 *   plt_stem(fig1, x_dt, y_dt, 11, "red", "$y_d[n]$");
 *
 *   plt_axes_type(fig1, "center");
 *   plt_xlim(fig1, 0, 10);
 *   plt_ylim(fig1, -1.1, 1.1);
 *   plt_grid(fig1);
 *   plt_dims(fig1, 12, 8); // width = 12cm, height = 8cm
 *   plt_xlabel(fig1, "$t$");
 *   plt_ylabel(fig1, "$y(t)$");
 *
 *   plt_save_fig(fig1);
 * @endcode
 *
 * @author Eric Gibbons
 * @date Fri Nov 10 15:24:37 2023
 * @copyright BSD-3-Clause
 */

#ifndef CPLOTGIBB_H
#define CPLOTGIBB_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @struct plot_data
 * @brief Represents a data structure for storing plot-related information.
 *
 * This structure is used to hold metadata and data for a plot. It includes
 * information about the plot's type, color, legend, and the actual data points.
 * It also supports linking to another plot_data structure, enabling the creation
 * of a linked list of plots.
 *
 * @var plot_data::length
 * The number of data points in the plot.
 *
 * @var plot_data::type
 * A string representing the type of the plot (e.g., "line", "bar").
 *
 * @var plot_data::color
 * A string specifying the color of the plot (e.g., "red", "blue").
 *
 * @var plot_data::legend
 * A string containing the legend or label for the plot.
 *
 * @var plot_data::next
 * A pointer to the next plot_data structure in a linked list.
 *
 * @var plot_data::data
 * A flexible array member to store the actual data points for the plot.
 * The size of this array is determined dynamically at runtime.
 */
typedef struct plot_data
{
    int length;
    char type[100];
    char color[20];
    char legend[50];
    struct plot_data *next;
    double data[0];
} plot_data;

/**
 * @struct plt
 * @brief Represents the configuration and data for a plot.
 *
 * This structure is used to define the properties and data associated with a plot,
 * including file information, axis ranges, dimensions, labels, and legend positioning.
 *
 * @var plt::filename
 * A string representing the name of the file associated with the plot. Maximum length: 100 characters.
 *
 * @var plt::type
 * A string representing the type of the plot (e.g., "line", "bar"). Maximum length: 10 characters.
 *
 * @var plt::xmin
 * The minimum value of the x-axis.
 *
 * @var plt::xmax
 * The maximum value of the x-axis.
 *
 * @var plt::ymin
 * The minimum value of the y-axis.
 *
 * @var plt::ymax
 * The maximum value of the y-axis.
 *
 * @var plt::grid
 * A flag indicating whether the grid is enabled (non-zero) or disabled (zero).
 *
 * @var plt::width
 * The width of the plot in user-defined units.
 *
 * @var plt::height
 * The height of the plot in user-defined units.
 *
 * @var plt::xlabel
 * A string representing the label for the x-axis. Maximum length: 100 characters.
 *
 * @var plt::ylabel
 * A string representing the label for the y-axis. Maximum length: 100 characters.
 *
 * @var plt::legend_position
 * A string representing the position of the legend (e.g., "top-right"). Maximum length: 20 characters.
 *
 * @var plt::data
 * A pointer to a plot_data structure containing the data to be plotted.
 */
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

/**
 * @brief Creates and initializes a new plt structure for plotting.
 *
 * This function allocates memory for a new plt structure and initializes its
 * fields with default values. The filename is set to the provided input, and
 * other fields are initialized to default values such as empty strings or zero.
 *
 * @param filename A string representing the name of the file associated with the plot.
 *                  This value is copied into the plt structure.
 *
 * @return A pointer to the newly created plt structure. The caller is responsible
 *         for freeing the allocated memory using `free()` when it is no longer needed.
 *
 * @note The plt structure contains the following fields:
 *       - `filename`: The name of the file associated with the plot.
 *       - `type`: The type of the plot (default is "standard").
 *       - `xlabel`: The label for the x-axis (default is an empty string).
 *       - `ylabel`: The label for the y-axis (default is an empty string).
 *       - `legend_position`: The position of the legend (default is an empty string).
 *       - `xmin`, `xmax`: The minimum and maximum values for the x-axis (default is 0.0).
 *       - `ymin`, `ymax`: The minimum and maximum values for the y-axis (default is 0.0).
 *       - `data`: A pointer to the data associated with the plot (default is NULL).
 */
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

/**
 * @brief Sets the type of axes for the given plot figure.
 *
 * This function assigns a specified type to the `type` field of the given
 * `plt` structure, which represents a plot figure.
 *
 * @param figure A pointer to the `plt` structure representing the plot figure.
 * @param type A string specifying the type of axes to set for the plot figure.
 *
 * @note Ensure that the `type` string is properly null-terminated and that
 *       the `figure->type` field has enough allocated memory to store the
 *       provided string.
 */
void plt_axes_type(plt *figure, char *type)
{
    strcpy(figure->type, type);
}

/**
 * @brief Sets the x-axis limits for a given plot.
 *
 * This function updates the minimum and maximum x-axis values for the specified plot.
 *
 * @param figure A pointer to the plt structure representing the plot.
 * @param xmin The minimum value for the x-axis.
 * @param xmax The maximum value for the x-axis.
 */
void plt_xlim(plt *figure, double xmin, double xmax)
{
    figure->xmin = xmin;
    figure->xmax = xmax;
}

/**
 * @brief Sets the y-axis limits for a given plot.
 *
 * This function updates the minimum and maximum y-axis values for the specified plot.
 *
 * @param figure A pointer to the plt structure representing the plot.
 * @param ymin The minimum value for the y-axis.
 * @param ymax The maximum value for the y-axis.
 */
void plt_ylim(plt *figure, double ymin, double ymax)
{
    figure->ymin = ymin;
    figure->ymax = ymax;
}

/**
 * @brief Sets the dimensions of a plot figure.
 *
 * This function updates the width and height of the given plot figure.
 *
 * @param figure A pointer to the plt structure representing the plot figure.
 * @param width The desired width of the plot figure.
 * @param height The desired height of the plot figure.
 */
void plt_dims(plt *figure, double width, double height)
{
    figure->width = width;
    figure->height = height;
}

/**
 * @brief Enables the grid for the given plot figure.
 *
 * This function sets the grid property of the provided `plt` figure
 * to enabled (1), allowing the grid to be displayed on the plot.
 *
 * @param figure A pointer to the `plt` structure representing the plot figure.
 */
void plt_grid(plt *figure)
{
    figure->grid = 1;
}

/**
 * @brief Sets the x-axis label for the given plot figure.
 *
 * @param figure A pointer to the plt structure representing the plot figure.
 * @param xlabel A string containing the label for the x-axis.
 *
 * @note The xlabel string is copied into the figure's xlabel field. Ensure
 *       that the figure's xlabel field has sufficient space to store the string.
 *       This function does not perform bounds checking.
 */
void plt_xlabel(plt *figure, char *xlabel)
{
    strcpy(figure->xlabel, xlabel);
}

/**
 * @brief Sets the label for the y-axis of a plot.
 *
 * This function assigns a specified label to the y-axis of the given plot figure.
 *
 * @param figure A pointer to the `plt` structure representing the plot.
 * @param ylabel A string containing the label for the y-axis.
 */
void plt_ylabel(plt *figure, char *ylabel)
{
    strcpy(figure->ylabel, ylabel);
}

/**
 * @brief Sets the position of the legend in the given plot figure.
 *
 * @param figure A pointer to the plt structure representing the plot figure.
 * @param position A string specifying the desired position of the legend.
 *                 This string is copied into the legend_position field of the plt structure.
 *
 * @note Ensure that the `position` string is null-terminated and does not exceed
 *       the allocated size of the `legend_position` field in the `plt` structure
 *       to avoid buffer overflows.
 */
void plt_legend_pos(plt *figure, char *position)
{
    strcpy(figure->legend_position, position);
}

/**
 * @brief Plots a dataset on the given figure.
 *
 * This function adds a plot to the specified figure using the provided x and y
 * data arrays. The plot can be customized with a color and an optional legend entry.
 *
 * @param figure       Pointer to the plt structure representing the figure to plot on.
 * @param x            Pointer to an array of x-axis data points.
 * @param y            Pointer to an array of y-axis data points.
 * @param data_len     The number of data points in the x and y arrays.
 * @param color        String specifying the color of the plot (e.g., "red", "blue").
 * @param legend_entry String specifying the legend entry for the plot. Can be NULL if no legend is needed.
 */
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

/**
 * @brief Plots a stem plot on the given figure.
 *
 * This function creates a stem plot on the specified figure using the provided
 * x and y data arrays. A stem plot is a discrete plot where vertical lines are
 * drawn from a baseline to the data points, with markers at the data points.
 *
 * @param figure       Pointer to the plt structure representing the figure
 *                     where the stem plot will be drawn.
 * @param x            Pointer to an array of x-coordinates for the data points.
 * @param y            Pointer to an array of y-coordinates for the data points.
 * @param data_len     The number of data points in the x and y arrays.
 * @param color        String specifying the color of the stem plot (e.g., "red",
 *                     "blue", etc.).
 * @param legend_entry String specifying the legend entry for the stem plot.
 *                     Pass NULL if no legend entry is required.
 */
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

/**
 * @brief Adds a plot to the specified file using the provided plot data.
 *
 * This function writes plot data to the given file pointer, allowing the
 * user to append or create plots in the specified file.
 *
 * @param fp A pointer to the file where the plot data will be written.
 *           The file must be opened in a mode that allows writing.
 * @param data_in A pointer to a `plot_data` structure containing the
 *                data to be added to the plot.
 */
void add_plot(FILE *fp, plot_data *data_in)
{

    fprintf(fp, "\\addplot [\n");

    if (strcmp(data_in->color, "") != 0)
    {
        fprintf(fp, "color=%s,\n", data_in->color);
    }

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

/**
 * @brief Adds a stem plot to the specified file.
 *
 * This function writes a stem plot representation of the provided data
 * to the given file pointer. The stem plot is typically used to visualize
 * discrete data points.
 *
 * @param fp A pointer to the file where the stem plot will be written.
 *           The file must be opened in write mode before calling this function.
 * @param data_in A pointer to the plot_data structure containing the data
 *                to be plotted. The structure should be properly initialized
 *                before passing it to this function.
 */
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
/**
 * @brief Writes plot data to a file.
 *
 * This function takes a file pointer and a pointer to plot data, and writes
 * the data to the specified file.
 *
 * @param fp A pointer to the file where the data will be written. The file
 *           must be opened in a mode that allows writing.
 * @param data_in A pointer to the plot_data structure containing the data
 *                to be written to the file.
 */

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

/**
 * @brief Cleans up and deallocates resources associated with a given plot figure.
 *
 * This function is responsible for releasing any memory or resources
 * allocated for the specified plot figure. It ensures that the resources
 * are properly freed to prevent memory leaks.
 *
 * @param figure A pointer to the plt structure representing the plot figure
 *               to be cleaned up. This must be a valid pointer.
 */
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

/**
 * @brief Saves the given plot figure to a file.
 *
 * This function takes a pointer to a `plt` structure representing a plot
 * figure and saves it to a file. The specific file format and location
 * depend on the implementation details of the function.
 *
 * @param figure A pointer to the `plt` structure representing the plot figure to be saved.
 */
void plt_save_fig(plt *figure)
{

    FILE *fp;
    char filename_no_ext[120];
    char filename_tex[120];

    // Check if the filename ends with .eps or .pdf
    char *ext = strrchr(figure->filename, '.');
    if (ext != NULL)
    {
        // Remove the extension from the filename
        strncpy(filename_no_ext, figure->filename, ext - figure->filename);
        filename_no_ext[ext - figure->filename] = '\0';
    }
    else
    {
        // No extension found, use the full filename
        strcpy(filename_no_ext, figure->filename);
    }

    if (ext != NULL && (strcmp(ext, ".eps") == 0 || strcmp(ext, ".pdf") == 0))
    {
        // Add LaTeX preamble for standalone compilation
        snprintf(filename_tex, sizeof(filename_tex), "%s.tex", filename_no_ext);

        fp = fopen(filename_tex, "w");
        if (fp == NULL)
        {
            fprintf(stderr, "Error: Could not open file %s for writing.\n", filename_tex);
            exit(1);
        }

        fprintf(fp, "\\documentclass{standalone}\n");
        fprintf(fp, "\\usepackage{filecontents,pgfplots,tikz}\n");
        fprintf(fp, "\\pgfplotsset{compat=1.18}\n");
        fprintf(fp, "\\begin{document}\n");
    }
    else
    {
        // If not .eps or .pdf, just create the TikZ file
        fp = fopen(figure->filename, "w");
    }

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

    // Check if the filename ends with .eps or .pdf.  Finish writing to file.
    if (ext != NULL && (strcmp(ext, ".eps") == 0 || strcmp(ext, ".pdf") == 0))
    {
        fprintf(fp, "\\end{document}\n");

        // Free memory
        clean_up(figure);

        // Close the file
        fclose(fp);

        // Compile the LaTeX file to generate the output
        char command[120];

        snprintf(command, sizeof(command), "pdflatex %s\n", filename_tex);

        int result = system(command);
        if (result != 0)
        {
            fprintf(stderr, "Error: Failed to compile LaTeX file.\n");
            exit(1);
        }

        // Remove the temporary .tex, .aux, and .log files
        snprintf(command, sizeof(command), "rm %s %s.aux %s.log\n", filename_tex, filename_no_ext, filename_no_ext);

        printf("%s", command);

        result = system(command);
        if (result != 0)
        {
            fprintf(stderr, "Error: Failed to remove temporary .tex file.\n");
            exit(1);
        }
    }
    else
    {
        // Free memory
        clean_up(figure);

        // Close the file
        fclose(fp);
    }
}

#endif /* CPLOTGIBB_H */