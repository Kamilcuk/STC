
#include <stc/cstr.h>

int main ()
{
    const char *base = "this is a test string.";
    const char *s2 = "n example";
    const char *s3 = "sample phrase";
    const char *s4 = "useful.";

    // replace signatures used in the same order as described above:

    // Ustring positions:                        0123456789*123456789*12345
    cstr s = cstr_from(base);              // "this is a test string."
    cstr m = cstr_clone(s);
    c_autoscope (0, cstr_del(&s), cstr_del(&m)) {
        cstr_append(&m, m.str);
        cstr_append(&m, m.str);
        printf("%s\n", m.str);

        cstr_replace(&s, 9, 5, s2);              // "this is an example string." (1)
        printf("(1) %s\n", s.str);
        cstr_replace_n(&s, 19, 6, s3+7, 6);      // "this is an example phrase." (2)
        printf("(2) %s\n", s.str);
        cstr_replace(&s, 8, 10, "just a");       // "this is just a phrase."     (3)
        printf("(3) %s\n", s.str);
        cstr_replace_n(&s, 8, 6,"a shorty", 7);  // "this is a short phrase."    (4)
        printf("(4) %s\n", s.str);
        cstr_replace(&s, 22, 1, "!!!");          // "this is a short phrase!!!"  (5)
        printf("(5) %s\n", s.str);
    }
}
