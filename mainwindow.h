#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <array>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    std::array<QPushButton *, 10> kartlar{};
    std::array<int, 10> degerler{};
    std::array<bool, 10> eslesenler{};

    int ilkKart = -1;
    int basarisizTahmin = 0;
    int bulunanCift = 0;
    bool bekleniyor = false;

    void oyunuBaslat();
    void kartaTiklandi(int indeks);
    void kartiAc(int indeks);
    void kartiKapat(int indeks);
};

#endif // MAINWINDOW_H