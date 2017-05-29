
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <string.h>


#define ERROR    (-1)
#define OK       (0)

/* struct spi_ioc_transfer  如果只想进行一个方向的传输，把另一个方向的缓冲置为0就可以了 
应用程序根据从设备的具体协议填写cmd/reg*/
int SPI_WriteBytes_LSB(int fd, unsigned char cmd, unsigned char reg, unsigned char *data, unsigned char length)
{ 
	int ret;
	struct spi_ioc_transfer xfer[3];

	memset(xfer, 0, 3*sizeof(struct spi_ioc_transfer));

	xfer[0].tx_buf = (unsigned long)(&cmd);
	xfer[0].len = 1;

	xfer[1].tx_buf = (unsigned long)(&reg);
	xfer[1].len = 1;

	xfer[2].tx_buf = (unsigned long)data;
	xfer[2].len = length;

	ret = ioctl(fd, SPI_IOC_MESSAGE(3), &xfer);

	if (ret < 1)
	perror("can't xfer spi message");
	
	return OK;  
}

int SPI_ReadBytes_LSB(int fd, unsigned char cmd, unsigned char reg, unsigned char *data, int length)
{ 
	int ret,i;
	struct spi_ioc_transfer xfer[3];

	memset(xfer, 0, 3*sizeof(struct spi_ioc_transfer));

	xfer[0].tx_buf = (unsigned long)(&cmd);
	xfer[0].len = 1;

	xfer[1].tx_buf = (unsigned long)(&reg);
	xfer[1].len = 1;

	xfer[2].rx_buf = (unsigned long)data;
	xfer[2].len = length;

	ret = ioctl(fd, SPI_IOC_MESSAGE(3), &xfer);

	if (ret < 1)
	perror("can't xfer spi message");

	for(i = length; i>0; i--)
	{
		printf(" rxbuf[%d] = 0x %02x \r\n", (i-1), data[i-1]);
	}

	return OK;  
}

