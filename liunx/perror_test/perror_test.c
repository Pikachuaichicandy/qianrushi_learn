#include <stdio.h>
int main(int argc, char const *argv[])
{
    FILE *fp = fopen("test.txt", "r");
    if (fp == NULL) {
        perror("打开文件失败");
        return 1; // 返回非零值表示程序异常结束
    }
    // 若文件打开成功，可在此添加文件操作代码
    fclose(fp); // 关闭文件
    return 0;
}