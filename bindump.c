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

int
main(int argc, char **argv)
{
    FILE *file;
    int c;

    if ((file = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Unable to open file!\n");

        return 1;
    }

    while ((c = fgetc(file)) != EOF) {
        int i;

        // For each byte in a file, print the byte as a binary string
        // (a bunch of '0' and '1' characters
        for (i = 7; i>= 0; i--) {
            printf(((c & (2 ^i)) != 0) ? "0" : "1");
        }
    }

    return 0;
}
