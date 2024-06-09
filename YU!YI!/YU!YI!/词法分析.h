#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#define keywordSum 15
#define MAXROWNUM 100

const char* keyword[keywordSum] = { "break", "call", "case", "default", "do", "else", "for", "function", "if", "int", "main", "read", "switch", "while", "write" };
char singleword[50] = "+-*(){};,:[]";
char doubleword[10] = "><=!";
char doubleword1[10] = "&&";
char doubleword2[10] = "||";
char Scanin[300], Scanout[300];
FILE* fin, * fout;
int errornum = 0;
int rowTokenNum[MAXROWNUM] = { 0 };
int rowIDNum[MAXROWNUM] = { 0 };
int rowIndex = 1;

int TESTscan()
{
	char ch, token[40];
	int es = 0, j;

	strcpy(Scanin, "in.txt");
	strcpy(Scanout, "out.txt");
	 printf ("请输入源程序文件名（包括路径）:");
     scanf ("%s",Scanin);
	 printf ("请输入词法分析输出文件名（包括路径）:");
	 scanf ("%s",Scanout);
	if ((fin = fopen(Scanin, "r")) == NULL)
	{
		printf("打开词法分析输入文件%s出错！\n", Scanin);
		return (1);
	}
	if ((fout = fopen(Scanout, "w")) == NULL)
	{
		printf("创建词法分析输出文件%s出错！\n", Scanout);
		return (2);
	}

	ch = getc(fin);
	while (ch != EOF)
	{
		if (ch == '\n')
		{
			rowIndex++;
		}

		while (ch == ' ' || ch == '\n' || ch == '\t')
			ch = getc(fin);

		/* 标识符或保留字 */
		if (isalpha(ch))
		{
			char copy[40];
			int hasNum = 0;

			token[0] = ch;
			copy[0] = tolower(token[0]);
			j = 1;
			ch = getc(fin);
			while (isalnum(ch) || isdigit(ch))
			{
				if (isdigit(ch))
					hasNum = 1;
				token[j] = ch;
				copy[j] = tolower(token[j]);
				j++;
				ch = getc(fin);
			}
			copy[j] = token[j] = '\0';
			if (ch != ' ')
			{
				errornum++;
				printf("%s %s %s RowNum:%d\n", "Illegal variable decaleration:", token, "!!!", rowIndex);
				es = 1;
			}


			//折半查找保留字
			int lef = 0;
			int rig = keywordSum - 1;
			int mid = (lef + rig) / 2;
			int flag = 1;

			if (!hasNum)
				while ((lef <= rig) && flag)
				{
					mid = (lef + rig) / 2;
					flag = strcmp(copy, keyword[mid]);

					if (flag > 0)
						lef = mid + 1;
					else
						rig = mid - 1;
				}

			rowTokenNum[rowIndex]++;
			if (!flag)
				fprintf(fout, "%s\t%s\n", copy, copy);
			else
			{
				rowIDNum[rowIndex]++;
				fprintf(fout, "%s\t%s\n", "ID", token);
			}

		}

		/* 无符号整数 */
		else if (isdigit(ch))
		{
			token[0] = ch;
			j = 1;
			ch = getc(fin);
			while (isdigit(ch))
			{
				token[j++] = ch;
				ch = getc(fin);
			}
			token[j] = '\0';
			if (ch != ' ')
			{
				errornum++;
				printf("%s %s %s RowNum:%d\n", "Illegal number decaleration:", token, "!!!", rowIndex);
				es = 2;
			}
			rowTokenNum[rowIndex]++;
			fprintf(fout, "%s\t%s\n", "NUM", token);
		}

		/* 单分界符 */
		else if (strchr(singleword, ch) != NULL)
		{
			token[0] = ch;
			token[1] = '\0';
			ch = getc(fin);
			rowTokenNum[rowIndex]++;
			fprintf(fout, "%s\t%s\n", token, token);
		}

		/* 双分界符 != >= <= ==  单分界符! > < =  */
		else if (strchr(doubleword, ch) != NULL)
		{
			token[0] = ch;
			ch = getc(fin);

			if (ch == '=')
			{
				token[1] = ch;
				token[2] = '\0';
				ch = getc(fin);
			}
			else
				token[1] = '\0';
			rowTokenNum[rowIndex]++;
			fprintf(fout, "%s\t%s\n", token, token);
		}

		/* 双分界符 &&   单分界符& */
		else if (strchr(doubleword1, ch) != NULL)
		{
			token[0] = ch;
			ch = getc(fin);
			if (ch == '&')
			{
				token[1] = ch;
				token[2] = '\0';
				ch = getc(fin);
			}
			else
				token[1] = '\0';
			rowTokenNum[rowIndex]++;
			fprintf(fout, "%s\t%s\n", token, token);
		}

		/* 双分界符 || 单分界符| */
		else if (strchr(doubleword2, ch) != NULL)
		{
			token[0] = ch;
			ch = getc(fin);
			if (ch == '|')
			{
				token[1] = ch;
				token[2] = '\0';
				ch = getc(fin);
			}
			else
				token[1] = '\0';
			rowTokenNum[rowIndex]++;
			fprintf(fout, "%s\t%s\n", token, token);
		}

		else if (ch == '/')
		{
			token[0] = ch;
			ch = getc(fin);
			if (ch == '*')
			{
				token[0] = '\0';
				char ch1;
				ch1 = getc(fin);
				do
				{
					ch = ch1;
					ch1 = getc(fin);
				} while ((ch != '*' || ch1 != '/') && ch1 != EOF);
				ch = getc(fin);
			}
			else
			{
				token[1] = '\0';
				rowTokenNum[rowIndex]++;
				fprintf(fout, "%s\t%s\n", token, token);
			}
		}

		/* 非法字符 */
		else
		{
			errornum++;
			token[0] = ch;
			token[1] = '\0';
			ch = getc(fin);
			es = 3;
			printf("%s %s %s RowNum:%d\n", "Illegal character:", token, "!!!", rowIndex);
		}
	}
	fclose(fin);
	fclose(fout);
	return (es);
}
