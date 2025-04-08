# cplotgibb

C to TikZ plotting utility.

## Overview

`cplotgibb` is a lightweight C library for generating TikZ plots from C arrays. It supports line plots, stem plots, and various plot customizations. The output is a TikZ/PGFPlots figure that can be included in LaTeX documents.

## Features

- Line plots and stem plots
- Customizable axes, labels, grid, and dimensions
- Easy-to-use API for creating and saving plots

## Installation

To install run 
```bash
make install
```
which will copy the `cplotgibb.h` file to `/usr/local/include` file.  You can override but predefining `INCLUDEDIR` or `PREFIX`.

To remove the header file, run
```bash
make uninstall
```

## Example Usage

Below is an example of how to use `cplotgibb` to create a plot with multiple datasets.

### Code Example

```c
...
#include "cplotgibb.h"

...

int main()
{
    ...

    plt *fig1 = plt_figure("out.pdf");

    plt_plot(fig1, x, y1, 1000, "teal", "$y_1(t)$");
    plt_plot(fig1, x, y2, 1000, "orange", "$y_2(t)$");
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
```

### Output

The above code generates a TikZ/PGFPlots figure saved as `out.pdf`. The figure includes:

- Two line plots: `$y_1(t)$` and `$y_2(t)$`
- A stem plot: `$y_d[n]$`
- Centered axes, grid lines, and labeled axes

### How to Compile and Run

1. Include `cplotgibb.h` in your project.
2. Compile your program with a C compiler:
   ```bash
   gcc -o plotter plotter.c -lm
   ```
3. Run the program:
   ```bash
   ./plotter
   ```
4. The output file (e.g., `out.pdf`) will be generated in the current directory.

## Notes

- Ensure that `pdflatex` is installed if you want to generate `.pdf` or `.eps` files.
- If you would rather to get the raw `.tex` code, save the file as `[filename].tex` when calling the `plt_figure()` function.
- The library is designed to work seamlessly with LaTeX documents.

For more details, refer to the function documentation in `cplotgibb.h`.


