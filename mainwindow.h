#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void serialReceived();

    void on_list_port_itemClicked(QListWidgetItem *item);

    void on_list_baud_itemClicked(QListWidgetItem *item);

    void on_textIn_textChanged();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
