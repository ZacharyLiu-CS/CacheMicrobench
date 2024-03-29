#pragma once
enum {
    NUM_SOCKET = 2,
    NUM_PHYSICAL_CPU_PER_SOCKET = 20,
    SMT_LEVEL = 2,
};

const unsigned int OS_CPU_ID[NUM_SOCKET][NUM_PHYSICAL_CPU_PER_SOCKET][SMT_LEVEL] = {
    { /* socket id: 0 */
        { /* physical cpu id: 0 */
          0, 40,     },
        { /* physical cpu id: 1 */
          4, 44,     },
        { /* physical cpu id: 2 */
          8, 48,     },
        { /* physical cpu id: 3 */
          6, 46,     },
        { /* physical cpu id: 4 */
          2, 42,     },
        { /* physical cpu id: 8 */
          12, 52,     },
        { /* physical cpu id: 9 */
          16, 56,     },
        { /* physical cpu id: 10 */
          18, 58,     },
        { /* physical cpu id: 11 */
          14, 54,     },
        { /* physical cpu id: 12 */
          10, 50,     },
        { /* physical cpu id: 16 */
          20, 60,     },
        { /* physical cpu id: 17 */
          24, 64,     },
        { /* physical cpu id: 18 */
          28, 68,     },
        { /* physical cpu id: 19 */
          26, 66,     },
        { /* physical cpu id: 20 */
          22, 62,     },
        { /* physical cpu id: 24 */
          32, 72,     },
        { /* physical cpu id: 25 */
          36, 76,     },
        { /* physical cpu id: 26 */
          38, 78,     },
        { /* physical cpu id: 27 */
          34, 74,     },
        { /* physical cpu id: 28 */
          30, 70,     },
    },
    { /* socket id: 1 */
        { /* physical cpu id: 0 */
          1, 41,     },
        { /* physical cpu id: 1 */
          5, 45,     },
        { /* physical cpu id: 2 */
          9, 49,     },
        { /* physical cpu id: 3 */
          7, 47,     },
        { /* physical cpu id: 4 */
          3, 43,     },
        { /* physical cpu id: 8 */
          13, 53,     },
        { /* physical cpu id: 9 */
          17, 57,     },
        { /* physical cpu id: 10 */
          19, 59,     },
        { /* physical cpu id: 11 */
          15, 55,     },
        { /* physical cpu id: 12 */
          11, 51,     },
        { /* physical cpu id: 16 */
          21, 61,     },
        { /* physical cpu id: 17 */
          25, 65,     },
        { /* physical cpu id: 18 */
          29, 69,     },
        { /* physical cpu id: 19 */
          27, 67,     },
        { /* physical cpu id: 20 */
          23, 63,     },
        { /* physical cpu id: 24 */
          33, 73,     },
        { /* physical cpu id: 25 */
          37, 77,     },
        { /* physical cpu id: 26 */
          39, 79,     },
        { /* physical cpu id: 27 */
          35, 75,     },
        { /* physical cpu id: 28 */
          31, 71,     },
    },
};
