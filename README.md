# ST7567-driver
A driver that implements functions to write characters and strings on monochromatic LCD displays with the controller ST7567.
author: felipeghleite (email: felipeghleite@gmail.com)

The A0 pin is connected to GPIO_DEV0 PIN16. This pin is used to select if the data sent on the bus is either data or commands.

The user need to implement the SPI functions and replace the following spi functions used on the code?
mq55_spi_transmit(MQ55_SPI_DEV1, &temp, 1);
mq55_spi_ss0_enable(MQ55_SPI_DEV1);
mq55_spi_ss0_disable(MQ55_SPI_DEV1);
