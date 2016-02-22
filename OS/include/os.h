#ifndef __OS_H_
#define __OS_H_

#include <ahci_structs.h>
#include <ata_structs.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct tagOS_DRIVE {
	HBA_PORT *port;
	uint32_t port_num;
	
	IDENTIFY_DEVICE_DATA id;
	void (*refresh_identify)();
	
	bool is_active;
} OS_DRIVE;

typedef struct tagOS_HBA {
	HBA_MEM *mem;
	
	uint32_t bus;
	uint32_t slot;
	uint32_t function;
	
	uint16_t vendorid;
	uint16_t deviceid;
	
	OS_DRIVE drives[32];
	void (*refresh_hba)();
	
	bool is_active;
} OS_HBA;

#ifdef KERNEL
OS_HBA hbas[8];
#else
extern OS_HBA hbas[8];
#endif

#endif