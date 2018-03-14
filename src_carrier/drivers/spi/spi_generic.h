//
// Created by atmelfan on 2017-10-28.
//

#ifndef SRC_SPI_GENERIC_H
#define SRC_SPI_GENERIC_H

typedef enum {
    /**
     * No error
     */
        SPI_NO_ERR = 0,

    /**
     * Hardware error
     */
        SPI_HW_ERR,
    /**
     * Unknown/device specific error
     */
        SPI_OTHER
} spi_status;

/**
 * Device specific, CS pin identifier
 */
typedef uint8_t spi_cs;

/**
 * Device specific, implemented in relevant device .c file
 */
struct spi_bus_struct;
typedef spi_bus_struct spi_bus;


/*CPOL*/
#define SPI_PROP_CPOL_LOW   0x00
#define SPI_PROP_CPOL_HIGH  0x01
/*CPHA*/
#define SPI_PROP_CPHA_TRAIL 0x00
#define SPI_PROP_CPHA_LEAD  0x01


typedef struct {
    uint8_t cpol : 1;/*Clock polarity, 1bit*/
    uint8_t cpha : 1;/*Data polarity, 1bit*/

    uint8_t extended;/*Device specific flags*/
} spi_bus_properties;


/**
 * Device structure, contains chip select identifier and bus handle
 */
typedef struct {
    spi_bus* bus;//Device bus
    spi_cs cs;//Chip select of device
    spi_bus_properties props;/*SPI bus properties of specific device. If NULL, defaults are used*/
} spi_device;

/**
 * Initializes spi system
 */
void spi_init_all();

/**
 * Initializes a specific bus. The spi_init_all() function must be called first.
 * @param bus
 * @return
 */
spi_status spi_init_bus(spi_bus* bus, const spi_bus_properties defprops);

spi_status spi_transfer(spi_device* dev, uint8_t*)


#endif
