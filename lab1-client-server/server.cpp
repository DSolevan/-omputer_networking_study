#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

const int max_marks = 10;

struct Person {
    char name[25];          // имя
    int count;              // сколько оценок
    int marks[max_marks];   // оценки
} B;

// структура ответа сервера
struct Answer {
    int scholarship;        // стипендия (-1 - неверные данные)
    int debts;              // количество задолженностей (каждая двойка - 1 задолженность)
    double average;         // средний балл
} R;

long size_pred;

int main() {

    ifstream fR;
    ofstream fA;

    const char* nameR = "REQUEST.bin";
    const char* nameA = "ANSWER.bin";

    cout << "server is working" << endl;

    ofstream(nameR, ios::app).close();
    ofstream(nameA, ios::app).close();

    // начальные установки
    fR.open(nameR, ios::binary);
    fR.seekg(0, ios::end);
    size_pred = fR.tellg();
    fR.close();

    // начало работы сервера
    while (true)
    {
        fR.open(nameR, ios::binary);
        fR.seekg(0, ios::end);

        // есть новые запросы от клиентов?
        while (size_pred >= fR.tellg()) {
            Sleep(100);
            fR.clear();
            fR.seekg(0, ios::end);
        }

        // получен новый запрос
        fR.seekg(size_pred, ios::beg);
        fR.read((char*)&B, sizeof(B));

        size_pred = fR.tellg();
        fR.close();

        // определение стипендии, задолженностей и среднего балла
        R.scholarship = -1;
        R.debts = 0;
        R.average = 0;

        if (B.count >= 1 && B.count <= max_marks) {

            bool hasThree = false;
            bool hasFour = false;
            bool allFive = true;
            bool valid = true;
            int debts = 0;
            int sum = 0;

            for (int i = 0; i < B.count; i++) {

                // проверка оценки
                if (B.marks[i] < 2 || B.marks[i] > 5) {
                    valid = false;
                    break;
                }

                if (B.marks[i] == 2) {
                    debts++;   // двойка - это задолженность
                }

                if (B.marks[i] == 3) {
                    hasThree = true;
                }

                if (B.marks[i] == 4) {
                    hasFour = true;
                }

                if (B.marks[i] != 5) {
                    allFive = false;
                }

                sum += B.marks[i];
            }

            if (valid) {
                R.debts = debts;
                R.average = (double)sum / B.count;

                if (debts > 0 || hasThree) {
                    R.scholarship = 0;
                }
                else if (allFive) {
                    R.scholarship = 4000;
                }
                else if (hasFour) {
                    R.scholarship = 2000;
                }
            }
        }

        cout << B.name << ": scholarship " << R.scholarship
             << ", debts " << R.debts << ", average " << R.average << endl;

        // передача ответа клиенту
        fA.open(nameA, ios::binary | ios::app);
        fA.write((char*)&R, sizeof(R));
        fA.close();
    }

    return 0;
}
