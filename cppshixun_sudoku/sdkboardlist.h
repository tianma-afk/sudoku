#ifndef SDKBOARDLIST_H
#define SDKBOARDLIST_H
#include<QDataStream>
class SDKBoardList{
public:
    char data[3][9][9];
    SDKBoardList();
    SDKBoardList(char otherData[3][9][9]);
    friend QDataStream&operator<<(QDataStream&out,const SDKBoardList&list);
    friend QDataStream&operator>>(QDataStream&in,SDKBoardList&list);
};
#endif // SDKBOARDLIST_H
