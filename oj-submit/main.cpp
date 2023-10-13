#include <bits/stdc++.h>

const int N = 1e5 + 5;

int id, n, m, currentBlock, arr[N], exitCode;
bool existCurrentBlock, existArr[N];

std::string cmd;
std::queue<int> qIn, qOut, ans;
std::set<std::string> cmdSet;

void Error(int step) {
    std::cout << "Error on instruction " << step << std::endl;
    exitCode = 2;
}

int main() {

    std::cin >> id;

    if (id == 1) {
        qIn.push(1), qIn.push(2);
        ans.push(1), ans.push(2);
        n = -1;
        cmdSet.insert("inbox"), cmdSet.insert("outbox");
    } else if (id == 2) {
        qIn.push(3), qIn.push(9), qIn.push(5), qIn.push(1), qIn.push(-2), qIn.push(-2), qIn.push(9), qIn.push(-9);
        ans.push(-6), ans.push(6), ans.push(4), ans.push(-4), ans.push(0), ans.push(0), ans.push(18), ans.push(-18);
        n = 2;
        cmdSet.insert("inbox"), cmdSet.insert("outbox"), cmdSet.insert("copyfrom"), cmdSet.insert("copyto");
        cmdSet.insert("add"), cmdSet.insert("sub"), cmdSet.insert("jump"), cmdSet.insert("jumpifzero");
    } else if (id == 3) {
        qIn.push(6), qIn.push(2), qIn.push(7), qIn.push(7), qIn.push(-9), qIn.push(3), qIn.push(-3), qIn.push(-3);
        ans.push(7), ans.push(-3);
        n = 2;
        cmdSet.insert("inbox"), cmdSet.insert("outbox"), cmdSet.insert("copyfrom"), cmdSet.insert("copyto");
        cmdSet.insert("add"), cmdSet.insert("sub"), cmdSet.insert("jump"), cmdSet.insert("jumpifzero");
    }

    std::cin >> m;
    for (int i = 1; i <= m; i++) {
        std::cin >> cmd;
        if (cmdSet.find(cmd) != cmdSet.end()) {
            if (cmd == "inbox") {
                if (qIn.empty()) {
                    break;
                }
                currentBlock = qIn.front();
                qIn.pop();
                existCurrentBlock = true;

            } else if (cmd == "outbox") {
                if (existCurrentBlock == false) {
                    Error(i);
                    break;
                }
                qOut.push(currentBlock);
                existCurrentBlock = false;
            } else if (cmd == "add") {
                int x;
                std::cin >> x;
                if (existCurrentBlock == false || x > n || existArr[x] == false) {
                    Error(i);
                    break;
                }
                currentBlock += arr[x];
            } else if (cmd == "sub") {
                int x;
                std::cin >> x;
                if (existCurrentBlock == false || x > n || existArr[x] == false) {
                    Error(i);
                    break;
                }
                currentBlock -= arr[x];
            } else if (cmd == "copyto") {
                int x;
                std::cin >> x;
                if (existCurrentBlock == false || x > n) {
                    Error(i);
                    break;
                }
                arr[x] = currentBlock;
                existArr[x] = true;
            } else if (cmd == "copyfrom") {
                int x;
                std::cin >> x;
                if (x > n || existArr[x] == false) {
                    Error(i);
                    break;
                }
                currentBlock = arr[x];
            } else if (cmd == "jump") {
                int x;
                std::cin >> x;
                if (x > m) {
                    Error(i);
                    break;
                }
                i = x;
            } else if (cmd == "jumpifzero") {
                int x;
                std::cin >> x;
                if (existCurrentBlock == false || x > m) {
                    Error(i);
                    break;
                }
                if (currentBlock == 0) {
                    i = x;
                }
            } else {
                Error(i);
                break;
            }
        } else {
            Error(i);
            break;
        }
    }

    if (exitCode != 2) {

        if (qOut.size() != ans.size()) {
            exitCode = 1;
        } else {
            while (!qOut.empty()) {
                int a = qOut.front(), b = ans.front();
                qOut.pop(), ans.pop();
                if (a != b) {
                    exitCode = 1;
                    break;
                }
            }
        }

        if (exitCode == 0) {
            puts("Success");
        } else if (exitCode == 1) {
            puts("Fail");
        }
    }
    
    return 0;
}