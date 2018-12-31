/*
 * gpio-utils.h
 *
 *  Created on: Oct 30, 2017
 *      Author: Oguzhan Armutcu
 */

#ifndef SRC_GPIO_UTILS_H_
#define SRC_GPIO_UTILS_H_



#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 64
#define NIRQ_GPIO (4)
int gpio_export(unsigned int gpio);
int gpio_unexport(unsigned int gpio);
int gpio_set_dir(unsigned int gpio, const char* dir);
int gpio_set_value(unsigned int gpio, unsigned int value);
int gpio_get_value(unsigned int gpio, unsigned int *value);
int gpio_set_edge(unsigned int gpio, const char *edge);
int gpio_fd_open(unsigned int gpio, unsigned int dir);
int gpio_fd_close(int fd);
void _epoc(void);
unsigned long GetTickCount(void);

#endif /* SRC_GPIO_UTILS_H_ */
