#include "aodv_routing_table.h"

int routing_table_lookup(route_table_entry * routing_table, uint32_t dstAddress){	
	for(int i = 0; i < MAX_TABLE_ENTRIES; i++){
		if(routing_table[i].dstAddress == dstAddress){
			return i;
		}
	}
	return ENTRY_NOT_PRESENT;
}
