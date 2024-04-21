 #include "test.h"

Test::Test() {}

Test::Test(QString name, QString desc, QList<Action> actions)
{
    this->name_ = name;
    this->desc_ = desc;
    this->actions_ = actions;
}

QString Test::name() const
{
    return this->name_;
}

QString Test::desc() const
{
    return this->desc_;
}

QList<Action> Test::actions() const
{
    return this->actions_;
}


bool Test::isEmpty()
{
    return this->name_.isEmpty() && this->desc_.isEmpty() && this->actions_.isEmpty();
}

QDebug operator<<(QDebug stream, const Test &test)
{
    stream << "Текущий тест: " << test.name() << Qt::endl;
    stream << "Описание: " << test.desc();

    return stream;
}
