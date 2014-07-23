#pragma comment


#include <QWidget>
#include <QHBoxLayout>

class MouseGroup : public QWidget {
    Q_OBJECT

    QHBoxLayout * layout;

public:

    MouseGroup() {
        layout = new QHBoxLayout;
        this->setLayout(layout);
    }

};

