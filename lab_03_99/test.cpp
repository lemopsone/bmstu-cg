 #include "test.h"

Test::Test() {}

Test::Test(QString name, QString desc, QList<LineData> lines, QList<SpectreData> spectres, QColor backgroundColor)
{
    this->name_ = name;
    this->desc_ = desc;
    this->lines_ = lines;
    this->spectres_ = spectres;
    this->backgroundColor_ = backgroundColor;
}

QString Test::name() const
{
    return this->name_;
}

QString Test::desc() const
{
    return this->desc_;
}

QList<LineData> Test::lines() const
{
    return this->lines_;
}

QList<SpectreData> Test::spectres() const
{
    return this->spectres_;
}

QColor Test::backgroundColor() const
{
    return this->backgroundColor_;
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
