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
	 printf ("������Դ�����ļ���������·����:");
     scanf ("%s",Scanin);
	 printf ("������ʷ���������ļ���������·����:");
	 scanf ("%s",Scanout);
	if ((fin = fopen(Scanin, "r")) == NULL)
	{
		printf("�򿪴ʷ����������ļ�%s����\n", Scanin);
		return (1);
	}
	if ((fout = fopen(Scanout, "w")) == NULL)
	{
		printf("�����ʷ���������ļ�%s����\n", Scanout);
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

		/* ��ʶ�������� */
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


			//�۰���ұ�����
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

		/* �޷������� */
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

		/* ���ֽ�� */
		else if (strchr(singleword, ch) != NULL)
		{
			token[0] = ch;
			token[1] = '\0';
			ch = getc(fin);
			rowTokenNum[rowIndex]++;
			fprintf(fout, "%s\t%s\n", token, token);
		}

		/* ˫�ֽ�� != >= <= ==  ���ֽ��! > < =  */
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

		/* ˫�ֽ�� &&   ���ֽ��& */
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

		/* ˫�ֽ�� || ���ֽ��| */
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

		/* �Ƿ��ַ� */
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
