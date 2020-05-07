#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QLabel>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <algorithm>
#include <utility>
#include <QPushButton>
#include <QDir>

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
    void Quit();
    void Save();
    void MainMenu();

private:
    Ui::MainWindow *ui;
    QLabel *vmap[10][10], *bmap[10][10], *background = new QLabel(this);
    int map[10][10], backmap[10][10], N, M, step, cnt, all, lvl;
    std::pair<int, int> play;
    QFile f;
    QPixmap pixs[5], none;
    void start_level();
    void start_save();
    void paintmap();
    QPushButton *New_game, *Load_game, *Exit;
    QDir dir;
    void hide_main();
    void show_main();

};

#endif // MAINWINDOW_H
