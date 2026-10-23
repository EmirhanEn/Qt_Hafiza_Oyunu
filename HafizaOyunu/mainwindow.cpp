#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QIcon>
#include <QMessageBox>
#include <QPushButton>
#include <QSize>
#include <QString>
#include <QTimer>

#include <algorithm>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Hafıza Oyunu");

    kartlar = {
        ui->kart0, ui->kart1, ui->kart2, ui->kart3, ui->kart4,
        ui->kart5, ui->kart6, ui->kart7, ui->kart8, ui->kart9
    };

    for (int i = 0; i < 10; ++i) {
        kartlar[i]->setFixedSize(80, 90);
        kartlar[i]->setIconSize(QSize(64, 64));

        connect(kartlar[i], &QPushButton::clicked,
                this, [this, i]() {
                    kartaTiklandi(i);
                });
    }

    oyunuBaslat();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::oyunuBaslat()
{
    // Her resimden iki adet bulunur.
    degerler = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
    eslesenler.fill(false);

    std::mt19937 rastgele(std::random_device{}());
    std::shuffle(degerler.begin(), degerler.end(), rastgele);

    ilkKart = -1;
    basarisizTahmin = 0;
    bulunanCift = 0;
    bekleniyor = false;

    ui->lblBasarisizSayisi->setText("0");

    for (int i = 0; i < 10; ++i) {
        kartiKapat(i);
    }
}

void MainWindow::kartiAc(int indeks)
{
    const QString yol =
        QString(":/images/resim%1.png").arg(degerler[indeks]);

    kartlar[indeks]->setText("");
    kartlar[indeks]->setIcon(QIcon(yol));

    kartlar[indeks]->setStyleSheet(
        "QPushButton {"
        "background-color: white;"
        "border: 2px solid #777;"
        "border-radius: 6px;"
        "}"
        );
}

void MainWindow::kartiKapat(int indeks)
{
    kartlar[indeks]->setIcon(QIcon());
    kartlar[indeks]->setText("?");

    kartlar[indeks]->setStyleSheet(
        "QPushButton {"
        "background-color: #444;"
        "color: white;"
        "font-size: 30px;"
        "font-weight: bold;"
        "border: 2px solid #222;"
        "border-radius: 6px;"
        "}"
        "QPushButton:hover {"
        "background-color: #606060;"
        "}"
        );
}

void MainWindow::kartaTiklandi(int indeks)
{
    // Beklerken, eşleşmiş karta veya aynı karta tıklamayı yok say.
    if (bekleniyor || eslesenler[indeks] || indeks == ilkKart) {
        return;
    }

    kartiAc(indeks);

    // İlk kart seçildi; ikinci kartı bekle.
    if (ilkKart == -1) {
        ilkKart = indeks;
        return;
    }

    const int oncekiKart = ilkKart;
    ilkKart = -1;

    if (degerler[oncekiKart] == degerler[indeks]) {
        eslesenler[oncekiKart] = true;
        eslesenler[indeks] = true;
        ++bulunanCift;

        if (bulunanCift == 5) {
            bekleniyor = true;

            QTimer::singleShot(300, this, [this]() {
                const auto cevap = QMessageBox::question(
                    this,
                    "Tebrikler!",
                    QString(
                        "Tüm kartları eşleştirdin!\n"
                        "Başarısız tahmin sayısı: %1\n\n"
                        "Tekrar oynamak ister misin?"
                        ).arg(basarisizTahmin),
                    QMessageBox::Yes | QMessageBox::No
                    );

                if (cevap == QMessageBox::Yes) {
                    oyunuBaslat();
                } else {
                    bekleniyor = false;
                }
            });
        }

        return;
    }

    // Yanlış tahminde iki kartı kısa süre göster, sonra kapat.
    ++basarisizTahmin;
    ui->lblBasarisizSayisi->setText(
        QString::number(basarisizTahmin)
        );

    bekleniyor = true;

    QTimer::singleShot(1000, this, [this, oncekiKart, indeks]() {
        kartiKapat(oncekiKart);
        kartiKapat(indeks);
        bekleniyor = false;
    });
}