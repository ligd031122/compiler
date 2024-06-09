#define _CRT_SECURE_NO_WARNINGS
#include "词法分析.h"
#include <ctype.h>
#include <iostream>
#include <set>
#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;

#define maxRow 200
#define maxColumn 2000
#define maxLen 11

char token[40], token1[40];
char Parsein[300];
char Parseout[300];
char tree_map[maxRow][maxColumn];
int maxRowNum = 0;

struct node
{
    string st;
    vector<node*> son;
    int cnt;
    node* addSon(string s)
    {
        node* Son = new node();
        Son->st = s;
        son.push_back(Son);
        return Son;
    }
} *Root;

void rowLookup();
int program();
int declaration_list(node* fa);
int declaration_stat(node* fa);
int statement_list(node* fa);
int statement(node* fa);
int if_stat(node* fa);
int while_stat(node* fa);
int for_stat(node* fa);
int read_stat(node* fa);
int write_stat(node* fa);
int switch_stat(node* fa);
int compound_stat(node* fa);
int expression_stat(node* fa);
int expression(node* fa);
int bool_expr(node* fa);
int additive_expr(node* fa);
int term(node* fa);
int factor(node* fa);
int fun_declaration(node* fa);
int fun_body(node* fa);
int main_declaration(node* fa);
int call_stat(node* fa);

void rowLookup()
{
    // 如果当前行的token数减1后为0，则移到下一行
    if (!(--rowTokenNum[rowIndex]))
        rowIndex++;
}

void out(node* p)
{  //输出
    p->addSon(token1);
}

void draw_line(int x, int y1, int y2)
{
    for (int i = y1 + 1; i < y2; i++)
        tree_map[x][i] = '-';
}

//输出字符串
void string_out(int x, int y, string s)
{
    int l = y - maxLen / 2;
    int len = s.length();
    l += (maxLen - len) / 2;

    for (int i = 0; i < len; i++)
        tree_map[x][l++] = s[i];
}
//输出语法树
void tree_out(int x, int y, node* p) {
    string_out(x, y, p->st);
    int num = p->son.size();
    if (num == 0) return;
    tree_map[x + 1][y] = ' ';
    int len = p->cnt;
    std::vector<int> pos(num);  // 使用 vector 来动态分配数组
    pos[0] = y - len * maxLen / 2 + p->son[0]->cnt * maxLen / 2;
    for (int i = 1; i < num; i++) {
        pos[i] = pos[i - 1] + (p->son[i - 1]->cnt + p->son[i]->cnt) / 2 * maxLen + 1;
    }

    for (int i = 0; i < num; i++) {
        tree_map[x + 3][pos[i]] = ' ';
        tree_out(x + 4, pos[i], p->son[i]);
    }
    return;
}

//让我们计算以下结点的数量
int tree_cnt(node* p)
{
    int nc = p->son.size();
    if (nc == 0)
    {
        p->cnt = 1;
        return 1;
    }
    int sum = 0;
    for (int i = 0; i < nc; i++)
    {
        sum += tree_cnt(p->son[i]);
    }
    p->cnt = sum;
    return sum;
}

//打印树
void print_tree()
{
    for (int i = 0; i < maxRow; i++)
    {
        for (int j = 0; j < maxColumn; j++)
        {
            if (tree_map[i][j] == 0 && tree_map[i][j + 1] != 0) {
                fprintf(fout, " ");
            }
            else if (tree_map[i][j] == 0) {
                fprintf(fout, "");
            }
            else
                fprintf(fout, "%c", tree_map[i][j]);
        }
        fprintf(fout, "\n");
    }
}

//打开测试
int TESTparse()
{
    int es = 0;
    strcpy(Parsein, Scanout);
    if ((fin = fopen(Parsein, "r")) == NULL)
    {
        printf("打开输入文件失败\n", Parsein);
        return es = 10;
    }
    strcpy(Parseout, "parseTREE.txt");
    if ((fout = fopen(Parseout, "w")) == NULL)
    {
        printf("打开输出文件失败\n");
        return es = 10;
    }

    maxRowNum = rowIndex;
    rowIndex = 1;

    es = program();

    return es;
}

int program()
{
    Root = new node();
    Root->st = "<TEST>";
    int es = 0;
    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    if (strcmp(token, "function") && strcmp(token, "main"))
    {
        printf("%s RowNum:%d\n", "Lack of function/main declare!!!", rowIndex);
        return es = 11;
    }

    while (strcmp(token, "function") == 0)
    {
        es = fun_declaration(Root);
        if (es > 0)
            return es;
    }

    if (strcmp(token, "main"))
    {
        printf("%s RowNum:%d\n", "Lack of main declare!!!", rowIndex);
        return es = 9;
    }
    es = main_declaration(Root);

    return es;
}
//解析函数声明
int fun_declaration(node* fa)
{
    node* p = fa->addSon("<fun_decl>");//创建节点
    out(p);
    int es = 0;

    fscanf(fin, "%s %s\n", token, token1);//读取
    rowLookup();
    if (strcmp(token, "ID"))//检测是否是“ID”
    {
        printf("%s RowNum:%d\n", "Lack of function name!!!", rowIndex);
        return es = 3;
    }
    out(p->addSon(token));

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    if (strcmp(token, "("))//检测是不是左括号
    {
        printf("%s RowNum:%d\n", "Lack of ( !!!", rowIndex);
        return es = 5;
    }
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    while (strcmp(token, "int") == 0)
    {
        out(p);
        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
        if (strcmp(token, "ID") == 0)//继续检测ID
            out(p->addSon(token));
        else
        {
            printf("%s RowNum:%d\n", "Lack of variable !!!", rowIndex);
            return es = 5;
        }

        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
    }
    if (strcmp(token, ")"))//检测右括号（结束
    {
        printf("%s RowNum:%d\n", "Lack of ) !!!", rowIndex);
        return es = 6;
    }
    out(p);

    es = fun_body(p);
    return es;
}

int main_declaration(node* fa)
{
    node* p = fa->addSon("<main_decl>");
    out(p);
    int es = 0;

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    if (strcmp(token, "("))
    {
        printf("%s RowNum:%d\n", "Lack of ( !!!", rowIndex);
        return es = 5;
    }
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    if (strcmp(token, ")"))
    {
        printf("%s RowNum:%d\n", "Lack of ) !!!", rowIndex);
        return es = 6;
    }
    out(p);

    es = fun_body(p);
    return es;
}

int fun_body(node* fa)
{
    node* p = fa->addSon("<fun_body>");
    int es = 0;

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    if (strcmp(token, "{"))
    {
        printf("%s RowNum:%d\n", "Lack of { !!!", rowIndex);
        return es = 1;
    }
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    es = declaration_list(p);
    if (es > 0)
        return es;

    es = statement_list(p);
    if (es > 0)
        return es;

    if (strcmp(token, "}"))
    {
        printf("%s RowNum:%d\n", "Lack of } !!!", rowIndex);
        return es = 2;
    }
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    return es;
}

int declaration_list(node* fa)
{
    node* p = fa->addSon("<decl_list>");
    int es = 0;
    while (strcmp(token, "int") == 0)
    {
        es = declaration_stat(p);
        if (es > 0)
            return es;
    }
    return es;
}

int declaration_stat(node* fa)
{
    node* p = fa->addSon("<decl_stat>");
    int es = 0;
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    if (strcmp(token, "ID"))
    {
        printf("%s RowNum:%d\n", "Lack of variable name !!!", rowIndex);
        return es = 3;
    }
    out(p->addSon(token));
    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    if (strcmp(token, "[") == 0)
    {
        out(p);

        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
        if (strcmp(token, "NUM"))
        {
            printf("%s RowNum:%d\n", "Lack of array size !!!", rowIndex);
            return es = 40;
        }
        out(p->addSon(token));

        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
        if (strcmp(token, "]"))
        {
            printf("%s RowNum:%d\n", "Lack of ] !!!", rowIndex);
            return es = 41;
        }
        out(p);

        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
    }

    if (strcmp(token, ";"))
    {
        printf("%s RowNum:%d\n", "Lack of ; !!!", rowIndex);
        return es = 4;
    }
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    return es;
}

int statement_list(node* fa)
{
    node* p = fa->addSon("<stat_list>");
    int es = 0;
    while (strcmp(token, "}"))
    {
        if (strcmp(token, "function") == 0 || strcmp(token, "main") == 0 || rowIndex > maxRowNum)
        {
            printf("%s RowNum:%d %d\n", "Lack of } !!!", rowIndex, maxRowNum);
            return es = 2;
        }
        es = statement(p);
        if (es > 0)
            return es;
    }
    return es;
}

int statement(node* fa)
{
    node* p = fa->addSon("<statement>");
    if (strcmp(token, "if") == 0)
        return if_stat(p);
    if (strcmp(token, "switch") == 0)
        return switch_stat(p);
    if (strcmp(token, "while") == 0)
        return while_stat(p);
    if (strcmp(token, "for") == 0)
        return for_stat(p);
    if (strcmp(token, "read") == 0)
        return read_stat(p);
    if (strcmp(token, "write") == 0)
        return write_stat(p);
    if (strcmp(token, "{") == 0)
        return compound_stat(p);
    if (strcmp(token, "call") == 0)
        return call_stat(p);
    if ((strcmp(token, "ID") == 0 || strcmp(token, "NUM") == 0 || strcmp(token, "(") == 0) || strcmp(token, ";") == 0)
        return expression_stat(p);

    return 0;
}

int if_stat(node* fa)
{
    node* p = fa->addSon("<if_stat>");
    int es = 0;
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    if (strcmp(token, "("))
    {
        printf("%s RowNum:%d\n", "Lack of ( !!!", rowIndex);
        return es = 5;
    }
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    es = expression(p);
    if (es > 0)
        return es;

    if (strcmp(token, ")"))
    {
        printf("%s RowNum:%d\n", "Lack of ) !!!", rowIndex);
        return es = 6;
    }
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    es = statement(p);
    if (es > 0)
        return es;

    if (strcmp(token, "else") == 0)
    {
        out(p);
        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
        es = statement(p);
        if (es > 0)
            return es;
    }
    return es;
}

int switch_stat(node* fa)
{
    node* p = fa->addSon("<swit_stat>");
    int es = 0;
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    if (strcmp(token, "("))
    {
        printf("%s RowNum:%d\n", "Lack of ( !!!", rowIndex);
        return es = 5;
    }
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    if (strcmp(token, "ID"))
    {
        printf("%s RowNum:%d\n", "Lack of variable name !!!", rowIndex);
        return es = 3;
    }
    out(p->addSon(token));

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    if (strcmp(token, ")"))
    {
        printf("%s RowNum:%d\n", "Lack of ) !!!", rowIndex);
        return es = 6;
    }
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    if (strcmp(token, "{"))
    {
        printf("%s RowNum:%d\n", "Lack of { !!!", rowIndex);
        return es = 1;
    }
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    while (strcmp(token, "case") == 0)
    {
        out(p);
        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
        if (strcmp(token, ":"))
        {
            printf("%s RowNum:%d\n", "Lack of :(switch) !!!", rowIndex);
            return es = 13;
        }
        out(p);

        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
        if (strcmp(token, "NUM"))
        {
            printf("%s RowNum:%d\n", "Lack of number(switch) !!!", rowIndex);
            return es = 7;
        }
        out(p->addSon(token));

        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
        es = statement(p);
        if (es > 0)
            return es;

        if (strcmp(token, "break"))
        {
            printf("%s RowNum:%d\n", "Lack of break(switch) !!!", rowIndex);
            return es = 15;
        }
        out(p);

        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
        if (strcmp(token, ";"))
        {
            printf("%s RowNum:%d\n", "Lack of ; !!!", rowIndex);
            return es = 4;
        }
        out(p);

        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
    }
    if (strcmp(token, "default") == 0)
    {
        out(p);
        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
        if (strcmp(token, ":"))
        {
            printf("%s RowNum:%d\n", "Lack of : !!!", rowIndex);
            return es = 13;
        }
        out(p);
        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
        es = statement(p);
        if (es > 0)
            return es;
    }

    if (strcmp(token, "}"))
    {
        printf("%s RowNum:%d\n", "Lack of } !!!", rowIndex);
        return es = 2;
    }
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    return es;
}

int while_stat(node* fa)
{
    node* p = fa->addSon("<whil_stat>");
    int es = 0;
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    out(p);
    if (strcmp(token, "("))
    {
        printf("%s RowNum:%d\n", "Lack of ( !!!", rowIndex);
        return es = 5;
    }

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();

    es = expression(p);
    if (es > 0)
        return es;
    out(p);

    if (strcmp(token, ")"))
    {
        printf("%s RowNum:%d\n", "Lack of ) !!!", rowIndex);
        return es = 6;
    }
    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    es = statement(p);
    return es;
}

int for_stat(node* fa)
{
    node* p = fa->addSon("<for_stat>");
    out(p);
    int es = 0;
    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    out(p);

    if (strcmp(token, "("))
    {
        printf("%s RowNum:%d\n", "Lack of ( !!!", rowIndex);
        return es = 5;
    }
    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    es = expression(p);
    if (es > 0)
        return es;
    if (strcmp(token, ";"))
    {
        printf("%s RowNum:%d\n", "Lack of ; !!!", rowIndex);
        return es = 4;
    }
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    es = expression(p);
    if (es > 0)
        return es;
    if (strcmp(token, ";"))
    {
        printf("%s RowNum:%d\n", "Lack of ; !!!", rowIndex);
        return es = 4;
    }
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    es = expression(p);
    if (es > 0)
        return es;
    if (strcmp(token, ")"))
    {
        printf("%s RowNum:%d\n", "Lack of ) !!!", rowIndex);
        return es = 6;
    }
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    es = statement(p);
    return es;
}

int write_stat(node* fa)
{
    node* p = fa->addSon("<writ_stat>");
    out(p);
    int es = 0;
    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    es = expression(p);
    if (es > 0)
        return es;
    if (strcmp(token, ";"))
    {
        printf("%s RowNum:%d\n", "Lack of ; !!!", rowIndex);
        return es = 4;
    }
    out(p);
    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    return es;
}

int read_stat(node* fa)
{
    node* p = fa->addSon("<read_stat>");
    out(p);
    int es = 0;
    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    if (strcmp(token, "ID"))
    {
        printf("%s RowNum:%d\n", "Lack of variable name !!!", rowIndex);
        return es = 3;
    }
    out(p->addSon(token));

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    if (strcmp(token, ";"))
    {
        printf("%s RowNum:%d\n", "Lack of ; !!!", rowIndex);
        return es = 4;
    }
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    return es;
}

int compound_stat(node* fa)
{
    node* p = fa->addSon("<comp_stat>");
    out(p);
    int es = 0;
    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    es = statement_list(p);

    if (strcmp(token, "}"))
    {
        printf("%s RowNum:%d\n", "Lack of } !!!", rowIndex);
        return es = 2;
    }
    out(p);
    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    return es;
}

int expression_stat(node* fa)
{
    node* p = fa->addSon("<expr_stat>");
    int es = 0;
    if (strcmp(token, ";") == 0)
    {
        out(p);
        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
        return es;
    }
    es = expression(p);
    if (es > 0)
        return es;
    if (strcmp(token, ";") == 0)
    {
        out(p);
        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
        return es;
    }
    else
    {
        printf("%s RowNum:%d\n", "Lack of ; !!!", rowIndex);
        return es = 4;
    }
}

int call_stat(node* fa)
{
    node* p = fa->addSon("<call_stat>");
    int es = 0;
    out(p);
    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    if (strcmp(token, "ID"))
    {
        printf("%s RowNum:%d\n", "Lack of function name !!!", rowIndex);
        return es = 3;
    }
    out(p->addSon(token));

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    if (strcmp(token, "("))
    {
        printf("%s RowNum:%d\n", "Lack of ( !!!", rowIndex);
        return es = 5;
    }
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    while (strcmp(token, "NUM") == 0)
    {
        out(p->addSon(token));
        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
    }

    if (strcmp(token, ")"))
    {
        printf("%s RowNum:%d\n", "Lack of ) !!!", rowIndex);
        return es = 6;
    }
    out(p);

    fscanf(fin, "%s %s\n", token, token1);
    rowLookup();
    return es;
}

int expression(node* fa)
{
    node* p = fa->addSon("<expression>");
    int es = 0, fileadd;
    int isArray = 0;
    char token2[40], token3[40];
    if (strcmp(token, "ID") == 0)
    {
        fileadd = ftell(fin);
        fscanf(fin, "%s %s\n", token2, token3);

        if (strcmp(token2, "[") == 0)
        {
            isArray = 1;
            // rowLookup();
            strcpy(token2, "ARRAY");
            node* q = p->addSon(token2);
            out(q->addSon(token));
            q->addSon(token3);

            fscanf(fin, "%s %s\n", token2, token3);
            // rowLookup();
            if (strcmp(token2, "NUM") == 0)
            {
                q->addSon(token2)->addSon(token3);
            }
            else
            {
                printf("%s RowNum:%d\n", "Lack of NUM !!!", rowIndex);
                return es = 6;
            }

            fscanf(fin, "%s %s\n", token2, token3);
            // rowLookup();
            if (strcmp(token2, "]") == 0)
            {
                q->addSon(token3);
            }
            else
            {
                printf("%s RowNum:%d\n", "Lack of ] !!!", rowIndex);
                return es = 6;
            }

            //fileadd = ftell(fin);
            fscanf(fin, "%s %s\n", token2, token3);
        }

        if (strcmp(token2, "=") == 0)
        {
            rowLookup();
            if (isArray)
            {
                rowLookup();
                rowLookup();
                rowLookup();
            }

            if (!isArray)
                out(p->addSon(token));
            p->addSon(token2);
            fscanf(fin, "%s %s\n", token, token1);
            rowLookup();
            es = bool_expr(p);
            if (es > 0)
                return es;
        }
        else
        {
            fseek(fin, fileadd, 0);
            if (isArray)
                p->son.pop_back();
            es = bool_expr(p);
            if (es > 0)
                return es;
        }
    }
    else
        es = bool_expr(p);
    return es;
}

int bool_expr(node* fa)
{
    node* p = fa->addSon("<bool_expr>");
    int es = 0;
    es = additive_expr(p);
    if (es > 0)
        return es;
    if (strcmp(token, ">") == 0 || strcmp(token, ">=") == 0 || strcmp(token, "<") == 0 || strcmp(token, "<=") == 0 || strcmp(token, "==") == 0 || strcmp(token, "!=") == 0)
    {
        out(p);
        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
        es = additive_expr(p);
    }
    return es;
}

int additive_expr(node* fa)
{
    node* p = fa->addSon("<addi_expr>");
    int es = 0;
    es = term(p);
    if (es > 0)
        return es;
    while (strcmp(token, "+") == 0 || strcmp(token, "-") == 0)
    {
        out(p);
        fscanf(fin, "%s %s", token, token1);
        rowLookup();
        es = term(p);
    }
    return es;
}

int term(node* fa)
{
    node* p = fa->addSon("<term>");
    int es = 0;
    es = factor(p);
    if (es > 0)
        return es;
    while (strcmp(token, "*") == 0 || strcmp(token, "/") == 0)
    {
        out(p);
        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
        es = factor(p);
    }
    return es;
}

int factor(node* fa)
{
    node* p = fa->addSon("<factor>");
    int es = 0;
    int fileadd;
    char tokenc[40], token1c[40];

    if (strcmp(token, "(") == 0)
    {
        out(p);
        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
        es = additive_expr(p);
        if (es > 0)
            return es;
        if (strcmp(token, ")"))
        {
            printf("%s RowNum:%d\n", "Lack of ) !!!", rowIndex);
            return es = 6;
        }
        out(p);
        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
    }

    if (strcmp(token, "ID") == 0)
    {
        fileadd = ftell(fin);
        fscanf(fin, "%s %s\n", tokenc, token1c);
        if (strcmp(tokenc, "[") == 0)
        {
            rowLookup();
            strcpy(tokenc, "ARRAY");
            node* q = p->addSon(tokenc);
            out(q->addSon(token));
            q->addSon(token1c);

            fscanf(fin, "%s %s\n", tokenc, token1c);
            rowLookup();
            if (strcmp(tokenc, "NUM") == 0)
            {
                node* r = q->addSon(tokenc);
                r->addSon(token1c);
            }
            else
            {
                printf("%s RowNum:%d\n", "Lack of NUM !!!", rowIndex);
                return es = 6;
            }

            fscanf(fin, "%s %s\n", tokenc, token1c);
            rowLookup();
            if (strcmp(tokenc, "]") == 0)
            {
                q->addSon(tokenc);
            }
            else
            {
                printf("%s RowNum:%d\n", "Lack of ] !!!", rowIndex);
                return es = 6;
            }
        }
        else
        {
            out(p->addSon(token));
            fseek(fin, fileadd, 0);
        }

        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
        return es;
    }

    if (strcmp(token, "NUM") == 0)
    {
        out(p->addSon(token));
        fscanf(fin, "%s %s\n", token, token1);
        rowLookup();
        return es;
    }
    else
    {
        printf("%s RowNum:%d\n", "Lack of variable/number !!!", rowIndex);
        return es = 7;
    }

    return es;
}
