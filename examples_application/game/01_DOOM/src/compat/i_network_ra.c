// Network stub for Renesas RA6M5
// Networking is disabled

#include "config.h"
#include "doomtype.h"
#include "d_net.h"
#include "i_network.h"

void I_InitNetwork(void) {
}

void I_ShutdownNetwork(void) {
}

void I_WaitForPacket(int ms) {
}

void I_SendPacket(packet_info_t *packet, size_t len) {
}

const char *I_GetNetworkError(void) {
    return "Network not supported";
}
