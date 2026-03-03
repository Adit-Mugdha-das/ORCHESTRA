#include "symbol_table.h"

#include <map>
#include <string>
#include <vector>

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {
struct Scope {
    std::map<std::string, Symbol> symbols;
};

static std::vector<Scope> g_scopes = { Scope{} }; /* always at least one */

static int current_scope_level() {
    return (int)g_scopes.size() - 1;
}

static void set_symbol_value(Symbol &s, const char *type, double num_value, const char *str_value, int bool_value) {
    std::strncpy(s.type, type ? type : "unknown", sizeof(s.type) - 1);
    s.type[sizeof(s.type) - 1] = '\0';
    s.num_value = num_value;
    s.bool_value = bool_value;
    s.ptr_value = nullptr;
    s.struct_type[0] = '\0';

    if (str_value) {
        std::strncpy(s.str_value, str_value, sizeof(s.str_value) - 1);
        s.str_value[sizeof(s.str_value) - 1] = '\0';
    } else {
        s.str_value[0] = '\0';
    }

    s.scope_level = current_scope_level();
}

static void set_symbol_struct(Symbol &s, const char *struct_type, void *ptr_value) {
    std::strncpy(s.type, "struct", sizeof(s.type) - 1);
    s.type[sizeof(s.type) - 1] = '\0';
    s.num_value = 0;
    s.bool_value = 0;
    s.str_value[0] = '\0';
    s.ptr_value = ptr_value;
    if (struct_type) {
        std::strncpy(s.struct_type, struct_type, sizeof(s.struct_type) - 1);
        s.struct_type[sizeof(s.struct_type) - 1] = '\0';
    } else {
        s.struct_type[0] = '\0';
    }
    s.scope_level = current_scope_level();
}

static void set_symbol_array(Symbol &s, void *ptr_value) {
    std::strncpy(s.type, "array", sizeof(s.type) - 1);
    s.type[sizeof(s.type) - 1] = '\0';
    s.num_value = 0;
    s.bool_value = 0;
    s.str_value[0] = '\0';
    s.ptr_value = ptr_value;
    s.struct_type[0] = '\0';
    s.scope_level = current_scope_level();
}

static Symbol* lookup_any(const char *name) {
    if (!name) return nullptr;
    for (int i = (int)g_scopes.size() - 1; i >= 0; --i) {
        auto it = g_scopes[(size_t)i].symbols.find(name);
        if (it != g_scopes[(size_t)i].symbols.end()) return &it->second;
    }
    return nullptr;
}

static Symbol* lookup_in_current_scope(const char *name) {
    if (!name) return nullptr;
    auto &cur = g_scopes.back().symbols;
    auto it = cur.find(name);
    if (it == cur.end()) return nullptr;
    return &it->second;
}
} // namespace

extern "C" {

void push_scope(void) {
    g_scopes.push_back(Scope{});
}

void pop_scope(void) {
    if (g_scopes.size() <= 1) return; /* keep global scope */
    g_scopes.pop_back();
}

Symbol* get_symbol_or_null(const char *name) {
    return lookup_any(name);
}

Symbol* get_symbol_or_error(const char *name) {
    Symbol *s = lookup_any(name);
    if (!s) {
        std::printf("Error: Undeclared variable '%s'\n", name ? name : "(null)");
        std::exit(1);
    }
    return s;
}

void insert_or_update_value(const char *name, const char *type, double num_value, const char *str_value, int bool_value) {
    if (!name) return;

    Symbol *existing = lookup_any(name);
    if (existing) {
        set_symbol_value(*existing, type, num_value, str_value, bool_value);
        return;
    }

    Symbol s{};
    std::strncpy(s.name, name, sizeof(s.name) - 1);
    s.name[sizeof(s.name) - 1] = '\0';
    set_symbol_value(s, type, num_value, str_value, bool_value);

    g_scopes.back().symbols[std::string(name)] = s;
}

void declare_or_update_current_scope_value(const char *name, const char *type, double num_value, const char *str_value, int bool_value) {
    if (!name) return;

    Symbol *existing = lookup_in_current_scope(name);
    if (existing) {
        set_symbol_value(*existing, type, num_value, str_value, bool_value);
        return;
    }

    Symbol s{};
    std::strncpy(s.name, name, sizeof(s.name) - 1);
    s.name[sizeof(s.name) - 1] = '\0';
    set_symbol_value(s, type, num_value, str_value, bool_value);

    g_scopes.back().symbols[std::string(name)] = s;
}

void insert_or_update_struct(const char *name, const char *struct_type, void *ptr_value) {
    if (!name) return;

    Symbol *existing = lookup_any(name);
    if (existing) {
        set_symbol_struct(*existing, struct_type, ptr_value);
        return;
    }

    Symbol s{};
    std::strncpy(s.name, name, sizeof(s.name) - 1);
    s.name[sizeof(s.name) - 1] = '\0';
    set_symbol_struct(s, struct_type, ptr_value);
    g_scopes.back().symbols[std::string(name)] = s;
}

void declare_or_update_current_scope_struct(const char *name, const char *struct_type, void *ptr_value) {
    if (!name) return;

    Symbol *existing = lookup_in_current_scope(name);
    if (existing) {
        set_symbol_struct(*existing, struct_type, ptr_value);
        return;
    }

    Symbol s{};
    std::strncpy(s.name, name, sizeof(s.name) - 1);
    s.name[sizeof(s.name) - 1] = '\0';
    set_symbol_struct(s, struct_type, ptr_value);
    g_scopes.back().symbols[std::string(name)] = s;
}

void insert_or_update_array(const char *name, void *ptr_value) {
    if (!name) return;

    Symbol *existing = lookup_any(name);
    if (existing) {
        set_symbol_array(*existing, ptr_value);
        return;
    }

    Symbol s{};
    std::strncpy(s.name, name, sizeof(s.name) - 1);
    s.name[sizeof(s.name) - 1] = '\0';
    set_symbol_array(s, ptr_value);
    g_scopes.back().symbols[std::string(name)] = s;
}

void declare_or_update_current_scope_array(const char *name, void *ptr_value) {
    if (!name) return;

    Symbol *existing = lookup_in_current_scope(name);
    if (existing) {
        set_symbol_array(*existing, ptr_value);
        return;
    }

    Symbol s{};
    std::strncpy(s.name, name, sizeof(s.name) - 1);
    s.name[sizeof(s.name) - 1] = '\0';
    set_symbol_array(s, ptr_value);
    g_scopes.back().symbols[std::string(name)] = s;
}

const char* get_type_or_error(const char *name) {
    return get_symbol_or_error(name)->type;
}

} // extern "C"
