#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    background->setGeometry(0, -100, 1000, 740);
    background->setPixmap(QPixmap(":/images/Title.jpg"));
    background->setScaledContents(1);
    none.load(":/images/none.png");
    pixs[0].load(":/images/box.png");
    pixs[1].load(":/images/wall.png");
    pixs[2].load(":/images/goal.png");
    pixs[3].load(":/images/ground.png");
    pixs[4].load(":/images/player.png");
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            bmap[i][j] = new QLabel(this);
            vmap[i][j] = new QLabel(this);
        }
    }
    cleared = new QLabel(this);
    New_game = new QPushButton(this);
    New_game->setGeometry(390, 200, 200, 50);
    New_game->setText("New Game");
    Load_game = new QPushButton(this);
    Load_game->setGeometry(390, 270, 200, 50);
    Load_game->setText("Load Game");
    Exit = new QPushButton(this);
    Exit->setGeometry(390, 340, 200, 50);
    Exit->setText("Exit");
    show_main();
    QFile f;
    f.setFileName(QDir("datas/save.dat").absolutePath());
    if(!f.open(QIODevice::ReadOnly))
        Load_game->setEnabled(0);
    else
    {
        QTextStream in(&f);
        if(in.atEnd())
            Load_game->setEnabled(0);
    }
    f.close();
    connect(New_game, SIGNAL(clicked()), this, SLOT(New_Game()));
    connect(Load_game, SIGNAL(clicked()), this, SLOT(Load_Game()));
    connect(Exit, SIGNAL(clicked()), this, SLOT(Quit()));
    connect(ui->NextLevel_Button, SIGNAL(clicked()), this, SLOT(NextLevel()));
    connect(ui->Replay_Button, SIGNAL(clicked()), this, SLOT(Replay()));
    connect(ui->Save_Button, SIGNAL(clicked()), this, SLOT(Save()));
    connect(ui->MainMenu_Button, SIGNAL(clicked()), this, SLOT(MainMenu()));
}

void MainWindow::show_main()
{
    background->show();
    New_game->show();
    Load_game->show();
    Exit->show();
    ui->Step_Label->hide();
    ui->Step->hide();
    ui->Level_Label->hide();
    ui->Level->hide();
    ui->NextLevel_Button->hide();
    ui->Replay_Button->hide();
    ui->Save_Button->hide();
    ui->MainMenu_Button->hide();
    cleared->hide();
    QFile f;
    f.setFileName(QDir("datas/save.dat").absolutePath());
    if(!f.open(QIODevice::ReadOnly))
        Load_game->setEnabled(0);
    else
    {
        QTextStream in(&f);
        if(in.atEnd())
            Load_game->setEnabled(0);
    }
    f.close();
}

void MainWindow::hide_main()
{
    background->hide();
    New_game->hide();
    Load_game->hide();
    Exit->hide();
    ui->Step_Label->show();
    ui->Step->show();
    ui->Level_Label->show();
    ui->Level->show();
    ui->Replay_Button->show();
    ui->Save_Button->show();
    ui->MainMenu_Button->show();
}

void MainWindow::New_Game()
{
    hide_main();
    lvl = 1;
    start_level();
}

void MainWindow::Load_Game()
{
    hide_main();
    start_save();
}

void MainWindow::Quit()
{
    this->close();
}

void MainWindow::NextLevel()
{
    lvl++, start_level();
}

void MainWindow::Replay()
{
    start_level();
}

void MainWindow::Save()
{
    QFile f;
    if(!QDir("datas").exists())
        QDir(".").mkdir("datas");
    f.setFileName(QDir("datas/save.dat").absolutePath());
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&f);
    out << lvl << " " << step << "\n";
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < M; j++)
        {
            if(map[i][j] == -1)
                out << backmap[i][j] << " ";
            else
                out << map[i][j] << " ";
        }
        if(i < N - 1)
            out << "\n";
    }
    f.close();
}

void MainWindow::MainMenu()
{
    show_main();
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < M; j++)
        {
            bmap[i][j]->clear();
            vmap[i][j]->clear();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start_level()
{
    QString s;
    QStringList lst;
    QFile f;
    f.setFileName(":/datas/level_" + QString::number(lvl) + ".dat");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&f);
    all = step = 0;
    for(int i = 0; !in.atEnd(); N = ++i)
    {
        s = in.readLine();
        lst = s.split(" ");
        for(int j = 0; j < lst.size() && lst[j] != ""; M = ++j)
        {
            backmap[i][j] = map[i][j] = lst[j].toInt();
            if(map[i][j] == 2)
                map[i][j] = -1, all++;
            if(map[i][j] == 3)
                map[i][j] = -1;
            if(map[i][j] == 4)
                backmap[i][j] = 3, play = std::make_pair(i, j);
            if(map[i][j] > 4)
                backmap[i][j] = 3;
        }
    }
    f.close();
    ui->Level->display(QString::number(lvl));
    ui->NextLevel_Button->hide();
    ui->Save_Button->setEnabled(1);
    cleared->hide();
    paintmap();
}

void MainWindow::start_save()
{
    QString s;
    QStringList lst;
    QFile f;
    f.setFileName(QDir("datas/save.dat").absolutePath());
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&f);
    s = in.readLine();
    lst = s.split(" ");
    lvl = lst[0].toInt();
    step = lst[1].toInt();
    for(int i = 0; !in.atEnd(); N = ++i)
    {
        s = in.readLine();
        lst = s.split(" ");
        for(int j = 0; j < lst.size() && lst[j] != ""; M = ++j)
        {
            backmap[i][j] = map[i][j] = lst[j].toInt();
            if(map[i][j] == 2)
                map[i][j] = -1, all++;
            if(map[i][j] == 3)
                map[i][j] = -1;
            if(map[i][j] == 4)
                backmap[i][j] = 3, play = std::make_pair(i, j);
            if(map[i][j] > 4)
                backmap[i][j] = 3;
        }
    }
    f.close();
    ui->Level->display(QString::number(lvl));
    ui->NextLevel_Button->hide();
    ui->Save_Button->setEnabled(1);
    cleared->hide();
    paintmap();
}

void MainWindow::paintmap()
{
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < M; j++)
        {
            bmap[i][j]->setGeometry(j * 80, i * 80, 80, 80);
            vmap[i][j]->setGeometry(j * 80, i * 80, 80, 80);
            if(backmap[i][j] == 1)
                bmap[i][j]->setPixmap(pixs[1]);
            else if(backmap[i][j] == 2)
                bmap[i][j]->setPixmap(pixs[2]);
            else
                bmap[i][j]->setPixmap(pixs[3]);
            if(map[i][j] == -1)
                vmap[i][j]->setPixmap(none);
            else if(map[i][j] <= 4)
                vmap[i][j]->setPixmap(pixs[map[i][j]]);
            else
                vmap[i][j]->setPixmap(pixs[0]);
            vmap[i][j]->setScaledContents(1);
            bmap[i][j]->setScaledContents(1);
        }
    }
    ui->Step->display(QString::number(step));
    cnt = all;
    for(int i = 0; i < N; i++)
        for(int j = 0; j < M; j++)
            if(backmap[i][j] == 2 && map[i][j] >= 5)
                cnt--;
    if(cnt == 0)
    {
        if(QFile(":/datas/level_" + QString::number(lvl + 1) + ".dat").exists())
        {
            ui->NextLevel_Button->show();
            ui->Save_Button->setEnabled(0);
            level_clear();
        }
        else
        {
            ui->Save_Button->setEnabled(0);
            all_clear();
        }
    }
}

void MainWindow::level_clear()
{
    cleared->setGeometry(210, 130, 400, 300);
    cleared->setPixmap(QPixmap(":/images/Level_Cleared.png"));
    cleared->setScaledContents(1);
    cleared->show();
}

void MainWindow::all_clear()
{
    cleared->setGeometry(190, 120, 400, 300);
    cleared->setPixmap(QPixmap(":/images/All_Cleared.png"));
    cleared->setScaledContents(1);
    cleared->show();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(!cnt)
        return;
    if(e->key() == Qt::Key_W)
    {
        if(map[play.first - 1][play.second] > 4 || map[play.first - 1][play.second] == -1)
        {
            if(map[play.first - 1][play.second] > 4)
                if(map[play.first - 2][play.second] == 1 || map[play.first - 2][play.second] > 4)
                    goto end;
                else
                    std::swap(map[play.first - 1][play.second], map[play.first - 2][play.second]);
            step++;
            std::swap(map[play.first][play.second], map[play.first - 1][play.second]);
            play = std::make_pair(play.first - 1, play.second);
        }
    }
    if(e->key() == Qt::Key_A)
    {
        if(map[play.first][play.second - 1] > 4 || map[play.first][play.second - 1] == -1)
        {
            if(map[play.first][play.second - 1] > 4)
                if(map[play.first][play.second - 2] == 1 || map[play.first][play.second - 2] > 4)
                    goto end;
                else
                    std::swap(map[play.first][play.second - 1], map[play.first][play.second - 2]);
            step++;
            std::swap(map[play.first][play.second], map[play.first][play.second - 1]);
            play = std::make_pair(play.first, play.second - 1);
        }
    }
    if(e->key() == Qt::Key_S)
    {
        if(map[play.first + 1][play.second] > 4 || map[play.first + 1][play.second] == -1)
        {
            if(map[play.first + 1][play.second] > 4)
                if(map[play.first + 2][play.second] == 1 || map[play.first + 2][play.second] > 4)
                    goto end;
                else
                    std::swap(map[play.first + 1][play.second], map[play.first + 2][play.second]);
            step++;
            std::swap(map[play.first][play.second], map[play.first + 1][play.second]);
            play = std::make_pair(play.first + 1, play.second);
        }
    }
    if(e->key() == Qt::Key_D)
    {
        if(map[play.first][play.second + 1] > 4 || map[play.first][play.second + 1] == -1)
        {
            if(map[play.first][play.second + 1] > 4)
                if(map[play.first][play.second + 2] == 1 || map[play.first][play.second + 2] > 4)
                    goto end;
                else
                    std::swap(map[play.first][play.second + 1], map[play.first][play.second + 2]);
            step++;
            std::swap(map[play.first][play.second], map[play.first][play.second + 1]);
            play = std::make_pair(play.first, play.second + 1);
        }
    }
    end:
    paintmap();
}
