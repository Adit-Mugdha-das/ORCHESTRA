#ifndef FLOW_REGISTRY_H
#define FLOW_REGISTRY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "interpreter.h"

/* Opaque handle to a registered flow. */
typedef struct FlowHandle FlowHandle;

typedef void (*flow_registry_iter_cb)(const char *name, void *user);

void flow_registry_register(char *name /* takes ownership */, NameList *params /* takes ownership */, Stmt *body /* takes ownership */);
FlowHandle* flow_registry_find(const char *name);
NameList* flow_registry_params(FlowHandle *h);
Stmt* flow_registry_body(FlowHandle *h);

/* Iterate all registered flow names (user-defined functions/methods). */
void flow_registry_for_each(flow_registry_iter_cb cb, void *user);
void flow_registry_free_all(void);

#ifdef __cplusplus
}
#endif

#endif
