#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "stdio.h"
#include <cmath>

#define N 1000
using namespace std;
double time_for_message(double m_lamba)//Время, через которое пришло следующее сообщение
{
    double U, T;
    U = (double)(rand()) / RAND_MAX;
    T = (double)(-log(U) / m_lamba);
    return T;
}

int main()
{
    FILE* res;
    double Time_For_Last = 0;//Время, когда закончило отправляться последнее сообщение
    double Time_For_New = 0;//Время, когда пришло новое сообщение
    double Delay_Sync = 0;//Задержка для синхронной сисемы
    double Delay_ASync = 0;//Задержка для асинхронной системы
    double Average_Delay_Sync = 0;//Средняя задержка для синхронной системы 
    double Average_Delay_ASync = 0;//Средняя задержка для асинхроной системы
    int windows_count = 0;
    double Requests;//Запросы
    double RequestsT;//Запросы теоретические
    double m_lamba = 0.0001;//Лямбда
    double Average_Delay_SyncT;//Средняя теоретическая задержка для синхронной системы
    double Average_Delay_ASyncT;//Средняя теоретическая задержка для асинхроной системы
    res = fopen("results.csv", "w");//Открыть файл для записи результатов
    fprintf(res, "Lambda_Delay_ASync;Delay_Sync;Delay_Sync_Theory;Delay_ASync;Delay_Async_Theory;N(l);N()Theory\n");//Вывод названий столбцов
    while (m_lamba < 1)//Обход всех лямбд
    {

        Time_For_Last = 0;
        Time_For_New = 0;
        for (int i = 0; i < N; i++)
        {
            Time_For_New = Time_For_Last + time_for_message(m_lamba);//Время прихода нового сообщения равняется текущему времени плюс время через которое пришло новое сообщение

            if (i == N - 1)
            {
                windows_count = ceil(Time_For_New);
            }
            if (Time_For_New > Time_For_Last + Delay_Sync)
            {
                Delay_ASync = 1;//Т.к система асинхронная, мы не будем ждать нового окна, а передадим сразу
                Delay_Sync = 1 + ceil(Time_For_New) - Time_For_New; // Т.к Система синхронная, мы дождемся нового окна и отправим
            }
            else
            {
                Delay_ASync = 1 + Delay_ASync - (Time_For_New - Time_For_Last);  //Перед отправкой нового сообщения мы дождемся отправки прошлого + время окна
                Delay_Sync = 1 + Delay_Sync - (Time_For_New - Time_For_Last);  //Перед отправкой нового сообщения мы дождемся отправки прошлого + время окна
            }
            Time_For_Last = Time_For_New;


            Average_Delay_ASync += Delay_ASync;
            Average_Delay_Sync += Delay_Sync;
        }
        Average_Delay_Sync /= N;
        Average_Delay_ASync /= N;

        RequestsT = ((m_lamba) * (2 - (m_lamba))) / (2 * (1 - (m_lamba)));
        Average_Delay_ASyncT = (double)RequestsT / m_lamba;
        Average_Delay_SyncT = Average_Delay_ASyncT + 0.5;
        Requests = (double)N / windows_count;
        fprintf(res, " %4.6f; %4.6f; %4.6f; %4.6f; %4.6f; %4.6f; %4.6f\n", m_lamba, Average_Delay_Sync, Average_Delay_SyncT, Average_Delay_ASync, Average_Delay_ASyncT, Requests, RequestsT);
        m_lamba += 0.1;
    }


    return 0;
}