#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char** row;
    size_t* len;
    size_t h, cap;
} Grid;

static void fail(const char* m) {
    fprintf(stderr, "%s\n", m);
    exit(1);
}

static char* read_line(FILE* f) {
    size_t cap = 128, n = 0;
    char* buf = (char*)malloc(cap);
    int ch, seen = 0;

    if (!buf) return NULL;

    while ((ch = fgetc(f)) != EOF) {
        seen = 1;
        if (ch == '\r') continue;
        if (ch == '\n') break;
        if (n + 1 >= cap) {
            cap *= 2;
            char* p = (char*)realloc(buf, cap);
            if (!p) { free(buf); return NULL; }
            buf = p;
        }
        buf[n++] = (char)ch;
    }
    if (!seen && n == 0) { free(buf); return NULL; }
    buf[n] = '\0';
    return buf;
}

static void push_line(Grid* g, char* s) {
    if (g->h == g->cap) {
        size_t ncap = g->cap ? g->cap * 2 : 64;
        char** nrow = (char**)realloc(g->row, ncap * sizeof(*nrow));
        size_t* nlen = (size_t*)realloc(g->len, ncap * sizeof(*nlen));
        if (!nrow || !nlen) fail("oom");
        g->row = nrow;
        g->len = nlen;
        g->cap = ncap;
    }
    g->row[g->h] = s;
    g->len[g->h] = strlen(s);
    g->h++;
}

static int ok(const Grid* g, long r, long c) {
    if (r < 0 || (size_t)r >= g->h) return 0;
    if (c < 0 || (size_t)c >= g->len[r]) return 0;
    return 1;
}

/* Part 1: count "XMAS" in 8 directions */
static unsigned long long part1(const Grid* g) {
    static const int dir[8][2] = {
        {0,1},{0,-1},{1,0},{-1,0},{1,1},{1,-1},{-1,1},{-1,-1}
    };
    const char* w = "XMAS";
    unsigned long long cnt = 0;

    for (long r = 0; r < (long)g->h; r++) {
        for (long c = 0; c < (long)g->len[r]; c++) {
            if (g->row[r][c] != 'X') continue;
            for (int d = 0; d < 8; d++) {
                long rr = r, cc = c;
                int k;
                for (k = 0; k < 4; k++) {
                    if (!ok(g, rr, cc) || g->row[rr][cc] != w[k]) break;
                    rr += dir[d][0];
                    cc += dir[d][1];
                }
                if (k == 4) cnt++;
            }
        }
    }
    return cnt;
}

/* Part 2: "X-MAS" — two MAS diagonals crossing at 'A' */
static unsigned long long part2(const Grid* g) {
    unsigned long long cnt = 0;

    for (long r = 0; r < (long)g->h; r++) {
        for (long c = 0; c < (long)g->len[r]; c++) {
            if (g->row[r][c] != 'A') continue;

            long r0 = r - 1, r1 = r + 1, c0 = c - 1, c1 = c + 1;
            if (!ok(g, r0, c0) || !ok(g, r0, c1) || !ok(g, r1, c0) || !ok(g, r1, c1))
                continue;

            char tl = g->row[r0][c0], tr = g->row[r0][c1];
            char bl = g->row[r1][c0], br = g->row[r1][c1];

            int d1 = (tl == 'M' && br == 'S') || (tl == 'S' && br == 'M');
            int d2 = (tr == 'M' && bl == 'S') || (tr == 'S' && bl == 'M');

            if (d1 && d2) cnt++;
        }
    }
    return cnt;
}

int main(void) {
    Grid g = { 0 };
    for (;;) {
        char* s = read_line(stdin);
        if (!s) break;
        push_line(&g, s);
    }

    if (g.h == 0) {
        puts("Part 1: 0");
        puts("Part 2: 0");
    }
    else {
        printf("Part 1: %llu\n", part1(&g));
        printf("Part 2: %llu\n", part2(&g));
    }

    for (size_t i = 0; i < g.h; i++) free(g.row[i]);
    free(g.row);
    free(g.len);
    return 0;
}
