#include <stc/cstr.h>

void check_del(float* v) {printf("destroy %g\n", *v);}

#define i_tag f
#define i_val float
#define i_valdel check_del
#include <stc/cstack.h>

#define i_tag arr
#define i_val cstack_f
#define i_cmp c_no_compare
#define i_valdel cstack_f_del
#include <stc/clist.h>

#define i_tag lst
#define i_key int
#define i_val clist_arr
#define i_valdel clist_arr_del
#include <stc/cmap.h>

#define i_tag map
#define i_key_str
#define i_val cmap_lst
#define i_valdel cmap_lst_del
#include <stc/cmap.h>

// c++:
// using cstack_f = std::stack<float>;
// using map_lst = std::unordered_map<int, std::forward_list<array2f>>;
// using map_map = std::unordered_map<std::string, map_lst>;

int main() {
    int xdim = 4, ydim = 6;
    int x = 1, y = 3, tableKey = 42;
    const char* strKey = "first";

    cmap_map myMap = cmap_map_init();
    cmap_lst listMap = cmap_lst_init();
    clist_arr tableList = clist_arr_init();
    cstack_f stk = cstack_f_with_capacity(xdim * ydim);
    memset(stk.data, 0, xdim*ydim*sizeof *stk.data);
    stk.size = stk.capacity;

    printf("stk size: %zu\n", cstack_f_size(stk));

    // Put in some data in stack array
    stk.data[x] = 3.1415927f;
    clist_arr_push_back(&tableList, stk);
    cmap_lst_insert(&listMap, tableKey, tableList);
    cmap_map_insert(&myMap, cstr_from(strKey), listMap);

    // Access the data entry
    cmap_lst* mapL = &cmap_map_find(&myMap, strKey).ref->second;
    clist_arr* lstA = &cmap_lst_find(mapL, tableKey).ref->second;
    cstack_f arr = *clist_arr_back(lstA);
    printf("value (%d) is: %f\n", x, arr.data[x]);

    stk.data[x] = 1.41421356f; // change the value in array
    cmap_map_del(&myMap); // free up everything!
}
