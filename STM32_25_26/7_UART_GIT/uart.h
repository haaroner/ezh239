namespace usart6
{
	void usart6Init(uint32_t speed, uint8_t word_length, float stop_bits);
	void write(uint8_t byte);
	uint16_t read();
	uint16_t available();
}