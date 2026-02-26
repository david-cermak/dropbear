#ifndef MAIN_MEM_STATS_H
#define MAIN_MEM_STATS_H

#define ENABLE_MEMORY_STATS 0
// need to enable freertos stats for this to work
// FREERTOS_USE_TRACE_FACILITY=y
// FREERTOS_USE_STATS_FORMATTING_FUNCTIONS=y
// FREERTOS_VTASKLIST_INCLUDE_COREID=y


#if ENABLE_MEMORY_STATS
void print_mem_stats(const char *label);
#else
static inline void print_mem_stats(const char *label) { (void)label; }
#endif

#endif /* MAIN_MEM_STATS_H */
