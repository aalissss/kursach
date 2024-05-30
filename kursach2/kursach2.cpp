#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int id = 0;

//структура с информацией для узла
struct Information {
	string name = "";
	int group = 0;
    int marks[5] = {};
	int stipend = 0;
	int yearOfBirth = 0;
    int id = 0;
};

//структура узла двусвязного списка
template <typename T>
struct Node {
	T Information;
	Node* next = nullptr;
	Node* prev = nullptr;
};

//структура списка
template <typename T>
struct List {
	Node<T>* first = nullptr;
	Node<T>* last = nullptr;
	Node<T>* now = nullptr;
	int position = 0;
	int cnt = 0;

    //функция доступа к индексу
    void toIndex(int index) {
        if (abs(index - position) > (cnt - 1 - index)) {
            now = last;
            position = cnt - 1;
        }
        if (abs(index - position) > index) {
            now = first;
            position = 0;
        }
        if (index > position) {
            for (int i = 0; i < index - position; i++) {
                now = now->next;
            }
        }
        else {
            for (int i = 0; i < position - index; i++) {
                now = now->prev;
            }
        }
        position = index;
    }

    //добавление элемента в конец списка 
    void add(T information) {
        Node<T>* newNode = new Node<T>;
        newNode->Information = information;
        if (cnt == 0) {
            first = newNode;
            last = newNode;
            now = newNode;
        }
        else {
            newNode->prev = last;
            last->next = newNode;
            last = newNode;
        }
        cnt++;
    }

    //удаление по индексу 
    void removeAt(int index) {
        if (index < 0 || index >= cnt) {
            return;
        }
        toIndex(index);
        Node<T>* temp = now;
        if (cnt == 1) {
            clear();
            return;
        }
        else if (index == 0) {
            first = now->next;
            first->prev = nullptr;
            now = first;
        }
        else if (index == cnt - 1) {
            last = now->prev;
            last->next = nullptr;
            now = last;
            position--;
        }
        else {
            now = now->next;
            temp->next->prev = temp->prev;
            temp->prev->next = temp->next;
        }
        delete temp;
        cnt--;
    }

    //удаление всех элементов 
    void clear() {
        int kol = cnt;
        for (int i = 0; i < kol; i++) {
            Node<T>* temp = first;
            first = first->next;
            delete temp;
            cnt--;
        }
        first = nullptr;
        last = nullptr;
        now = nullptr;
        cnt = 0;
        position = 0;
    }

    //вставка по индексу 
    void insert(int index, T information) {
        if (index < 0 || index > cnt) {
            return;
        }
        if (index == cnt) {
            add(information);
            return;
        }
        toIndex(index);
        Node<T>* newNode = new Node<T>;
        newNode->Information = information;
        newNode->prev = now->prev;
        newNode->next = now;
        now->prev = newNode;
        if (newNode->prev != nullptr) {
            newNode->prev->next = newNode;
        }
        else {
            first = newNode;
        }
        cnt++;
        position++;
    }

    //доступ к информационной части элемента в заданной позиции 
    T elementAt(int index) {
        if (index < 0 || index >= cnt) {
            throw 0;
        }
        toIndex(index);
        return now->Information;
    }

    //количество элементов в коллекции 
    int count() {
        return cnt;
    }
};

//средняя стипендия по потоку
int averageStip(List<Information> list) {
    int sumStipend = 0;
    if (list.count() == 0) {
        return 0;
    }
    for (int i = 0; i < list.count(); i++) {
        sumStipend += list.elementAt(i).stipend;
    }
    int averageStip = sumStipend / list.count();
    return averageStip;
}

//создание списка студентов со стипендией, ниже средней в алфавитном порядке
List<string> studentsWithLowStip(List<Information> list, int group) {
    List<string> listResult;
    int average = averageStip(list);
    for (int i = 0; i < list.count(); i++) {
        if (list.elementAt(i).stipend >= average) {
            continue;
        }
        if (list.elementAt(i).group == group) {
            listResult.add(list.elementAt(i).name);
        }
    }
    for (int i = 0; i < listResult.count(); i++) {
        string temp = listResult.elementAt(i);
        for (int j = i; j < listResult.count() - 1; j++) {
            if (temp > listResult.elementAt(j)) {
                listResult.insert(i, listResult.elementAt(j));
                listResult.removeAt(j + 1);
            }
        }
    }
    return listResult;
}

//удаление группы
void removeGroup(int group, List<Information>& list) {
    for (int i = 0; i < list.count(); i++) {
        if (list.elementAt(i).group == group) {
            list.removeAt(i);
            i--;
        }
    }
}

//удаление студента
void removeStudent(List<Information> list, int id) {
    for (int j = 0; j < list.count(); j++) {
        if (list.elementAt(j).id == id) {
            list.removeAt(j);
        }
    }
}

//ввод информации о студенте
Information input() {
    Information information;
    string f, i, o;
    cout << "Введите ФИО студента: ";
    cin >> f >> i >> o;
    information.name = f + " " + i + " " + o;
    cout << "Введите номер группы студента: ";
    cin >> information.group;
    cout << "Введите 5 оценок за последнюю сессию: ";
    for (int i = 0; i < 5; i++) {
        cin >> information.marks[i];
    }
    cout << "Введите размер стипендии студента: ";
    cin >> information.stipend;
    cout << "Введите год рождения студента: ";
    cin >> information.yearOfBirth;
    id++;
    information.id = id;
    return information;
}

//преобразование строки в число
int toint(string str) {
    int* arr = new int[str.size()];
    for (int i = 0; i < str.size(); i++) {
        arr[i] = str[i] - 48;
    }
    int num = 0, k = 0;
    for (int i = str.size() - 1; i >= 0; i--, k++) {
        num += arr[i] * pow(10, k);
    }
    return num;
}

//преобразование строки в массив
int* tointArr(string str) {
    int arr[5] = {}, k = 0;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] != ' ') {
            arr[k] = str[i] - 48;
            k++;
        }
    }
    return arr;
}

//редактирование студента
void redactInformation(List<Information>& list, int id) {
    int num;
    cin >> num;
    for (int j = 0; j < list.count(); j++) {
        if (id == list.elementAt(j).id) {
            Information newinfo = list.elementAt(j);
            int value;
            string fio, f, i, o;
            cin >> num;
            switch (num) {
            case 1:
                cout << "Введите новое ФИО: ";
                cin >> f >> i >> o;
                fio = f + " " + i + " " + o;
                newinfo.name = fio;
                break;
            case 2:
                cout << "Введите новую группу: ";
                cin >> value;
                newinfo.group = value;
                break;
            case 3:
                cout << "Введите новые оценки: ";
                for (int i = 0; i < 5; i++) {
                    cin >> newinfo.marks[i];
                }
                break;
            case 4:
                cout << "Введите новый размер стипендии: ";
                cin >> value;
                newinfo.stipend = value;
                break;
            case 5:
                cout << "Введите новый год рождения: ";
                cin >> value;
                newinfo.yearOfBirth = value;
                break;
            }
            list.removeAt(j);
            list.add(newinfo);
            break;
        }
    }
}

//редактирование группы
void redactGroup(List<Information>& list, int group) {
    for (int j = 0; j < list.count(); j++) {
        if (group == list.elementAt(j).group) {
            Information newinfo = list.elementAt(j);
            int value;
            cout << "Введите новую группу: ";
            cin >> value;
            newinfo.group = value;
            list.removeAt(j);
            list.add(newinfo);
            break;
        }
    }
}

//фильтрация по заданному параметру
List<Information> filterStudents(List<Information> list, string parameter, string value) {
    List<Information> result;
    for (int i = 0; i < list.count(); i++) {
        Information information = list.elementAt(i);
        if (parameter == "name" && information.name == value) {
            result.add(information);
        }
        else if (parameter == "group" && information.group == toint(value)) {
            result.add(information);
        }
        else if (parameter == "marks" && information.marks == tointArr(value)) {
            result.add(information);
        }
        else if (parameter == "stipend" && information.stipend == toint(value)) {
            result.add(information);
        }
        else if (parameter == "year" && information.yearOfBirth == toint(value)) {
            result.add(information);
        }
    }
    return result;
}

//добавление группы
Information inputGroup(int group) {
    Information information;
    string f, i, o;
    cout << "Введите ФИО студента: ";
    cin >> f >> i >> o;
    information.name = f + " " + i + " " + o;
    information.group = group;
    cout << "Введите 5 оценок за последнюю сессию: ";
    for (int i = 0; i < 5; i++) {
        cin >> information.marks[i];
    }
    cout << "Введите размер стипендии студента: ";
    cin >> information.stipend;
    cout << "Введите год рождения студента: ";
    cin >> information.yearOfBirth;
    information.id = id;
    id++;
    return information;
}

//файловый ввод
void fileInput(List<Information>& list) {
    Information information;
    ifstream input;
    input.open("input.txt");
    string str;
    int numb = 0;
    int* array;
    while (getline(input, str)) {
        numb++;
        switch (numb) {
        case 1:
            information.name = str;
            break;
        case 2:
            information.group = toint(str);
            break;
        case 3:
            array = tointArr(str);
            for (int i = 0; i < 5; i++) {
                information.marks[i] = array[i];
            }
            break;
        case 4:
            information.stipend = toint(str);
            break;
        case 5:
            information.yearOfBirth = toint(str);
            id++;
            information.id = id;
            list.add(information);
            numb = 0;
        }
    }
    input.close();
}

//вывод отсортированных студентов с подходящей стипендией
void outputAlphabet(List<string> list) {
    if (list.count() == 0) {
        cout << "Нет ни одного студента cо стипендией, ниже средней" << endl;
        return;
    }
    for (int i = 0; i < list.count(); i++) {
        cout << list.elementAt(i) << endl;
    }
}

//вывод информации о студенте
void output(Information information) {
    cout << "ФИО студента: " << information.name << endl;
    cout << "Номер группы студента: " << information.group << endl;
    cout << "Оценки студента: ";
    for (int i = 0; i < 5; i++) {
        cout << information.marks[i] << " ";
    }
    cout << endl;
    cout << "Стипендия студента: " << information.stipend << endl;
    cout << "Год рождения студента: " << information.yearOfBirth << endl;
    cout << "ID студента: " << information.id << endl;
}

//вывод информации о студенте по заданному параметру
void outputStudent(List<Information>& list, int id) {
    for (int j = 0; j < list.count(); j++) {
        Information information = list.elementAt(j);
        if (id == list.elementAt(j).id) {
            output(information);
        }
    }
}

//вывод информации о группе
void outputGroup(int group, List <Information> list) {
    for (int i = 0; i < list.count(); i++) {
        if (list.elementAt(i).group == group) {
            output(list.elementAt(i));
        }
    }
}

//файловый вывод
void fileOutput(List<Information> list) {
    ofstream output;
    output.open("output.txt");
    for (int i = 0; i < list.count(); i++) {
        Information information = list.elementAt(i);
        output << information.name << endl;
        output << information.group << endl;
        for (int j = 0; j < 5; j++) { 
            output << information.marks[j] << " "; 
        }
        output << endl << information.stipend << endl;
        output << information.yearOfBirth << endl;
    }
}

//вывод информации о всех студентах
void outputAll(List<Information> list) {
    if (list.count() == 0) {
        cout << "Нет ни одного студента" << endl;
        return;
    }
    for (int i = 0; i < list.count(); i++) {
        output(list.elementAt(i));
        cout << endl;
    }
}

int main() {
    List<Information> listOfStudents;
    List<string> alphabet;
    system("chcp 1251>NULL");

    cout << "\tРАБОТА СО СТУДЕНТАМИ\n";
    cout << "1 - Добавление студента\n";
    cout << "2 - Удаление студента\n";
    cout << "3 - Редактирование данных студента\n";
    cout << "4 - Вывести информацию о студенте\n";
    cout << "5 - Вывести информацию о всех студентах\n";
    cout << "6 - Вывод списка студентов заданной группы со стипендией, ниже средней, в алфавитном порядке\n";
    cout << "\n\tРАБОТА С ГРУППАМИ\n";
    cout << "7 - Добавление группы студентов\n";
    cout << "8 - Удаление группы студентов\n";
    cout << "9 - Редактирование номера группы\n";
    cout << "10 - Вывести информацию о студентах группы\n";
    cout << "\n\tРАБОТА С ФАЙЛАМИ\n";
    cout << "11 - Файловый ввод данных о студентах\n";
    cout << "12 - Файловый вывод данных о студентах\n";
    cout << "\n\tЗАВЕРШЕНИЕ РАБОТЫ\n";
    cout << "13 - Очищение базы данных\n";
    cout << "0 - Завершение работы программы\n\n";


    while (true) {
        try {
            int numb, group, kol, id;
            string parameter, value, student;
            int k = 0;
            List<int> result;
            cin >> numb;
            switch (numb) {
            case 1:
                listOfStudents.add(input());
                cout << "Студент добавлен" << endl;
                break;
            case 2:
                cout << "1. Найти по ФИО (введите name)" << endl;
                cout << "2. Найти по группе (введите group)" << endl;
                cout << "3. Найти по оценкам (введите marks)" << endl;
                cout << "4. Найти по стипендии (введите stipend)" << endl;
                cout << "Выберите, по какому типу данных найти студента: ";
                cin >> parameter;
                cout << "Введите данные: ";
                cin >> value;
                cout << endl << endl;
                outputAll(filterStudents(listOfStudents, parameter, value));
                cout << "Введите ID подходящего для удаления студента";
                cin >> id;
                removeStudent(listOfStudents, id);
                cout << "Студент удален" << endl;
                break;
            case 3:
                cout << "1. Найти по ФИО (введите name)" << endl;
                cout << "2. Найти по группе (введите group)" << endl;
                cout << "3. Найти по оценкам (введите marks)" << endl;
                cout << "4. Найти по стипендии (введите stipend)" << endl;
                cout << "Выберите, по какому типу данных найти студента: ";
                cin >> parameter;
                cout << "Введите данные: ";
                cin >> value;
                cout << endl << endl;
                outputAll(filterStudents(listOfStudents, parameter, value));
                cout << endl;
                cout << "Введите ID подходящего для редактирования студента: ";
                cin >> id;
                cout << endl << "Выберите (цифра) параметр для изменения: ";
                cout << "1. Изменить ФИО" << endl;
                cout << "2. Изменить группу" << endl;
                cout << "3. Изменить оценки" << endl;
                cout << "4. Изменить стипендию" << endl;
                cout << "5. Изменить годрождения" << endl;
                redactInformation(listOfStudents, id);
                break;
            case 4:
                cout << "1. Найти по ФИО (введите name)" << endl;
                cout << "2. Найти по группе (введите group)" << endl;
                cout << "3. Найти по оценкам (введите marks)" << endl;
                cout << "4. Найти по стипендии (введите stipend)" << endl;
                cout << "Выберите, по какому типу данных найти студента: ";
                cin >> parameter;
                cout << "Введите данные: ";
                cin >> value;
                cout << endl << endl;
                cout << "Все подходящие по критерию студенты:" << endl;
                outputAll(filterStudents(listOfStudents, parameter, value));
                break;
            case 5:
                cout << "Информация о всех студентах потока:" << endl;
                outputAll(listOfStudents);
                break;
            case 6:
                cout << "Введите номер группы: ";
                cin >> group;
                cout << endl;
                cout << "Имена студентов со стипендией, ниже средней по потоку:" << endl;
                outputAlphabet(studentsWithLowStip(listOfStudents, group));
                break;
            case 7:
                cout << "Введите количество студентов, добавляемых в группу: ";
                cin >> kol;
                cout<< endl << "Введите группу, в которую надо добавить студентов: ";
                cin >> group;
                while (k < kol) {
                    listOfStudents.add(inputGroup(group));
                    k++;
                }
                break;
            case 8:
                cout << "Введите группу для удаления: ";
                cin >> group;
                removeGroup(group, listOfStudents);
                cout << "Группа удалена" << endl;
                break;
            case 9:
                cout << "1. Найти по ФИО (введите name)" << endl;
                cout << "2. Найти по группе (введите group)" << endl;
                cout << "3. Найти по оценкам (введите marks)" << endl;
                cout << "4. Найти по стипендии (введите stipend)" << endl;
                cout << "Выберите, по какому типу данных найти студента: ";
                cin >> parameter;
                cout << "Введите данные: ";
                cin >> value;
                cout << endl << endl;
                outputAll(filterStudents(listOfStudents, parameter, value));
                cout << "Введите группу подходящую для редактирования: ";
                cin >> group;
                cout << endl;
                redactGroup(listOfStudents, group);
                break;
            case 10:
                cout << "Введите группу, чтобы узнать информацию о её студентах: ";
                cin >> group;
                outputGroup(group, listOfStudents);
                break;
            case 11:
                fileInput(listOfStudents);
                cout << "Студенты добавлены" << endl;
                break;
            case 12:
                fileOutput(listOfStudents);
                cout << "Данные выведены в файл" << endl;
                break;
            case 13:
                listOfStudents.clear();
                cout << "База данных очищена" << endl;
                break;
            case 0:
                return 0;
            default:
                cout << "Ошибка!!! Введите корректный номер команды!";
                break;
            }
        }
        catch (int error) {
            if (error == 0) {
                cout << "Введен некорректный индекс!";
            }
        }
    }
}