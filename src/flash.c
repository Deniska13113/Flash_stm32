#include <flash.h>


typedef enum {
	page_empty,
	page_not_empty,
	page_error
} is_page_empty;

typedef enum {
	page_full,
	page_not_full,
	page_error
} is_page_full;

is_page_empty is_flash_empty(select_page page); // Checking is page empty function
is_page_full is_flash_full(select_page page);


uint32_t read_fhash_with_address(uint32_t address)
{
	return *((uint32_t*)address);
}

uint8_t unlock_flash(void)
{
	uint8_t status;
	return status = HAL_FLASH_Unlock();
}

uint8_t lock_flash(void)
{
	uint8_t status;
	return status = HAL_FLASH_lock();
}

is_page_empty is_flash_empty(select_page page)
{
	if(page == total_page) // need new version which will work with define
	{
		for (uint32_t address_read = address_page_1; address_read<address_page_1+0x400;address_read+=4) // Change 0x400 to define
			// because depends of mcu; change 4 depends of define half_word or word use
		{
			uint32_t value = read_fhash_with_address(address_read);
			if(value!= 0xFFFFFFFF) return page_not_empty;
		}
		return page_empty;
	}
	else if (page == display_page)
	{
		for (uint32_t address_read = address_page_2; address_read<address_page_2+0x400;address_read+=4)
		{
			uint32_t value = read_fhash_with_address(address_read);
			if(value!= 0xFFFFFFFF) return page_not_empty;
		}
		return page_empty;
	}
	else
		return page_error;
}

is_page_full is_flash_full(select_page page)
{
	if (page == total_page)
	{
		for (uint32_t address_read = address_page_1;
				address_read < address_page_1 + 0x400; address_read += 4) // same 41 line
		{
			uint32_t value = read_fhash_with_address(address_read);
			if (value == 0xFFFFFFFF)
				return page_not_full;
		}
		return page_full;
	}
	else if (page == display_page)
	{
		for (uint32_t address_read = address_page_2;
				address_read < address_page_2 + 0x400; address_read += 4)
		{
			uint32_t value = read_fhash_with_address(address_read);
			if (value != 0xFFFFFFFF)
				return page_not_full;
		}
		return page_full;
	}
	else
		return page_error;
}

uint32_t find_min_flash(select_page page)
{
	if(page == total_page)
	{
		uint32_t value_min=0xFFFFFFFF, address = 0;
		for (uint32_t address_find = address_page_1; address_find < address_page_1 + 0x400; address_find += 0x4) {
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
			for (uint32_t address_find = address_page_2; address_find < address_page_2 + 0x400; address_find += 4) {
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
		for (uint32_t address_find = address_page_1;
				address_find < address_page_1 + 0x400; address_find += 4)
		{
			uint32_t value = read_fhash_with_address(address_find);
			if (value_max < value && value != 0xFFFFFFFF)
			{
				value_max = value;
				address = address_find;
			}
		}
		if (value_max == 0)
			return address_page_1;
		return address;
	}
	else
	{
		uint32_t value_max = 0, address = 0;
		for (uint32_t address_find = address_page_2;
				address_find < address_page_2 + 0x400; address_find += 4)
		{
			uint32_t value = read_fhash_with_address(address_find);
			if (value_max < value && value != 0xFFFFFFFF)
			{
				value_max = value;
				address = address_find;
			}
		}
		if (value_max == 0)
			return address_page_2;
		return address;
	}
}

uint8_t erase_flash(select_page page)
{
	FLASH_EraseInitTypeDef flash;
	flash.TypeErase = FLASH_TYPEERASE_PAGES;
	flash.Banks = FLASH_BANK_BOTH;
	flash.NbPages = 1;
	uint32_t error;
	uint8_t status;
	if(page == total_page)
	{
		flash.PageAddress = total_page;
		status = HAL_FLASHEx_Erase(flash, &error);
	}
	return status;
}

uint8_t write_flash(select_page page, uint32_t data)
{
	uint8_t status;
	uint32_t address_write = find_min_flash(page);
	unlock_flash();
	status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address_write, data);
	lock_flash();
	return status;
}

uint32_t read_fhash(select_page page)
{
	uint32_t address = find_max_value_flash(page);
	return read_fhash_with_address(address);
}


uint8_t test1(void)
{
	uint32_t disp = read_fhash(display_page);
	uint32_t tot = read_fhash(total_page);
	if(disp != 0xFFFFFFFF && tot != 0xFFFFFFFF) return 1;
	for(uint16_t i = 0;i<256;i++)
	{
	write_flash(display_page, 0x0);
	write_flash(total_page, 0x0);
	disp = read_fhash(display_page);
	tot = read_fhash(total_page);
	}
	if(disp == 0x100 && tot == 0x100) return 0;
	else return 1;
}

uint8_t test2(void)
{
	uint32_t disp = read_fhash(display_page);
	uint32_t tot = read_fhash(total_page);
	for (uint16_t i = 0;i<10000;++i)
	{
		write_flash(display_page, i);
		write_flash(total_page, i);
		disp = read_fhash(display_page);
		tot = read_fhash(total_page);
		if (disp != i && tot != i)
			return 1;
	}
	return 0;
}

