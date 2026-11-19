#include <stdio.h>
#include <ctype.h>
#include <string.h>

int word_count(const char s[])
{
    int count = 0, inside = 0, i = 0;
    while (s[i] != '\0') {
        if (isspace((unsigned char)s[i])) {
            inside = 0;
        } else if (!inside) {
            inside = 1;
            count++;
        }
        i++;
    }
    return count;
}

int is_palindrome(const char s[])
{
    int left = 0, right = 0;
    while (s[right] != '\0') {
        right++;
    }
    right--;

    while (left < right) {
        while (left < right && !isalpha((unsigned char)s[left])) {
            left++;
        }
        while (left < right && !isalpha((unsigned char)s[right])) {
            right--;
        }
        if (tolower((unsigned char)s[left]) != tolower((unsigned char)s[right])) {
            return 0;
        }
        left++;
        right--;
    }
    return 1;
}

void custom_to_upper(char dst[], const char src[])
{
    int i = 0;
    while (src[i] != '\0') {
        if (src[i] >= 'a' && src[i] <= 'z') {
            dst[i] = (char)(src[i] - ('a' - 'A'));
        } else {
            dst[i] = src[i];
        }
        i++;
    }
    dst[i] = '\0';
}

void custom_reverse(char dst[], const char src[])
{
    int len = 0, i;
    while (src[len] != '\0') {
        len++;
    }
    for (i = 0; i < len; i++) {
        dst[i] = src[len - 1 - i];
    }
    dst[len] = '\0';
}

void lib_to_upper(char dst[], const char src[])
{
    size_t len, i;
    strcpy(dst, src);
    len = strlen(dst);
    for (i = 0; i < len; i++) {
        dst[i] = (char)toupper((unsigned char)dst[i]);
    }
}

int lib_reverse_and_palindrome(char rev_out[], const char src[])
{
    char clean[256];
    char clean_rev[256];
    int k = 0, i;
    size_t len = strlen(src);

    for (i = 0; i < (int)len; i++) {
        rev_out[i] = src[len - 1 - i];
        if (isalpha((unsigned char)src[i])) {
            clean[k++] = (char)tolower((unsigned char)src[i]);
        }
    }
    rev_out[len] = '\0';
    clean[k] = '\0';

    for (i = 0; i < k; i++) {
        clean_rev[i] = clean[k - 1 - i];
    }
    clean_rev[k] = '\0';

    return strcmp(clean, clean_rev) == 0;
}

int main(void)
{
    const char *tab_msg = "Hello\tWorld\tCS101";
    const char *tests[3];
    const char *long_sms = "Urgent: Campus bus schedule changed due to heavy rain!";
    char small_buf[16];
    int t;

    tests[0] = "  Extra   spaces   and, punctuation! ";
    tests[1] = "";
    tests[2] = "A man, a plan, a canal: Panama!";

    printf("=== Complete the Code Output ===\n");
    printf("Words: %d\n", word_count("  Meet at  the library at 5 PM "));
    printf("%d %d\n", is_palindrome("Madam, I'm Adam"), is_palindrome("Hello"));
    printf("Tab Test (\"Hello\\tWorld\\tCS101\") -> Words: %d\n\n", word_count(tab_msg));

    printf("=== Custom Loops vs string.h Comparison ===\n");
    for (t = 0; t < 3; t++) {
        char c_up[128], l_up[128], c_rev[128], l_rev[128];
        int l_pal, c_pal;

        custom_to_upper(c_up, tests[t]);
        lib_to_upper(l_up, tests[t]);
        custom_reverse(c_rev, tests[t]);
        l_pal = lib_reverse_and_palindrome(l_rev, tests[t]);
        c_pal = is_palindrome(tests[t]);

        printf("Input      : \"%s\"\n", tests[t]);
        printf("  Words    : %d\n", word_count(tests[t]));
        printf("  Upper    : Custom=\"%s\" | Lib=\"%s\"\n", c_up, l_up);
        printf("  Reverse  : Custom=\"%s\" | Lib=\"%s\"\n", c_rev, l_rev);
        printf("  Palindr. : Custom=%d | Lib=%d\n", c_pal, l_pal);
    }

    printf("\n=== Student B: Safe Buffer Handling Demo ===\n");
    snprintf(small_buf, sizeof(small_buf), "%s", long_sms);
    printf("Safely truncated SMS (16-byte buffer via snprintf): \"%s\"\n", small_buf);

    return 0;
}
