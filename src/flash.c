#include <flash.h>

uint32_t read_fhash_with_address(uint32_t address)
{
	return *((uint32_t*)address);
}
void unlock_flash(void)
{
	HAL_FLASH_Unlock();
}

void lock_flash(void)
{
	HAL_FLASH_Lock();
}

is_page_empty is_flash_empty(select_page page)
{
	if(page == total_page)
	{
		for (uint32_t address_read = address_total; address_read<address_total+0x400;address_read+=4)
		{
			uint32_t value = read_fhash_with_address(address_read);
			if(value!= 0xFFFFFFFF) return page_not_empty;
		}
		return page_empty;
	}
	else
	{
		for (uint32_t address_read = address_display; address_read<address_display+0x400;address_read+=4)
		{
			uint32_t value = read_fhash_with_address(address_read);
			if(value!= 0xFFFFFFFF) return page_not_empty;
		}
		return page_empty;
	}
}

uint32_t find_min_flash(select_page page)
{
	if(page == total_page)
	{
		uint32_t value_min=0xFFFFFFFF, address = 0;
		for (uint32_t address_find = address_total; address_find < address_total + 0x400; address_find += 4) {
			uint32_t value = read_fhash_with_address(address_find);
			if(value == 0xFFFFFFFF) return address_find;
			if(value_min>value)
			{
				value_min = value;
				address = address_find;
			}
		}
		return address;
	}
	else
		{
			uint32_t value_min=0xFFFFFFFF, address = 0;
			for (uint32_t address_find = address_display; address_find < address_display + 0x400; address_find += 4) {
				uint32_t value = read_fhash_with_address(address_find);
				if(value == 0xFFFFFFFF) return address_find;
				if(value_min>value)
				{
					value_min = value;
					address = address_find;
				}
			}
			return address;
		}
}
uint32_t find_max_value_flash(select_page page)
{
	if (page == total_page)
	{
		uint32_t value_max = 0, address = 0;
		for (uint32_t address_find = address_total;
				address_find < address_total + 0x400; address_find += 4)
		{
			uint32_t value = read_fhash_with_address(address_find);
			if (value_max < value && value != 0xFFFFFFFF)
			{
				value_max = value;
				address = address_find;
			}
		}
		if (value_max == 0)
			return total_page;
		return address;
	}
	else
	{
		uint32_t value_max = 0, address = 0;
		for (uint32_t address_find = address_display;
				address_find < address_display + 0x400; address_find += 4)
		{
			uint32_t value = read_fhash_with_address(address_find);
			if (value_max < value && value != 0xFFFFFFFF)
			{
				value_max = value;
				address = address_find;
			}
		}
		if (value_max == 0)
			return display_page;
		return address;
	}
}
void write_flash(select_page page, uint32_t data)
{
		uint32_t address_write = find_min_flash(page);
		unlock_flash();
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address_write, data);
		lock_flash();
}

uint32_t read_fhash(select_page page)
{
	uint32_t address = find_max_value_flash(page);
	return read_fhash_with_address(address);
}

