/**
 * SG90舵机测试程序
 * 文件名: sg90_test.c
 * 编译: gcc -o sg90_test sg90_test.c
 * 运行: ./sg90_test
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

/* 全局变量 */
static int fd = -1;                     /* 设备文件描述符 */
static volatile int keep_running = 1;   /* 程序运行标志 */

/**
 * 信号处理函数 - 用于优雅退出
 */
void signal_handler(int sig)
{
    printf("\nReceived signal %d, shutting down...\n", sig);
    keep_running = 0;
}

/**
 * 打印使用说明
 */
void print_usage(const char *program_name)
{
    printf("SG90 Servo Motor Test Program\n");
    printf("Usage: %s [options]\n", program_name);
    printf("Options:\n");
    printf("  -h, --help     Show this help message\n");
    printf("  -a angle       Set servo to specific angle (0-180)\n");
    printf("  -s             Sweep mode (automatically sweep through angles)\n");
    printf("  -d delay       Delay between steps in milliseconds (default: 500)\n");
    printf("\nExamples:\n");
    printf("  %s -a 90        # Set servo to 90 degrees\n", program_name);
    printf("  %s -s -d 100    # Sweep mode with 100ms delay\n", program_name);
    printf("  %s              # Interactive mode\n", program_name);
}

/**
 * 设置舵机角度
 * @angle: 目标角度 (0-180)
 * @return: 0成功, -1失败
 */
int set_servo_angle(int angle)
{
    int ret;
    
    /* 角度范围检查 */
    if (angle < 0 || angle > 180) {
        printf("Error: Angle must be between 0 and 180 degrees\n");
        return -1;
    }
    
    /* 写入角度值到设备 */
    ret = write(fd, &angle, sizeof(angle));
    if (ret != sizeof(angle)) {
        printf("Error: Failed to set angle %d, return value: %d\n", angle, ret);
        perror("write");
        return -1;
    }
    
    printf("Successfully set servo angle to %d degrees\n", angle);
    return 0;
}

/**
 * 读取当前舵机角度
 * @return: 当前角度, -1表示失败
 */
int read_servo_angle(void)
{
    int angle;
    int ret;
    
    ret = read(fd, &angle, sizeof(angle));
    if (ret != sizeof(angle)) {
        printf("Error: Failed to read current angle\n");
        perror("read");
        return -1;
    }
    
    return angle;
}

/**
 * 舵机扫描模式 - 自动在0-180度之间来回扫描
 * @delay_ms: 每个角度之间的延迟(毫秒)
 */
void servo_sweep_mode(int delay_ms)
{
    int angles[] = {0, 45, 90, 135, 180, 135, 90, 45};
    int num_angles = sizeof(angles) / sizeof(angles[0]);
    int i;
    
    printf("Starting servo sweep mode (Ctrl+C to stop)\n");
    printf("Delay between angles: %d ms\n", delay_ms);
    
    while (keep_running) {
        for (i = 0; i < num_angles && keep_running; i++) {
            printf("Setting angle: %d degrees\n", angles[i]);
            
            if (set_servo_angle(angles[i]) != 0) {
                return;
            }
            
            /* 读取并验证当前角度 */
            int current_angle = read_servo_angle();
            if (current_angle >= 0) {
                printf("Verified current angle: %d degrees\n", current_angle);
            }
            
            usleep(delay_ms * 1000);  /* 转换为微秒 */
        }
    }
}

/**
 * 交互式模式 - 用户手动输入角度
 */
void interactive_mode(void)
{
    char input[32];
    int angle;
    
    printf("Interactive Mode - Enter angles (0-180) or 'q' to quit\n");
    printf("Current device: /dev/sg90\n");
    
    while (keep_running) {
        printf("\nEnter angle (0-180) or 'q' to quit: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        /* 去除换行符 */
        input[strcspn(input, "\n")] = 0;
        
        /* 检查退出命令 */
        if (strcmp(input, "q") == 0 || strcmp(input, "quit") == 0) {
            break;
        }
        
        /* 解析角度值 */
        if (sscanf(input, "%d", &angle) == 1) {
            set_servo_angle(angle);
        } else {
            printf("Invalid input. Please enter a number between 0-180\n");
        }
    }
}

/**
 * 测试基本功能
 */
void basic_function_test(void)
{
    int test_angles[] = {0, 45, 90, 135, 180, 90};
    int num_tests = sizeof(test_angles) / sizeof(test_angles[0]);
    int i;
    
    printf("Starting basic function test...\n");
    
    for (i = 0; i < num_tests; i++) {
        printf("Test %d/%d: Setting angle to %d degrees\n", 
               i + 1, num_tests, test_angles[i]);
        
        if (set_servo_angle(test_angles[i]) != 0) {
            printf("Test failed at angle %d\n", test_angles[i]);
            return;
        }
        
        /* 验证角度设置 */
        int current_angle = read_servo_angle();
        if (current_angle >= 0) {
            printf("  -> Verified: %d degrees\n", current_angle);
        }
        
        sleep(1);  /* 等待1秒 */
    }
    
    printf("Basic function test completed successfully!\n");
}

/**
 * 主函数
 */
int main(int argc, char *argv[])
{
    int angle = -1;
    int sweep_mode = 0;
    int delay_ms = 500;
    int i;
    
    /* 注册信号处理 */
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    /* 解析命令行参数 */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
            angle = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-s") == 0) {
            sweep_mode = 1;
        } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            delay_ms = atoi(argv[++i]);
        }
    }
    
    /* 打开设备文件 */
    fd = open("/dev/sg90", O_RDWR);
    if (fd < 0) {
        printf("Error: Cannot open device /dev/sg90\n");
        perror("open");
        printf("Please make sure:\n");
        printf("  1. SG90 driver is loaded (insmod sg90.ko)\n");
        printf("  2. Device node exists (ls -l /dev/sg90)\n");
        printf("  3. You have permission to access the device\n");
        return -1;
    }
    
    printf("SG90 Servo Motor Test Program\n");
    printf("Device opened successfully: /dev/sg90\n");
    
    /* 根据命令行参数选择模式 */
    if (angle >= 0) {
        /* 设置特定角度模式 */
        set_servo_angle(angle);
    } else if (sweep_mode) {
        /* 扫描模式 */
        servo_sweep_mode(delay_ms);
    } else if (argc == 1) {
        /* 无参数: 先运行基本测试，然后进入交互模式 */
        basic_function_test();
        interactive_mode();
    }
    
    /* 关闭设备 */
    close(fd);
    printf("Test program exited gracefully\n");
    
    return 0;
}



