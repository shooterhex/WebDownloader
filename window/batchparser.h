#ifndef BATCHPARSER_H
#define BATCHPARSER_H

#include <variant>
#include <QString>
#include <QVector>

struct TaskProperty
{
    QString url;
    QString dir;
};

std::variant<QString, QVector<TaskProperty>> parseBatch(const QString& urlBatch, const QString& dirBatch, const QString& args);

#endif // BATCHPARSER_H
