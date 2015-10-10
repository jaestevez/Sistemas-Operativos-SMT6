#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>

using namespace std;

const string FILE_DIR_NAME = "/home/jestevez/file_text.txt";
mutex mtx;
mutex db;

int cl = 0;

void file_write();
void file_read();

void lector()
{
    //while(1)
    {
        mtx.lock();
        cl++;
        db.try_lock();
        mtx.unlock();
        file_read();
        mtx.lock();
        cl--;
        if(cl == 0) db.unlock();
        mtx.unlock();
    }
}
void escritor()
{
    //while(1)
    {
        db.lock();
        file_write();
        db.unlock();
    }
}

int main()
{

    thread th1 (lector);
    thread th2 (escritor);
    thread th3 (lector);
    thread th4 (lector);
    thread th5 (escritor);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();

    return 0;
}

void file_write()
{
    cout<<"escribiendo"<<endl;
    ofstream fs;
    fs.open(FILE_DIR_NAME, ofstream::app);
    fs << "Sistemas Operativos\n";
    fs.close();
}

void file_read()
{
    char cadena[128];
    ifstream fe(FILE_DIR_NAME);

    string tx = "leyendo\n--------\n";
    unsigned int n = 1;
    if(fe.fail())
        cerr << "Error al abrir el archivo" << endl;
    else
        while(!fe.eof())
        {
            fe.getline(cadena, sizeof(cadena));
            tx += to_string(n)+"\t";
            tx += cadena;
            tx += "\n";
            n++;
        }
    fe.close();
    tx += "\n--------\n";
    cout<<tx;
}
