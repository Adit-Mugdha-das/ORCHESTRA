#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 1000

typedef struct {
    char name[64];
    char type[16];     // "int", "float", "string", "bool", "unknown"
    int scope_level;
} Symbol;

static Symbol symtab[MAX_SYMBOLS];
static int symcount = 0;
static int current_scope = 0;

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
static void insert_or_update(const char *name, const char *type) {
    int idx = lookup_any(name);
    if (idx == -1) {
        if (symcount >= MAX_SYMBOLS) {
            printf("Error: Symbol table overflow\n");
            exit(1);
        }
        strcpy(symtab[symcount].name, name);
        strcpy(symtab[symcount].type, type);
        symtab[symcount].scope_level = current_scope;
        symcount++;
    } else {
        strcpy(symtab[idx].type, type);
    }
}

// For NOTE: keep it “declaration-like” but still Python-friendly.
// If declared again in SAME scope → update type (no duplicate error).
static void declare_or_update_current_scope(const char *name, const char *type) {
    int idx = lookup_in_scope(name, current_scope);
    if (idx == -1) {
        if (symcount >= MAX_SYMBOLS) {
            printf("Error: Symbol table overflow\n");
            exit(1);
        }
        strcpy(symtab[symcount].name, name);
        strcpy(symtab[symcount].type, type);
        symtab[symcount].scope_level = current_scope;
        symcount++;
    } else {
        strcpy(symtab[idx].type, type);
    }
}

static const char* get_type_or_error(const char *name) {
    int idx = lookup_any(name);
    if (idx == -1) {
        printf("Error: Undeclared variable '%s'\n", name);
        exit(1);
    }
    return symtab[idx].type;
}

#endif