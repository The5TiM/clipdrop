#ifndef CONTENTANALYZER_H
#define CONTENTANALYZER_H

#include <QString>
#include <QColor>
#include <QStringList>
#include <QRegularExpression>

#include <contenttype.h>

class ContentAnalyzer
{
public:
    ContentAnalyzer();
    static ContentType analyzeContent(const QString &text);
    static bool isLink(const QString &text);
    static bool isCode(const QString &text);
    static bool isColor(const QString &text);

};

#endif // CONTENTANALYZER_H
