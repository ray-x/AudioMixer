#ifndef __DSP_H__
#define __DSP_H__

unsigned char spi_dsp_read(unsigned char *buf, unsigned char length);
unsigned char spi_dsp_write(unsigned char *buf, unsigned char length);
void dsp_init();
void dsp_polling();

#endif
