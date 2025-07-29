# 编译器定义
CC := gcc

# 编译选项，开启所有常见警告并包含调试信息
CFLAGS := -Wall -Wextra -g

# 链接选项，可按需添加库链接，如 -lpthread
LDFLAGS := -lpthread

# 目标文件列表，可按需添加其他目标
TARGETS := server_2 client_2

# 所有目标
all: $(TARGETS)

# 通用规则，用于编译可执行文件，自动处理头文件依赖
%: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

# 清理规则，删除生成的可执行文件
clean:
	rm -f $(TARGETS)

# 伪目标声明，确保 all 和 clean 总是被执行
.PHONY: all clean