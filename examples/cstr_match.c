#include <stc/cstr.h>
#include <stdio.h>

int main()
{
    c_autovar (cstr ss = cstr_lit("The quick brown fox jumps over the lazy dog.JPG"), cstr_del(&ss)) {
        size_t pos = cstr_find_n(ss, "brown", 0, 5);
        printf("%zu [%s]\n", pos, pos == cstr_npos ? "<NULL>" : &ss.str[pos]);
        printf("equals: %d\n", cstr_equalto(ss, "The quick brown fox jumps over the lazy dog.JPG"));
        printf("contains: %d\n", cstr_contains(ss, "umps ove"));
        printf("starts_with: %d\n", cstr_starts_with(ss, "The quick brown"));
        printf("ends_with: %d\n", cstr_ends_with(ss, ".jpg"));
        printf("ends_with: %d\n", cstr_ends_with(ss, ".JPG"));
    }
}