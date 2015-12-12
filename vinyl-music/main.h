#ifndef MAIN
#define MAIN
class DataObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString color READ color WRITE setColor)

    QString name;
    QString color;

    void setName(QString name) { this->name = name;}
    void setColor(QString color) {this->color = color;}
    // getters

    QString getName() {return name;}
    QString getColor() {return color;}

};
#endif // MAIN

