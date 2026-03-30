#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdint.h>

#define CMD_TRIG 3

int main(int argc, char *argv[])
{
    int fd, retvalue;
    char *filename;
    uint64_t time_ns;     // 纳秒时间
    float distance_cm;    // 厘米距离
    
    if(argc != 2){
        printf("Usage: %s <device>\n", argv[0]);
        printf("Example: %s /dev/sr04\n", argv[0]);
        return -1;
    }

    filename = argv[1];

    /* 打开设备 */
    fd = open(filename, O_RDWR | O_NONBLOCK);  // 使用非阻塞模式
    if(fd < 0){
        printf("file %s open failed!\r\n", argv[1]);
        return -1;
    }

    printf("SR04 Ultrasonic Sensor Test Program\n");
    printf("Press Ctrl+C to exit\n\n");

    while(1){
        printf("Starting measurement cycle...\n");
        
        // 清空缓冲区（非阻塞模式）
        int cleared_count = 0;
        int ret;
        do {
            ret = read(fd, &time_ns, 8);
            if(ret == 8) {
                if(cleared_count == 0) {
                    printf("Clearing old data: %llu ns\n", (unsigned long long)time_ns);
                }
                cleared_count++;
            }
        } while(ret == 8);  // 继续读取直到缓冲区为空
        
        if(cleared_count > 0) {
            printf("Cleared %d data points from buffer\n", cleared_count);
        } else {
            printf("Buffer is empty\n");
        }
        
        // 发送触发信号
        retvalue = ioctl(fd, CMD_TRIG);
        if(retvalue < 0){
            printf("Trigger failed!\n");
            break;
        }
        
        // 等待测量完成（使用更短的等待时间）
        usleep(10000); // 等待10ms，足够大多数测量
        
        // 尝试读取测量时间（纳秒）
        int attempts = 0;
        do {
            retvalue = read(fd, &time_ns, 8);
            if(retvalue == 8){
                // 计算距离：声速340m/s = 34000cm/s
                // 时间单位转换为秒：time_ns / 1e9
                // 距离 = 速度 × 时间 / 2（来回距离）
                distance_cm = (time_ns * 34000.0) / (2.0 * 1000000000.0);
                
                // 统一有效范围：与驱动程序保持一致
                // 最小有效时间：1000 ns (1μs) - 过滤噪声
                // 最大有效时间：60000000 ns (60ms) - 对应10米距离
                if(time_ns > 1000 && time_ns < 60000000) { 
                    printf("Time: %llu ns, Distance: %.2f cm\n", 
                           (unsigned long long)time_ns, distance_cm);
                    break;
                } else {
                    printf("Invalid time: %llu ns (%.2f cm)\n", 
                           (unsigned long long)time_ns, distance_cm);
                    // 继续尝试读取
                    continue;
                }
            }
            else if(retvalue == 0){
                // 没有数据，等待一段时间再试
                usleep(1000); // 等待1ms
                attempts++;
            }
            else if(retvalue < 0){
                printf("Read failed!\n");
                break;
            }
            else {
                printf("Read incomplete: got %d bytes\n", retvalue);
                break;
            }
        } while(attempts < 50); // 最多尝试50次（约50ms）
        
        if(attempts >= 50) {
            printf("Timeout: No valid data received\n");
        }
        
        // 等待100ms再下一次测量
        usleep(100000);
    }

    close(fd);
    return 0;
}