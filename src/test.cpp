#include <stdio.h>
#include "core_math.h"

int main(int argc, char **argv)
{
    {
        printf("mat3_identity\n");
        auto m = math::mat3_identity();
        math::print(m);
        printf("\n");
    }
    {
        printf("mat4_identity\n");
        auto m = math::mat4_identity();
        math::print(m);
        printf("\n");
    }
    {
        printf("mat4_row\n");
        auto m = math::mat4{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        printf("row 0\n");
        math::print(math::row_vector(m, 0));

        printf("row 1\n");
        math::print(math::row_vector(m, 1));

        printf("row 2\n");
        math::print(math::row_vector(m, 2));

        printf("row 3\n");
        math::print(math::row_vector(m, 3));
        printf("\n");
    }

    {
        printf("mat4_col\n");
        auto m = math::mat4{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        printf("col 0\n");
        math::print(math::column_vector(m, 0));

        printf("col 1\n");
        math::print(math::column_vector(m, 1));

        printf("col 2\n");
        math::print(math::column_vector(m, 2));

        printf("col 3\n");
        math::print(math::column_vector(m, 3));
        printf("\n");
    }

    {
        printf("mat4_mul\n");
        auto m = math::mat4_identity();
        auto t = math::mat4_identity();
        t[11] = -10.0f;

        auto transformed = math::mul(m, t);
        printf("transformed: m x t\n");
        math::print(transformed);
    }

    printf("Press enter to continue...\n");
    getchar();
}