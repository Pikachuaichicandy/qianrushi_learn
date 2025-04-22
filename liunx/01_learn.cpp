#include <iostream>
#include <vector>
#include <limits>
#include <cctype>
#include <conio.h>
using namespace std;

// 父类，封装通用属性
class Character {
protected:
    int hp;
    int atk;
public:
    Character(int hp, int atk) : hp(hp), atk(atk) {}
    virtual ~Character() {}
    // 虚函数，用于攻击
    virtual void attackTarget(Character* target) = 0;
    int getHp() const { return hp; }
    int getAtk() const { return atk; }
    void takeDamage(int damage) {
        hp -= damage;
        if (hp < 0) hp = 0;
    }
    bool isAlive() const { return hp > 0; }
};

// 英雄类，继承自 Character
class Hero : public Character {
private:
    int mp;
    int mpRegen;
    vector<int> skillMpCost;
    vector<int> skillDamage;
    vector<int> skillCooldown;
    vector<int> currentCooldown;
public:
    Hero(int hp, int mp, int atk) : Character(hp, atk), mp(mp), mpRegen(10) {
        // 初始化技能，蓝耗、伤害和冷却时间
        skillMpCost = {30, 40, 50, 60};
        // 设置技能伤害值
        skillDamage = {15, 20, 30, 30}; 
        skillCooldown = {1, 3, 4, 5};
        currentCooldown.resize(4, 0);
    }

    /**
     * @brief 英雄进行攻击操作的函数，重写父类的纯虚函数
     * 
     * @param target 攻击目标，指向 Character 类对象的指针
     */
    void attackTarget(Character* target) override {
        // 提示用户选择攻击方式
        cout << "请选择攻击方式：" << endl;
        // 显示鼠标右键普通攻击选项
        cout << "R. 鼠标右键普通攻击" << endl;
        // 显示基础技能选项，包含蓝耗和冷却时间信息
        cout << "E. 基础技能（蓝耗：" << skillMpCost[0] << "，冷却：" << currentCooldown[0] << "）" << endl;
        // 显示二技能选项，包含蓝耗和冷却时间信息
        cout << "W. 二技能（蓝耗：" << skillMpCost[1] << "，冷却：" << currentCooldown[1] << "）" << endl;
        // 修改为显示 V 键对应大招选项，包含蓝耗和冷却时间信息
        cout << "V. 大招（蓝耗：" << skillMpCost[3] << "，冷却：" << currentCooldown[3] << "）" << endl;
        // 显示结束游戏选项
        cout << "q. 结束游戏" << endl;
        // 显示查看双方信息选项
        cout << "i. 查看双方信息" << endl;

        // 用于存储用户输入的字符
        char choice = _getch(); // 使用 _getch() 获取输入，无需按回车键
        // 将输入字符转换为大写
        choice = toupper(choice);

        // 修改为处理大写 V，将其作为大招选择
        if (choice == 'V') {
            choice = '4';
        } 
        // 处理小写 q，结束游戏
        else if (choice == 'q') {
            exit(0);
        } 
        // 处理 i 输入，查看双方信息后重新选择攻击方式
        else if (choice == 'I') {
            showInfo(target);
            attackTarget(target);
            return;
        } 
        // 处理 W 输入，将其转换为二技能选择
        else if (choice == 'W') {
            choice = '3';
        } 
        // 处理 E 输入，将其转换为基础技能选择
        else if (choice == 'E') {
            choice = '2';
        } 
        // 处理 R 输入，将其转换为普通攻击选择
        else if (choice == 'R') {
            choice = '1';
        }

        // 初始化造成的伤害为 0
        int damage = 0;
        // 处理普通攻击
        if (choice == '1') {
            // 伤害值为英雄的攻击力
            damage = atk;
            cout << "英雄进行鼠标右键普通攻击，造成 " << damage << " 点伤害。" << endl;
        } 
        // 处理技能攻击
        else if (choice >= '2' && choice <= '4') {
            // 计算技能索引
            int skillIndex = choice - '2';
            // 检查技能是否处于冷却中
            if (currentCooldown[skillIndex] > 0) {
                cout << "技能处于冷却中，剩余冷却回合：" << currentCooldown[skillIndex] << endl;
                return;
            }
            // 检查蓝量是否足够释放技能
            if (mp < skillMpCost[skillIndex]) {
                cout << "蓝量不足，无法释放技能。" << endl;
                return;
            }
            // 消耗相应的蓝量
            mp -= skillMpCost[skillIndex];
            // 获取技能造成的伤害
            damage = skillDamage[skillIndex];
            cout << "英雄释放技能" << (skillIndex + 1) << "，造成 " << damage << " 点伤害。" << endl;
            // 重置技能冷却时间
            currentCooldown[skillIndex] = skillCooldown[skillIndex];
        } 
        // 处理无效输入
        else {
            cout << "无效输入，请重新选择。" << endl;
            // 清除输入流错误标志
            cin.clear();
            // 忽略输入流中剩余的字符
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            // 重新选择攻击方式
            attackTarget(target);
            return;
        }

        // 让目标受到相应的伤害
        target->takeDamage(damage);
    }

    void showInfo(const Character* target) const {
        cout << "英雄信息：" << endl;
        cout << "血量：" << hp << "，蓝量：" << mp << "，攻击力：" << atk << "，蓝量回复：" << mpRegen << endl;
        cout << "怪物信息：" << endl;
        cout << "血量：" << target->getHp() << "，攻击力：" << target->getAtk() << endl;
    }

    void regenMp(int round) {
        mp += mpRegen + round / 2;
        cout << "英雄回复 " << (mpRegen + round / 2) << " 点蓝量。" << endl;
    }

    void updateCooldown() {
        for (int& cd : currentCooldown) {
            if (cd > 0) cd--;
        }
    }
};

// 怪物类，继承自 Character
class Monster : public Character {
public:
    Monster(int hp, int atk) : Character(hp, atk) {}

    void attackTarget(Character* target) override {
        int damage = atk;
        cout << "怪物进行普通攻击，造成 " << damage << " 点伤害。" << endl;
        target->takeDamage(damage);
    }
};

int main() {
    // 设置默认值
    int heroHp = 100, heroMp = 100, heroAtk = 10, monsterHp = 100, monsterAtk = 15;

    Hero hero(heroHp, heroMp, heroAtk);
    Monster monster(monsterHp, monsterAtk);

    int round = 1;
    while (hero.isAlive() && monster.isAlive()) {
        cout << "\n第 " << round << " 回合开始" << endl;

        // 英雄攻击
        hero.attackTarget(&monster);
        // 显示双方血量
        hero.showInfo(&monster);
        if (!monster.isAlive()) break;

        // 怪物攻击
        monster.attackTarget(&hero);
        // 显示双方血量
        hero.showInfo(&monster);
        if (!hero.isAlive()) break;

        // 英雄回复蓝量
        hero.regenMp(round);
        // 更新技能冷却
        hero.updateCooldown();

        round++;
    }

    if (hero.isAlive()) {
        cout << "英雄获胜！" << endl;
    } else {
        cout << "怪物获胜！" << endl;
    }

    return 0;
}
