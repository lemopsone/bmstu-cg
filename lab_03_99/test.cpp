 #include "test.h"

Test::Test() {}

Test::Test(QString name, QString desc)
{
    this->name_ = name;
    this->desc_ = desc;
}

QString Test::name() const
{
    return this->name_;
}

QString Test::desc() const
{
    return this->desc_;
}


bool Test::isEmpty()
{
    return this->name_.isEmpty() && this->desc_.isEmpty();
}

QDebug operator<<(QDebug stream, const Test &test)
{
    stream << "Текущий тест: " << test.name() << Qt::endl;
    stream << "Описание: " << test.desc();

    return stream;
}
