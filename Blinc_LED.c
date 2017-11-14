
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#define BLOCK_SIZE 4 * 1024
#define PERIPHERAL_BASE 0x3F000000
#define GPIO_BASE       PERIPHERAL_BASE + 0x00200000

typedef struct {
        unsigned long gpio_base;
        int           memory_fd;
        void          *map;
        volatile unsigned int *addr;
}rpi_gpio;

int MapGPIO(rpi_gpio *gpio);
void UnmapGPIO(rpi_gpio *gpio);
void BlinkLED(rpi_gpio *gpio);


int MapGPIO(rpi_gpio *gpio) {
        gpio->memory_fd = open("/dev/mem" , O_RDWR|O_SYNC);

        if(gpio->memory_fd < 0) {
                perror("Failed to open /dev/mem, try change permission.");
        return 1;
        }

        gpio->map = mmap(
                NULL,
                BLOCK_SIZE,
                PROT_READ|PROT_WRITE,
                MAP_SHARED,
                gpio->memory_fd,
                gpio->gpio_base
        );

if(gpio->map == MAP_FAILED) {
    perror("mmap");
    return 1;
  }

  gpio->addr = (volatile unsigned int *)gpio->map;
  return 0;
}

void UnmapGPIO(rpi_gpio *gpio) {
  munmap(gpio->map, BLOCK_SIZE);
  close(gpio->memory_fd);
}

void BlinkLED(rpi_gpio *gpio) {
  int n = 5;

  *(gpio->addr + 2)  = 0x00000200;

  while(n -= 1) {
    *(gpio->addr + 7)  = 0x00800000;
    usleep(500 * 1000);
    *(gpio->addr + 10) = 0x00800000;
    usleep(500 * 1000);
  }
  

int main() {
  rpi_gpio gpio = {GPIO_BASE};
  int map_status;

  map_status = MapGPIO(&gpio);
  if(map_status) {
    printf("Failed to blink LED.\n");
    return map_status;
  }

  BlinkLED(&gpio);
  UnmapGPIO(&gpio);
  return 0;
}
  