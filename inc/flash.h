
#include "main.h"
#include "stm32f1xx_hal.h"

#define address_total  0x801F800 // The address of the page where the total time working value will be saved
#define address_display  0x0801FC00 // The address of the page where the display time working value will be saved


typedef enum {
	total_page,
	display_page
} select_page;

typedef enum {
	page_empty,
	page_not_empty
} is_page_empty;


void lock_flash(void); // Locking flash writing function
void unlock_flash(void); // Unlocking flash writing function

uint32_t find_max_value_flash(select_page page); // Finding address with max value function
uint32_t find_min_flash(select_page page); // Finding address with max value function

void write_flash(select_page page, uint32_t data); // Writing data to page function
uint32_t read_fhash(select_page page); // Reading data from page function

is_page_empty is_flash_empty(select_page page); // Checking is page empty function
