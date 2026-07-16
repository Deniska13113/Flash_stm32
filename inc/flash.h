
#include "main.h"
#include "flash_conf.h"


	#ifdef STM32F0
		#include "stm32f0xx_hal.h"       /* Import HAL library */
	#elif defined(STM32F1)
		#include "stm32f1xx_hal.h"       /* Import HAL library */
	#elif defined(STM32F2)
		#include "stm32f2xx_hal.h"       /* Import HAL library */
	#elif defined(STM32F3)
		#include "stm32f3xx_hal.h"       /* Import HAL library */
	#elif defined(STM32F4)
		#include "stm32f4xx_hal.h"       /* Import HAL library */
	#elif defined(STM32F7)
		#include "stm32f7xx_hal.h"       /* Import HAL library */
	#elif defined(STM32G0)
		#include "stm32g0xx_hal.h"       /* Import HAL library */
	#elif defined(STM32G4)
		#include "stm32g4xx_hal.h"       /* Import HAL library */
	#elif defined(STM32H7)
		#include "stm32h7xx_hal.h"       /* Import HAL library */
	#elif defined(STM32L0)
		#include "stm32l0xx_hal.h"       /* Import HAL library */
	#elif defined(STM32L1)
		#include "stm32l1xx_hal.h"       /* Import HAL library */
	#elif defined(STM32L5)
		#include "stm32l5xx_hal.h"       /* Import HAL library */
	#elif defined(STM32L4)
		#include "stm32l4xx_hal.h"       /* Import HAL library */
	#elif defined(STM32H7)
		#include "stm32h7xx_hal.h"       /* Import HAL library */
	#else
	#endif /* STM32F1 */


typedef enum {
	total_page,
	display_page
} select_page;


uint8_t lock_flash(void); // Locking flash writing function
uint8_t unlock_flash(void); // Unlocking flash writing function

uint32_t find_max_value_flash(select_page page); // Finding address with max value function
uint32_t find_min_flash(select_page page); // Finding address with max value function

#ifdef USE_WORD

uint8_t write_flash(select_page page, uint32_t data); // Writing data to page function
uint32_t read_fhash(select_page page); // Reading data from page function

#elif defined(USE_HALF_WORD)

uint8_t write_flash(select_page page, uint16_t data); // Writing data to page function
uint16_t read_fhash(select_page page); // Reading data from page function

#endif

uint8_t erase_flash(select_page page);

uint8_t test1(void);
uint8_t test2(void);
