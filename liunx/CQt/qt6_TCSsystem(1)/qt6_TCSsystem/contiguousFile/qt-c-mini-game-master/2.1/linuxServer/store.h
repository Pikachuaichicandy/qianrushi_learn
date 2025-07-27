#include<map>
#include<vector>
using namespace std;

map<int,vector<int>> m_roomIp;

map<int,int> m_isStartGame;    //游戏是否开始人数统计

//这里考虑使用一个数据保存全部套接字，然后每隔一段时间遍历，如果玩家下线则在对应游戏房间删除这个