#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static int Ldirs[8][2] = {
        {-1,-1},{-1,0},{-1,1},
        { 0,-1},        { 0,1},
        { 1,-1},{ 1,0},{ 1,1}
};

bool match_from(char** m, int rows, int cols, int sr, int sc, const char* w, int dr, int dc)
{
    for (int k = 0; w[k]; k++) {
        int r = sr + k * dr, c = sc + k * dc;
        if (r < 0 || r >= rows || c < 0 || c >= cols) return false;
        if (m[r][c] != w[k]) return false;
    }
    return true;
}

int count_matches(char** m, int rows, int cols, const char* w) {
    int cnt = 0;
    for (int i = 0;i < rows;i++) {
        for (int j = 0;j < cols;j++) {
            for (int d = 0; d < 8; d++) {
                if (match_from(m, rows, cols, i, j, w, Ldirs[d][0], Ldirs[d][1]))
                    cnt++;
            }
        }
    }
    return cnt;
}

int count_xmas(char** m, int rows, int cols) {
    int cnt = 0;
    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            if (m[i][j] != 'A') continue;

            char nw = m[i - 1][j - 1];
            char ne = m[i - 1][j + 1];
            char sw = m[i + 1][j - 1];
            char se = m[i + 1][j + 1];

            bool diag1 = ((nw == 'M' && se == 'S') || (nw == 'S' && se == 'M'));
            bool diag2 = ((ne == 'M' && sw == 'S') || (ne == 'S' && sw == 'M'));

            if (diag1 && diag2) cnt++;
        }
    }
    return cnt;
}

int main(void)
{
	FILE* fp;
	int counter = 0, rows = 0, cols = 0, i = 0, j = 0, hits = 0;
	char FILENAME[64] = "input.txt", line[BUFSIZ];

    if ((fp = fopen(FILENAME, "r")) == NULL) {
        printf("The file \"%s\" cannot be opened\n", FILENAME);
        return 1;
    }

    // First pass: count rows and columns (count tokens on first line)
    while (fgets(line, sizeof line, fp)) {
        if (*line == '\n' || *line == '\r') continue; // skip blank lines
        rows++;
        if (cols == 0) {
            // count tokens (chars) in first non-empty line
            char* p = line;
            char tmp;
            while (sscanf(p, " %c", &tmp) == 1) {
                cols++;
                // advance p past this token
                while (*p && *p == ' ') p++;
                if (*p) p++; // move past the char we just read
                while (*p == ' ' || *p == '\t') p++;
            }
        }
    }

    rewind(fp);

    // Allocate matrix of chars
    char** matrix = malloc(rows * sizeof *matrix);
    if (!matrix) { fclose(fp); return 1; }
    for (i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof *matrix[i]);
        if (!matrix[i]) return 1;
    }

    // Read chars (space/newline separated)
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (fscanf(fp, " %c", &matrix[i][j]) != 1) {
                fprintf(stderr, "File format error at row %d, col %d\n", i, j);
                return 1;
            }
        }
    }

    fclose(fp);

    // Print
    printf("Matrix (%d x %d):\n", rows, cols);
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%c ", matrix[i][j]); 
        }
        printf("\n");
    }

    hits = count_matches(matrix, rows, cols, "XMAS");
    printf("hits: %d\n", hits);
    int xmases = count_xmas(matrix, rows, cols);
    printf("X-MAS count: %d\n", xmases);

    for (i = 0; i < rows; i++) free(matrix[i]);
    free(matrix);
    return 0;
}