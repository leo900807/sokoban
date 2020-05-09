#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    background->setGeometry(0, 0, 1000, 800);
    background->setPixmap(QPixmap(":/images/Title.jpg"));
    background->setScaledContents(1);
    none.load(":/images/none.png");
    pixs[0].load(":/images/box.png");
    pixs[1].load(":/images/wall.png");
    pixs[2].load(":/images/goal.png");
    pixs[3].load(":/images/ground.png");
    pixs[4].load(":/images/player.png");
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            bmap[i][j] = new QLabel(this);
            vmap[i][j] = new QLabel(this);
        }
    }
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            level_Button[i][j] = new QPushButton(this);
            level_Button[i][j]->setGeometry(j * 70 + 370, i * 70 + 250, 50, 50);
            level_Button[i][j]->setText(QString::number(i * 4 + j + 1));
            if(!QFile(":/datas/level_" + QString::number(i * 4 + j + 1) + ".dat").exists())
                level_Button[i][j]->setEnabled(0);
            level_Button[i][j]->hide();
            mapper->setMapping(level_Button[i][j], i * 4 + j + 1);
            connect(level_Button[i][j], SIGNAL(clicked()), mapper, SLOT(map()));
        }
    }
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(Level_Choose(int)));
    cleared = new QLabel(this);
    New_game = new QPushButton(this);
    New_game->setGeometry(400, 300, 200, 50);
    New_game->setText("New Game");
    Choose_level = new QPushButton(this);
    Choose_level->setGeometry(400, 370, 200, 50);
    Choose_level->setText("Choose Level");
    Load_game = new QPushButton(this);
    Load_game->setGeometry(400, 440, 200, 50);
    Load_game->setText("Load Game");
    Exit = new QPushButton(this);
    Exit->setGeometry(400, 510, 200, 50);
    Exit->setText("Exit");
    Back_Button = new QPushButton(this);
    Back_Button->setGeometry(400, 570, 200, 50);
    Back_Button->setText("Back");
    Back_Button->hide();
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
        else
            Load_game->setEnabled(1);
    }
    f.close();
    connect(New_game, SIGNAL(clicked()), this, SLOT(New_Game()));
    connect(Choose_level, SIGNAL(clicked()), this, SLOT(Choose_Level()));
    connect(Load_game, SIGNAL(clicked()), this, SLOT(Load_Game()));
    connect(Exit, SIGNAL(clicked()), this, SLOT(Quit()));
    connect(Back_Button, SIGNAL(clicked()), this, SLOT(Back()));
    connect(ui->NextLevel_Button, SIGNAL(clicked()), this, SLOT(NextLevel()));
    connect(ui->Replay_Button, SIGNAL(clicked()), this, SLOT(Replay()));
    connect(ui->Save_Button, SIGNAL(clicked()), this, SLOT(Save()));
    connect(ui->MainMenu_Button, SIGNAL(clicked()), this, SLOT(MainMenu()));
}

void MainWindow::show_main()
{
    lvl = 0;
    background->show();
    New_game->show();
    Choose_level->show();
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
        else
            Load_game->setEnabled(1);
    }
    f.close();
}

void MainWindow::hide_main()
{
    background->hide();
    New_game->hide();
    Choose_level->hide();
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

void MainWindow::Choose_Level()
{
    New_game->hide();
    Choose_level->hide();
    Load_game->hide();
    Exit->hide();
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            level_Button[i][j]->show();
        }
    }
    Back_Button->show();
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

void MainWindow::Back()
{
    New_game->show();
    Choose_level->show();
    Load_game->show();
    Exit->show();
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            level_Button[i][j]->hide();
        }
    }
    Back_Button->hide();
}

void MainWindow::Level_Choose(int chlvl)
{
    hide_main();
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            level_Button[i][j]->hide();
        }
    }
    Back_Button->hide();
    lvl = chlvl, start_level();
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
            out << map[i][j] << " ";
        }
        out << "\n";
    }
    out << "bmap\n";
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < M; j++)
        {
            out << backmap[i][j] << " ";
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
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            bmap[i][j]->clear();
            vmap[i][j]->clear();
        }
    }
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
    mx = std::max(N, M);
    f.close();
    ui->Level->display(QString::number(lvl));
    ui->NextLevel_Button->hide();
    cleared->hide();
    paintmap();
}

void MainWindow::start_save()
{
    QString s;
    QStringList lst;
    QFile f;
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            bmap[i][j]->clear();
            vmap[i][j]->clear();
        }
    }
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
        if(s == "bmap")
            break;
        lst = s.split(" ");
        for(int j = 0; j < lst.size() && lst[j] != ""; M = ++j)
        {
            map[i][j] = lst[j].toInt();
            if(map[i][j] == 4)
                play = std::make_pair(i, j);
        }
    }
    for(int i = 0; !in.atEnd(); ++i)
    {
        s = in.readLine();
        lst = s.split(" ");
        for(int j = 0; j < lst.size() && lst[j] != ""; ++j)
        {
            backmap[i][j] = lst[j].toInt();
        }
    }
    mx = std::max(N, M);
    f.close();
    ui->Level->display(QString::number(lvl));
    ui->NextLevel_Button->hide();
    cleared->hide();
    paintmap();
}

void MainWindow::paintmap()
{
    if(!lvl)
        return;
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < M; j++)
        {
            bmap[i][j]->setGeometry(j * (800. / mx), i * (800. / mx), (800. / mx), (800. / mx));
            vmap[i][j]->setGeometry(j * (800. / mx), i * (800. / mx), (800. / mx), (800. / mx));
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
            level_clear();
        }
        else
            all_clear();
    }
}

void MainWindow::level_clear()
{
    cleared->setGeometry(210, 250, 400, 300);
    cleared->setPixmap(QPixmap(":/images/Level_Cleared.png"));
    cleared->setScaledContents(1);
    cleared->show();
}

void MainWindow::all_clear()
{
    cleared->setGeometry(190, 240, 400, 300);
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
