/*_____ I N C L U D E S ____________________________________________________*/
#include "retarget.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/

/*_____ D E F I N I T I O N S ______________________________________________*/

/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

//retarget printf
int putchar(int c)
{
    #if 1
    // if (c == '\r') 
    // {        
    //     SendChar('\n');
    // }
    SendChar((char)c);
    return (char)c;
    #else
    DISABLE_UART_INT_STATUS_FLAG = 1U;    /* disable INTST0 interrupt */
    UART_XFER = (unsigned char)c;
    while(CLEAR_UART_INT_STATUS_FLAG == 0)
    {

    }
    CLEAR_UART_INT_STATUS_FLAG = 0U;    /* clear INTST0 interrupt flag */
    return c;
    #endif
}

#if 1
#include <stdarg.h>
// #include <stdint.h>
#include <stdbool.h>
// #include <stdio.h>   // for putchar()

// Flags for formatting
#define FLAG_LEFT   (1 << 0)  // '-'
#define FLAG_PLUS   (1 << 1)  // '+'
#define FLAG_SPACE  (1 << 2)  // ' '
#define FLAG_ZERO   (1 << 3)  // '0'
#define FLAG_HASH   (1 << 4)  // '#'

// Forward declarations of helper functions
static void print_string(const char *s, int width, int precision, int flags);
static void print_char(char c, int width, int flags);
static void print_integer(long long value, int base, int width, int precision, int flags, bool uppercase);
static void print_unsigned(unsigned long long value, int base, int width, int precision, int flags, bool uppercase);
static void print_pointer(void *ptr, int width, int precision, int flags);
static void print_float(double value, int width, int precision, int flags, char spec);

typedef unsigned long uintptr_t;

// The main super_printf function
int tiny_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
	int printed = 0;
    char ch;
    while ((ch = *fmt++) != '\0') {
        if (ch != '%') {
            // Literal character
            putchar(ch);
        } else {
            // Parse flags
            int flags = 0;
            bool parsing = true;
            while (parsing) {
                switch (*fmt) {
                    case '-': flags |= FLAG_LEFT; fmt++; break;
                    case '+': flags |= FLAG_PLUS; fmt++; break;
                    case ' ': flags |= FLAG_SPACE; fmt++; break;
                    case '0': flags |= FLAG_ZERO; fmt++; break;
                    case '#': flags |= FLAG_HASH; fmt++; break;
                    default: parsing = false; break;
                }
            }
            // Parse width (number)
            int width = 0;
            while (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt - '0');
                fmt++;
            }
            // Parse precision
            int precision = -1;
            if (*fmt == '.') {
                fmt++;
                precision = 0;
                while (*fmt >= '0' && *fmt <= '9') {
                    precision = precision * 10 + (*fmt - '0');
                    fmt++;
                }
            }
            // Parse length modifier (l or ll)
            int length = 0; // 0=none, 1=l, 2=ll
            if (*fmt == 'l') {
                fmt++;
                if (*fmt == 'l') { length = 2; fmt++; }
                else length = 1;
            }
            // Specifier
            char spec = *fmt++;
            switch (spec) {
                case '%':
                    putchar('%');
                    break;
                case 'c': {
                    // Character
                    char c = (char)va_arg(args, int);
                    print_char(c, width, flags);
                    break;
                }
                case 's': {
                    // String
                    char *s = va_arg(args, char*);
                    print_string(s, width, precision, flags);
                    break;
                }
                case 'd':
                case 'i': {
                    // Signed integer
                    long long val;
                    if (length == 2) val = va_arg(args, long long);
                    else if (length == 1) val = va_arg(args, long);
                    else val = va_arg(args, int);
                    print_integer(val, 10, width, precision, flags, false);
                    break;
                }
                case 'u': {
                    // Unsigned integer
                    unsigned long long val;
                    if (length == 2) val = va_arg(args, unsigned long long);
                    else if (length == 1) val = va_arg(args, unsigned long);
                    else val = va_arg(args, unsigned int);
                    print_unsigned(val, 10, width, precision, flags, false);
                    break;
                }
                case 'x': {
                    // Hex lowercase
                    unsigned long long val;
                    if (length == 2) val = va_arg(args, unsigned long long);
                    else if (length == 1) val = va_arg(args, unsigned long);
                    else val = va_arg(args, unsigned int);
                    print_unsigned(val, 16, width, precision, flags, false);
                    break;
                }
                case 'X': {
                    // Hex uppercase
                    unsigned long long val;
                    if (length == 2) val = va_arg(args, unsigned long long);
                    else if (length == 1) val = va_arg(args, unsigned long);
                    else val = va_arg(args, unsigned int);
                    print_unsigned(val, 16, width, precision, flags, true);
                    break;
                }
                case 'o': {
                    // Octal
                    unsigned long long val;
                    if (length == 2) val = va_arg(args, unsigned long long);
                    else if (length == 1) val = va_arg(args, unsigned long);
                    else val = va_arg(args, unsigned int);
                    print_unsigned(val, 8, width, precision, flags, false);
                    break;
                }
                case 'p': {
                    // Pointer (print as 0xHEX)
                    void *ptr = va_arg(args, void*);
                    print_pointer(ptr, width, precision, flags);
                    break;
                }
                case 'f':
                case 'F': {
                    // Floating point fixed
                    double val = va_arg(args, double);
                    if (precision < 0) precision = 6;
                    print_float(val, width, precision, flags, spec);
                    break;
                }
                case 'e':
                case 'E': {
                    // Floating point scientific
                    double val = va_arg(args, double);
                    if (precision < 0) precision = 6;
                    print_float(val, width, precision, flags, spec);
                    break;
                }
                default:
                    // Unsupported specifier - print literally
                    putchar('%');
                    putchar(spec);
                    break;
            }
        }
    }
    va_end(args);

	return printed;   	
}

// Print a single char with optional padding
static void print_char(char c, int width, int flags) {
    int pad = (width > 1) ? width - 1 : 0;
    if (!(flags & FLAG_LEFT)) {
        for (int i = 0; i < pad; i++) putchar(' ');
    }
    putchar(c);
    if (flags & FLAG_LEFT) {
        for (int i = 0; i < pad; i++) putchar(' ');
    }
}

// Print a string with width and precision
static void print_string(const char *s, int width, int precision, int flags) {
    if (!s) s = "(null)";
    // Compute string length up to precision
    int len = 0;
    while (s[len] && (precision < 0 || len < precision)) len++;
    int pad = (width > len) ? width - len : 0;
    if (!(flags & FLAG_LEFT)) {
        for (int i = 0; i < pad; i++) putchar(' ');
    }
    for (int i = 0; i < len; i++) {
        putchar(s[i]);
    }
    if (flags & FLAG_LEFT) {
        for (int i = 0; i < pad; i++) putchar(' ');
    }
}

// Print a signed integer (possibly 64-bit) in given base
static void print_integer(long long value, int base, int width, int precision, int flags, bool uppercase) {
    bool negative = false;
    unsigned long long uval;
    if (value < 0) {
        negative = true;
        uval = (unsigned long long)(-value);
    } else {
        uval = (unsigned long long)value;
    }
    // Convert number to string (in reverse)
    char buf[32];
    int len = 0;
    if (uval == 0) {
        buf[len++] = '0';
    } else {
        while (uval > 0) {
            int digit = uval % base;
            if (digit < 10) buf[len++] = '0' + digit;
            else buf[len++] = (uppercase ? 'A' : 'a') + (digit - 10);
            uval /= base;
        }
    }
    // Apply precision (minimum digits)
    int num_len = len;
    if (precision > num_len) {
        for (int i = num_len; i < precision; i++) {
            buf[len++] = '0';
        }
    }
    // Determine if we need a sign or space
    int sign_char = 0;
    if (negative) sign_char = 1;
    else if (flags & FLAG_PLUS) sign_char = 1;
    else if (flags & FLAG_SPACE) sign_char = 1;
    int total_len = len + sign_char;
    int pad = (width > total_len) ? width - total_len : 0;
    // Print left spaces if needed
    if (!(flags & FLAG_LEFT) && !(flags & FLAG_ZERO)) {
        for (int i = 0; i < pad; i++) putchar(' ');
    }
    // Print sign or space
    if (negative) {
        putchar('-');
    } else if (flags & FLAG_PLUS) {
        putchar('+');
    } else if (flags & FLAG_SPACE) {
        putchar(' ');
    }
    // Print zero padding
    if (!(flags & FLAG_LEFT) && (flags & FLAG_ZERO)) {
        for (int i = 0; i < pad; i++) putchar('0');
    }
    // Print the digits in correct order
    for (int i = len - 1; i >= 0; i--) {
        putchar(buf[i]);
    }
    // Print right spaces if left-aligned
    if (flags & FLAG_LEFT) {
        for (int i = 0; i < pad; i++) putchar(' ');
    }
}

// Print an unsigned integer in given base (e.g. decimal, hex, octal)
static void print_unsigned(unsigned long long uval, int base, int width, int precision, int flags, bool uppercase) {
    char buf[32];
    int len = 0;
    if (uval == 0) {
        buf[len++] = '0';
    } else {
        while (uval > 0) {
            int digit = uval % base;
            if (digit < 10) buf[len++] = '0' + digit;
            else buf[len++] = (uppercase ? 'A' : 'a') + (digit - 10);
            uval /= base;
        }
    }
    int num_len = len;
    if (precision > num_len) {
        for (int i = num_len; i < precision; i++) {
            buf[len++] = '0';
        }
    }
    int total_len = len;
    int pad = (width > total_len) ? width - total_len : 0;
    if (!(flags & FLAG_LEFT) && !(flags & FLAG_ZERO)) {
        for (int i = 0; i < pad; i++) putchar(' ');
    }
    if (!(flags & FLAG_LEFT) && (flags & FLAG_ZERO)) {
        for (int i = 0; i < pad; i++) putchar('0');
    }
    for (int i = len - 1; i >= 0; i--) {
        putchar(buf[i]);
    }
    if (flags & FLAG_LEFT) {
        for (int i = 0; i < pad; i++) putchar(' ');
    }
}

// Print a pointer (treated as hex), with leading 0x
static void print_pointer(void *ptr, int width, int precision, int flags) {
    unsigned long long uval = (unsigned long long)(uintptr_t)ptr;
    char buf[32];
    int len = 0;
    if (uval == 0) {
        buf[len++] = '0';
    } else {
        while (uval > 0) {
            int digit = uval % 16;
            if (digit < 10) buf[len++] = '0' + digit;
            else buf[len++] = 'a' + (digit - 10);
            uval /= 16;
        }
    }
    int total_len = len + 2; // "0x" prefix
    int pad = (width > total_len) ? width - total_len : 0;
    if (!(flags & FLAG_LEFT)) {
        for (int i = 0; i < pad; i++) putchar(' ');
    }
    // "0x" prefix
    putchar('0');
    putchar('x');
    for (int i = len - 1; i >= 0; i--) {
        putchar(buf[i]);
    }
    if (flags & FLAG_LEFT) {
        for (int i = 0; i < pad; i++) putchar(' ');
    }
}

// Print a floating-point value (%f or %e/%E)
static void print_float(double value, int width, int precision, int flags, char spec) {
    // Handle sign
    bool negative = false;
    if (value < 0.0 || (value == 0.0 && 1.0/value < 0.0)) {
        negative = true;
        value = -value;
    }
    // Special case: value == 0.0
    if (value == 0.0) {
        // Build "0.000..." string manually
        char buf[64];
        int len = 0;
        buf[len++] = '0';
        if (precision > 0 || (flags & FLAG_HASH)) {
            buf[len++] = '.';
            for (int i = 0; i < precision; i++) {
                buf[len++] = '0';
            }
        }
        if (spec == 'e' || spec == 'E') {
            buf[len++] = spec;
            buf[len++] = '+';
            buf[len++] = '0';
            buf[len++] = '0';
        }
        int sign_char = negative ? 1 : ((flags & FLAG_PLUS) ? 1 : ((flags & FLAG_SPACE) ? 1 : 0));
        int total_len = len + sign_char;
        int pad = (width > total_len) ? width - total_len : 0;
        if (!(flags & FLAG_LEFT) && !(flags & FLAG_ZERO)) {
            for (int i = 0; i < pad; i++) putchar(' ');
        }
        if (negative) putchar('-');
        else if (flags & FLAG_PLUS) putchar('+');
        else if (flags & FLAG_SPACE) putchar(' ');
        if (!(flags & FLAG_LEFT) && (flags & FLAG_ZERO)) {
            for (int i = 0; i < pad; i++) putchar('0');
        }
        for (int i = 0; i < len; i++) {
            putchar(buf[i]);
        }
        if (flags & FLAG_LEFT) {
            for (int i = 0; i < pad; i++) putchar(' ');
        }
        return;
    }
    // Handle scientific exponent if needed
    int exp = 0;
    if (spec == 'e' || spec == 'E') {
        // Normalize to 1 <= value < 10
        while (value >= 10.0) {
            value /= 10.0;
            exp++;
        }
        while (value < 1.0) {
            value *= 10.0;
            exp--;
        }
    }
    // Apply rounding by adding half-unit at last place
    double rounding = 0.5;
    for (int i = 0; i < precision; i++) {
        rounding /= 10.0;
    }
    value += rounding;
    // After rounding, check if it rolled over (e.g. 9.99 -> 10.0)
    if ((spec == 'e' || spec == 'E') && value >= 10.0) {
        value /= 10.0;
        exp++;
    }
    // Split integer and fractional parts
    unsigned long long int_part = (unsigned long long)value;
    double frac_part = value - (double)int_part;
    // Convert integer part to string
    char buf[64];
    int len = 0;
    if (int_part == 0) {
        buf[len++] = '0';
    } else {
        char tmp[32];
        int tlen = 0;
        while (int_part > 0) {
            tmp[tlen++] = '0' + (int_part % 10);
            int_part /= 10;
        }
        for (int i = tlen - 1; i >= 0; i--) {
            buf[len++] = tmp[i];
        }
    }
    // Decimal point and fraction digits
    if (precision > 0 || (flags & FLAG_HASH)) {
        buf[len++] = '.';
        for (int i = 0; i < precision; i++) {
            frac_part *= 10.0;
            int digit = (int)frac_part;
            buf[len++] = '0' + digit;
            frac_part -= digit;
        }
    }
    // Append exponent if scientific
    if (spec == 'e' || spec == 'E') {
        buf[len++] = spec;
        // Exponent sign
        if (exp < 0) {
            buf[len++] = '-';
            exp = -exp;
        } else {
            buf[len++] = '+';
        }
        // Two-digit exponent (zero-padded)
        int exp1 = exp / 10;
        int exp0 = exp % 10;
        buf[len++] = '0' + exp1;
        buf[len++] = '0' + exp0;
    }
    // Now buf[0..len-1] has the number string
    int sign_char = negative ? 1 : ((flags & FLAG_PLUS) ? 1 : ((flags & FLAG_SPACE) ? 1 : 0));
    int total_len = len + sign_char;
    int pad = (width > total_len) ? width - total_len : 0;
    if (!(flags & FLAG_LEFT) && !(flags & FLAG_ZERO)) {
        for (int i = 0; i < pad; i++) putchar(' ');
    }
    if (negative) putchar('-');
    else if (flags & FLAG_PLUS) putchar('+');
    else if (flags & FLAG_SPACE) putchar(' ');
    if (!(flags & FLAG_LEFT) && (flags & FLAG_ZERO)) {
        for (int i = 0; i < pad; i++) putchar('0');
    }
    for (int i = 0; i < len; i++) {
        putchar(buf[i]);
    }
    if (flags & FLAG_LEFT) {
        for (int i = 0; i < pad; i++) putchar(' ');
    }
}


#else

// Print unsigned number with given base, width, padding, alignment
static void mini_print_unsigned(unsigned int num, unsigned int base, int width, int left_align, char fill, int uppercase)
{
    char buf[16];
    int i = 0;
    const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";

    // Convert number to string (reversed)
    do {
        buf[i++] = digits[num % base];
        num /= base;
    } while (num);

    int pad = width - i;
    if (pad < 0) pad = 0;

    if (!left_align) {
        while (pad--) putchar(fill);
    }

    while (i--) {
        putchar(buf[i]);
    }

    if (left_align) {
        while (pad--) putchar(' ');
    }
}

// Print signed number with width, padding, alignment
static void mini_print_signed(int num, int width, int left_align, char fill)
{
    if (num < 0) {
        putchar('-');
        num = -num;
        width--;
    }
    mini_print_unsigned((unsigned int)num, 10, width, left_align, fill, 0);
}

// Print float number (only 2 decimal places)
static void mini_print_float(double f, int width, int left_align, char fill)
{
    if (f < 0) {
        putchar('-');
        f = -f;
        width--;
    }

    int int_part = (int)f;
    int frac_part = (int)((f - int_part) * 100 + 0.5); // 2 decimal places

    // Adjust width (e.g., for ".xx")
    width -= 3;
    int tmp = int_part;
    while (tmp) {
        width--;
        tmp /= 10;
    }

    if (!left_align) {
        while (width-- > 0) putchar(fill);
    }

    // Print integer part
    mini_print_unsigned(int_part, 10, 0, 0, fill, 0);
    putchar('.');
    // Print fractional part
    if (frac_part < 10) {
        putchar('0'); // leading zero
    }
    mini_print_unsigned(frac_part, 10, 0, 0, fill, 0);

    if (left_align) {
        while (width-- > 0) putchar(' ');
    }
}

int tiny_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
	int printed = 0;

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;

            int left_align = 0;
            char fill = ' ';
            int width = 0;
            // int uppercase = 0;

            // Check for left align
            if (*fmt == '-') {
                left_align = 1;
                fmt++;
            }

            // Check for zero padding
            if (*fmt == '0') {
                fill = '0';
                fmt++;
            }

            // Parse width
            while (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt - '0');
                fmt++;
            }

            // Format specifiers
            switch (*fmt) {
                case 'd':
                    mini_print_signed(va_arg(args, int), width, left_align, fill);
                    break;
                case 'u':
                    mini_print_unsigned(va_arg(args, unsigned int), 10, width, left_align, fill, 0);
                    break;
                case 'x':
                    mini_print_unsigned(va_arg(args, unsigned int), 16, width, left_align, fill, 0);
                    break;
                case 'X':
                    mini_print_unsigned(va_arg(args, unsigned int), 16, width, left_align, fill, 1);
                    break;
                case 'c':
                    putchar((char)va_arg(args, int));
                    break;
                case 's': {
                    const char *str = va_arg(args, const char *);
                    int len = 0;
                    const char *p = str;
                    while (*p++) len++;

                    int pad = width - len;
                    if (pad < 0) pad = 0;

                    if (!left_align) {
                        while (pad--) putchar(fill);
                    }

                    while (*str) {
                        putchar(*str++);
                    }

                    if (left_align) {
                        while (pad--) putchar(' ');
                    }
                    break;
                }
                case 'f':
                    mini_print_float(va_arg(args, double), width, left_align, fill);
                    break;
                case '%':
                    putchar('%');
                    break;
                default:
                    putchar('?');
                    break;
            }
        } else {
            putchar(*fmt);
        }
        fmt++;
    }

    va_end(args);

	return printed;    
}

#endif
