#include <iostream>
#include <fstream>
#include <iomanip>
#include <windows.h>
using namespace std;

// структура данных запроса клиента
const int max_marks = 10;

struct Person {
    char name[25];
    int count;
    int marks[max_marks];
} A;

// структура ответа сервера
struct Answer {
    int scholarship;
    int debts;
    double average;
} R;

int main() {

    const char* nameR = "REQUEST.bin";  // файл для запросов клиентов
    const char* nameA = "ANSWER.bin";   // файл для ответов сервера

    ofstream f_REQ;
    ifstream f_ANS;

    long pred_size;

    while (true)
    {
        // передача данных от клиента серверу
        cout << "Enter your surname, amount of marks and marks" << endl;
        cin >> setw(sizeof(A.name)) >> A.name >> A.count;

        for (int i = 0; i < A.count; i++) {
            int mark;
            cin >> mark;
            if (i < max_marks) {    // лишние оценки не записываем за границу массива
                A.marks[i] = mark;
            }
        }


        f_REQ.open(nameR, ios::app | ios::binary);  // открытие файла REQUEST
        f_REQ.write((char*)&A, sizeof(A));          // запись запроса в файл REQUEST
        f_REQ.close();

        f_ANS.open(nameA, ios::binary);             // открытие файла ANSWER

        f_ANS.seekg(0, ios::end);                   // переход в конец файла ANSWER
        pred_size = f_ANS.tellg();

        while (pred_size + (long)sizeof(R) > f_ANS.tellg()) {
            Sleep(100);   // ждем и переходим в конец файла ANSWER
            f_ANS.clear();
            f_ANS.seekg(0, ios::end);
        }

        f_ANS.seekg(pred_size, ios::beg);           // на начало нового ответа
        f_ANS.read((char*)&R, sizeof(R));           // считывание ответа
        f_ANS.close();

        if (R.scholarship == -1) {
            cout << "invalid data" << endl;
        } else {
            if (R.scholarship == 0) {
                cout << "without scholarship" << endl;
            } else {
                cout << "Scholarship: " << R.scholarship << endl;
            }
            cout << "Debts: " << R.debts << endl;
            cout << "Average: " << fixed << setprecision(2) << R.average << endl;
        }
        cout << endl;

    }
}
