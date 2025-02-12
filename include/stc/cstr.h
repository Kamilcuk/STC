/* MIT License
 *
 * Copyright (c) 2021 Tyge Løvset, NORCE, www.norceresearch.no
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef CSTR_H_INCLUDED
#define CSTR_H_INCLUDED

#include "ccommon.h"
#include <stdlib.h> /* malloc */
#include <string.h>
#include <stdarg.h>
#include <stdio.h> /* vsnprintf */
#include <ctype.h>

typedef                 struct cstr { char* str; } cstr;
typedef                 struct cstr_iter { char *ref; } cstr_iter_t;
typedef                 char cstr_value_t;

#define cstr_npos       (SIZE_MAX >> 1)
STC_LIBRARY_ONLY(       extern const cstr cstr_null; )

struct cstr_rep         { size_t size, cap; char str[sizeof(size_t)]; };
#define _cstr_rep(self) c_container_of((self)->str, struct cstr_rep, str)
STC_STATIC_ONLY(        static struct cstr_rep _cstr_nullrep = {0, 0, {0}};
                        static const cstr cstr_null = {_cstr_nullrep.str}; )
typedef const char      strlit_t[];
/* optimal memory: based on malloc_usable_size() sequence: 24, 40, 56, ... */
#define _cstr_opt_mem(cap)  ((((offsetof(struct cstr_rep, str) + (cap) + 8)>>4)<<4) + 8)
/* optimal string capacity: 7, 23, 39, ... */
#define _cstr_opt_cap(cap)  (_cstr_opt_mem(cap) - offsetof(struct cstr_rep, str) - 1)

STC_API cstr            cstr_from_n(const char* str, size_t n);
STC_API cstr            cstr_from_fmt(const char* fmt, ...);
STC_API cstr            cstr_from_replace_all(const char* str, size_t str_len,
                                              const char* find, size_t find_len,
                                              const char* repl, size_t repl_len);
STC_API size_t          cstr_reserve(cstr* self, size_t cap);
STC_API void            cstr_resize(cstr* self, size_t len, char fill);
STC_API cstr*           cstr_assign_n(cstr* self, const char* str, size_t n);
STC_API cstr*           cstr_assign_fmt(cstr* self, const char* fmt, ...);
STC_API cstr*           cstr_append_n(cstr* self, const char* str, size_t n);
STC_API void            cstr_replace_n(cstr* self, size_t pos, size_t len, const char* str, size_t n);
STC_API void            cstr_replace_all(cstr* self, const char* find, const char* replace);
STC_API void            cstr_erase_n(cstr* self, size_t pos, size_t n);
STC_API size_t          cstr_find(cstr s, const char* needle);
STC_API size_t          cstr_find_n(cstr s, const char* needle, size_t pos, size_t nmax);
STC_API bool            cstr_getdelim(cstr *self, int delim, FILE *stream);

STC_API char*           c_strnstrn(const char* s, const char* needle, size_t slen, size_t nlen);
STC_API int             c_strncasecmp(const char* s1, const char* s2, size_t nmax);

STC_INLINE cstr         cstr_init() { return cstr_null; }
#define                 cstr_lit(literal) \
                            cstr_from_n(literal, sizeof c_make(strlit_t){literal} - 1)
STC_INLINE cstr         cstr_from(const char* str)
                            { return cstr_from_n(str, strlen(str)); }
STC_INLINE const char*  cstr_str(const cstr* self) { return self->str; }
STC_INLINE char*        cstr_data(cstr* self) { return self->str; }
STC_INLINE size_t       cstr_size(cstr s) { return _cstr_rep(&s)->size; }
STC_INLINE size_t       cstr_length(cstr s) { return _cstr_rep(&s)->size; }
STC_INLINE size_t       cstr_capacity(cstr s) { return _cstr_rep(&s)->cap; }
STC_INLINE bool         cstr_empty(cstr s) { return _cstr_rep(&s)->size == 0; }
STC_INLINE void         cstr_del(cstr* self)
                            { if (_cstr_rep(self)->cap) c_free(_cstr_rep(self)); }
STC_INLINE cstr         cstr_clone(cstr s)
                            { return cstr_from_n(s.str, _cstr_rep(&s)->size); }
STC_INLINE void         cstr_clear(cstr* self)
                            { self->str[_cstr_rep(self)->size = 0] = '\0'; }
STC_INLINE cstr*        cstr_assign(cstr* self, const char* str)
                            { return cstr_assign_n(self, str, strlen(str)); }
STC_INLINE cstr*        cstr_copy(cstr* self, cstr s)
                            { return cstr_assign_n(self, s.str, _cstr_rep(&s)->size); }
STC_INLINE cstr*        cstr_append(cstr* self, const char* str)
                            { return cstr_append_n(self, str, strlen(str)); }
STC_INLINE cstr*        cstr_append_s(cstr* self, cstr s)
                            { return cstr_append_n(self, s.str, _cstr_rep(&s)->size); }
STC_INLINE void         cstr_push_back(cstr* self, char value)
                            { cstr_append_n(self, &value, 1); }
STC_INLINE void         cstr_pop_back(cstr* self)
                            { self->str[ --_cstr_rep(self)->size ] = '\0'; }
STC_INLINE void         cstr_insert_n(cstr* self, size_t pos, const char* str, size_t n)
                            { cstr_replace_n(self, pos, 0, str, n); }
STC_INLINE void         cstr_insert(cstr* self, size_t pos, const char* str)
                            { cstr_replace_n(self, pos, 0, str, strlen(str)); }
STC_INLINE void         cstr_insert_s(cstr* self, size_t pos, cstr s)
                            { cstr_replace_n(self, pos, 0, s.str, _cstr_rep(&s)->size); }
STC_INLINE void         cstr_replace(cstr* self, size_t pos, size_t len, const char* str)
                            { cstr_replace_n(self, pos, len, str, strlen(str)); }
STC_INLINE void         cstr_replace_s(cstr* self, size_t pos, size_t len, cstr s)
                            { cstr_replace_n(self, pos, len, s.str, _cstr_rep(&s)->size); }
STC_INLINE void         cstr_erase(cstr* self, size_t pos)
                            { cstr_erase_n(self, pos, 1); }
STC_INLINE char*        cstr_front(cstr* self) { return self->str; }
STC_INLINE char*        cstr_back(cstr* self)
                            { return self->str + _cstr_rep(self)->size - 1; }
STC_INLINE cstr_iter_t  cstr_begin(cstr* self)
                            { return c_make(cstr_iter_t){self->str}; }
STC_INLINE cstr_iter_t  cstr_end(cstr* self)
                            { return c_make(cstr_iter_t){self->str + _cstr_rep(self)->size}; }
STC_INLINE void         cstr_next(cstr_iter_t* it) {++it->ref; }
STC_INLINE bool         cstr_equalto(cstr s, const char* str)
                            { return strcmp(s.str, str) == 0; }
STC_INLINE bool         cstr_equalto_s(cstr s1, cstr s2)
                            { return strcmp(s1.str, s2.str) == 0; }
STC_INLINE bool         cstr_contains(cstr s, const char* needle)
                            { return strstr(s.str, needle) != NULL; }
STC_INLINE bool         cstr_getline(cstr *self, FILE *stream)
                            { return cstr_getdelim(self, '\n', stream); }

STC_INLINE cstr
cstr_with_capacity(size_t cap) {
    cstr s = cstr_null;
    cstr_reserve(&s, cap);
    return s;
}

STC_INLINE cstr
cstr_with_size(size_t len, char fill) {
    cstr s = cstr_null;
    cstr_resize(&s, len, fill);
    return s;
}

STC_INLINE cstr*
cstr_take(cstr* self, cstr s) {
    if (self->str != s.str && _cstr_rep(self)->cap)
        c_free(_cstr_rep(self));
    self->str = s.str;
    return self;
}

STC_INLINE cstr
cstr_move(cstr* self) {
    cstr tmp = *self;
    *self = cstr_null;
    return tmp;
}

STC_INLINE bool
cstr_starts_with(cstr s, const char* sub) {
    while (*sub && *s.str == *sub) ++s.str, ++sub;
    return *sub == 0;
}

STC_INLINE bool
cstr_ends_with(cstr s, const char* sub) {
    size_t n = strlen(sub), sz = _cstr_rep(&s)->size;
    return n <= sz && !memcmp(s.str + sz - n, sub, n);
}

/* container adaptor functions: */
#define  cstr_toraw(xp)           ((xp)->str)  // deprecated
#define  cstr_compare(xp, yp)     strcmp((xp)->str, (yp)->str)
#define  cstr_equals(xp, yp)      (strcmp((xp)->str, (yp)->str) == 0)
#define  cstr_hash(xp, ...)       c_default_hash((xp)->str, cstr_size(*(xp)))

/* -------------------------- IMPLEMENTATION ------------------------- */

#if !defined(STC_HEADER) || defined(STC_IMPLEMENTATION)

STC_LIBRARY_ONLY( static struct cstr_rep _cstr_nullrep = {0, 0, {0}};
                  const cstr cstr_null = {_cstr_nullrep.str}; )

STC_DEF size_t
cstr_reserve(cstr* self, size_t cap) {
    struct cstr_rep* rep = _cstr_rep(self);
    size_t oldcap = rep->cap;
    if (cap > oldcap) {
        rep = (struct cstr_rep*) c_realloc(oldcap ? rep : NULL, _cstr_opt_mem(cap));
        self->str = rep->str;
        if (oldcap == 0) self->str[rep->size = 0] = '\0';
        return (rep->cap = _cstr_opt_cap(cap));
    }
    return oldcap;
}

STC_DEF void
cstr_resize(cstr* self, size_t len, char fill) {
    size_t n =  _cstr_rep(self)->size;
    cstr_reserve(self, len);
    if (len > n) memset(self->str + n, fill, len - n);
    if (len | n) self->str[_cstr_rep(self)->size = len] = '\0';
}

STC_DEF cstr
cstr_from_n(const char* str, size_t n) {
    if (n == 0) return cstr_null;
    struct cstr_rep* rep = (struct cstr_rep*) c_malloc(_cstr_opt_mem(n));
    cstr s = {(char *) memcpy(rep->str, str, n)};
    s.str[rep->size = n] = '\0';
    rep->cap = _cstr_opt_cap(n);
    return s;
}

#if defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable: 4996)
#endif

STC_DEF void
cstr_vfmt(cstr* self, const char* fmt, va_list args) {
    va_list args2;
    va_copy(args2, args);
    int len = vsnprintf(NULL, (size_t)0, fmt, args);
    cstr_reserve(self, len);
    vsprintf(self->str, fmt, args2);
    va_end(args2);
    _cstr_rep(self)->size = len;
}

#if defined(__clang__)
#  pragma clang diagnostic pop
#elif defined(_MSC_VER)
#  pragma warning(pop)
#endif

STC_DEF cstr
cstr_from_fmt(const char* fmt, ...) {
    cstr ret = cstr_null;
    va_list args; va_start(args, fmt);
    cstr_vfmt(&ret, fmt, args);
    va_end(args);
    return ret;
}

STC_DEF cstr*
cstr_assign_fmt(cstr* self, const char* fmt, ...) {
    cstr ret = cstr_null;
    va_list args; va_start(args, fmt);
    cstr_vfmt(&ret, fmt, args);
    va_end(args);
    cstr_del(self); *self = ret;
    return self;
}

STC_DEF cstr*
cstr_assign_n(cstr* self, const char* str, size_t n) {
    if (n || _cstr_rep(self)->cap) {
        cstr_reserve(self, n);
        memmove(self->str, str, n);
        self->str[_cstr_rep(self)->size = n] = '\0';
    }
    return self;
}

STC_DEF cstr*
cstr_append_n(cstr* self, const char* str, size_t n) {
    if (n == 0) return self;
    size_t oldlen = _cstr_rep(self)->size, newlen = oldlen + n;
    if (newlen > _cstr_rep(self)->cap) {
        size_t off = (size_t) (str - self->str); /* handle self append */
        cstr_reserve(self, (oldlen*13 >> 3) + n);
        if (off <= oldlen) str = self->str + off;
    }
    memcpy(&self->str[oldlen], str, n);
    self->str[_cstr_rep(self)->size = newlen] = '\0';
    return self;
}

STC_INLINE void _cstr_internal_move(cstr* self, size_t pos1, size_t pos2) {
    if (pos1 == pos2)
        return;
    size_t len = _cstr_rep(self)->size, newlen = len + pos2 - pos1;
    if (newlen > _cstr_rep(self)->cap)
        cstr_reserve(self, (len*13 >> 3) + pos2 - pos1);
    memmove(&self->str[pos2], &self->str[pos1], len - pos1);
    self->str[_cstr_rep(self)->size = newlen] = '\0';
}

STC_DEF void
cstr_replace_n(cstr* self, size_t pos, size_t len, const char* str, size_t n) {
    size_t sz = cstr_size(*self);
    if (len > sz - pos) len = sz - pos;
    c_autobuf (xstr, char, n) {
        memcpy(xstr, str, n);
        _cstr_internal_move(self, pos + len, pos + n);
        memcpy(&self->str[pos], xstr, n);
    }
}

STC_DEF cstr
cstr_from_replace_all(const char* str, size_t str_len,
                      const char* find, size_t find_len,
                      const char* repl, size_t repl_len) {
    cstr out = cstr_null;
    size_t from = 0, pos; char* res;
    if (find_len)
        while ((res = c_strnstrn(str + from, find, str_len - from, find_len))) {
            pos = res - str;
            cstr_append_n(&out, str + from, pos - from);
            cstr_append_n(&out, repl, repl_len);
            from = pos + find_len;
        }
    cstr_append_n(&out, str + from, str_len - from);
    return out;
}

STC_DEF void
cstr_replace_all(cstr* self, const char* find, const char* repl) {
    cstr_take(self, cstr_from_replace_all(self->str, _cstr_rep(self)->size,
                                          find, strlen(find), repl, strlen(repl)));
}

STC_DEF void
cstr_erase_n(cstr* self, size_t pos, size_t n) {
    size_t len = _cstr_rep(self)->size;
    if (n > len - pos) n = len - pos;
    if (len) {
        memmove(&self->str[pos], &self->str[pos + n], len - (pos + n));
        self->str[_cstr_rep(self)->size -= n] = '\0';
    }
}

STC_DEF bool
cstr_getdelim(cstr *self, int delim, FILE *fp) {
    size_t pos = 0, cap = _cstr_rep(self)->cap;
    int c = fgetc(fp);
    if (c == EOF)
        return false;
    for (;;) {
        if (c == delim || c == EOF) {
            if (cap) self->str[_cstr_rep(self)->size = pos] = '\0';
            return true;
        }
        if (pos == cap)
            cap = cstr_reserve(self, (cap*13 >> 3) + 16);
        self->str[pos++] = (char) c;
        c = fgetc(fp);
    }
}

STC_DEF size_t
cstr_find(cstr s, const char* needle) {
    char* res = strstr(s.str, needle);
    return res ? res - s.str : cstr_npos;
}

STC_DEF size_t
cstr_find_n(cstr s, const char* needle, size_t pos, size_t nmax) {
    if (pos > _cstr_rep(&s)->size) return cstr_npos;
    size_t nlen = strlen(needle);
    char* res = c_strnstrn(s.str + pos, needle, _cstr_rep(&s)->size - pos, nmax < nlen ? nmax : nlen);
    return res ? res - s.str : cstr_npos;
}

STC_DEF int
c_strncasecmp(const char* s1, const char* s2, size_t nmax) {
    int ret = 0;
    while (nmax-- && (ret = tolower(*s1++) - tolower(*s2)) == 0 && *s2++) ;
    return ret;
}

STC_DEF char*
c_strnstrn(const char *s, const char *needle, size_t slen, size_t nlen) {
    if (!nlen) return (char *)s;
    if (nlen > slen) return NULL;
    slen -= nlen;
    do {
        if (*s == *needle && !memcmp(s, needle, nlen)) return (char *)s;
        ++s;
    } while (slen--);
    return NULL;
}

#endif
#endif