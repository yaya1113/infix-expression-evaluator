#include <iostream>
#include <cstring> // 用于 strlen
using namespace std;

const int MAX_SIZE = 100; // 栈的最大容量

// ================= 工具函数 =================
// 判断字符是否为数字
bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

// 获取运算符优先级
int getPriority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0; // 比如 '(' 或其他字符
}

// ================= 栈的结构定义（使用数组模拟） =================
struct CharStack {
    char data[MAX_SIZE];
    int top; // 栈顶指针

    CharStack() : top(-1) {} // 构造函数，初始化栈为空

    bool isEmpty() {
        return top == -1;
    }

    bool isFull() {
        return top == MAX_SIZE - 1;
    }

    void push(char c) {
        if (isFull()) {
            cout << "字符栈已满!" << endl;
            return;
        }
        data[++top] = c;
    }

    char pop() {
        if (isEmpty()) {
            return '#'; // 返回一个特殊字符表示错误
        }
        return data[top--];
    }

    char getTop() {
        if (isEmpty()) return '#';
        return data[top];
    }
};

struct IntStack {
    int data[MAX_SIZE];
    int top;

    IntStack() : top(-1) {}

    bool isEmpty() { return top == -1; }
    bool isFull() { return top == MAX_SIZE - 1; }

    void push(int n) {
        if (isFull()) {
            cout << "数字栈已满!" << endl;
            return;
        }
        data[++top] = n;
    }

    int pop() {
        if (isEmpty()) {
            cout << "数字栈为空!" << endl;
            return 0;
        }
        return data[top--];
    }
};

// ================= 核心算法：中缀转后缀 =================
void infixToPostfix(char* infix, char* postfix) {
    CharStack opStack; // 运算符栈
    int j = 0; // 后缀表达式索引

    for (int i = 0; infix[i] != '\0'; i++) {
        char c = infix[i];

        if (c == ' ') continue; // 跳过空格

        // 1. 如果是数字，直接输出
        if (isDigit(c)) {
            postfix[j++] = c;
            // 处理多位数，加空格分隔（便于后续计算识别）
            if (!isDigit(infix[i+1])) {
                postfix[j++] = ' ';
            }
        }
        // 2. 如果是左括号，直接入栈
        else if (c == '(') {
            opStack.push(c);
        }
        // 3. 如果是右括号，弹出直到遇到左括号
        else if (c == ')') {
            while (opStack.getTop() != '(') {
                postfix[j++] = opStack.pop();
                postfix[j++] = ' ';
            }
            opStack.pop(); // 弹出 '('
        }
        // 4. 如果是运算符
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            // 栈顶运算符优先级 >= 当前运算符，则弹出
            while (!opStack.isEmpty() && 
                   opStack.getTop() != '(' && 
                   getPriority(opStack.getTop()) >= getPriority(c)) {
                postfix[j++] = opStack.pop();
                postfix[j++] = ' ';
            }
            opStack.push(c); // 当前运算符入栈
        }
    }

    // 5. 将栈中剩余运算符全部弹出
    while (!opStack.isEmpty()) {
        postfix[j++] = opStack.pop();
        postfix[j++] = ' ';
    }
    postfix[j] = '\0'; // 字符串结束符
}

// ================= 核心算法：后缀表达式求值 =================
int evaluatePostfix(char* postfix) {
    IntStack numStack;
    int i = 0;
    int len = strlen(postfix);

    while (i < len) {
        char c = postfix[i];

        if (c == ' ') {
            i++;
            continue;
        }

        // 1. 如果是数字
        if (isDigit(c)) {
            int num = 0;
            // 处理多位数
            while (i < len && isDigit(postfix[i])) {
                num = num * 10 + (postfix[i] - '0');
                i++;
            }
            numStack.push(num);
            continue; // 跳过最后的 i++，因为上面循环已经处理了
        }
        // 2. 如果是运算符
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            int b = numStack.pop(); // 注意顺序：后进先出
            int a = numStack.pop();
            int result = 0;

            switch (c) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/': 
                    if (b == 0) {
                        cout << "错误：除零" << endl;
                        return 0;
                    }
                    result = a / b; 
                    break;
            }
            numStack.push(result);
        }
        i++;
    }
    return numStack.pop();
}

// ================= 简单的增删改查 (CRUD) 演示 =================
// 为了简单，我们用一个全局数组来模拟数据存储
int dataArray[MAX_SIZE] = {10, 20, 30};
int dataCount = 3; // 当前数据个数

void showData() {
    cout << "\n--- 当前数据 ---" << endl;
    for (int i = 0; i < dataCount; i++) {
        cout << "[" << i << "]: " << dataArray[i] << " ";
    }
    cout << endl;
}

void createData() {
    if (dataCount < MAX_SIZE) {
        cout << "请输入要添加的数字: ";
        int val; cin >> val;
        dataArray[dataCount++] = val;
        cout << "添加成功!" << endl;
    } else {
        cout << "数组已满，无法添加!" << endl;
    }
}

void deleteData() {
    if (dataCount == 0) {
        cout << "数组为空!" << endl;
        return;
    }
    cout << "请输入要删除的索引 (0 到 " << dataCount-1 << "): ";
    int index; cin >> index;
    if (index >= 0 && index < dataCount) {
        // 将后面的数据前移
        for (int i = index; i < dataCount - 1; i++) {
            dataArray[i] = dataArray[i + 1];
        }
        dataCount--;
        cout << "删除成功!" << endl;
    } else {
        cout << "索引无效!" << endl;
    }
}

void updateData() {
    if (dataCount == 0) {
        cout << "数组为空!" << endl;
        return;
    }
    cout << "请输入要修改的索引 (0 到 " << dataCount-1 << "): ";
    int index; cin >> index;
    if (index >= 0 && index < dataCount) {
        cout << "请输入新值: ";
        int val; cin >> val;
        dataArray[index] = val;
        cout << "修改成功!" << endl;
    } else {
        cout << "索引无效!" << endl;
    }
}

void readData() {
    showData();
}
int main() {
    char choice;
    char expression[MAX_SIZE];
    char postfix[MAX_SIZE];

    while (true) {
        cout << "\n===== 功能菜单 =====" << endl;
        cout << "1. 计算表达式 (中缀转后缀并求值)" << endl;
        cout << "2. 数据管理 (增删改查演示)" << endl;
        cout << "3. 退出" << endl;
        cout << "请选择 (1-3): ";
        cin >> choice;

        switch (choice) {
            case '1': {
                cout << "请输入中缀表达式 (例如: 3+5*2): ";
                cin >> expression;

                infixToPostfix(expression, postfix);
                cout << "后缀表达式: " << postfix << endl;
                
                int result = evaluatePostfix(postfix);
                cout << "计算结果: " << result << endl;
                break;
            }
            case '2': {
                char op;
                while (true) {
                    cout << "\n--- 数据管理 ---" << endl;
                    cout << "A: 查看数据 | B: 添加 | C: 删除 | D: 修改 | E: 返回主菜单" << endl;
                    cout << "请选择: ";
                    cin >> op;
                    if (op == 'E' || op == 'e') break;
                    
                    switch (op) {
                        case 'A': case 'a': readData(); break;
                        case 'B': case 'b': createData(); break;
                        case 'C': case 'c': deleteData(); break;
                        case 'D': case 'd': updateData(); break;
                        default: cout << "无效选项!" << endl;
                    }
                }
                break;
            }
            case '3':
                cout << "程序结束，再见!" << endl;
                return 0;
            default:
                cout << "无效选择，请重试!" << endl;
        }
    }

    return 0;
}

