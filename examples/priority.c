
#include <stdio.h>
#include <time.h>
#include <stc/crandom.h>

#define i_tag i
#define i_val int64_t
#define i_cmp -c_default_compare  // min-heap (increasing values)
#include <stc/cpque.h>

int main() {
    size_t N = 10000000;
    stc64_t rng = stc64_init(time(NULL));
    stc64_uniform_t dist = stc64_uniform_init(0, N * 10);
    c_auto (cpque_i, heap)
    {
        // Push ten million random numbers to priority queue
        printf("Push %zu numbers\n", N);
        c_forrange (N)
            cpque_i_push(&heap, stc64_uniform(&rng, &dist));

        // push some negative numbers too.
        c_apply(cpque_i, push, &heap, {-231, -32, -873, -4, -343});

        c_forrange (N)
            cpque_i_push(&heap, stc64_uniform(&rng, &dist));

        puts("Extract the hundred smallest.");
        c_forrange (100) {
            printf("%zd ", *cpque_i_top(&heap));
            cpque_i_pop(&heap);
        }
    }
}
