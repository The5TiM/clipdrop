#include "contentanalyzer.h"


ContentAnalyzer::ContentAnalyzer() {}

ContentType ContentAnalyzer::analyzeContent(const QString &text) {
    if (text.isEmpty()) {
        return ContentType::text;
    } else if (isLink(text)) {
        return ContentType::link;
    } else if (isColor(text)) {
        return ContentType::color;
    } else if (isCode(text)) {
        return ContentType::code;
    } else {
        return ContentType::text;
    }
}

bool ContentAnalyzer::isLink(const QString &text) {
    return text.startsWith("http://") || text.startsWith("https://") || text.startsWith("ftp://");
}

bool ContentAnalyzer::isCode(const QString &text) {
    if (text.isEmpty()) return false;

    QString codeSymbols = "{}[]();=<>+*/%|&_";
    int specialCharCount = 0;
    for (QChar c : text) {
        if (codeSymbols.contains(c)) {
            specialCharCount++;
        }
    }

    int keywordCount = 0;
    QStringList keywords = {
        // C/C++/Java
        "if", "else", "for", "while", "return", "int", "float", "double", "void", "char", "bool",
        "class", "struct", "public", "private", "protected", "static", "const", "new", "delete",
        "try", "catch", "switch", "case", "break", "continue", "namespace", "using", "typedef",
        "include", "define", "ref", "nullptr", "malloc", "realloc", "sizeof", 

        // Python
        "def", "import", "from", "elif", "except", "print", "True", "False", "len", 

        // JavaScript / TypeScript
        "function", "var", "let", "const", "async", "await", "typeof", 

        "main", "true", "false",

        // SQL
        "SELECT", "INSERT", "UPDATE", "DELETE", "FROM", "WHERE", "JOIN", "INNER", "CREATE", "TABLE", "ALTER", "DROP", "ORDER BY", "GROUP BY", "HAVING", "DISTINCT", "VALUES", "SET", "NULL", "NOT NULL", "PRIMARY KEY", "FOREIGN KEY",

        // Html / Css
        "div", "span", "id", "href", "src", "margin", "padding", "p", "img", "ul", "li", "ol", "tr", "td", "th", "meta",
    };

    static const QRegularExpression wordSeparator("\\W+");
    const QStringList words = text.split(wordSeparator);

    for (const QString &word : words) {
        if (keywords.contains(word)) {
            keywordCount++;
        }
    }

    int length = text.length();
    int requiredSpecialCharCount = qMax(3, length / 50);
    int requiredKeywordCount = qMax(2, length / 150);

    if (specialCharCount >= 2 * requiredSpecialCharCount || keywordCount >= 2 * requiredKeywordCount) {
        return true;
    }

    return specialCharCount >= requiredSpecialCharCount && keywordCount >= requiredKeywordCount;
}

bool ContentAnalyzer::isColor(const QString &text) {
    QColor color(text);
    if (color.isValid()) {
        return true;
    } else {
        if (text.length() != 6 && text.length() != 3) {
            return false;
        }
        QString colorText = "#" + text;
        QColor color2(colorText);
        return color2.isValid();
    }
}

