void flash_unlock(void);																		// ������������� FLASH ������
void flash_lock();																						// ���������� FLASH ������
uint32_t flash_read(uint32_t address);													// ������ ������ �� ���������� ������
uint8_t flash_ready(void);																		// �������� ���������� FLASH ������
void flash_erase_page(uint32_t address);											// �������� �������� �� ���������� ������
void flash_write(uint32_t address,uint32_t data);								// ������ ������ �� ���������� ������
