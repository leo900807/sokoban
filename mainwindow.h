#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QLabel>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <algorithm>
#include <utility>
#include <QPushButton>
#include <QDir>
#include <QSignalMapper>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void keyPressEvent(QKeyEvent*);

public slots:
    void New_Game();
    void Load_Game();
    void Choose_Level();
    void Quit();
    void Back();
    void Level_Choose(int);
    void NextLevel();
    void Replay();
    void Save();
    void MainMenu();

private:
    Ui::MainWindow *ui;
    QLabel *vmap[20][20], *bmap[20][20], *background = new QLabel(this), *cleared;
    int map[20][20], backmap[20][20], N, M, step, cnt, all, lvl, mx;
    std::pair<int, int> play;
    QFile f;
    QPixmap pixs[5], none;
    void start_level();
    void start_save();
    void paintmap();
    QPushButton *New_game, *Choose_level, *Load_game, *Exit, *level_Button[4][4], *Back_Button;
    QDir dir;
    void hide_main();
    void show_main();
    void level_clear();
    void all_clear();
    QSignalMapper *mapper = new QSignalMapper();

};

#endif // MAINWINDOW_H
