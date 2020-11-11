//HW2_2016706018
#include <iostream>
#include <fstream>
#include <stack>

using namespace std;

/*함수 프로토타입 선언부*/


//postcondition: Operation Table 읽어오고 계산 구현
void RunCalculator();

//postcondition: 버퍼를 비워준다.
void clear_stdin();

//precondition: Infix_to_Postfix 스택 과 Postfix 스택, operation table 배열
//postcondition: operation table에 대조하여 Infix_to_Postfix의 top에 있는 연산자와 Postfix의 operand 2개를 pop한 뒤 연산 진행
void calculator(stack<char>& Infix_to_Postfix, stack<char>& Postfix, const char Operation_Table[][3][3]);

//precondition: keyboard로 받는 입력, Operation table 배열
//postcondition: 받은 입력을 토대로 각종 경우에 따라 연산 진행 후 결과 리턴
char read_and_evaluate(istream& ins, const char Operation_Table[][3][3]);


/*메인 함수*/
int main() {
	RunCalculator();	//함수 실행
}


/*함수 구현부*/

//RunCalculator
void RunCalculator() {
	/*
	operations.txt 읽어오고 Operation_Table 배열에 @#& 순서대로 저장
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
	식 입출력 및 반복
	버퍼를 비워야 입력을 지속적으로 받음
	*/
	while (1) {
		cout << read_and_evaluate(cin, Operation_Table) << endl;
		clear_stdin();
	}

}

/*버퍼 비우는 함수*/
void clear_stdin() {
	int ch;
	while ((ch = getchar()) != EOF && ch != '\n') {};
}


/*
계산 진행 함수
 Imfix_to_Postfix에서 operator pop 및 Postfix에서 인자 2개를 pop 하고 연산 진행
 Operation_Table에 매칭되는 값 찾은 후 Postfix에 push
*/
void calculator(stack<char>& Infix_to_Postfix, stack<char>& Postfix, const char Operation_Table[][3][3]) {
	//ope에 operator pop하여 저장
	//c1,c2에 operand 2개 pop하여 저장
	//switch 구문을 통해 Operation_Table에서 해당하는 결과 값을 찾아 Postfix 스택에 push
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
입력받고 각 경우에 따라 진행 후 최종 출력 리턴
*/
char read_and_evaluate(istream& ins, const char Operation_Table[][3][3]) {

	stack<char> Infix_to_Postfix;
	stack<char> Postfix;

	char input;


	//개행까지 입력 받기
	while (ins && ins.peek() != '\n') {

		/*입력이 EOI면 종료*/
		if (strchr("E", ins.peek()) != NULL)
		{
			ins.ignore();
			if (ins.peek() == 'O') {
				ins.ignore();
				if (ins.peek() == 'I')
					exit(0);
			}

		}


		/*입력이 [{(인 경우*/
		/*Infix_to_Postfix에 push*/
		if (strchr("[{(", ins.peek()) != NULL)
		{
			ins >> input;
			Infix_to_Postfix.push(input);
		}

		/*입력이 operand인 경우*/
		/*Postfix에 push*/
		else if (strchr("abc", ins.peek()) != NULL)
		{
			ins >> input;
			Postfix.push(input);

		}
		/*입력이 operator인 경우*/
		/*입력이 Infix_to_Postfix 스택의 탑보다 연산자 우선순위가 낮거나 같을때 계산 진행 */
		/*입력은 마지막에 push*/
		else if (strchr("@#&", ins.peek()) != NULL)
		{
			ins >> input;
			if (Infix_to_Postfix.size() != 0)
				if ((strchr("@#&", Infix_to_Postfix.top()) && strchr("@#", input)) || ((Infix_to_Postfix.top() == '&') && (input == '&'))) {
					calculator(Infix_to_Postfix, Postfix, Operation_Table);
					//계산 후에도 Infix_to_Postfix 스택에 input과 동급의 operator가 있는지 확인한다.
					//있으면 다시 한번 계산 진행
					if (Infix_to_Postfix.size() != 0)
						if ((strchr("@#", Infix_to_Postfix.top()) && strchr("@#", input))) {
							calculator(Infix_to_Postfix, Postfix, Operation_Table);
						}
				}

			Infix_to_Postfix.push(input);

		}

		/*입력이 ]})인 경우*/
		/*
		스택의 탑이 앞괄호가 나올때까지 루프문에서 연산 진행,
		괄호가 매칭되지 않거나 나오지 않으면 Error출력
		*/
		else if (strchr(")}]", ins.peek()) != NULL)
		{
			ins >> input;

			while (1) {
				/*size가 0보다 클 때 반복*/
				if (Infix_to_Postfix.size() != 0) {
					/*괄호가 매칭되면 루프문 탈출*/
					if (((Infix_to_Postfix.top() == '(') && (input == ')')) || ((Infix_to_Postfix.top() == '{') && (input == '}')) || ((Infix_to_Postfix.top() == '[') && (input == ']'))) {
						Infix_to_Postfix.pop();
						break;
					}
					/*operator면 연산 진행*/
					else if (strchr("@#&", Infix_to_Postfix.top())) {
						calculator(Infix_to_Postfix, Postfix, Operation_Table);
					}
					/*괄호가 매칭이 안 된 경우*/
					else {
						cout << "Error!";
						return ' ';
					}
				}
				/*괄호가 모자란 경우*/
				else {
					cout << "Error!";
					return ' ';
				}

			}
		}
		/*식 및 EOI입력을 제외한 문자는 무시*/
		else {
			ins.ignore();
		}
	}

	/* Infix_to_Postfix 스택에 연산자가 남은 경우 비울때까지 연산 진행*/
	/*단 스택에 [{(가 남아 있으면 괄호갯수가 잘못 된 경우로 error 출력*/
	while (Infix_to_Postfix.size() != 0) {
		if (strchr("[{(", Infix_to_Postfix.top()) != NULL) {
			cout << "Error!";
			return ' ';
		}
		calculator(Infix_to_Postfix, Postfix, Operation_Table);
	}
	/*최종 계산 값 리턴*/
	return Postfix.top();
}