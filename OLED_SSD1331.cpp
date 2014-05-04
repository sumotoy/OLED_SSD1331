

#include "Adafruit_GFX.h"
#include "OLED_SSD1331.h"
#include "glcdfont.c"
#include <avr/pgmspace.h>
#include "pins_arduino.h"
#include "wiring_private.h"
#include <SPI.h>



//Begin function
void OLED_SSD1331::begin(void) {
	commonInit();
	chipInit();
}


/***********************************/
void OLED_SSD1331::goTo(int x, int y) {
	if ((x >= WIDTH) || (y >= HEIGHT)) return;
    uint8_t cmd[] = {_CMD_SETCOLUMN,(uint8_t)x,OLED_MW,_CMD_SETROW,(uint8_t)y,OLED_MH};
    writeCommands(cmd, 6);
}

void OLED_SSD1331::goHome(void) {
	goTo(0,0);
}


uint16_t OLED_SSD1331::Color565(uint8_t r, uint8_t g, uint8_t b) {
	uint16_t c;
	c = r >> 3;
	c <<= 6;
	c |= g >> 2;
	c <<= 5;
	c |= b >> 3;
	return c;
}

//glue between Adafruit_GFX and Hardware accellerated function
void OLED_SSD1331::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
	hdwre_drawRect(x,y,w,h,color,false);
}

//glue between Adafruit_GFX and Hardware accellerated function
void OLED_SSD1331::fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t fillcolor) {
	hdwre_drawRect(x,y,w,h,fillcolor,true);
}

/*
hardware accellerated line draw
*/
void OLED_SSD1331::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {	
    uint8_t c1 = (color & 0xF800) >> 10, c2 = (color & 0x07E0) >> 5, c3 = (color & 0x001F) << 1;
	//rotation??
	switch (getRotation()) {
		case 1:
		swap(x0, y0);
		swap(x1, y1);
		x0 = WIDTH - x0 - 1;
		x1 = WIDTH - x1 - 1;
		break;
	case 2:
		x0 = WIDTH - x0 - 1;
		y0 = HEIGHT - y0 - 1;
		x1 = WIDTH - x1 - 1;
		y1 = HEIGHT - y1 - 1;
		break;
	case 3:
		swap(x0, y0);
		swap(x1, y1);
		y0 = HEIGHT - y0 - 1;
		y1 = HEIGHT - y1 - 1;
		break;
	}
	// Boundary check
	if ((y0 >= HEIGHT) && (y1 >= HEIGHT)) return;
	if ((x0 >= WIDTH) && (x1 >= WIDTH)) return;	
	if (x0 >= WIDTH) x0 = OLED_MW;
	if (y0 >= HEIGHT) y0 = OLED_MH;
	if (x1 >= WIDTH) x1 = OLED_MW;
	if (y1 >= HEIGHT) y1 = OLED_MH;
	
	uint8_t cmds[12];//container for commands
	
    if (x0 < x1) {
        if (y0 < y1) {
            if (reversal) reversal = reversalTool(false);
			cmds[0] = _CMD_DRAWLINE;
			cmds[1] = x0; cmds[2] = y0; cmds[3] = x1; cmds[4] = y1; cmds[5] = c1; cmds[6] = c2; cmds[7] = c3;
            writeCommands(cmds, 8);
        } else if (y0 > y1) {
            if (!reversal) reversal = reversalTool(true);
			cmds[0] = _CMD_DRAWLINE;
			cmds[1] = OLED_MW - x1; cmds[2] = y1; cmds[3] = OLED_MW - x0; cmds[4] = y0; cmds[5] = c1; cmds[6] = c2; cmds[7] = c3;
            writeCommands(cmds, 8);
        } else {  //  y0 == y1
            if (reversal) reversal = reversalTool(false);
			cmds[0] = _CMD_DRAWLINE;
			cmds[1] = x0; cmds[2] = y0; cmds[3] = x1; cmds[4] = y1; cmds[5] = c1; cmds[6] = c2; cmds[7] = c3;
            writeCommands(cmds, 8);
        }
		//end x0 < x1
    } else if (x0 > x1) {
        if (y0 < y1) {
            if (!reversal) reversal = reversalTool(true);
			cmds[0] = _CMD_DRAWLINE;
			cmds[1] = OLED_MW - x1; cmds[2] = y1; cmds[3] = OLED_MW - x0; cmds[4] = y0; cmds[5] = c1; cmds[6] = c2; cmds[7] = c3;
            writeCommands(cmds, 8);
        } else if (y0 > y1) {
            if (reversal) reversal = reversalTool(false);
			cmds[0] = _CMD_DRAWLINE;
			cmds[1] = x0; cmds[2] = y0; cmds[3] = x1; cmds[4] = y1; cmds[5] = c1; cmds[6] = c2; cmds[7] = c3;
            writeCommands(cmds, 8);
        } else {  //  y0 == y1
			setRegister(_CMD_SETREMAP,0x70);
			cmds[0] = _CMD_DRAWLINE;
			cmds[1] = OLED_MW - x1; cmds[2] = y1; cmds[3] = OLED_MW - x0; cmds[4] = y0; cmds[5] = c1; cmds[6] = c2; cmds[7] = c3;
			writeCommands(cmds, 8);
			setRegister(_CMD_SETREMAP,0x72);
        }
		// end x0 > x1
    } else {  //  x0 == x1
        if (y0 < y1) {
            if (reversal) reversal = reversalTool(false);
			cmds[0] = _CMD_DRAWLINE;
			cmds[1] = x0; cmds[2] = y0; cmds[3] = x1; cmds[4] = y1; cmds[5] = c1; cmds[6] = c2; cmds[7] = c3;
            writeCommands(cmds, 8);
        } else if (y0 > y1) {
			cmds[0] = _CMD_DRAWLINE;
			cmds[1] = x1; cmds[2] = y1; cmds[3] = x0; cmds[4] = y0; cmds[5] = c1; cmds[6] = c2; cmds[7] = c3;
            writeCommands(cmds, 8);
        } else {  //  y0 == y1
            if (reversal) reversal = reversalTool(false);
			cmds[0] = _CMD_DRAWLINE;
			cmds[1] = x0; cmds[2] = y0; cmds[3] = x1; cmds[4] = y1; cmds[5] = c1; cmds[6] = c2; cmds[7] = c3;
            writeCommands(cmds, 8);
        }
    }//end  x1 == x2
	delayMicroseconds(_DLY_LINE);
}

/*
Hardware accellerated pixel set by drawing a 1 pixel rectangle
*/
void OLED_SSD1331::drawPixel(int16_t x, int16_t y, uint16_t color){
    uint8_t c1 = (color & 0xf800) >> 10, c2 = (color & 0x07e0) >> 5, c3 = (color & 0x001f) << 1;
	if ((x < 0) || (x >= width()) || (y < 0) || (y >= height())) return;
	// check rotation, move pixel around if necessary
	switch (getRotation()) {
	case 1:
		swap(x, y);
		x = WIDTH - x - 1;
		break;
	case 2:
		x = WIDTH - x - 1;
		y = HEIGHT - y - 1;
		break;
	case 3:
		swap(x, y);
		y = HEIGHT - y - 1;
		break;
	}
    uint8_t cmd[] = {_CMD_DRAWRECT, (uint8_t)x, (uint8_t)y, (uint8_t)x, (uint8_t)y, c1, c2, c3,  c1, c2, c3};
    writeCommands(cmd, 11);
}

void OLED_SSD1331::pushColor(uint16_t color) {
  // setup for data
	uint8_t cmd[] = {(uint8_t)(color >> 8), (uint8_t)color};
	writeCommands(cmd, 2);
}

void OLED_SSD1331::clearScreen(int16_t color) {
	if (color == -1){
		uint8_t cmd[] = {_CMD_CLRWINDOW,0,0,(uint8_t)WIDTH,(uint8_t)HEIGHT};
		writeCommands(cmd, 5);
	} else {
		hdwre_drawRect(0,0,width(),height(),color,true);
	}
}

//not tested
void OLED_SSD1331::setBrightness(byte val){
	writeCommand(_CMD_MASTERCURRENT);
	if (val < 0x17) {
		writeData(val);
	} else {
		writeData(0x0F);
	}
}

/********************************* low level pin initialization */


OLED_SSD1331::OLED_SSD1331(uint8_t cs, uint8_t rs, uint8_t rst) : Adafruit_GFX(OLED_WIDTH, OLED_HEIGHT) {
    _cs = cs;
    _rs = rs;
    _rst = rst;
	_sid  = _sclk = 0;
}

OLED_SSD1331::OLED_SSD1331(uint8_t cs, uint8_t rs) : Adafruit_GFX(OLED_WIDTH, OLED_HEIGHT) {
    _cs = cs;
    _rs = rs;
    _rst = 0;
	_sid  = _sclk = 0;
}

//helper
void OLED_SSD1331::setRegister(const uint8_t reg,uint8_t val){
	uint8_t cmd[2] = {reg,val};
	writeCommands(cmd,2);
}

/********************************** low level pin interface */
#ifdef __AVR__

	inline void OLED_SSD1331::writebegin()
	{
	}

	inline void OLED_SSD1331::spiwrite(uint8_t c){
		SPDR = c;
		while(!(SPSR & _BV(SPIF)));
	}

	void OLED_SSD1331::writeCommand(uint8_t c){
		*rsport &= ~rspinmask;
		*csport &= ~cspinmask;
		spiwrite(c);
		*csport |= cspinmask;
	}

	void OLED_SSD1331::writeCommands(uint8_t *cmd, uint8_t length){
		*rsport &= ~rspinmask;
		*csport &= ~cspinmask;
		for (uint8_t i = 0; i < length; i++) {
			spiwrite(*cmd++);
		}
		*csport |= cspinmask;
	}
	
	void OLED_SSD1331::writeData(uint8_t c){
		*rsport |=  rspinmask;
		*csport &= ~cspinmask;
		spiwrite(c);
		*csport |= cspinmask;
	} 

	void OLED_SSD1331::setBitrate(uint32_t n){
		if (n >= 8000000) {
			SPI.setClockDivider(SPI_CLOCK_DIV2);
		} else if (n >= 4000000) {
			SPI.setClockDivider(SPI_CLOCK_DIV4);
		} else if (n >= 2000000) {
			SPI.setClockDivider(SPI_CLOCK_DIV8);
		} else {
			SPI.setClockDivider(SPI_CLOCK_DIV16);
		}
	}
#elif defined(__SAM3X8E__)

	inline void OLED_SSD1331::writebegin()
	{
	}
	
	inline void OLED_SSD1331::spiwrite(uint8_t c){
		SPI.transfer(c);
	}
	
	void OLED_SSD1331::writeCommand(uint8_t c){
		rsport->PIO_CODR |=  rspinmask;
		csport->PIO_CODR  |=  cspinmask;
		spiwrite(c);
		csport->PIO_SODR  |=  cspinmask;
	}
	
	void OLED_SSD1331::writeCommands(uint8_t *cmd, uint8_t length){
		rsport->PIO_CODR |=  rspinmask;
		csport->PIO_CODR  |=  cspinmask;
		for (uint8_t i = 0; i < length; i++) {
			spiwrite(*cmd++);
		}
		csport->PIO_SODR  |=  cspinmask;
	}
	
	void OLED_SSD1331::writeData(uint8_t c){
		rsport->PIO_SODR |=  rspinmask;
		csport->PIO_CODR  |=  cspinmask;
		spiwrite(c);
		csport->PIO_SODR  |=  cspinmask;
	} 
	
	
	void OLED_SSD1331::setBitrate(uint32_t n){
		uint32_t divider=1;
		while (divider < 255) {
			if (n >= 84000000 / divider) break;
			divider = divider - 1;
		}
		SPI.setClockDivider(divider);
	}
#elif defined(__MK20DX128__) || defined(__MK20DX256__)
	
	inline void OLED_SSD1331::writebegin()
	{
	}
	
	inline void OLED_SSD1331::spiwrite(uint8_t c){
		for (uint8_t bit = 0x80; bit; bit >>= 1) {
			*datapin = ((c & bit) ? 1 : 0);
			*clkpin = 1;
			*clkpin = 0;
		}
	}
	
	void OLED_SSD1331::writeCommand(uint8_t c){
		SPI0.PUSHR = c | (pcs_command << 16) | SPI_PUSHR_CTAS(0);
		while (((SPI0.SR) & (15 << 12)) > (3 << 12)) ; // wait if FIFO full
	}

	void OLED_SSD1331::writeCommands(uint8_t *cmd, uint8_t length){
		for (uint8_t i = 0; i < length; i++) {
			SPI0.PUSHR = *cmd++ | (pcs_command << 16) | SPI_PUSHR_CTAS(0);
			while (((SPI0.SR) & (15 << 12)) > (3 << 12)) ; // wait if FIFO full
		}
	}
	
	void OLED_SSD1331::writeData(uint8_t c){
		SPI0.PUSHR = c | (pcs_data << 16) | SPI_PUSHR_CTAS(0);
		while (((SPI0.SR) & (15 << 12)) > (3 << 12)) ; // wait if FIFO full
	}

	/*
	Helper:
	This function return true only if the choosed pin can be used for CS or RS
	*/
	static bool spi_pin_is_cs(uint8_t pin){
		if (pin == 2 || pin == 6 || pin == 9) return true;
		if (pin == 10 || pin == 15) return true;
		if (pin >= 20 && pin <= 23) return true;
		return false;
	}
	
	/*
	Helper:
	This function configure the correct pin
	*/
	static uint8_t spi_configure_cs_pin(uint8_t pin){
		switch (pin) {
			case 10: CORE_PIN10_CONFIG = PORT_PCR_MUX(2); return 0x01; // PTC4
			case 2:  CORE_PIN2_CONFIG  = PORT_PCR_MUX(2); return 0x01; // PTD0
			case 9:  CORE_PIN9_CONFIG  = PORT_PCR_MUX(2); return 0x02; // PTC3
			case 6:  CORE_PIN6_CONFIG  = PORT_PCR_MUX(2); return 0x02; // PTD4
			case 20: CORE_PIN20_CONFIG = PORT_PCR_MUX(2); return 0x04; // PTD5
			case 23: CORE_PIN23_CONFIG = PORT_PCR_MUX(2); return 0x04; // PTC2
			case 21: CORE_PIN21_CONFIG = PORT_PCR_MUX(2); return 0x08; // PTD6
			case 22: CORE_PIN22_CONFIG = PORT_PCR_MUX(2); return 0x08; // PTC1
			case 15: CORE_PIN15_CONFIG = PORT_PCR_MUX(2); return 0x10; // PTC0
		}
		return 0;
	}

	/*
	Helper:
	This function set the speed of the SPI interface
	*/
	void OLED_SSD1331::setBitrate(uint32_t n){
		if (n >= 24000000) {
			ctar = CTAR_24MHz;
		} else if (n >= 16000000) {
			ctar = CTAR_16MHz;
		} else if (n >= 12000000) {
			ctar = CTAR_12MHz;
		} else if (n >= 8000000) {
			ctar = CTAR_8MHz;
		} else if (n >= 6000000) {
			ctar = CTAR_6MHz;
		} else {
			ctar = CTAR_4MHz;
		}
		SIM_SCGC6 |= SIM_SCGC6_SPI0;
		SPI0.MCR = SPI_MCR_MDIS | SPI_MCR_HALT;
		SPI0.CTAR0 = ctar | SPI_CTAR_FMSZ(7);
		SPI0.CTAR1 = ctar | SPI_CTAR_FMSZ(15);
		SPI0.MCR = SPI_MCR_MSTR | SPI_MCR_PCSIS(0x1F) | SPI_MCR_CLR_TXF | SPI_MCR_CLR_RXF;
	}
#endif

/********************************** common helpers */

bool OLED_SSD1331::reversalTool(bool rev){
	if (rev == true){
		setRegister(_CMD_SETREMAP,0x72);
	} else {
		setRegister(_CMD_SETREMAP,0x70);
	}
	return rev;
}


bool OLED_SSD1331::fillTool(bool fillState){
	if (fillState == true){
		setRegister(_CMD_FILL,1);
	} else {
		setRegister(_CMD_FILL,0);
	}
	return fillState;
}

/********************************** special hardware functions */
/*
Hardware accellerated Draw Rect (filled or not)
*/
void OLED_SSD1331::hdwre_drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, bool filled){
    uint8_t c1 = (color & 0xF800) >> 10, c2 = (color & 0x07E0) >> 5, c3 = (color & 0x001F) << 1;
	//rotation??
	switch (getRotation()) {
	case 1:
		swap(x, y);
		swap(w, h);
		x = WIDTH - x - 1;
		break;
	case 2:
		x = WIDTH - x - 1;
		y = HEIGHT - y - 1;
		break;
	case 3:
		swap(x, y);
		swap(w, h);
		y = HEIGHT - y - 1;
		break;
	}
	// Bounds check
	if ((x >= WIDTH) || (y >= HEIGHT)) return;
	if (y+h > HEIGHT) h = HEIGHT - y;
	if (x+w > WIDTH) w = WIDTH - x;
	
	uint8_t cmds[11];
	
    //  to fill or not to?
    if (filled) {
        if (!filling) filling = fillTool(true);
    } else {
        if (filling) filling = fillTool(false);
    }

    if (reversal) reversal = reversalTool(false);
    //  draw/fill the rectangle
	cmds[0] = _CMD_DRAWRECT;
	cmds[1] = x; cmds[2] = y; cmds[3] = x + w - 1; cmds[4] = y + h - 1; cmds[5] = c1; cmds[6] = c2;
	cmds[7] = c3; cmds[8] = c1; cmds[9] = c2; cmds[10] = c3;
	writeCommands(cmds, 11);
	delayMicroseconds(_DLY_FILL);
}

/*
----------------------------------------- HI SPEED SPI
*/

/*
Initialize PIN, direction and stuff related to hardware on CPU
*/
void OLED_SSD1331::commonInit(){
#if defined(__AVR__) 
	pinMode(_rs, OUTPUT);
	pinMode(_cs, OUTPUT);
	csport    = portOutputRegister(digitalPinToPort(_cs));
	rsport    = portOutputRegister(digitalPinToPort(_rs));
	cspinmask = digitalPinToBitMask(_cs);
	rspinmask = digitalPinToBitMask(_rs);
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV4); // 4 MHz (half speed)
    //Due defaults to 4mHz (clock divider setting of 21)
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
	*csport &= ~cspinmask;
#elif defined(__SAM3X8E__) 
	pinMode(_rs, OUTPUT);
	pinMode(_cs, OUTPUT);
	csport    = digitalPinToPort(_cs);
	rsport    = digitalPinToPort(_rs);
	cspinmask = digitalPinToBitMask(_cs);
	rspinmask = digitalPinToBitMask(_rs);
    SPI.begin();
    SPI.setClockDivider(21); // 4 MHz
    //Due defaults to 4mHz (clock divider setting of 21), but we'll set it anyway 
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
	// toggle RST low to reset; CS low so it'll listen to us
	csport ->PIO_CODR  |=  cspinmask; // Set control bits to LOW (idle)
#elif defined(__MK20DX128__) || defined(__MK20DX256__)
	if (_sid == 0) _sid = 11;
	if (_sclk == 0) _sclk = 13;
	if (spi_pin_is_cs(_cs) && spi_pin_is_cs(_rs)
	 && (_sid == 7 || _sid == 11) && (_sclk == 13 || _sclk == 14)
	 && !(_cs ==  2 && _rs == 10) && !(_rs ==  2 && _cs == 10)
	 && !(_cs ==  6 && _rs ==  9) && !(_rs ==  6 && _cs ==  9)
	 && !(_cs == 20 && _rs == 23) && !(_rs == 20 && _cs == 23)
	 && !(_cs == 21 && _rs == 22) && !(_rs == 21 && _cs == 22)) {
		if (_sclk == 13) {
			CORE_PIN13_CONFIG = PORT_PCR_MUX(2) | PORT_PCR_DSE;
			SPCR.setSCK(13);
		} else {
			CORE_PIN14_CONFIG = PORT_PCR_MUX(2);
			SPCR.setSCK(14);
		}
		if (_sid == 11) {
			CORE_PIN11_CONFIG = PORT_PCR_MUX(2);
			SPCR.setMOSI(11);
		} else {
			CORE_PIN7_CONFIG = PORT_PCR_MUX(2);
			SPCR.setMOSI(7);
		}
		ctar = CTAR_12MHz;
		pcs_data = spi_configure_cs_pin(_cs);
		pcs_command = pcs_data | spi_configure_cs_pin(_rs);
		SIM_SCGC6 |= SIM_SCGC6_SPI0;
		SPI0.MCR = SPI_MCR_MDIS | SPI_MCR_HALT;
		SPI0.CTAR0 = ctar | SPI_CTAR_FMSZ(7);
		SPI0.CTAR1 = ctar | SPI_CTAR_FMSZ(15);
		SPI0.MCR = SPI_MCR_MSTR | SPI_MCR_PCSIS(0x1F) | SPI_MCR_CLR_TXF | SPI_MCR_CLR_RXF;
	} else {
		//error! cannot continue
		// TODO!  Escape code to stop all
	}	
#endif
	if (_rst) {
		pinMode(_rst, OUTPUT);
		digitalWrite(_rst, HIGH);
		delay(500);
		digitalWrite(_rst, LOW);
		delay(500);
		digitalWrite(_rst, HIGH);
		delay(500);
	}
}

/*
Here's the
*/
void OLED_SSD1331::chipInit() {
	//set SSD chip registers
	uint8_t cmd[33];
	writeCommand(_CMD_DISPLAYOFF);  	
	writeCommand(_CMD_SETREMAP); 	
#if defined SSD1331_COLORORDER_RGB
    writeCommand(0x72);				// RGB Color
#else
    writeCommand(0x76);				// BGR Color
#endif
	//writeCommand(0b01100010);
	setRegister(_CMD_FILL,0x01);
	setRegister(_CMD_STARTLINE,0x00);//default 0x00	
	setRegister(_CMD_DISPLAYOFFSET,0x01);//default 0x00
	setRegister(_CMD_PHASEPERIOD,0b10110001);
	setRegister(_CMD_SETMULTIPLEX,0x3F);
	setRegister(_CMD_SETMASTER,0x8E);
	setRegister(_CMD_POWERMODE,0x0B);
	setRegister(_CMD_PRECHARGE,0x31);//0x1F - 0x31
	setRegister(_CMD_CLOCKDIV,0xF0);
	setRegister(_CMD_PRECHARGEA,0x64);
	setRegister(_CMD_PRECHARGEB,0x78);
	setRegister(_CMD_PRECHARGEC,0x64);
	setRegister(_CMD_PRECHARGELEVEL,0x3A);//0x3A - 0x00
	setRegister(_CMD_VCOMH,0x3E);//0x3E - 0x3F
	setRegister(_CMD_MASTERCURRENT,0x06);//0x06 - 0x0F
	setRegister(_CMD_CONTRASTA,0x91);//0xEF - 0x91
	setRegister(_CMD_CONTRASTB,0x50);//0x11 - 0x50
	setRegister(_CMD_CONTRASTC,0x7D);//0x48 - 0x7D
	//setRegister(_CMD_VPACOLORLVL,0x40);
	//setRegister(_CMD_VPBCOLORLVL,0x40);
	//setRegister(_CMD_VPCCOLORLVL,0x40);
	cmd[0] = _CMD_GRAYSCALE;
	cmd[1] =  0x01; cmd[2] =  0x03; cmd[3] =  0x05; cmd[4] =  0x07; cmd[5] =  0x0A; cmd[6] =  0x0D; cmd[7] =  0x10; cmd[8] =  0x13;
	cmd[9] =  0x16; cmd[10] = 0x19; cmd[11] = 0x1C; cmd[12] = 0x20; cmd[13] = 0x24; cmd[14] = 0x28; cmd[15] = 0x2C; cmd[16] = 0x30;
	cmd[17] = 0x34; cmd[18] = 0x38; cmd[19] = 0x3C; cmd[20] = 0x40; cmd[21] = 0x44; cmd[22] = 0x48; cmd[23] = 0x4C; cmd[24] = 0x50;
	cmd[25] = 0x54; cmd[26] = 0x58; cmd[27] = 0x5C; cmd[28] = 0x60; cmd[29] = 0x64; cmd[30] = 0x68; cmd[31] = 0x6C; cmd[32] = 0x70;
	writeCommands(cmd, 33);
	writeCommand(_CMD_NORMALDISPLAY);
	writeCommand(_CMD_DISPLAYON);
	filling = true;
	reversal = false;
	clearScreen(0x0000);
}
