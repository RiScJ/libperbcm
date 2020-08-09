#include "gpio.hpp"

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

GPIO::GPIO(int pin) : _pin(pin) {
	if (!initialized) {
		map();
	}
};

/******************************************************************************/

void GPIO::mode(Mode io) {
	io ? make_input(_pin) : make_output(_pin);
};


void GPIO::pull(PUD pud) {
	switch (pud) {
	case NONE: {
		pull_none(_pin);
		break;
	}
	case DOWN: {
		pull_down(_pin);
		break;
	}
	case UP: {
		pull_up(_pin);
		break;
	}
	default: {
		break;
	}
	}
};


void GPIO::drive(Level level) {
	switch (level) {
	case LOW: {
		clear_pin(_pin);
		break;
	}
	case HIGH: {
		set_pin(_pin);
		break;
	}
	default: {
		break;
	}
	}
};


Mode GPIO::mode(void) {
	return pin_mode(_pin);
};


PUD	GPIO::pull(void) {
	return pin_pull(_pin);
};


Level GPIO::read(void) {
	return pin_level(_pin);
};

/******************************************************************************/

void GPIO::pull_up(int pin) {
	int offset = pin / 16; // 16 pins per register
	int shift = pin % 16;
	shift *= 2; // 2 bits per pin

	pull_none(pin);
	*(_addr + GPPUD + offset) |= 1UL << shift;
};


void GPIO::pull_down(int pin) {
	int offset = pin / 16;
	int shift = pin % 16;
	shift *= 2;

	pull_none(pin);
	*(_addr + GPPUD + offset) |= 2UL << shift;
};


void GPIO::pull_none(int pin) {
	int offset = pin / 16; // 16 pins per register
	int shift = pin % 16;
	shift *= 2; // 2 bits per pin

	*(_addr + GPPUD + offset) &= ~(3UL << shift);
};


void GPIO::make_input(int pin) {
	int offset = pin / 10;
	int shift = pin % 10;
	shift *= 3;

	*(_addr + GPFSEL + offset) &= ~(7UL << shift);
};


void GPIO::make_output(int pin) {
	int offset = pin / 10;
	int shift = pin % 10;
	shift *= 3;

	make_input(pin);
	*(_addr + GPFSEL + offset) |=  (1UL << shift);
};


void GPIO::set_pin(int pin) {
	int offset = pin / 32;
	int shift = pin % 32;

	*(_addr + GPSET + offset) |= 1UL << shift;
};


void GPIO::clear_pin(int pin) {
	int offset = pin / 32;
	int shift = pin % 32;

	*(_addr + GPCLR + offset) |= 1UL << shift;
};


Level GPIO::pin_level(int pin) {
	int offset = pin / 32;
	int shift = pin % 32;
	return *(_addr + GPLEV + offset) & (1UL << shift) ? HIGH : LOW;
};


PUD GPIO::pin_pull(int pin) {
	int offset = pin / 16;
	int shift = pin % 16;
	shift *= 2;
	switch (*(_addr + GPPUD + offset) & (3UL << shift)) {
	case 0: {
		return NONE;
	}
	case 1: {
		return DOWN;
	}
	case 2: {
		return UP;
	}
	case 3:
	default: {
		return NONE;
	}
	}
};


Mode GPIO::pin_mode(int pin) {
	int offset = pin / 10;
	int shift = pin % 10;
	shift *= 3;

	switch (*(_addr + GPFSEL + offset) & (7UL << shift)) {
	case 0: {
		return IN;
	}
	case 1: {
		return OUT;
	}
	default: {
		return ALT;
	}
	}
};

/******************************************************************************/

bool GPIO::initialized = false;
int GPIO::_mem_fd;
void* GPIO::_map;
volatile unsigned int* GPIO::_addr;
unsigned long GPIO::_addr_p = 0xFE000000 + 0x200000;

int GPIO::map(void)
{
	if ((_mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
		return 0x4011;
	}

	_map = mmap(NULL,
			   BLOCK_SIZE,
			   PROT_READ|PROT_WRITE,
			   MAP_SHARED,
			   _mem_fd,
			   _addr_p
			   );

	if (_map == MAP_FAILED) {
		return 0x3012;
	}

	_addr = (volatile unsigned int *)_map;
	initialized = true;
	return 0x8000;
};


int GPIO::unmap(void) {
	munmap(_map, BLOCK_SIZE);
	close(_mem_fd);
	return 0;
};









