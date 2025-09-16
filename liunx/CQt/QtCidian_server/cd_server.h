#ifndef CD_SERVER_H
#define CD_SERVER_H

#include <QWidget>

namespace Ui {
class cd_server;
}

class cd_server : public QWidget
{
    Q_OBJECT

public:
    explicit cd_server(QWidget *parent = nullptr);
    ~cd_server();

private:
    Ui::cd_server *ui;
};

#endif // CD_SERVER_H
