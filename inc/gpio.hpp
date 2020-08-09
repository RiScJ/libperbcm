#ifndef GPIO_HPP
#define GPIO_HPP

#define BLOCK_SIZE 		(4*1024)

typedef unsigned long ulong;
typedef unsigned int  uint;

enum PUD {
	NONE,
	DOWN,
	UP
};

enum Mode {
	IN,
	OUT,
	ALT
};

enum Level {
	LOW = false,
	HIGH = true
};

enum Bank {
	GPFSEL = 0x00/4,	// 6	// function select
	GPSET  = 0x1C/4,	// 2	// pin output set
	GPCLR  = 0x28/4,	// 2	// pin output clear
	GPLEV  = 0x34/4,	// 2	// pin level
	GPEDS  = 0x40/4,	// 2	// pin event detect status
	GPREN  = 0x4C/4,	// 2	// pin rising edge detect enable
	GPFEN  = 0x58/4,	// 2	// pin falling edge detect enable
	GPHEN  = 0x64/4,	// 2	// pin high detect enable
	GPLEN  = 0x70/4,	// 2	// pin low detect enable
	GPAREN = 0x7C/4,	// 2	// pin async rising edge detect
	GPAFEN = 0x88/4,	// 2	// pin async falling edge detect
	GPPUD  = 0xE4/4		// 4	// pull up/down
};

class GPIO {
public:
	GPIO(int pin);

					void	mode		(Mode io);
					void	pull		(PUD pud);
					void	drive		(Level level);
					Mode	mode		(void);
					PUD		pull		(void);
					Level	read		(void);

private:
	static			void	pull_up		(int pin);
	static			void	pull_down	(int pin);
	static			void	pull_none	(int pin);
	static			void	make_input	(int pin);
	static			void	make_output	(int pin);
	static			void	set_pin		(int pin);
	static			void	clear_pin	(int pin);
	static			Level	pin_level	(int pin);
	static			PUD		pin_pull	(int pin);
	static			Mode	pin_mode	(int pin);

	static			int		map			(void);
	static			int		unmap		(void);

	static			void*	_map;
	static			int		_mem_fd;
	static			ulong	_addr_p;
	static volatile uint*	_addr;

					int		_pin;
	static			bool	initialized;
};


#endif // GPIO_HPP
