#include "batchparser.h"
#include <QStringList>

namespace
{
struct BatchArgument
{
    QString name;
    QVector<int> values;
};

enum class CharType {
    Null,
    Digit,
    Hyphen,
    Comma
};

QChar charTypeToQChar(CharType t)
{
    if (t == CharType::Hyphen) {
        return '-';
    }
    if (t == CharType::Comma) {
        return ',';
    }
    return QChar(-1); //Null character
}

std::variant<QString, QVector<int>> parseValues(const QString& token)
{
    int lastValue = 0;
    int pos = 0;
    int hyphenPairValue = 0;
    int hyphenPairFlag = 0;
    CharType lastCharType = CharType::Null;

    auto reset = [&] () {
        lastValue = 0;
        hyphenPairFlag = 0;
        hyphenPairValue = 0;
    };

    QVector<int> res;

    for (; pos <= token.length(); ++pos) {
        if (pos == token.length() || token[pos] == ',') {
            if (lastCharType == CharType::Hyphen || lastCharType == CharType::Comma) {
                return QStringLiteral("\"%1\"h中出现非法表达式：冗余的运算符'%2'").arg(token).arg(charTypeToQChar(lastCharType));
            }
            if (hyphenPairFlag == 1) {
                if (lastValue < hyphenPairValue) {
                    return QStringLiteral("\"%1\"中出现非法表达式：范围终止值%2小于起始值%3").arg(token).arg(lastValue).arg(hyphenPairValue);
                }
                for (int i = hyphenPairValue; i <= lastValue; i++) {
                    res.push_back(i);
                }
            }
            else {
                res.push_back(lastValue);
            }

            if (pos == token.length()) {
                return res;
            }
            else {
                reset();
                lastCharType = CharType::Comma;
            }
        }
        else if (token[pos].isDigit()) {
            lastValue = token[pos].toLatin1() - '0' + lastValue * 10;
            lastCharType = CharType::Digit;
        }
        else if (token[pos] == '-') {
            if (lastCharType != CharType::Digit || hyphenPairFlag >= 1) {
                return QStringLiteral("\"%1\"中出现非法表达式：冗余的运算符'-'").arg(token);
            }
            hyphenPairFlag = 1;
            hyphenPairValue = lastValue;
            lastValue = 0;
            lastCharType = CharType::Hyphen;
        }
        else {
            return QStringLiteral("\"%1\"中出现非法表达式：无法识别的字符%2").arg(token).arg(token[pos]);
        }
    }

    return res;
}

std::variant<QString, BatchArgument> parseToken(const QString& token)
{
    QStringList splitByAssignment = token.split(QChar('='), Qt::SkipEmptyParts);
    if (splitByAssignment.length() > 2) {
        return QStringLiteral("\"%1\"中出现非法表达式：冗余的运算符'='").arg(token);
    }

    BatchArgument res;
    res.name = splitByAssignment[0];

    auto valueRes = parseValues(splitByAssignment[1]);
    if (valueRes.index() == 0) {
        return std::get<0>(valueRes);
    }
    res.values = std::get<1>(valueRes);

    return res;
}

std::variant<QString, QVector<BatchArgument>> parseArgs(const QString& args)
{
    QVector<BatchArgument> res;
    QStringList tokens = args.split(QChar(' '), Qt::SkipEmptyParts);

    for (const auto& token: tokens) {
        auto tokenRes = parseToken(token);
        if (tokenRes.index() == 0) {
            return std::get<0>(tokenRes);
        }
        res.push_back(std::get<1>(tokenRes));
    }
    return res;
}

int findArgument(const QString& argName, const QVector<BatchArgument>& args)
{
    for (int i = 0; i < args.length(); i++) {
        if (args[i].name == argName) {
            return i;
        }
    }
    return -1;
}

std::variant<QString, QVector<QString>> parsePath(const QString& batch, const QVector<BatchArgument>& args)
{
    if (args.empty()) {
        return QVector<QString>{batch}; //Single element
    }

    QVector<QString> res;
    QVector<int> indices(args.length(), 0);
    while (indices[0] < args[0].values.length()) {
        QString cur;
        for (int i = 0; i < batch.length(); i++) {
            if (batch[i] == '{') {
                int next = i + 1;
                for (; next < batch.length() && batch[next] != '}'; next++) {}
                if (next == batch.length()) {
                    return QStringLiteral("\"%1\"中出现非法表达式：缺失定界符'}'").arg(batch);
                }
                //batch[next] == '}'
                QString argName = batch.mid(i + 1, next - i - 1);
                int ai = findArgument(argName, args);
                if (ai == -1) {
                    return QStringLiteral("\"%1\"中出现非法表达式：参数%2未定义").arg(batch).arg(argName);
                }
                cur.push_back(QString::number(args[ai].values[indices[ai]]));
                i = next;
            }
            else {
                cur.push_back(batch[i]);
            }
        }
        res.push_back(cur);

        for (int i = indices.length() - 1; i >= 0; i--) {
            indices[i] += 1;
            if (indices[i] == args[i].values.length() && i > 0) {
                indices[i] = 0;
            }
            else {
                break;
            }
        }
    }

    return res;
}

} //end anonymous namespace

std::variant<QString, QVector<TaskProperty>> parseBatch(const QString& urlBatch, const QString& dirBatch, const QString& args)
{
    auto argsRes = parseArgs(args);
    if (argsRes.index() == 0) {
        return QStringLiteral("解析参数时出现错误：%1").arg(std::get<0>(argsRes));
    }
    const QVector<BatchArgument>& argsVector = std::get<1>(argsRes);

    auto urlRes = parsePath(urlBatch, argsVector);
    auto dirRes = parsePath(dirBatch, argsVector);
    if (urlRes.index() == 0) {
        return std::get<0>(urlRes);
    }
    if (dirRes.index() == 0) {
        return std::get<0>(dirRes);
    }

    QVector<TaskProperty> res;
    const auto& u = std::get<1>(urlRes);
    const auto& d = std::get<1>(dirRes);

    for (int i = 0; i < u.length(); i++) {
        TaskProperty p;
        p.dir = d[i];
        p.url = u[i];
        res.push_back(p);
    }
    return res;
}
