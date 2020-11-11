//HW2_2016706018
#include <iostream>
#include <fstream>
#include <stack>

using namespace std;

/*�Լ� ������Ÿ�� �����*/


//postcondition: Operation Table �о���� ��� ����
void RunCalculator();

//postcondition: ���۸� ����ش�.
void clear_stdin();

//precondition: Infix_to_Postfix ���� �� Postfix ����, operation table �迭
//postcondition: operation table�� �����Ͽ� Infix_to_Postfix�� top�� �ִ� �����ڿ� Postfix�� operand 2���� pop�� �� ���� ����
void calculator(stack<char>& Infix_to_Postfix, stack<char>& Postfix, const char Operation_Table[][3][3]);

//precondition: keyboard�� �޴� �Է�, Operation table �迭
//postcondition: ���� �Է��� ���� ���� ��쿡 ���� ���� ���� �� ��� ����
char read_and_evaluate(istream& ins, const char Operation_Table[][3][3]);


/*���� �Լ�*/
int main() {
	RunCalculator();	//�Լ� ����
}


/*�Լ� ������*/

//RunCalculator
void RunCalculator() {
	/*
	operations.txt �о���� Operation_Table �迭�� @#& ������� ����
	*/
	ifstream read;
	char ch;
	char Operation_Table[3][3][3] = { 0, };
	int i = 0, j = 0, k = 0;
	read.open("operations.txt");
	while (!read.eof()) {
		read.get(ch);
		if (ch == 'a' || ch == 'b' || ch == 'c')
		{
			Operation_Table[i][j][k++] = ch;
			if (k == 3) { k = 0; j++; if (j == 3) { j = 0; i++; } }
		}
	}
	read.close();

	/*
	�� ����� �� �ݺ�
	���۸� ����� �Է��� ���������� ����
	*/
	while (1) {
		cout << read_and_evaluate(cin, Operation_Table) << endl;
		clear_stdin();
	}

}

/*���� ���� �Լ�*/
void clear_stdin() {
	int ch;
	while ((ch = getchar()) != EOF && ch != '\n') {};
}


/*
��� ���� �Լ�
 Imfix_to_Postfix���� operator pop �� Postfix���� ���� 2���� pop �ϰ� ���� ����
 Operation_Table�� ��Ī�Ǵ� �� ã�� �� Postfix�� push
*/
void calculator(stack<char>& Infix_to_Postfix, stack<char>& Postfix, const char Operation_Table[][3][3]) {
	//ope�� operator pop�Ͽ� ����
	//c1,c2�� operand 2�� pop�Ͽ� ����
	//switch ������ ���� Operation_Table���� �ش��ϴ� ��� ���� ã�� Postfix ���ÿ� push
	char ope = Infix_to_Postfix.top();
	Infix_to_Postfix.pop();
	char c1, c2;
	c2 = Postfix.top();
	Postfix.pop();
	c1 = Postfix.top();
	Postfix.pop();
	int x1, x2;
	switch (c1) {
	case 'a': x1 = 0; break;

	case 'b': x1 = 1; break;

	case 'c': x1 = 2; break;
	}
	switch (c2) {
	case 'a': x2 = 0; break;

	case 'b': x2 = 1; break;

	case 'c': x2 = 2; break;
	}
	switch (ope) {
	case '@':	Postfix.push(Operation_Table[0][x1][x2]);  break;
	case '#':	Postfix.push(Operation_Table[1][x1][x2]);  break;
	case '&':	Postfix.push(Operation_Table[2][x1][x2]);  break;
	}

}

/*
�Է¹ް� �� ��쿡 ���� ���� �� ���� ��� ����
*/
char read_and_evaluate(istream& ins, const char Operation_Table[][3][3]) {

	stack<char> Infix_to_Postfix;
	stack<char> Postfix;

	char input;


	//������� �Է� �ޱ�
	while (ins && ins.peek() != '\n') {

		/*�Է��� EOI�� ����*/
		if (strchr("E", ins.peek()) != NULL)
		{
			ins.ignore();
			if (ins.peek() == 'O') {
				ins.ignore();
				if (ins.peek() == 'I')
					exit(0);
			}

		}


		/*�Է��� [{(�� ���*/
		/*Infix_to_Postfix�� push*/
		if (strchr("[{(", ins.peek()) != NULL)
		{
			ins >> input;
			Infix_to_Postfix.push(input);
		}

		/*�Է��� operand�� ���*/
		/*Postfix�� push*/
		else if (strchr("abc", ins.peek()) != NULL)
		{
			ins >> input;
			Postfix.push(input);

		}
		/*�Է��� operator�� ���*/
		/*�Է��� Infix_to_Postfix ������ ž���� ������ �켱������ ���ų� ������ ��� ���� */
		/*�Է��� �������� push*/
		else if (strchr("@#&", ins.peek()) != NULL)
		{
			ins >> input;
			if (Infix_to_Postfix.size() != 0)
				if ((strchr("@#&", Infix_to_Postfix.top()) && strchr("@#", input)) || ((Infix_to_Postfix.top() == '&') && (input == '&'))) {
					calculator(Infix_to_Postfix, Postfix, Operation_Table);
					//��� �Ŀ��� Infix_to_Postfix ���ÿ� input�� ������ operator�� �ִ��� Ȯ���Ѵ�.
					//������ �ٽ� �ѹ� ��� ����
					if (Infix_to_Postfix.size() != 0)
						if ((strchr("@#", Infix_to_Postfix.top()) && strchr("@#", input))) {
							calculator(Infix_to_Postfix, Postfix, Operation_Table);
						}
				}

			Infix_to_Postfix.push(input);

		}

		/*�Է��� ]})�� ���*/
		/*
		������ ž�� �հ�ȣ�� ���ö����� ���������� ���� ����,
		��ȣ�� ��Ī���� �ʰų� ������ ������ Error���
		*/
		else if (strchr(")}]", ins.peek()) != NULL)
		{
			ins >> input;

			while (1) {
				/*size�� 0���� Ŭ �� �ݺ�*/
				if (Infix_to_Postfix.size() != 0) {
					/*��ȣ�� ��Ī�Ǹ� ������ Ż��*/
					if (((Infix_to_Postfix.top() == '(') && (input == ')')) || ((Infix_to_Postfix.top() == '{') && (input == '}')) || ((Infix_to_Postfix.top() == '[') && (input == ']'))) {
						Infix_to_Postfix.pop();
						break;
					}
					/*operator�� ���� ����*/
					else if (strchr("@#&", Infix_to_Postfix.top())) {
						calculator(Infix_to_Postfix, Postfix, Operation_Table);
					}
					/*��ȣ�� ��Ī�� �� �� ���*/
					else {
						cout << "Error!";
						return ' ';
					}
				}
				/*��ȣ�� ���ڶ� ���*/
				else {
					cout << "Error!";
					return ' ';
				}

			}
		}
		/*�� �� EOI�Է��� ������ ���ڴ� ����*/
		else {
			ins.ignore();
		}
	}

	/* Infix_to_Postfix ���ÿ� �����ڰ� ���� ��� ��ﶧ���� ���� ����*/
	/*�� ���ÿ� [{(�� ���� ������ ��ȣ������ �߸� �� ���� error ���*/
	while (Infix_to_Postfix.size() != 0) {
		if (strchr("[{(", Infix_to_Postfix.top()) != NULL) {
			cout << "Error!";
			return ' ';
		}
		calculator(Infix_to_Postfix, Postfix, Operation_Table);
	}
	/*���� ��� �� ����*/
	return Postfix.top();
}