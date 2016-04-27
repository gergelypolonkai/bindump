/*
 * This file was originally written by Gergely Polonkai, and is
 * licensed inder the DBAD:
 *
 * Do whatever you like with the original work, just don't be a dick.
 *
 * Being a dick includes - but is not limited to - the following
 * instances:

 *   1a. Outright copyright infringement - Don't just copy this and
 *       change the name.
 *   1b. Selling the unmodified original with no work done
 *       what-so-ever, that's REALLY being a dick.
 *   1c. Modifying the original work to contain hidden harmful
 *       content. That would make you a PROPER dick.
 *
 * If you become rich through modifications, related works/services,
 * or supporting the original work, share the love. Only a dick would
 * make loads off this work and not buy the original work's creator(s)
 * a pint.
 *
 * Code is provided with no warranty. Using somebody else's code and
 * bitching when it goes wrong makes you a DONKEY dick. Fix the
 * problem yourself. A non-dick would submit the fix back.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>

int
main(int argc, char **argv)
{
    FILE *binfile;
    struct stat ret;
    int i;
    signed int found_width = -1;
    unsigned int less_height = -1;
    int found_height;
    void *filemap;
    off_t a;
    char *xpm_line;

    static int valid_widths[] = {320, 640, 800, 1024, 1280, 1600};

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <binfile>\n", argv[0]);
        fprintf(stderr,
                "\tWhere <binfile> is a binary representation of a file,\n"
                "created with bindump\n");

        return 1;
    }

    if ((binfile = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Unable to open file '%s'\n", argv[1]);

        return 1;
    }

    if (stat(argv[1], &ret) != 0) {
        fprintf(stderr, "Unable to stat file '%s'\n", argv[1]);

        return 1;
    }

    for (i = 0; i < sizeof(valid_widths) / sizeof(int); i++) {
        int height;
        int best_height;
        int diff;

        best_height = (int)ceil(valid_widths[i] * 0.75);
        height = (int)ceil(ret.st_size / valid_widths[i]);
        diff = abs(best_height - height);

        if (diff < less_height) {
            less_height = diff;
            found_width = i;
        }
    }

    if (found_width == -1) {
        fprintf(stderr,
                "Couldn't find the best size for the image\n"
                "(VERY strange... should never happen)\n");

        return 1;
    }

    found_height = (int)ceil((float)ret.st_size
                   / (float)valid_widths[found_width]);

    filemap = mmap(NULL,
                   ret.st_size,
                   PROT_READ,
                   MAP_PRIVATE,
                   fileno(binfile),
                   0);

    if (filemap == NULL) {
        fprintf(stderr, "Unable to mmap file '%s'\n", argv[1]);
        return 1;
    }

    fprintf(stdout, "/* XPM */\n");
    fprintf(stdout, "static char * data[] = {\n");
    fprintf(stdout,
            "\"%d %d 3 1\",\n",
            valid_widths[found_width],
            found_height);
    fprintf(stdout, "\"0\tc #000000\",\n");
    fprintf(stdout, "\"1\tc #ffffff\",\n");
    fprintf(stdout, "\"2\tc #ff00ff\"");

    xpm_line = malloc(valid_widths[found_width] + 1);

    for (a = 0; a < ret.st_size; a++) {
        if (a % valid_widths[found_width] == 0) {
            if (a != 0) {
                fprintf(stdout, ",\n");
                fprintf(stdout, "\"%s\"", xpm_line);
            }

            memset(xpm_line, 0, valid_widths[found_width] + 1);
        }

        xpm_line[a % valid_widths[found_width]] = ((char *)filemap)[a];
    }

    if (*xpm_line != 0) {
        for (i = strlen(xpm_line); i < valid_widths[found_width]; i++) {
            xpm_line[i] = '2';
        }

        fprintf(stdout, ",\n\"%s\"};\n", xpm_line);
    }

    munmap(filemap, ret.st_size);

    return 0;
}
