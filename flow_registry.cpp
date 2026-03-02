#include "flow_registry.h"

#include <string>
#include <unordered_map>
#include <utility>

#include <cstdlib>
#include <cstring>

namespace {
struct Flow {
    std::string name;
    NameList *params{nullptr};
    Stmt *body{nullptr};
};

static std::unordered_map<std::string, Flow> g_flows;

static void free_namelist_local(NameList *list) {
    while (list) {
        NameList *next = list->next;
        std::free(list->name);
        std::free(list);
        list = next;
    }
}
} // namespace

extern "C" {

struct FlowHandle {
    Flow *flow;
};

static void runtime_error_local(const char *msg) {
    std::fprintf(stdout, "Runtime Error: %s\n", msg);
    std::exit(1);
}

void flow_registry_register(char *name, NameList *params, Stmt *body) {
    if (!name) runtime_error_local("Null flow name");

    std::string key(name);
    /* name is arena-owned (arena_strdup); it will be released by arena_free_all() */

    if (g_flows.find(key) != g_flows.end()) runtime_error_local("Duplicate flow name");

    Flow f;
    f.name = key;
    f.params = params;
    f.body = body;

    g_flows.emplace(key, std::move(f));
}

FlowHandle* flow_registry_find(const char *name) {
    if (!name) return nullptr;
    auto it = g_flows.find(name);
    if (it == g_flows.end()) return nullptr;

    FlowHandle *h = (FlowHandle*)std::malloc(sizeof(FlowHandle));
    if (!h) std::exit(1);
    h->flow = &it->second;
    return h;
}

NameList* flow_registry_params(FlowHandle *h) {
    if (!h || !h->flow) return nullptr;
    return h->flow->params;
}

Stmt* flow_registry_body(FlowHandle *h) {
    if (!h || !h->flow) return nullptr;
    return h->flow->body;
}

void flow_registry_free_all(void) {
    /* params/body are arena-owned; releasing the registry just drops references */
    g_flows.clear();
}

} // extern "C"
