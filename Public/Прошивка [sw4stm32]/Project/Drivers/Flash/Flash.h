void flash_unlock(void);																		// Разблокировка FLASH памяти
void flash_lock();																						// Блокировка FLASH памяти
uint32_t flash_read(uint32_t address);													// Чтение ячейки по указанному адресу
uint8_t flash_ready(void);																		// Проверка готовности FLASH памяти
void flash_erase_page(uint32_t address);											// Стирание страницы по указанному адресу
void flash_write(uint32_t address,uint32_t data);								// Запись ячейки по указанному адресу
