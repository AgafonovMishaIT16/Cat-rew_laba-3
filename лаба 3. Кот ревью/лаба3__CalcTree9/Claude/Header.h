#pragma once

#include <string>
#include <vector>


/**
 * Определяет три типа узлов, которые могут присутствовать
 * в дереве выражений.
 */
enum class ENodeType
{
    Number,          // Целочисленный литерал в диапазоне [1 .. 30]
    Variable,        // Идентификатор 'x'
    BinaryOperator   // Один из:  +  -  * /  %  ^
};


/**
 * Отдельный узел в дереве выражений.
 *
 * Модель владения: FExpressionTree владеет всеми узлами, выделенными во время Build().
 * Узлы не должны удаляться извне дерева.
 */
struct FTreeNode
{
    ENodeType    NodeType;    // Какая ветвь перечисления активна
    int          Value;       // Действительно, когда NodeType == Number
    char         Op;          // Действительно, когда NodeType == BinaryOperator
    FTreeNode* Left;        // nullptr для Number / Variable
    FTreeNode* Right;       // nullptr для Number / Variable

    /** Создает узел типа Number (число). */
    explicit FTreeNode(int InValue);

    /** Создает узел типа Variable (представляет 'x'). */
    FTreeNode();

    /** Создает узел типа BinaryOperator (бинарный оператор). Берет на себя владение потомками. */
    FTreeNode(char InOp, FTreeNode* InLeft, FTreeNode* InRight);
};


/**
 * Владеет и управляет бинарным деревом выражений, построенным из строки в инфиксной нотации.
 *
 * Поддерживаемые операторы (по возрастанию приоритета):
 * + -   (аддитивные,        левоассоциативные)
 * * / % (мультипликативные, левоассоциативные)
 * ^     (возведение в степень, левоассоциативное по спецификации)
 *
 * Операнды: целочисленные литералы в диапазоне [1 .. 30] и переменная 'x'.
 */
class FExpressionTree
{
public:
    FExpressionTree();
    ~FExpressionTree();


    /**
     * Выполняет синтаксический анализ InExpression (в инфиксной нотации) и строит внутреннее дерево.
     * return true в случае успеха; false при любой ошибке токенизации или парсинга.
     * Вызовите GetLastError() для получения понятного человеку описания ошибки.
     */
    bool Build(const std::string& InExpression);



    /**
     * Вычисляет значение дерева, где переменная 'x' принимает значение XValue.
     * Все промежуточные результаты используют стандартную арифметику int.
     * throws std::runtime_error при делении или взятии остатка от деления на ноль.
     */
    int Evaluate(int XValue) const;



    /**
     * Обходит все дерево снизу вверх и заменяет каждое поддерево, корнем которого
     * является оператор '+', а правым потомком — переменная 'x', на
     * эквивалентное поддерево, где потомки поменяны местами:
     *
     * (A + x)   →   (x + A)
     */
    void TransformAddX();


    /** Возвращает текущее дерево, сериализованное в префиксной (польской) нотации. */
    std::string ToPrefixString()  const;

    /** Возвращает текущее дерево, сериализованное в постфиксной (ОПН) нотации. */
    std::string ToPostfixString() const;

    /**
     * Возвращает текущее дерево, сериализованное в инфиксной нотации с полным
     * (избыточным) набором скобок вокруг каждой бинарной операции.
     */
    std::string ToInfixString() const;



    /** Возвращает последнее сообщение об ошибке, установленное функцией Build(). */
    const std::string& GetLastError() const;

private:

    bool TokenizeExpression(const std::string& InExpression);

    FTreeNode* ParseAddSub();
    FTreeNode* ParseMulDivMod();
    FTreeNode* ParsePower();
    FTreeNode* ParsePrimary();

    int         EvaluateNode(const FTreeNode* Node, int XValue) const;
    void        TransformNode(FTreeNode* Node);

    std::string NodeToPrefix(const FTreeNode* Node) const;
    std::string NodeToPostfix(const FTreeNode* Node) const;
    std::string NodeToInfix(const FTreeNode* Node) const;

    void DeleteSubtree(FTreeNode* Node);

    FTreeNode* Root;
    std::vector<std::string> Tokens;
    size_t                   TokenPos;
    std::string              LastError;
};



/**
 * Проверяет, что каждый символ в Content принадлежит набору символов,
 * которые может содержать допустимое арифметическое выражение в инфиксной нотации.
 * Эту функцию следует вызывать перед Build(), чтобы предоставить четкую ошибку
 * при некорректном формате входного файла.
 *
 * Разрешенные символы:  0-9  x  +  -  * /  %  ^  (  )  пробельные символы
 *
 * param[in]  Content   Исходный текст, прочитанный из входного файла.
 * param[out] OutError  Человекочитаемое описание первого недопустимого символа.
 * return true, когда Content проходит проверку; false в противном случае.
 */
bool ValidateRawInput(const std::string& Content, std::string& OutError);