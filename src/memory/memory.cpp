#include <memory.hpp>

Memory::Memory(shared_ptr<Log> log_ptr) {
    g_log_ptr = log_ptr;
}

Memory::~Memory() {

}

void Memory::initMemory() {
    srand(time(0));
    uint32_t *g_mem_phy_ptr = (uint32_t *)g_mem_phy;
    for (int i = 0; i < (int)(CONFIG_MEMORY_SIZE / sizeof(g_mem_phy_ptr[0])); i++) {
        g_mem_phy_ptr[i] = rand();
    }
    g_log_ptr->printLog("success", "[loader] [init] [mem]\n", "");
}
