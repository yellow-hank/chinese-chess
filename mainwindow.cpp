#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    putpic();
    initialboard();
    gameover=true;
    setupmode=0;



    ui->deletebutton->hide();
    ui->deleteopen->hide();
    ui->setupopen->hide();
    pressLabel = ui->label;
    pressLabel->setText(tr(""));
    pressLabel->setAlignment(Qt::AlignCenter);
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(pressLabel);
    setMouseTracking(true);
    connect(ui->startbutton,SIGNAL(clicked()),this,SLOT(start_slot()));
    connect(ui->setupbutton,SIGNAL(clicked()),this,SLOT(setup_slot()));
    connect(ui->deletebutton,SIGNAL(clicked()),this,SLOT(delete_slot()));


}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);

    QSize picSize(800,600);
    QPixmap scaledPixmap = QPixmap(":/picture/background.png").scaled(picSize, Qt::KeepAspectRatio);
    painter.drawPixmap(QRect(100,50,555,620),scaledPixmap);



    int distance_x = board_distance_x;
    int distance_y = board_distance_y;
    for(int i=0;i<9;i++){
        for(int j=0;j<10;j++){
            if(chessboard[i][j]<14 && chessboard[i][j]>=0){
                painter.drawImage(QRect(110+i*distance_x,50+j*distance_y,50,50),chess_pic.at(chessboard[i][j]));
            }
            if(move_available[i][j] == 15){
                painter.drawImage(QRect(110+i*distance_x,50+j*distance_y,50,50),chess_pic.at(15));
            }


        }
    }
    painter.drawImage(QRect(110+choose_x*distance_x,50+choose_y*distance_y,50,50),chess_pic.at(14));
}
void MainWindow::putpic(){
    chess_pic.push_back(QImage(":/picture/rook_black.png"));
    chess_pic.push_back(QImage(":/picture/knight_black.png"));
    chess_pic.push_back(QImage(":/picture/ele_black.png"));
    chess_pic.push_back(QImage(":/picture/ad_black.png"));
    chess_pic.push_back(QImage(":/picture/king_black.png"));
    chess_pic.push_back(QImage(":/picture/can_black.png"));
    chess_pic.push_back(QImage(":/picture/pawn_black.png"));
    chess_pic.push_back(QImage(":/picture/rook_red.png"));
    chess_pic.push_back(QImage(":/picture/knight_red.png"));
    chess_pic.push_back(QImage(":/picture/ele_red.png"));
    chess_pic.push_back(QImage(":/picture/ad_red.png"));
    chess_pic.push_back(QImage(":/picture/king_red.png"));
    chess_pic.push_back(QImage(":/picture/can_red.png"));
    chess_pic.push_back(QImage(":/picture/pawn_red.png"));
    chess_pic.push_back(QImage(":/picture/pick.png"));
    chess_pic.push_back(QImage(":/picture/available.png"));
}
void MainWindow::initialboard(){
    for(int i=0;i<9;i++){
        for(int j=0;j<10;j++){
            chessboard[i][j]=-1;
        }
    }
    //red
    chessboard[0][0]=7;
    chessboard[1][0]=8;
    chessboard[2][0]=9;
    chessboard[3][0]=10;
    chessboard[4][0]=11;
    chessboard[5][0]=10;
    chessboard[6][0]=9;
    chessboard[7][0]=8;
    chessboard[8][0]=7;
    chessboard[1][2]=12;
    chessboard[7][2]=12;
    for(int i=0;i<5;i++){
        chessboard[2*i][3]=13;
    }
    //black
    chessboard[0][9]=0;
    chessboard[1][9]=1;
    chessboard[2][9]=2;
    chessboard[3][9]=3;
    chessboard[4][9]=4;
    chessboard[5][9]=3;
    chessboard[6][9]=2;
    chessboard[7][9]=1;
    chessboard[8][9]=0;
    chessboard[1][7]=5;
    chessboard[7][7]=5;
    for(int i=0;i<5;i++){
        chessboard[2*i][6]=6;
    }

}

void MainWindow::mousePressEvent(QMouseEvent *event){

    if(100<event->x() && event->x()<655 && event->y()>50 && event->y()< 670 && !gameover){
        pressLabel->setText(QString(tr("點擊處：(%1, %2)")).arg(QString::number((event->x()-110)/63), QString::number((event->y()-50)/61)));
        int current_x=(event->x()-110)/63;
        int current_y=(event->y()-50)/61;
        if (initial_start ==0){
            choose_x=current_x;
            choose_y=current_y;
            initial_start++;
        }
        if(deletemode == 1){
            chessboard[current_x][current_y]=-1;   
        }if(setupmode%2 == 1){
            if(chessboard[current_x][current_y] == -1){
                attackchess(choose_x,choose_y,chessboard[choose_x][choose_y],current_x,current_y,chessboard[current_x][current_y]);
            }
        }else if(move_available[current_x][current_y] == 15){
            //attack
            attackchess(choose_x,choose_y,chessboard[choose_x][choose_y],current_x,current_y,chessboard[current_x][current_y]);
        }else if(chessboard[current_x][current_y] == -1){
            //recover
            copychessboard();
            return;
        }else if(chessboard[current_x][current_y]<7 && chessboard[current_x][current_y] >-1 && step_count%2 ==1){
            //black
            copychessboard();
            available_move(current_x,current_y,chessboard[current_x][current_y]);

        }else if(chessboard[current_x][current_y]<14 && chessboard[current_x][current_y] >6 && step_count%2 ==0){
            //red
            copychessboard();
            available_move(current_x,current_y,chessboard[current_x][current_y]);
        }else{
            copychessboard();
            return;
        }

        choose_x = (event->x()-110)/63;
        choose_y = (event->y()-50)/61;


        this->repaint();

    }


}
void MainWindow::start_slot(){
    step_count=0;
    delete_count=0;
    deletemode=0;
    setupmode=0;
    ui->setupopen->hide();
    ui->deleteopen->hide();
    ui->setupbutton->hide();
    ui->deletebutton->hide();
    copychessboard();
    gameover=false;
    initial_start=0;
    gamewin();

}
void MainWindow::setup_slot(){
    ui->deletebutton->show();
    setupmode++;
    initial_start=0;
    if(setupmode%2==0){
        ui->setupopen->hide();
    }else{
        ui->setupopen->show();
    }
    delete_count=0;
    gameover=false;

}
void MainWindow::delete_slot(){
    delete_count++;
    deletemode=delete_count%2;
    if(deletemode ==1){
        ui->deleteopen->show();
    }else {
        ui->deleteopen->hide();
    }
}

void MainWindow::copychessboard(){
    for(int i=0;i<9;i++){
        for(int j=0;j<10;j++){
            move_available[i][j]=chessboard[i][j];
        }
    }
}

//rules
void MainWindow::available_move(int x,int y,int chessnum){
    int can_fly=0;
    switch(chessnum){
        //black rules
        //rook
        case 0:
            for(int i=1;i<9;i++){
                if(chessboard[i+x][y] == -1 && i+x<9){
                    move_available[i+x][y] = 15;
                }else if(chessboard[i+x][y]<14 && chessboard[i+x][y]>6 && i+x<9){
                    move_available[i+x][y] = 15;
                    break;
                }else{
                    break;
                }
            }
            for(int i=1;i<9;i++){
                if(chessboard[x-i][y] == -1 && x-i>=0){
                    move_available[x-i][y] = 15;
                }else if(chessboard[x-i][y]<14 && chessboard[x-i][y]>6 && x-i>=0){
                    move_available[x-i][y] = 15;
                    break;
                }else{
                    break;
                }
            }
            for(int j=1;j<10;j++){
                if(chessboard[x][y-j] == -1 && y-j>=0){
                    move_available[x][y-j] = 15;
                }else if(chessboard[x][y-j]<14 && chessboard[x][y-j]>6 && y-j>=0){
                    move_available[x][y-j] = 15;
                    break;
                }else{
                    break;
                }
            }
            for(int j=1;j<10;j++){
                if(chessboard[x][y+j] == -1 && y+j<10){
                    move_available[x][y+j] = 15;
                }else if(chessboard[x][y+j]<14 && chessboard[x][y+j]>6 && y+j<10){
                    move_available[x][y+j] = 15;
                    break;
                }else{
                    break;
                }
            }
        break;
        //knight
        case 1:
            if(x-2>=0 && y-1 >= 0 && chessboard[x-1][y]==-1 && ((chessboard[x-2][y-1] < 14 && chessboard[x-2][y-1] > 6)||chessboard[x-2][y-1] == -1)) move_available[x-2][y-1] = 15;
            if(x-2>=0 && y+1 <= 9 && chessboard[x-1][y]==-1 && ((chessboard[x-2][y+1] < 14 && chessboard[x-2][y+1] > 6)||chessboard[x-2][y+1] == -1)) move_available[x-2][y+1] = 15;
            if(x-1>=0 && y-2 >= 0 && chessboard[x][y-1]==-1 && ((chessboard[x-1][y-2] < 14 && chessboard[x-1][y-2] > 6)||chessboard[x-1][y-2] == -1)) move_available[x-1][y-2] = 15;
            if(x+1<=8 && y-2 >= 0 && chessboard[x][y-1]==-1 && ((chessboard[x+1][y-2] < 14 && chessboard[x+1][y-2] > 6)||chessboard[x+1][y-2] == -1)) move_available[x+1][y-2] = 15;
            if(x+2<=8 && y-1 >= 0 && chessboard[x+1][y]==-1 && ((chessboard[x+2][y-1] < 14 && chessboard[x+2][y-1] > 6)||chessboard[x+2][y-1] == -1)) move_available[x+2][y-1] = 15;
            if(x+2<=8 && y+1 <= 9 && chessboard[x+1][y]==-1 && ((chessboard[x+2][y+1] < 14 && chessboard[x+2][y+1] > 6)||chessboard[x+2][y+1] == -1)) move_available[x+2][y+1] = 15;
            if(x-1>=0 && y+2 <= 9 && chessboard[x][y+1]==-1 && ((chessboard[x-1][y+2] < 14 && chessboard[x-1][y+2] > 6)||chessboard[x-1][y+2] == -1)) move_available[x-1][y+2] = 15;
            if(x+1<=8 && y+2 <= 9 && chessboard[x][y+1]==-1 && ((chessboard[x+1][y+2] < 14 && chessboard[x+1][y+2] > 6)||chessboard[x+1][y+2] == -1)) move_available[x+1][y+2] = 15;

        break;
        //ele
        case 2:
            if(x-2>=0 && y-2>=5 && ((chessboard[x-2][y-2] < 14 && chessboard[x-2][y-2] > 6)||chessboard[x-2][y-2] == -1)) move_available[x-2][y-2] = 15;
            if(x-2>=0 && y+2<=9 && ((chessboard[x-2][y+2] < 14 && chessboard[x-2][y+2] > 6)||chessboard[x-2][y+2] == -1)) move_available[x-2][y+2] = 15;
            if(x+2<=8 && y-2>=5 && ((chessboard[x+2][y-2] < 14 && chessboard[x+2][y-2] > 6)||chessboard[x+2][y-2] == -1)) move_available[x+2][y-2] = 15;
            if(x+2<=8 && y+2<=9 && ((chessboard[x+2][y+2] < 14 && chessboard[x+2][y+2] > 6)||chessboard[x+2][y+2] == -1)) move_available[x+2][y+2] = 15;
        break;
        //ad
        case 3:
            if(x-1>=3 && y-1>=7 && ((chessboard[x-1][y-1] < 14 && chessboard[x-1][y-1] > 6)||chessboard[x-1][y-1] == -1)) move_available[x-1][y-1] = 15;
            if(x-1>=3 && y+1<=9 && ((chessboard[x-1][y+1] < 14 && chessboard[x-1][y+1] > 6)||chessboard[x-1][y+1] == -1)) move_available[x-1][y+1] = 15;
            if(x+1<=5 && y-1>=7 && ((chessboard[x+1][y-1] < 14 && chessboard[x+1][y-1] > 6)||chessboard[x+1][y-1] == -1)) move_available[x+1][y-1] = 15;
            if(x+1<=5 && y+1<=9 && ((chessboard[x+1][y+1] < 14 && chessboard[x+1][y+1] > 6)||chessboard[x+1][y+1] == -1)) move_available[x+1][y+1] = 15;
        break;
        //king
        case 4:
            if(x-1>=3 && ((chessboard[x-1][y] < 14 && chessboard[x-1][y] > 6)||chessboard[x-1][y] == -1)) move_available[x-1][y] = 15;
            if(x+1<=5 && ((chessboard[x+1][y] < 14 && chessboard[x+1][y] > 6)||chessboard[x+1][y] == -1)) move_available[x+1][y] = 15;
            if(y-1>=7 && ((chessboard[x][y-1] < 14 && chessboard[x][y-1] > 6)||chessboard[x][y-1] == -1)) move_available[x][y-1] = 15;
            if(y+1<=9 && ((chessboard[x][y+1] < 14 && chessboard[x][y+1] > 6)||chessboard[x][y+1] == -1)) move_available[x][y+1] = 15;
        break;
        //can
        case 5:
            can_fly=0;
            for(int i=1;i<9;i++){
                if(chessboard[i+x][y] == -1 && i+x<9 && can_fly==0){
                    move_available[i+x][y] = 15;
                }else if(can_fly == 0 && chessboard[i+x][y]!=-1 && i+x<9){
                    can_fly=1;

                }else if(i+x<9 && chessboard[i+x][y]>6 && can_fly==1){
                    move_available[i+x][y] = 15;
                    break;
                }
            }
            can_fly=0;
            for(int i=1;i<9;i++){
                if(chessboard[x-i][y] == -1 && x-i>-1 && can_fly==0){
                    move_available[x-i][y] = 15;
                }else if(can_fly == 0 && chessboard[x-i][y]!=-1 && x-i>-1){
                    can_fly=1;

                }else if(x-i>-1 && chessboard[x-i][y]>6 && can_fly==1){
                    move_available[x-i][y] = 15;
                    break;
                }
            }
            can_fly=0;
            for(int j=1;j<10;j++){
                if(chessboard[x][y+j] == -1 && y+j<10 && can_fly==0){
                    move_available[x][y+j] = 15;
                }else if(can_fly == 0 && chessboard[x][y+j]!=-1 && y+j<10){
                    can_fly=1;

                }else if(y+j<10 && chessboard[x][y+j]>6 && can_fly==1){
                    move_available[x][y+j] = 15;
                    break;
                }
            }
            can_fly=0;
            for(int j=1;j<10;j++){
                if(chessboard[x][y-j] == -1 && y-j>-1 && can_fly==0){
                    move_available[x][y-j] = 15;
                }else if(can_fly == 0 && chessboard[x][y-j]!=-1 && y-j>-1){
                    can_fly=1;

                }else if(y-j>-1 && chessboard[x][y-j]>6 && can_fly==1){
                    move_available[x][y-j] = 15;
                    break;
                }
            }
        break;
        //pawn
        case 6:
        if(y > 4){
            move_available[x][y-1] =15;
        }else{
            if(x-1>=0 && ((chessboard[x-1][y] < 14 && chessboard[x-1][y] > 6)||chessboard[x-1][y] == -1)) move_available[x-1][y] = 15;
            if(x+1<=8 && ((chessboard[x+1][y] < 14 && chessboard[x+1][y] > 6)||chessboard[x+1][y] == -1)) move_available[x+1][y] = 15;
            if(y-1>=0 && ((chessboard[x][y-1] < 14 && chessboard[x][y-1] > 6)||chessboard[x][y-1] == -1)) move_available[x][y-1] = 15;

        }
        break;

        //red rules
        //rook
        case 7:
            for(int i=1;i<9;i++){
                if(chessboard[i+x][y] == -1 && i+x<9){
                    move_available[i+x][y] = 15;
                }else if(chessboard[i+x][y]<7 && chessboard[i+x][y]>-1 && i+x<9){
                    move_available[i+x][y] = 15;
                    break;
                }else{
                    break;
                }
            }
            for(int i=1;i<9;i++){
                if(chessboard[x-i][y] == -1 && x-i>=0){
                    move_available[x-i][y] = 15;
                }else if(chessboard[x-i][y]<7 && chessboard[x-i][y]>-1 && x-i>=0){
                    move_available[x-i][y] = 15;
                    break;
                }else{
                    break;
                }
            }
            for(int j=1;j<10;j++){
                if(chessboard[x][y-j] == -1 && y-j>=0){
                    move_available[x][y-j] = 15;
                }else if(chessboard[x][y-j]<7 && chessboard[x][y-j]>-1 && y-j>=0){
                    move_available[x][y-j] = 15;
                    break;
                }else{
                    break;
                }
            }
            for(int j=1;j<10;j++){
                if(chessboard[x][y+j] == -1 && y+j<10){
                    move_available[x][y+j] = 15;
                }else if(chessboard[x][y+j]<7 && chessboard[x][y+j]>-1 && y+j<10){
                    move_available[x][y+j] = 15;
                    break;
                }else{
                    break;
                }
            }
        break;
        //knight
        case 8:
            if(x-2>=0 && y-1 >= 0 && chessboard[x-1][y]==-1 && ((chessboard[x-2][y-1] < 7 && chessboard[x-2][y-1] > -1)||chessboard[x-2][y-1] == -1)) move_available[x-2][y-1] = 15;
            if(x-2>=0 && y+1 <= 9 && chessboard[x-1][y]==-1 && ((chessboard[x-2][y+1] < 7 && chessboard[x-2][y+1] > -1)||chessboard[x-2][y+1] == -1)) move_available[x-2][y+1] = 15;
            if(x-1>=0 && y-2 >= 0 && chessboard[x][y-1]==-1 && ((chessboard[x-1][y-2] < 7 && chessboard[x-1][y-2] > -1)||chessboard[x-1][y-2] == -1)) move_available[x-1][y-2] = 15;
            if(x+1<=8 && y-2 >= 0 && chessboard[x][y-1]==-1 && ((chessboard[x+1][y-2] < 7 && chessboard[x+1][y-2] > -1)||chessboard[x+1][y-2] == -1)) move_available[x+1][y-2] = 15;
            if(x+2<=8 && y-1 >= 0 && chessboard[x+1][y]==-1 && ((chessboard[x+2][y-1] < 7 && chessboard[x+2][y-1] > -1)||chessboard[x+2][y-1] == -1)) move_available[x+2][y-1] = 15;
            if(x+2<=8 && y+1 <= 9 && chessboard[x+1][y]==-1 && ((chessboard[x+2][y+1] < 7 && chessboard[x+2][y+1] > -1)||chessboard[x+2][y+1] == -1)) move_available[x+2][y+1] = 15;
            if(x-1>=0 && y+2 <= 9 && chessboard[x][y+1]==-1 && ((chessboard[x-1][y+2] < 7 && chessboard[x-1][y+2] > -1)||chessboard[x-1][y+2] == -1)) move_available[x-1][y+2] = 15;
            if(x+1<=8 && y+2 <= 9 && chessboard[x][y+1]==-1 && ((chessboard[x+1][y+2] < 7 && chessboard[x+1][y+2] > -1)||chessboard[x+1][y+2] == -1)) move_available[x+1][y+2] = 15;

        break;
        //ele
        case 9:
            if(x-2>=0 && y-2>=0 && ((chessboard[x-2][y-2] < 7 && chessboard[x-2][y-2] > -1)||chessboard[x-2][y-2] == -1)) move_available[x-2][y-2] = 15;
            if(x-2>=0 && y+2<=4 && ((chessboard[x-2][y+2] < 7 && chessboard[x-2][y+2] > -1)||chessboard[x-2][y+2] == -1)) move_available[x-2][y+2] = 15;
            if(x+2<=8 && y-2>=0 && ((chessboard[x+2][y-2] < 7 && chessboard[x+2][y-2] > -1)||chessboard[x+2][y-2] == -1)) move_available[x+2][y-2] = 15;
            if(x+2<=8 && y+2<=4 && ((chessboard[x+2][y+2] < 7 && chessboard[x+2][y+2] > -1)||chessboard[x+2][y+2] == -1)) move_available[x+2][y+2] = 15;
        break;
        //ad
        case 10:
            if(x-1>=3 && y-1>=0 && ((chessboard[x-1][y-1] < 7 && chessboard[x-1][y-1] > -1)||chessboard[x-1][y-1] == -1)) move_available[x-1][y-1] = 15;
            if(x-1>=3 && y+1<=2 && ((chessboard[x-1][y+1] < 7 && chessboard[x-1][y+1] > -1)||chessboard[x-1][y+1] == -1)) move_available[x-1][y+1] = 15;
            if(x+1<=5 && y-1>=0 && ((chessboard[x+1][y-1] < 7 && chessboard[x+1][y-1] > -1)||chessboard[x+1][y-1] == -1)) move_available[x+1][y-1] = 15;
            if(x+1<=5 && y+1<=2 && ((chessboard[x+1][y+1] < 7 && chessboard[x+1][y+1] > -1)||chessboard[x+1][y+1] == -1)) move_available[x+1][y+1] = 15;
        break;
        //king
        case 11:
            if(x-1>=3 && ((chessboard[x-1][y] < 7 && chessboard[x-1][y] > -1)||chessboard[x-1][y] == -1)) move_available[x-1][y] = 15;
            if(x+1<=5 && ((chessboard[x+1][y] < 7 && chessboard[x+1][y] > -1)||chessboard[x+1][y] == -1)) move_available[x+1][y] = 15;
            if(y-1>=0 && ((chessboard[x][y-1] < 7 && chessboard[x][y-1] > -1)||chessboard[x][y-1] == -1)) move_available[x][y-1] = 15;
            if(y+1<=2 && ((chessboard[x][y+1] < 7 && chessboard[x][y+1] > -1)||chessboard[x][y+1] == -1)) move_available[x][y+1] = 15;
        break;
        //can
        case 12:
            can_fly=0;
            for(int i=1;i<9;i++){
                if(chessboard[i+x][y] == -1 && i+x<9 && can_fly==0){
                    move_available[i+x][y] = 15;
                }else if(can_fly == 0 && chessboard[i+x][y]!=-1 && i+x<9){
                    can_fly=1;

                }else if(i+x<9 && chessboard[i+x][y]<7 && can_fly==1 && chessboard[x+i][y]>-1){
                    move_available[i+x][y] = 15;
                    break;
                }
            }
            can_fly=0;
            for(int i=1;i<9;i++){
                if(chessboard[x-i][y] == -1 && x-i>-1 && can_fly==0){
                    move_available[x-i][y] = 15;
                }else if(can_fly == 0 && chessboard[x-i][y]!=-1 && x-i>-1){
                    can_fly=1;

                }else if(x-i>-1 && chessboard[x-i][y]<7 && can_fly==1 && chessboard[x-i][y]>-1){
                    move_available[x-i][y] = 15;
                    break;
                }
            }
            can_fly=0;
            for(int j=1;j<10;j++){
                if(chessboard[x][y+j] == -1 && y+j<10 && can_fly==0){
                    move_available[x][y+j] = 15;
                }else if(can_fly == 0 && chessboard[x][y+j]!=-1 && y+j<10){
                    can_fly=1;

                }else if(y+j<10 && chessboard[x][y+j]<7 && can_fly==1 && chessboard[x][y+j]>-1){
                    move_available[x][y+j] = 15;
                    break;
                }
            }
            can_fly=0;
            for(int j=1;j<10;j++){
                if(chessboard[x][y-j] == -1 && y-j>-1 && can_fly==0){
                    move_available[x][y-j] = 15;
                }else if(can_fly == 0 && chessboard[x][y-j]!=-1 && y-j>-1){
                    can_fly=1;

                }else if(y-j>-1 && chessboard[x][y-j]<7 && can_fly==1 && chessboard[x][y-j]>-1){
                    move_available[x][y-j] = 15;
                    break;
                }
            }
        break;
        //pawn
        case 13:
        if(y < 5){
            move_available[x][y+1] =15;
        }else{
            if(x-1>=0 && ((chessboard[x-1][y] < 7 && chessboard[x-1][y] > -1)||chessboard[x-1][y] == -1)) move_available[x-1][y] = 15;
            if(x+1<=8 && ((chessboard[x+1][y] < 7 && chessboard[x+1][y] > -1)||chessboard[x+1][y] == -1)) move_available[x+1][y] = 15;
            if(y+1<=9 && ((chessboard[x][y+1] < 7 && chessboard[x][y+1] > -1)||chessboard[x][y+1] == -1)) move_available[x][y+1] = 15;

        }
        break;
        default:
            break;
    }





}

void MainWindow::attackchess(int attacker_x,int attacker_y,int attacker_value,int beattack_x,int beattack_y,int beattack_value){
    step_count++;
    if(beattack_value == -1){
        chessboard[beattack_x][beattack_y] = attacker_value;
        chessboard[attacker_x][attacker_y] = -1;
    }else{
        chessboard[beattack_x][beattack_y] = attacker_value;
        chessboard[attacker_x][attacker_y] = -1;
    }
    copychessboard();
    if(setupmode==0){
        gamewin();
    }

}
void MainWindow::gamewin(){
    gameover=true;
    int red_king_x,red_king_y,black_king_x,black_king_y;

    for(int i=1;i<2;i++){

        findking(&black_king_x,&black_king_y,2);
        if(gameover) break;
        findking(&red_king_x,&red_king_y,1);
        if(gameover) break;

        if(red_king_x==black_king_x){
            gameover=true;
            for(int j=red_king_y+1;j<black_king_y;j++){
                if(chessboard[black_king_x][j]!=-1){
                    gameover=false;
                    break;
                }
            }
            if(gameover){
                step_count++;
            }
        }
    }
    if(gameover){
        QString win;

        if(step_count%2 ==0){
            win="player2";
        }else{
            win="player1";
        }
        step_count=0;
        if(QMessageBox::Yes==QMessageBox::question(this,tr("gameover"),win+ "wins! \n Want to restart?",QMessageBox::Yes|QMessageBox::No)){
            initialboard();
        }
        ui->setupbutton->show();
    }
}
void MainWindow::findking(int *x,int *y,int player){
    if(player==1){
        for(int i=3;i<6;i++){
            for(int j=0;j<3;j++){
                if(chessboard[i][j]==11){
                    gameover=false;
                    *x=i;
                    *y=j;
                }
            }
        }
    }else{
        for(int i=3;i<6;i++){
            for(int j=7;j<10;j++){
                if(chessboard[i][j]==4){
                    gameover=false;
                    *x=i;
                    *y=j;
                }
            }
        }
    }

}
