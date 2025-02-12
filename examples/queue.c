#include <stc/crandom.h>
#include <stdio.h>

#define i_tag i
#define i_val int
#include <stc/cqueue.h>

int main() {
    int n = 100000000;
    stc64_uniform_t dist;
    stc64_t rng = stc64_init(1234);
    dist = stc64_uniform_init(0, n);

    c_auto (cqueue_i, queue)
    {
        // Push ten million random numbers onto the queue.
        c_forrange (n)
            cqueue_i_push(&queue, stc64_uniform(&rng, &dist));

        // Push or pop on the queue ten million times
        printf("%d\n", n);
        c_forrange (n) { // forrange uses initial n only.
            int r = stc64_uniform(&rng, &dist);
            if (r & 1)
                ++n, cqueue_i_push(&queue, r);
            else
                --n, cqueue_i_pop(&queue);
        }
        printf("%d, %zu\n", n, cqueue_i_size(queue));
    }
}
