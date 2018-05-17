#ifndef __COM_MANAGER_H__
#define __COM_MANAGER_H__

#include "tff.h"

#ifndef PAYLOAD_T
#define PAYLOAD_T char
#endif

#ifndef PAYLOAD_SIZE_T
#define PAYLOAD_SIZE_T unsigned char
#endif

typedef struct{
  PAYLOAD_T *buf_begin;
  PAYLOAD_T *current;
  PAYLOAD_T *buf_end;
} packet_t;

#define PAGE_SIZE 32 // ŒÅ’è

void com_manager_init();
void com_manager_load_config(char *fname, void (*func)(FIL *));
PAYLOAD_SIZE_T com_manager_assign_page(void (*call_back)(packet_t *));
void com_manager_polling();

#endif /* __COM_MANAGER_H__ */
