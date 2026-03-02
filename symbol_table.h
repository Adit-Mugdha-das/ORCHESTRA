#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Keep this struct C-compatible so Flex/Bison generated C code can include it.
   Implementation lives in symbol_table.cpp. */
typedef struct {
    char name[64];
    char type[16];     /* "int", "float", "string", "bool", "unknown" */
    double num_value;  /* for int/float */
    char str_value[256];
    int bool_value;
    void *ptr_value;   /* for "struct" instances */
    char struct_type[64];
    int scope_level;
} Symbol;

void push_scope(void);
void pop_scope(void);

Symbol* get_symbol_or_null(const char *name);
Symbol* get_symbol_or_error(const char *name);

/* Python-like: if exists (nearest), update; else insert in current scope. */
void insert_or_update_value(const char *name, const char *type, double num_value, const char *str_value, int bool_value);

/* For NOTE: declaration-like in current scope (update if already in current scope). */
void declare_or_update_current_scope_value(const char *name, const char *type, double num_value, const char *str_value, int bool_value);

/* For ensembles (struct instances) */
void insert_or_update_struct(const char *name, const char *struct_type, void *ptr_value);
void declare_or_update_current_scope_struct(const char *name, const char *struct_type, void *ptr_value);

const char* get_type_or_error(const char *name);

#ifdef __cplusplus
}
#endif

#endif
