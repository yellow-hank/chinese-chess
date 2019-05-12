#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QMessageBox>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QSize>
#include <QPushButton>
#include <QList>

#include <QString>
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>



#define board_distance_x 61;//distance between squares
#define board_distance_y 63;//distance between squares
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *);
    void putpic();
    void initialboard();
    void mousePressEvent(QMouseEvent *event);
    void copychessboard();
    void available_move(int,int,int);
    void attackchess(int,int,int,int,int,int);
    void gamewin();
    void findking(int *,int * ,int);

public slots:

    void start_slot();
    void setup_slot();
    void delete_slot();
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *item;
    QPushButton *button;

    int step_count;
    int chessboard[9][10];
    int move_available[9][10];
    int deletemode;
    int setupmode;
    int initial_start;
    int choose_x,choose_y,delete_count;
    bool gameover;

    QList<QImage> chess_pic;
    QLabel *pressLabel;
    QVBoxLayout *mainLayout;


};

#endif // MAINWINDOW_H
