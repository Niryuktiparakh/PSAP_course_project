#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tinyexpr.h"

// Function to decode URL-encoded input (e.g., %2B becomes '+')
void url_decode(char *dst, const char *src)
{
    char a, b;
    while (*src)
    {
        if ((*src == '%') &&
            ((a = src[1]) && (b = src[2])) &&
            (isxdigit(a) && isxdigit(b)))
        {
            if (a >= 'a')
                a -= 'a' - 'A';
            if (a >= 'A')
                a -= ('A' - 10);
            else
                a -= '0';
            if (b >= 'a')
                b -= 'a' - 'A';
            if (b >= 'A')
                b -= ('A' - 10);
            else
                b -= '0';
            *dst++ = 16 * a + b;
            src += 3;
        }
        else if (*src == '+')
        {
            *dst++ = ' ';
            src++;
        }
        else
        {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}

int main()
{
    // Output content type for CGI
    printf("Content-type: text/plain\n\n");

    // Get query string from browser
    char *query = getenv("QUERY_STRING");
    if (!query)
    {
        fprintf(stderr, "No query string.\n");
        return 1;
    }

    // Extract and decode the equation
    char equation_raw[256], equation[256];
    char *eq_start = strstr(query, "expr=");
    if (!eq_start)
    {
        fprintf(stderr, "No expr parameter.\n");
        return 1;
    }

    eq_start += 5; // skip "expr="
    strncpy(equation_raw, eq_start, sizeof(equation_raw) - 1);
    equation_raw[sizeof(equation_raw) - 1] = '\0';
    url_decode(equation, equation_raw);

    // Prepare variables for evaluation
    double x;
    te_variable vars[] = {{"x", &x}};
    int err;

    te_expr *expr = te_compile(equation, vars, 1, &err);
    if (!expr)
    {
        printf("Parse error at %d\n", err);
        return 1;
    }

    for (x = -10; x <= 10; x += 0.5)
    {
        double y = te_eval(expr);
        printf("%f %f\n", x, y);
    }

    te_free(expr);

    return 0;
}
