#ifndef ISTORIJAGRESAKA_H
#define ISTORIJAGRESAKA_H

#include <QDialog>

namespace Ui {
class IstorijaGresaka;
}

class IstorijaGresaka : public QDialog
{
    Q_OBJECT

public:
    explicit IstorijaGresaka(QWidget *parent = nullptr);
    ~IstorijaGresaka();

private:
    Ui::IstorijaGresaka *ui;
public slots:
    void istorijagresaka();
    void resizeEvent(QResizeEvent *evt);
private slots:
    void on_commandLinkButton_clicked();
    void on_comboBox_activated(int index);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
};

#endif // ISTORIJAGRESAKA_H
