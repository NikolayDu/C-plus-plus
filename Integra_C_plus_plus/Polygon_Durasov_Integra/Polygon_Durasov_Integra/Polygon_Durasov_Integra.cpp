#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream> // для split
using namespace std;

/* Класс Polygon хранит в себе поле вектор с координатами полигона, а так же 4 метода:
    1) printVectorPolygon;
    2) addNewPointToCoordVector;
    3) readPointsFromFileToCoordVector;
    4) checkRectanglePolygon.
*/
class Polygon {
private:
    vector <vector <int>> coordVectorPolygon;// Вектор с координатами полигона.
public:
    // Метод, который позволяет вывести в консоль вектор с координатами полигона.
    void printVectorPolygon(vector <vector <int>> sample) {
        for (int i = 0; i < sample.size(); i++) {
            for (int j = 0; j < sample[i].size(); j++) {
                cout << sample[i][j] << " ";
            }
            cout << endl;
        }
    }

    // Метод, который позволяет добавить новую точку в вектор с координатами полигона.
    vector <vector<int>> addNewPointToCoordVector(vector <vector<int>> coordinatesVector) {
        int x, y;
        cout << "Введите координаты точки, которую вы хотите добавить." << endl;
        cout << "Координата x: ";
        cin >> x;
        cout << "Координата y: ";
        cin >> y;
        coordinatesVector.push_back({ x, y });
        return coordinatesVector;
    }

    // Метод, который позволяет считать координаты полигона из файла в вектор.
    vector <vector <int>> readPointsFromFileToCoordVector(string filePath) {
        vector <string> stringData; // Вектор, в котором будут записаны координаты в виде строк и далее преобразовываться в координаты
        string line;
        ifstream polygonCoordinates(filePath);
        if (!polygonCoordinates) {
            cout << "Невозможно открыть указанный файл." << endl;
            system("pause");
        }
        while (getline(polygonCoordinates, line))
        {
            stringData.push_back(line);
        }

        // Далее из stringData со строковыми значениями я делаю вектор со значениями int
        vector <int> intCoordPolygon;
        for (int i = 0; i < stringData.size(); i++) {
            stringstream ss(stringData[i]);
            string del;
            while (getline(ss, del, ' ')) {
                intCoordPolygon.push_back((stoi(del)));
            }
        }

        // И с помощью обычного вектора с Int значениями точек я делаю vector <vector<int>>, чтобы точки были упорядочены по парам
        for (int i = 0; i < intCoordPolygon.size(); i += 2) {
            coordVectorPolygon.push_back({ intCoordPolygon[i], intCoordPolygon[i + 1] });
        }

        return coordVectorPolygon;
    
    }

    // Метод, который выводит конечный флаг с ответом в консоль, где находится прямоугольник относительно полигона
    string checkRectanglePolygon(vector <vector<int>> coordinatesVector, int *LB, int *LT, int *RT, int *RB, int *CTR) {

        coordinatesVector.push_back(coordinatesVector[0]); // Первая координата нужна, чтобы замкнуть полигон (алгоритм постоен на проверке пересечений с РЁБРАМИ)
        /*
        ФЛАГИ:
        "Полигон не пересекается с прямоугольником".
        "Полигон включает какую-то точку прямоугольника".
        "Полигон включает все точки прямоугольника".
        "Полигон включает центр прямоугольника".
        */
        string flag = "Полигон не пересекается с прямоугольником"; // По умолчанию флаг будет такой.
        int cnt1 = 0, cnt2 = 0, cnt3 = 0, cnt4 = 0, cnt5 = 0; // Это костыль, от которого нужно избавиться (по-хорошему). Это счётчики,
                                                              //которые нужны для подсчёта количества пересечений через рёбра многоугольника (полигона).
        double x1, x2, y1, y2; // ((x1, y1), (x2, y2)) - координаты ребра
        double xp, yp; // Координаты точки прямоугольника. Далее они будут преприсваиваться.
        vector <int> temp1; 
        vector <int> temp2;

        for (int i = 0; i < coordinatesVector.size() - 1; i++) {
            temp1 = coordinatesVector[i];
            temp2 = coordinatesVector[i+1];

            x1 = temp1[0];
            y1 = temp1[1];
            x2 = temp2[0];
            y2 = temp2[1];
            
            xp = LB[0];
            yp = LB[1];

            // Это условия для определения пресечения точки с ребром.
            if (((yp < y1) != (yp < y2)) && (xp < (x1 + (((yp - y1) / (y2 - y1)) * (x2 - x1))))) {
                cnt1 += 1;
            }

            xp = LT[0];
            yp = LT[1];
            // Это условия для определения пресечения точки с ребром.
            if (((yp < y1) != (yp < y2)) && (xp < (x1 + (((yp - y1) / (y2 - y1)) * (x2 - x1))))) {
                cnt2 += 1;
            }

            xp = RT[0];
            yp = RT[1];
            // Это условия для определения пресечения точки с ребром.
            if (((yp < y1) != (yp < y2)) && (xp < (x1 + (((yp - y1) / (y2 - y1)) * (x2 - x1))))) {
                cnt3 += 1;
            }

            xp = RB[0];
            yp = RB[1];
            // Это условия для определения пресечения точки с ребром.
            if (((yp < y1) != (yp < y2)) && (xp < (x1 + (((yp - y1) / (y2 - y1)) * (x2 - x1))))) {
                cnt4 += 1;
            }

            xp = CTR[0];
            yp = CTR[1];
            if (((yp < y1) != (yp < y2)) && (xp < (x1 + (((yp - y1) / (y2 - y1)) * (x2 - x1))))) {
                cnt5 += 1;
            }
        }

        // Проверки для флагов
        if (((cnt1 % 2) == 1) && ((cnt2 % 2) == 1) && ((cnt3 % 2) == 1) && ((cnt4 % 2) == 1)) {
            flag = "Полигон включает все точки прямоугольника.";
            return flag;
        }
        else if ((cnt5 % 2) == 1) {
            flag = "Полигон включает центр прямоугольника.";
            return flag;
        }
        else if (((cnt1 % 2) == 1) || ((cnt2 % 2) == 1) || ((cnt3 % 2) == 1) || ((cnt4 % 2) == 1)) {
            flag = "Полигон включает какую-то точку прямоугольника.";
            return flag;
        }
        else {
            return flag;
        }
    }
};

int main() {
    setlocale(LC_ALL, "Rus");
    
    int recX, recY, height, width; // Данные прямоугольинка, которые потом считываются с клавиатуры
    string filePath; // Путь к файлу, в котором хранятся координаты полигона

    cout << "Добро пожаловать в программу для определения пересечения прямоугольника с полигоном!\n" << endl;
    cout << "Введите данные исходного прямоугольника." << endl;
    cout << "Координата x: ";
    cin >> recX;
    cout << "Координата y: ";
    cin >> recY;
    cout << "Высота: ";
    cin >> height;
    cout << "Ширина: ";
    cin >> width;
    cout << endl;

    cout << "Введите путь к файлу, откуда будет считываться координаты полигона: ";
    cin >> filePath;
    cout << endl;



    // Координаты точек прямоугольинка  (включая его центр)
    int leftBottom[] = {recX, recY};
    int leftTop[] = { recX, recY + height };
    int rightTop[] = { recX + width, recY + height };
    int rightBottom[] = { recX + width, recY };
    int Centre[] = { recX + width / 2, recY + height / 2 };

    Polygon firstPolygon;
    vector <vector<int>> coordinatesVector = firstPolygon.readPointsFromFileToCoordVector(filePath);


    //Чтобы добавить новую точку в полигон нужно переприсваивать вектор, в котором находятся координата полигона
    //coordinatesVector = firstPolygon.addNewPointToCoordVector(coordinatesVector);

   
    string result = firstPolygon.checkRectanglePolygon(coordinatesVector, leftBottom, leftTop, rightTop, rightBottom, Centre);
    cout << "================================================" << endl;
    cout << result << endl;
    cout << "================================================" << endl;

    system("PAUSE");
} 