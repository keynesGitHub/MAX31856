

#include "NUC100Series.h"

void SPI_WriteByte(uint8_t);
void SPI_WriteBlock(uint8_t * , uint16_t);

uint8_t SPI_ReadByte(void);
void SPI_ReadBlock(uint8_t * , uint16_t);

void SPI_Master_Init(void);
