#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 1000

typedef struct {
    char name[64];
    char type[16];     // "int", "float", "string", "bool", "unknown"
    double num_value;  // for int/float
    char str_value[256];
    int bool_value;
    int scope_level;
} Symbol;

static Symbol symtab[MAX_SYMBOLS];
static int symcount = 0;
static int current_scope = 0;

static int lookup_in_scope(const char *name, int scope);
static int lookup_any(const char *name);

static void push_scope() {
    current_scope++;
}

static void pop_scope() {
    while (symcount > 0 && symtab[symcount - 1].scope_level == current_scope) {
        symcount--;
    }
    current_scope--;
    if (current_scope < 0) current_scope = 0;
}

static Symbol* get_symbol_or_null(const char *name) {
    int idx = lookup_any(name);
    if (idx == -1) return NULL;
    return &symtab[idx];
}

static Symbol* get_symbol_or_error(const char *name) {
    Symbol *s = get_symbol_or_null(name);
    if (!s) {
        printf("Error: Undeclared variable '%s'\n", name);
        exit(1);
    }
    return s;
}

static void set_symbol_value(Symbol *s, const char *type, double num_value, const char *str_value, int bool_value) {
    strcpy(s->type, type);
    s->num_value = num_value;
    s->bool_value = bool_value;
    if (str_value) {
        strncpy(s->str_value, str_value, sizeof(s->str_value) - 1);
        s->str_value[sizeof(s->str_value) - 1] = '\0';
    } else {
        s->str_value[0] = '\0';
    }
}

static int lookup_in_scope(const char *name, int scope) {
    for (int i = symcount - 1; i >= 0; i--) {
        if (symtab[i].scope_level == scope && strcmp(symtab[i].name, name) == 0)
            return i;
    }
    return -1;
}

static int lookup_any(const char *name) {
    for (int i = symcount - 1; i >= 0; i--) {
        if (strcmp(symtab[i].name, name) == 0)
            return i;
    }
    return -1;
}

// Python-like: if exists (nearest), update; else insert in current scope.
static void insert_or_update_value(const char *name, const char *type, double num_value, const char *str_value, int bool_value) {
    int idx = lookup_any(name);
    if (idx == -1) {
        if (symcount >= MAX_SYMBOLS) {
            printf("Error: Symbol table overflow\n");
            exit(1);
        }
        strcpy(symtab[symcount].name, name);
        set_symbol_value(&symtab[symcount], type, num_value, str_value, bool_value);
        symtab[symcount].scope_level = current_scope;
        symcount++;
    } else {
        set_symbol_value(&symtab[idx], type, num_value, str_value, bool_value);
    }
}

// For NOTE: keep it “declaration-like” but still Python-friendly.
// If declared again in SAME scope → update type (no duplicate error).
static void declare_or_update_current_scope_value(const char *name, const char *type, double num_value, const char *str_value, int bool_value) {
    int idx = lookup_in_scope(name, current_scope);
    if (idx == -1) {
        if (symcount >= MAX_SYMBOLS) {
            printf("Error: Symbol table overflow\n");
            exit(1);
        }
        strcpy(symtab[symcount].name, name);
        set_symbol_value(&symtab[symcount], type, num_value, str_value, bool_value);
        symtab[symcount].scope_level = current_scope;
        symcount++;
    } else {
        set_symbol_value(&symtab[idx], type, num_value, str_value, bool_value);
    }
}

static const char* get_type_or_error(const char *name) {
    return get_symbol_or_error(name)->type;
}

#endif