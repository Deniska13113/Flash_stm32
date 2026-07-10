
#include "main.h"
#include "stm32f1xx_hal.h"

#define address_total  0x801F800
#define address_display  0x0801FC00
typedef enum {
	total_page,
	display_page
} select_page;

typedef enum {
	page_empty,
	page_not_empty
} is_page_empty;

void lock_flash(void);
void unlock_flash(void);

uint32_t find_max_value_flash(select_page page);
uint32_t find_min_flash(select_page page);

void write_flash(select_page page, uint32_t data);
uint32_t read_fhash(select_page page);

is_page_empty is_flash_empty(select_page page);
