#define _CRT_SECURE_NO_WARNINGS
#include "�﷨����.h"
#include <cstring>
#include <ctype.h>
#include<stdlib.h>
#include <iostream>
#include <set>
#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;
#define maxSymbolIndex 100
#define maxCodeIndex 100
//��׼����Զ�����﷨����ͷ�ļ�
char tokenCopy[40];
int codesIndex = 0;
int symbolIndex = 0;
int offset = 0;
int arraySize = 0;
int IDrow = 1;
int funcIndex = 0;
int errorNum = 0;
FILE* fpCodeText;
FILE* fpCodeBinary;
//�������е�ȫ�ֱ���
enum Category_symbol
{
    variable,
    function,
    array1,
};
//������ŵ���𣨰������������������飩
//array��VS��ԭ������ͻ
struct symbolNode
{
    char name[40];
    enum Category_symbol kind;
    int address;
    int funcIndex;
    int para;
} symbol[maxSymbolIndex];
//�������ű��е����ƣ���𣬵�ַ�Ͳ���
typedef struct codeNode
{
    char opt[10];
    int operand;
} Code;
Code codes[maxCodeIndex];
//�����������Ͳ�����
int insert_Symbol(Category_symbol category, char* name);
int lookup_Symbol(char* name, int* position, int funcIndex);
int lookup_Func_Symbol(char* name, int* position);
void rowIDLookup();
int fun_name_code(node* fa);
int var_name_code(node* fa);
int arr_name_code(node* fa);

int fun_declaration_code(node* fa);
int main_declaration_code(node* fa);
int fun_body_code(node* fa);
int declaration_list_code(node* fa);
int declaration_stat_code(node* fa);
int statement_list_code(node* fa);
int statement_code(node* fa);

int if_stat_code(node* fa);
int switch_stat_code(node* fa);
int while_stat_code(node* fa);
int for_stat_code(node* fa);
int read_stat_code(node* fa);
int write_stat_code(node* fa);
int compound_stat_code(node* fa);
int call_stat_code(node* fa);

int expression_stat_code(node* fa);
int expression_code(node* fa);
int bool_expr_code(node* fa);
int additive_expr_code(node* fa);
int term_code(node* fa);
int factor_code(node* fa);
//��������

//�����Ų�����ű������ݷ������ͼ���ظ����塣
int insert_Symbol(Category_symbol category, char* name)
{
    int i, es = 0;

    if (symbolIndex >= maxSymbolIndex)
        return es = 21;

    switch (category)
    {

    case function:
    {
        for (i = symbolIndex - 1; i >= 0; i--)
            if (strcmp(symbol[i].name, name) == 0 && symbol[i].kind == function)
            {
                printf("��%d�к�����%s�Ѷ����!\n", IDrow, name);
                errorNum++;
                return es = 32;
            }
        symbol[symbolIndex].kind = function;
        funcIndex = symbolIndex;
        symbol[symbolIndex].funcIndex = funcIndex;
        break;
    }
    case variable:
    {
        for (i = symbolIndex - 1; i >= 0; i--)
            if (strcmp(symbol[i].name, name) == 0 && symbol[i].kind == variable && symbol[i].funcIndex == funcIndex)
            {
                printf("��%d�б���%s�Ѿ������!\n", IDrow, name);
                errorNum++;
                return es = 22;
            }
        symbol[symbolIndex].kind = variable;
        symbol[symbolIndex].address = offset++;
        symbol[symbolIndex].funcIndex = funcIndex;
        break;
    }

    case array1:
    {
        for (i = symbolIndex - 1; i >= 0; i--)
            if (strcmp(symbol[i].name, name) == 0 && symbol[i].kind == array1 && symbol[i].funcIndex == funcIndex)
            {
                printf("��%d������%s�Ѿ��������\n", IDrow, name);
                errorNum++;
                return es = 22;
            }
        symbol[symbolIndex].kind = array1;
        symbol[symbolIndex].address = offset;
        offset += arraySize;
        symbol[symbolIndex].funcIndex = funcIndex;
        break;
    }
    }
    strcpy(symbol[symbolIndex].name, name);
    symbolIndex++;
    return es;
}

//�ڷ��ű��в��ұ������ţ�����ҵ��򷵻���λ�ã����򷵻ش���
int lookup_Symbol(char* name, int* position, int funcIndex)
{
    int i, es = 0;
    for (i = 0; i < symbolIndex; i++)
        if ((strcmp(symbol[i].name, name) == 0) && symbol[i].kind == variable && symbol[i].funcIndex == funcIndex)
        {
            *position = i;
            return es;
        }
    errorNum++;
    return es = 23;
}
//�˺����ڷ��ű��в����������
int lookup_Arr_Symbol(char* name, int* position, int funcIndex)
{
    int i, es = 0;
    for (i = 0; i < symbolIndex; i++)
        if ((strcmp(symbol[i].name, name) == 0) && symbol[i].kind == array1 && symbol[i].funcIndex == funcIndex)
        {
            *position = i;
            return es;
        }
    errorNum++;
    return es = 23;
}

//�˺����ڷ��ű��в��Һ�������
int lookup_Func_Symbol(char* name, int* position)
{
    int i, es = 0;
    for (i = 0; i < symbolIndex; i++)
        if ((strcmp(symbol[i].name, name) == 0) && symbol[i].kind == function)
        {
            *position = i;
            return es;
        }
    errorNum++;
    return es = 23;
}

//ID ROW ++,���µ�ǰ��
void rowIDLookup()
{
    while (!rowIDNum[IDrow])
        IDrow++;
    --rowIDNum[IDrow];
}

//��ʶ�𵽵ĺ������뵽���ű���
int fun_name_code(node* fa)
{
    int es = 0;
    for (int i = 0; i < fa->son.size(); i++)
    {
        strcpy(tokenCopy, fa->son[i]->st.c_str());
        es = insert_Symbol(function, tokenCopy);
    }
    return es;
}
//���ϣ����������뵽���ű���
int var_name_code(node* fa)
{
    int es = 0;
    for (int i = 0; i < fa->son.size(); i++)
    {
        strcpy(tokenCopy, fa->son[i]->st.c_str());
        es = insert_Symbol(variable, tokenCopy);
    }
    return es;
}
//���ϣ���������뵽���ű���
int arr_name_code(node* fa)
{
    int es = 0;
    for (int i = 0; i < fa->son.size(); i++)
    {
        strcpy(tokenCopy, fa->son[i]->st.c_str());
        es = insert_Symbol(array1, tokenCopy);
    }
    return es;
}

//���ļ�
//�����ɵĴ���������ı��Ͷ������ļ���
// �������﷨���еĽڵ�������Ӧ���м���롣
int TESTMediateCode(node* fa)
{
    int es = 0;
    if ((fpCodeText = fopen("code.txt", "w")) == NULL)
    {
        printf("���ļ�ʧ��\n");
        return (1);
    }
    if ((fpCodeBinary = fopen("codeBin.bin", "wb")) == NULL)
    {
        printf("���ļ�ʧ��\n");
        return (1);
    }

    strcpy(codes[codesIndex++].opt, "BR");

    for (int i = 0; i < fa->son.size(); i++)
    {
        strcpy(tokenCopy, fa->son[i]->st.c_str());
        if (strcmp(tokenCopy, "<fun_decl>") == 0)
        {
            es = fun_declaration_code(fa->son[i]);
        }
        if (strcmp(tokenCopy, "<main_decl>") == 0)
        {
            es = main_declaration_code(fa->son[i]);
        }
    }

    if ((es == 0) && errorNum == 0)
    {
        printf("\n\t\tsymbol_table\n");
        printf("index\tname\taddress\tkind\tfunc\tpara\n");
        for (int i = 0; i < symbolIndex; i++)
            printf("%d\t%s\t%d\t%d\t%d\t%d\n", i, symbol[i].name, symbol[i].address, symbol[i].kind, symbol[i].funcIndex, symbol[i].para);
        printf("\n");

        for (int i = 0; i < codesIndex; i++)
        {
            if (strcmp(codes[i].opt, "LOAD") == 0 || strcmp(codes[i].opt, "LOADI") == 0 || strcmp(codes[i].opt, "STO") == 0 ||
                strcmp(codes[i].opt, "BR") == 0 || strcmp(codes[i].opt, "BRF") == 0 || strcmp(codes[i].opt, "CAL") == 0 || strcmp(codes[i].opt, "ENTER") == 0)
                fprintf(fpCodeText, "%3d %-7s %d\n", i, codes[i].opt, codes[i].operand);
            else
                fprintf(fpCodeText, "%3d %-7s\n", i, codes[i].opt);
        }
        fclose(fpCodeText);

        fwrite(codes, sizeof(Code), codesIndex, fpCodeBinary);
        fclose(fpCodeBinary);
    }

    fclose(fin);
    fclose(fout);
    return errorNum;
}
//�����������ڵ㣬ʶ�𵽺���������"id"����
int fun_declaration_code(node* fa)
{
    int es = 0;
    offset = 2;

    strcpy(tokenCopy, fa->son[1]->st.c_str());
    if (strcmp(tokenCopy, "ID") == 0)
    {
        rowIDLookup();
        es = fun_name_code(fa->son[1]);
    }

    int i = 4;
    strcpy(tokenCopy, fa->son[i]->st.c_str());
    int para = 0;

    while (strcmp(tokenCopy, "ID") == 0)
    {
        rowIDLookup();
        es = var_name_code(fa->son[i]);
        i += 2;
        para++;
        strcpy(tokenCopy, fa->son[i]->st.c_str());
    }
    symbol[funcIndex].para = para;

    if (strcmp(tokenCopy, "<fun_body>") == 0)
    {
        es = fun_body_code(fa->son[i]);
    }

    return es;
}
//����������
int main_declaration_code(node* fa)
{
    int es = 0;
    offset = 2;
    codes[0].operand = codesIndex;
    for (int i = 0; i < fa->son.size(); i++)
    {
        strcpy(tokenCopy, fa->son[i]->st.c_str());
        if (strcmp(tokenCopy, "main") == 0)
        {
            es = insert_Symbol(function, const_cast<char*>("main"));
        }
        if (strcmp(tokenCopy, "<fun_body>") == 0)
        {
            es = fun_body_code(fa->son[i]);
        }
    }
    return es;
}


int fun_body_code(node* fa)
{
    symbol[funcIndex].address = codesIndex;

    int es = 0;
    for (int i = 0; i < fa->son.size(); i++)
    {
        strcpy(tokenCopy, fa->son[i]->st.c_str());
        if (strcmp(tokenCopy, "<decl_list>") == 0)
        {
            es = declaration_list_code(fa->son[i]);
        }
        else if (strcmp(tokenCopy, "<stat_list>") == 0)
        {
            strcpy(codes[codesIndex].opt, "ENTER");
            codes[codesIndex++].operand = offset;
            int off = 2;
            for (int i = 0; i < symbol[funcIndex].para; i++)
            {
                strcpy(codes[codesIndex].opt, "LOADI");
                codes[codesIndex++].operand = 0;
                strcpy(codes[codesIndex].opt, "STO");
                codes[codesIndex++].operand = off;
                off++;
            }
            es = statement_list_code(fa->son[i]);
        }
    }
    strcpy(codes[codesIndex++].opt, "RETURN");
    return es;
}
//
int declaration_list_code(node* fa)
{
    int es = 0;
    for (int i = 0; i < fa->son.size(); i++)
    {
        strcpy(tokenCopy, fa->son[i]->st.c_str());
        if (strcmp(tokenCopy, "<decl_stat>") == 0)
        {
            es = declaration_stat_code(fa->son[i]);
        }
    }
    return es;
}

int declaration_stat_code(node* fa)
{
    int es = 0;
    if (fa->son.size() == 3)
    {
        strcpy(tokenCopy, fa->son[1]->st.c_str());
        if (strcmp(tokenCopy, "ID") == 0)
        {
            rowIDLookup();
            es = var_name_code(fa->son[1]);
        }
    }
    if (fa->son.size() == 6)
    {
        strcpy(tokenCopy, fa->son[3]->st.c_str());
        if (strcmp(tokenCopy, "NUM") == 0)
        {
            strcpy(tokenCopy, fa->son[3]->son[0]->st.c_str());
            arraySize = atoi(tokenCopy);
        }

        strcpy(tokenCopy, fa->son[1]->st.c_str());
        if (strcmp(tokenCopy, "ID") == 0)
        {
            rowIDLookup();
            es = arr_name_code(fa->son[1]);
        }
    }
    return es;
}

int statement_list_code(node* fa)
{
    int es = 0;
    for (int i = 0; i < fa->son.size(); i++)
    {
        strcpy(tokenCopy, fa->son[i]->st.c_str());
        if (strcmp(tokenCopy, "<statement>") == 0)
        {
            es = statement_code(fa->son[i]);
        }
    }
    return es;
}

int statement_code(node* fa)
{
    for (int i = 0; i < fa->son.size(); i++)
    {
        strcpy(tokenCopy, fa->son[i]->st.c_str());
        if (strcmp(tokenCopy, "<if_stat>") == 0)
            return if_stat_code(fa->son[i]);
        if (strcmp(tokenCopy, "<swit_stat>") == 0)
            return switch_stat_code(fa->son[i]);
        if (strcmp(tokenCopy, "<whil_stat>") == 0)
            return while_stat_code(fa->son[i]);
        if (strcmp(tokenCopy, "<for_stat>") == 0)
            return for_stat_code(fa->son[i]);
        if (strcmp(tokenCopy, "<read_stat>") == 0)
            return read_stat_code(fa->son[i]);
        if (strcmp(tokenCopy, "<writ_stat>") == 0)
            return write_stat_code(fa->son[i]);
        if (strcmp(tokenCopy, "<comp_stat>") == 0)
            return compound_stat_code(fa->son[i]);
        if (strcmp(tokenCopy, "<call_stat>") == 0)
            return call_stat_code(fa->son[i]);
        if (strcmp(tokenCopy, "<expr_stat>") == 0)
            return expression_stat_code(fa->son[i]);
    }
    return 0;
}

int expression_stat_code(node* fa)
{
    int es = 0;
    for (int i = 0; i < fa->son.size(); i++)
    {
        strcpy(tokenCopy, fa->son[i]->st.c_str());
        if (strcmp(tokenCopy, "<expression>") == 0)
        {
            es = expression_code(fa->son[i]);
        }
    }
    return es;
}

int expression_code(node* fa)
{
    int es = 0;
    int symbolPos = 0;
    int arrOff = 0;
    if (fa->son.size() == 1)
    {
        strcpy(tokenCopy, fa->son[0]->st.c_str());
        if (strcmp(tokenCopy, "<bool_expr>") == 0)
        {
            es = bool_expr_code(fa->son[0]);
        }
    }
    else
    {
        for (int i = 0; i < fa->son.size(); i++)
        {
            strcpy(tokenCopy, fa->son[i]->st.c_str());
            if (strcmp(tokenCopy, "ID") == 0)
            {
                rowIDLookup();
                strcpy(tokenCopy, fa->son[i]->son[0]->st.c_str());
                es = lookup_Symbol(tokenCopy, &symbolPos, funcIndex);
                if (es > 0)
                {
                    printf("��%d�б���%sδ����!\n", IDrow, tokenCopy);
                    return es;
                }
            }

            if (strcmp(tokenCopy, "ARRAY") == 0)
            {
                rowIDLookup();
                strcpy(tokenCopy, fa->son[i]->son[0]->son[0]->st.c_str());
                es = lookup_Arr_Symbol(tokenCopy, &symbolPos, funcIndex);
                if (es > 0)
                {
                    printf("��%d������%sδ����!\n", IDrow, tokenCopy);
                    return es;
                }
                strcpy(tokenCopy, fa->son[i]->son[2]->son[0]->st.c_str());
                arrOff = atoi(tokenCopy);
            }

            if (strcmp(tokenCopy, "<bool_expr>") == 0)
            {
                es = bool_expr_code(fa->son[i]);
            }
        }

        strcpy(codes[codesIndex].opt, "STO");
        codes[codesIndex++].operand = symbol[symbolPos].address + arrOff;
    }
    return es;
}

int bool_expr_code(node* fa)
{
    int es = 0;
    strcpy(tokenCopy, fa->son[0]->st.c_str());
    if (strcmp(tokenCopy, "<addi_expr>") == 0)
    {
        es = additive_expr_code(fa->son[0]);
    }

    if (fa->son.size() == 3)
    {
        strcpy(tokenCopy, fa->son[2]->st.c_str());
        if (strcmp(tokenCopy, "<addi_expr>") == 0)
        {
            es = additive_expr_code(fa->son[2]);
        }

        strcpy(tokenCopy, fa->son[1]->st.c_str());
        if (strcmp(tokenCopy, ">") == 0)
            strcpy(codes[codesIndex++].opt, "GT");
        else if (strcmp(tokenCopy, ">=") == 0)
            strcpy(codes[codesIndex++].opt, "GE");
        else if (strcmp(tokenCopy, "<") == 0)
            strcpy(codes[codesIndex++].opt, "LES");
        else if (strcmp(tokenCopy, "<=") == 0)
            strcpy(codes[codesIndex++].opt, "LE");
        else if (strcmp(tokenCopy, "==") == 0)
            strcpy(codes[codesIndex++].opt, "EQ");
        else if (strcmp(tokenCopy, "!=") == 0)
            strcpy(codes[codesIndex++].opt, "NOTEQ");
    }
    return (es);
}

int additive_expr_code(node* fa)
{
    int es = 0;
    int i = 0;
    strcpy(tokenCopy, fa->son[i]->st.c_str());
    if (strcmp(tokenCopy, "<term>") == 0)
    {
        es = term_code(fa->son[i]);
    }
    i += 2;
    while (i < fa->son.size())
    {
        strcpy(tokenCopy, fa->son[i]->st.c_str());
        if (strcmp(tokenCopy, "<term>") == 0)
        {
            es = term_code(fa->son[i]);
        }

        strcpy(tokenCopy, fa->son[i - 1]->st.c_str());
        if (strcmp(tokenCopy, "+") == 0)
        {
            strcpy(codes[codesIndex++].opt, "ADD");
        }
        if (strcmp(tokenCopy, "-") == 0)
        {
            strcpy(codes[codesIndex++].opt, "SUB");
        }
        i += 2;
    }
    return es;
}

int term_code(node* fa)
{
    int es = 0;
    int i = 0;
    strcpy(tokenCopy, fa->son[i]->st.c_str());
    if (strcmp(tokenCopy, "<factor>") == 0)
    {
        es = factor_code(fa->son[i]);
    }
    i += 2;
    while (i < fa->son.size())
    {
        strcpy(tokenCopy, fa->son[i]->st.c_str());
        if (strcmp(tokenCopy, "<factor>") == 0)
        {
            es = factor_code(fa->son[i]);
        }

        strcpy(tokenCopy, fa->son[i - 1]->st.c_str());
        if (strcmp(tokenCopy, "*") == 0)
        {
            strcpy(codes[codesIndex++].opt, "MULT");
        }
        if (strcmp(tokenCopy, "/") == 0)
        {
            strcpy(codes[codesIndex++].opt, "DIV");
        }
        i += 2;
    }
    return es;
}

int factor_code(node* fa)
{
    int es = 0;
    strcpy(tokenCopy, fa->son[0]->st.c_str());
    if (strcmp(tokenCopy, "(") == 0)
    {
        es = additive_expr_code(fa->son[1]);
    }
    else if (strcmp(tokenCopy, "ID") == 0)
    {
        rowIDLookup();
        strcpy(tokenCopy, fa->son[0]->son[0]->st.c_str());
        int symbolPos = 0;
        es = lookup_Symbol(tokenCopy, &symbolPos, funcIndex);
        if (es > 0)
        {
            printf("��%d�б���%sδ����!\n", IDrow, tokenCopy);
            return es;
        }

        strcpy(codes[codesIndex].opt, "LOAD");
        codes[codesIndex++].operand = symbol[symbolPos].address;
    }
    else if (strcmp(tokenCopy, "ARRAY") == 0)
    {
        rowIDLookup();
        strcpy(tokenCopy, fa->son[0]->son[0]->son[0]->st.c_str());
        int symbolPos = 0;
        es = lookup_Arr_Symbol(tokenCopy, &symbolPos, funcIndex);
        if (es > 0)
        {
            printf("��%d������%sδ����!\n", IDrow, tokenCopy);
            return es;
        }
        strcpy(tokenCopy, fa->son[0]->son[2]->son[0]->st.c_str());

        strcpy(codes[codesIndex].opt, "LOAD");
        codes[codesIndex++].operand = symbol[symbolPos].address + atoi(tokenCopy);
    }
    else if (strcmp(tokenCopy, "NUM") == 0)
    {
        strcpy(tokenCopy, fa->son[0]->son[0]->st.c_str());
        strcpy(codes[codesIndex].opt, "LOADI");
        codes[codesIndex++].operand = atoi(tokenCopy);
    }
    return es;
}

int if_stat_code(node* fa)
{
    int es = 0, cx1, cx2;
    strcpy(tokenCopy, fa->son[2]->st.c_str());
    if (strcmp(tokenCopy, "<expression>") == 0)
    {
        es = expression_code(fa->son[2]);
    }
    strcpy(codes[codesIndex].opt, "BRF");
    cx1 = codesIndex++;

    strcpy(tokenCopy, fa->son[4]->st.c_str());
    if (strcmp(tokenCopy, "<statement>") == 0)
    {
        es = statement_code(fa->son[4]);
    }
    strcpy(codes[codesIndex].opt, "BR");
    cx2 = codesIndex++;
    codes[cx1].operand = codesIndex;

    if (fa->son.size() == 7)
    {
        strcpy(tokenCopy, fa->son[6]->st.c_str());
        if (strcmp(tokenCopy, "<statement>") == 0)
        {
            es = statement_code(fa->son[6]);
        }
    }
    codes[cx2].operand = codesIndex;
    return es;
}

int switch_stat_code(node* fa)
{
    int es = 0, caseNum = 0, symbolPos = 0;
    int casePos[20] = { 0 };
    int caseBRPos[20] = { 0 };
    int caseBRFPos[20] = { 0 };
    strcpy(tokenCopy, fa->son[2]->st.c_str());
    if (strcmp(tokenCopy, "ID") == 0)
    {
        rowIDLookup();
        strcpy(tokenCopy, fa->son[2]->son[0]->st.c_str());
        es = lookup_Symbol(tokenCopy, &symbolPos, funcIndex);
        if (es > 0)
        {
            printf("��%d�б���%sδ����!\n", IDrow, tokenCopy);
            return es;
        }
    }

    int i = 7;
    while (i < fa->son.size())
    {
        strcpy(tokenCopy, fa->son[i]->st.c_str());
        if (strcmp(tokenCopy, "NUM") == 0)
        {
            casePos[caseNum] = codesIndex;
            strcpy(tokenCopy, fa->son[i]->son[0]->st.c_str());
            strcpy(codes[codesIndex].opt, "LOADI");
            codes[codesIndex++].operand = atoi(tokenCopy);
            strcpy(codes[codesIndex].opt, "LOAD");
            codes[codesIndex++].operand = symbol[symbolPos].address;
            strcpy(codes[codesIndex++].opt, "EQ");
            strcpy(codes[codesIndex].opt, "BRF");
            caseBRFPos[caseNum] = codesIndex++;

            strcpy(tokenCopy, fa->son[i + 1]->st.c_str());
            if (strcmp(tokenCopy, "<statement>") == 0)
            {
                es = statement_code(fa->son[i + 1]);
            }

            strcpy(codes[codesIndex].opt, "BR");
            caseBRPos[caseNum] = codesIndex++;
            caseNum++;
        }
        i += 6;
    }

    for (int i = 0; i < caseNum - 1; i++)
    {
        codes[caseBRFPos[i]].operand = casePos[i + 1];
    }
    codes[caseBRFPos[caseNum - 1]].operand = codesIndex;

    i = fa->son.size() - 4;
    strcpy(tokenCopy, fa->son[i]->st.c_str());
    if (strcmp(tokenCopy, "default") == 0)
    {
        strcpy(tokenCopy, fa->son[i + 2]->st.c_str());
        if (strcmp(tokenCopy, "<statement>") == 0)
        {
            es = statement_code(fa->son[i + 2]);
        }
    }

    for (int i = 0; i < caseNum; i++)
    {
        codes[caseBRPos[i]].operand = codesIndex;
    }

    return es;
}

int while_stat_code(node* fa)
{
    int es = 0, cx1, cxEntrance;
    cxEntrance = codesIndex;
    strcpy(tokenCopy, fa->son[2]->st.c_str());
    if (strcmp(tokenCopy, "<expression>") == 0)
    {
        es = expression_code(fa->son[2]);
    }
    strcpy(codes[codesIndex].opt, "BRF");
    cx1 = codesIndex++;

    strcpy(tokenCopy, fa->son[4]->st.c_str());
    if (strcmp(tokenCopy, "<statement>") == 0)
    {
        es = statement_code(fa->son[4]);
    }
    strcpy(codes[codesIndex].opt, "BR");
    codes[codesIndex++].operand = cxEntrance;
    codes[cx1].operand = codesIndex;

    return es;
}

int for_stat_code(node* fa)
{
    int es = 0;
    int cx1, cx2, cxExp2, cxExp3;

    strcpy(tokenCopy, fa->son[2]->st.c_str());
    if (strcmp(tokenCopy, "<expression>") == 0)
    {
        es = expression_code(fa->son[2]);
    }

    cxExp2 = codesIndex;
    strcpy(tokenCopy, fa->son[4]->st.c_str());
    if (strcmp(tokenCopy, "<expression>") == 0)
    {
        es = expression_code(fa->son[4]);
    }
    strcpy(codes[codesIndex].opt, "BRF");
    cx1 = codesIndex++;
    strcpy(codes[codesIndex].opt, "BR");
    cx2 = codesIndex++;

    cxExp3 = codesIndex;
    strcpy(tokenCopy, fa->son[6]->st.c_str());
    if (strcmp(tokenCopy, "<expression>") == 0)
    {
        es = expression_code(fa->son[6]);
    }
    strcpy(codes[codesIndex].opt, "BR");
    codes[codesIndex++].operand = cxExp2;

    codes[cx2].operand = codesIndex;
    strcpy(tokenCopy, fa->son[8]->st.c_str());
    if (strcmp(tokenCopy, "<statement>") == 0)
    {
        es = statement_code(fa->son[8]);
    }
    strcpy(codes[codesIndex].opt, "BR");
    codes[codesIndex++].operand = cxExp3;

    codes[cx1].operand = codesIndex;

    return es;
}

int read_stat_code(node* fa)
{
    int es = 0;
    int symbolPos = 0;
    for (int i = 0; i < fa->son.size(); i++)
    {
        strcpy(tokenCopy, fa->son[i]->st.c_str());
        if (strcmp(tokenCopy, "ID") == 0)
        {
            rowIDLookup();
            strcpy(tokenCopy, fa->son[i]->son[0]->st.c_str());
            es = lookup_Symbol(tokenCopy, &symbolPos, funcIndex);
            if (es > 0)
            {
                printf("��%d�б���%sδ����!\n", IDrow, tokenCopy);
                return es;
            }
            if (symbol[symbolPos].kind != variable)
            {
                printf("�������������\n");
                return es = 35;
            }
            strcpy(codes[codesIndex++].opt, "IN");
            strcpy(codes[codesIndex].opt, "STO");
            codes[codesIndex++].operand = symbol[symbolPos].address;
        }
    }
    return es;
}

int write_stat_code(node* fa)
{
    int es = 0;
    for (int i = 0; i < fa->son.size(); i++)
    {
        strcpy(tokenCopy, fa->son[i]->st.c_str());
        if (strcmp(tokenCopy, "<expression>") == 0)
        {
            es = expression_code(fa->son[i]);
            strcpy(codes[codesIndex++].opt, "OUT");
        }
    }
    return es;
}

int compound_stat_code(node* fa)
{
    int es = 0;
    for (int i = 0; i < fa->son.size(); i++)
    {
        strcpy(tokenCopy, fa->son[i]->st.c_str());
        if (strcmp(tokenCopy, "<stat_list>") == 0)
        {
            es = statement_list_code(fa->son[i]);
        }
    }
    return es;
}

int call_stat_code(node* fa)
{
    int es = 0;
    int symbolPos = 0;

    int i = 1;
    strcpy(tokenCopy, fa->son[i]->st.c_str());
    if (strcmp(tokenCopy, "ID") == 0)
    {
        rowIDLookup();
        strcpy(tokenCopy, fa->son[i]->son[0]->st.c_str());
        es = lookup_Func_Symbol(tokenCopy, &symbolPos);
        if (es > 0)
        {
            printf("��%d�к���%sδ����!\n", tokenCopy, IDrow);
            return es;
        }
        strcpy(codes[codesIndex].opt, "CAL");
        codes[codesIndex++].operand = symbol[symbolPos].address;
    }

    i = 3;
    strcpy(tokenCopy, fa->son[i]->st.c_str());
    int off = 0;
    int codesIn = symbol[symbolPos].address + 1;
    while (strcmp(tokenCopy, "NUM") == 0)
    {
        strcpy(tokenCopy, fa->son[i]->son[0]->st.c_str());
        codes[codesIn].operand = atoi(tokenCopy);
        codesIn += 2;
        off++;
        i++;
        strcpy(tokenCopy, fa->son[i]->st.c_str());
    }
    if (off != symbol[symbolPos].para)
    {
        printf("��%d�к����Ĳ�������\n", IDrow);
        errorNum++;
    }

    return es;
}
int main(int argc, char** argv)
{
    int esScan = 0;
    printf("=====�ʷ���������======\n");
    esScan = TESTscan();
    if (esScan > 0)
        printf("�ʷ������д���������%d \n", errornum);
    else
        printf("�ʷ������ɹ��� \n");

    int esParse = 0;
    printf("=====�﷨��������======\n");
    if (esScan == 0)
        esParse = TESTparse();
    if (esParse > 0)
    {
        printf("�﷨�����д�! \n");
        return -1;
    }
    else
        printf("�﷨�����ɹ��� \n");

    memset(tree_map, 0, sizeof(tree_map));
    if (esScan == 0 && esParse == 0)
    {
        tree_cnt(Root);
        tree_out(0, maxColumn / 2, Root);
        print_tree();
    }

    int esMediateCode = 0;
    printf("=====����������̼��м��������======\n");
    esMediateCode = TESTMediateCode(Root);
    if (esMediateCode > 0)
        printf("�������ʧ��! ������:%d\n", esMediateCode);
    else
        printf("��������ɹ����м�������ɳɹ��� \n");

    return 0;
}
