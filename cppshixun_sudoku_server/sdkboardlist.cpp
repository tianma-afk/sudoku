#include "sdkboardlist.h"

SDKBoardList::SDKBoardList() {
    for(int i=0;i<3;i++){
        for(int j=0;j<9;j++){
            for(int k=0;k<9;k++){
                data[i][j][k]=0;
            }
        }
    }
}

SDKBoardList::SDKBoardList(char otherData[3][9][9])
{
    for(int i=0;i<3;i++){
        for(int j=0;j<9;j++){
            for(int k=0;k<9;k++){
                data[i][j][k]=otherData[i][j][k];
            }
        }
    }
}

QDataStream&operator<<(QDataStream&out,const SDKBoardList&list){
    for(int i=0;i<3;i++){
        for(int j=0;j<9;j++){
            for(int k=0;k<9;k++){
                out<<list.data[i][j][k];
            }
        }
    }
    return out;
}

QDataStream&operator>>(QDataStream&in,SDKBoardList&list){
    for(int i=0;i<3;i++){
        for(int j=0;j<9;j++){
            for(int k=0;k<9;k++){
                in>>list.data[i][j][k];
            }
        }
    }
    return in;
}
