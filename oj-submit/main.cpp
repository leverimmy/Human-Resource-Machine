#include <bits/stdc++.h>

const int N = 1e5 + 5;

int id, n, m, currentBlock, arr[N], exitCode, opn[N], vis[N];
bool existCurrentBlock, existArr[N];
std::string cmd[N];

std::queue<int> qIn, qOut, ans;
std::set<std::string> cmdSet;

void Error(int step) {
    std::cout << "Error on instruction " << step << std::endl;
    exitCode = 2;
}

bool refactor(std::string cmdLine, std::string & currentCmd, int &x) {
    currentCmd.clear(), x = -1;
    int len = cmdLine.length(), first = 1;
    int l1 = 0, l2 = len;
    while (cmdLine[l1] == ' ')
        l1++;
    for (int i = l1; i < len; i++) {
        if (cmdLine[i] == ' ') {
            l2 = i;
            break;
        }
        if (isalpha(cmdLine[i]))
            currentCmd.push_back(cmdLine[i]);
    }
    while (cmdLine[l2] == ' ')
        l2++;
    for (int i = l2; i < len; i++) {
        if (isdigit(cmdLine[i])) {
            if (first) {
                x = 0;
                first = 0;
            }
            x = x * 10 + (cmdLine[i] - '0');
        } else {
            return false;
        }
    }
    if (cmdSet.count(currentCmd)) {
        if (currentCmd == "inbox") {
            if (x != -1)
                return false;
        } else if (currentCmd == "outbox") {
            if (existCurrentBlock == false || x != -1)
                return false;
        } else if (currentCmd == "add") {
            if (existCurrentBlock == false || x >= n || existArr[x] == false || x == -1)
                return false;
        } else if (currentCmd == "sub") {
            if (existCurrentBlock == false || x >= n || existArr[x] == false || x == -1)
                return false;
        } else if (currentCmd == "copyto") {
            if (existCurrentBlock == false || x >= n || x == -1)
                return false;
        } else if (currentCmd == "copyfrom") {
            if (x >= n || existArr[x] == false || x == -1)
                return false;
        } else if (currentCmd == "jump") {
            if (x > m || x == -1)
                return false;
        } else if (currentCmd == "jumpifzero") {
            if (existCurrentBlock == false || x > m || x == -1)
                return false;
        }
    } else {
        return false;
    }
    return true;
}

int main() {

    std::cin >> id;
    if (id == 1) {
        qIn.push(1), qIn.push(2);
        ans.push(1), ans.push(2);
        n = 0;
        cmdSet.insert("inbox"), cmdSet.insert("outbox");
    } else if (id == 2) {
        qIn.push(3), qIn.push(9), qIn.push(5), qIn.push(1), qIn.push(-2), qIn.push(-2), qIn.push(9), qIn.push(-9);
        ans.push(-6), ans.push(6), ans.push(4), ans.push(-4), ans.push(0), ans.push(0), ans.push(18), ans.push(-18);
        n = 3;
        cmdSet.insert("inbox"), cmdSet.insert("outbox"), cmdSet.insert("copyfrom"), cmdSet.insert("copyto");
        cmdSet.insert("add"), cmdSet.insert("sub"), cmdSet.insert("jump"), cmdSet.insert("jumpifzero");
    } else if (id == 3) {
        qIn.push(6), qIn.push(2), qIn.push(7), qIn.push(7), qIn.push(-9), qIn.push(3), qIn.push(-3), qIn.push(-3);
        ans.push(7), ans.push(-3);
        n = 3;
        cmdSet.insert("inbox"), cmdSet.insert("outbox"), cmdSet.insert("copyfrom"), cmdSet.insert("copyto");
        cmdSet.insert("add"), cmdSet.insert("sub"), cmdSet.insert("jump"), cmdSet.insert("jumpifzero");
    }

    std::cin >> m;
    getchar();
    for (int i = 1; i <= m; ) {
        if (!vis[i]) {
            std::string cmdLine;
            std::getline(std::cin, cmdLine);
            bool flag = refactor(cmdLine, cmd[i], opn[i]);
            if (!flag) {
                Error(i);
                break;
            }
            vis[i] = 1;
        }
        if (cmd[i] == "inbox") {
            if (qIn.empty()) {
                break;
            }
            currentBlock = qIn.front();
            qIn.pop();
            existCurrentBlock = true;
            i++;
        } else if (cmd[i] == "outbox") {
            qOut.push(currentBlock);
            existCurrentBlock = false;
            i++;
        } else if (cmd[i] == "add") {
            currentBlock += arr[opn[i]];
            i++;
        } else if (cmd[i] == "sub") {
            currentBlock -= arr[opn[i]];
            i++;
        } else if (cmd[i] == "copyto") {
            arr[opn[i]] = currentBlock;
            existArr[opn[i]] = true;
            i++;
        } else if (cmd[i] == "copyfrom") {
            currentBlock = arr[opn[i]];
            existCurrentBlock = true;
            i++;
        } else if (cmd[i] == "jump") {
            i = opn[i];
        } else if (cmd[i] == "jumpifzero") {
            if (currentBlock == 0) {
                i = opn[i];
            } else {
                i++;
            }
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
/*
1
5
inbox
outbox
inbox
outbox
outbox

*/