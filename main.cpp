/*
=============================
Agenda Médica con Login (C++)
=============================
MediSchedule :0
=============================
Alejandro Ramirez y Pedro Gonzales :)
=============================
Este programa permite:
•⁠  ⁠Iniciar sesión con un login para ambos doctores, como pacientes.
•⁠  ⁠Crear, ver, eliminar citas médicas.
•⁠  ⁠Ver un calendario con resaltado de fechas(no se pudo terminar).
•⁠  ⁠Crea archivos de texto para organizar las citas de los pacientes.
*/
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <cctype>
#include <fstream>
#include <cstdlib>
using namespace std;
// Estructura para almacenar los datos de cada cita médica.
struct CitaMedica {
    string nombre;
    int edad = 0; //Se le implico 0 por razon de un error reucurrente.
    string tel;
    int SS = 0; //igual aqui
    string SM;
    string fecha;
    string hora;
    string especialidad;
    string motivo;
    string doctor;
};

// Menú para doctores.
void MenuDr() {
    cout << "1. Ver citas por paciente\n";
    cout << "2. Ver citas por doctor\n";
    cout << "3. Calendario\n";
    cout << "4. Eliminar citas\n";
    cout << "5. Salir\n";
}

// Menú para pacientes.
void MenuPac() {
    cout << "1. Ver citas\n";
    cout << "2. Crear citas\n";
    cout << "3. Cancelar cita\n";
    cout << "4. Salir\n";
}

// Función de login: define si el usuario es doctor o paciente usando la variable check.
bool loginCheck(string usr, string cont) {
    bool check = false;
    if (usr == "Admin" && cont == "admin123") {
        check = true;
    }
    else if (usr == "Paciente" && cont == "paciente123") {
        check = true;
    }
    else {
        check = false;
    }
    return check;
}

// Buscar y mostrar citas según nombre del paciente o doctor.
void verCitasPorNombre(string nombre) {
    ifstream archivo("citas.txt");
    string lineaDtexto;
    bool nomEncontrado = false;
    if (!archivo) {
        cout << "Hubo un error al abrir el archivo, favor de intentar de nuevo :).\n";
        return;
    }
    while (getline(archivo, lineaDtexto)) {
        if (lineaDtexto.find(nombre) != string::npos) {
            nomEncontrado = true;
            if (nomEncontrado == true) {
                cout << "-----------------";
                cout << "| " << nombre << " |";
                cout << "-----------------";
                for (int i = 0; i < 10; i++) {
                    getline(archivo, lineaDtexto);
                    cout << lineaDtexto << endl;
                }
            }
        }
    }
    if (nomEncontrado == false) {
        cout << "El nombre entrado no se encuentra en el archivo :(.\n";
    }
}

// Mostrar calendario con opción de resaltar un día en rojo
void Calendario(int mes, int diasDelMes, int primerDiaMes, string nombreDmes, int añoActual, vector<string>& diasDeSemana, int diaCita = -1) {
    cout << "\n[ " << nombreDmes << " " << añoActual << " ]" << endl;
    cout << "---------------------------------------" << endl;
    for (const string& dia : diasDeSemana) {
        cout << "| " << dia[0] << "  ";
    }
    cout << "|" << endl;
    cout << "---------------------------------------" << endl;

    int dia = 1;
    for (int i = 0; i < primerDiaMes; ++i) cout << "|    ";

    while (dia <= diasDelMes) {
        for (int i = primerDiaMes; i < 7 && dia <= diasDelMes; ++i) {
            if (dia == diaCita)
                cout << "| \033[31m" << setw(2) << dia << "\033[0m "; // Día resaltado en rojo
            else
                cout << "| " << setw(2) << dia << " ";
            dia++;
        }
        cout << "|" << endl;
        cout << "---------------------------------------" << endl;
        primerDiaMes = 0;
    }
}

// Crear una nueva cita médica, guardarla en archivo y mostrar calendario
void crearCitaNueva(vector<string>& opcEsp, int añoActual, vector<string>& diasDeSemana) {
    CitaMedica cita;
    ofstream archivo("citas.txt", ios::app);

    if (!archivo) {
        cout << "Error al abrir el archivo." << endl;
        return;
    }

    cin.ignore();
    cout << "Nombre del paciente: ";
    getline(cin, cita.nombre);
    cout << "Entre su edad: ";
    while (!(cin >> cita.edad)) {
        cout << "Edad inválida. Intente de nuevo: ";
        cin.clear(); cin.ignore(1000, '\n');
    }
    cin.ignore();
    cout << "Entre su número de teléfono: ";
    getline(cin, cita.tel);
    cout << "Entre los últimos 4 dígitos del Seguro Social: ";
    while (!(cin >> cita.SS)) {
        cout << "Entrada inválida. Intente de nuevo: ";
        cin.clear(); cin.ignore(1000, '\n');
    }
    cin.ignore();

    char tieneSM;
    cout << "¿Tiene seguro médico? (s/n): ";
    cin >> tieneSM; cin.ignore();
    tieneSM = tolower(tieneSM);
    if (tieneSM == 's') {
        cout << "Ingrese su seguro médico: ";
        getline(cin, cita.SM);
    }
    else {
        cita.SM = "El paciente no tiene seguro medico";
    }

    cout << "Fecha de la cita (YYYY/MM/DD): ";
    getline(cin, cita.fecha);
    int mesCita = stoi(cita.fecha.substr(5, 2));
    int diaCita = stoi(cita.fecha.substr(8, 2));

    cout << "Hora de la cita: ";
    getline(cin, cita.hora);

    cout << "Especialidad (seleccione una): \n";
    for (int i = 0; i < opcEsp.size(); i++)
        cout << i + 1 << ". " << opcEsp[i] << endl;
    int opt;
    cin >> opt; cin.ignore();
    if (opt < 1 || opt > 5) {
        cita.especialidad = "Otro";
    }
    cout << "Motivo de la cita: ";
    getline(cin, cita.motivo);
    cout << "Nombre del doctor: ";
    getline(cin, cita.doctor);

    // Guardar cita en archivo
    archivo << "Nombre: " << cita.nombre << endl;
    archivo << "Edad: " << cita.edad << endl;
    archivo << "Teléfono: " << cita.tel << endl;
    archivo << "SS (últimos 4): " << cita.SS << endl;
    archivo << "Seguro Médico: " << cita.SM << endl;
    archivo << "Fecha: " << cita.fecha << endl;
    archivo << "Hora: " << cita.hora << endl;
    archivo << "Especialidad: " << cita.especialidad << endl;
    archivo << "Motivo: " << cita.motivo << endl;
    archivo << "Doctor: " << cita.doctor << endl;
    archivo << "------------------------" << endl;
    archivo.close();

    cout << "\nCita guardada correctamente." << endl;

    // Mostrar calendario con día resaltado
    if (mesCita == 4) Calendario(4, 30, 2, "abril", añoActual, diasDeSemana, diaCita);
    else if (mesCita == 5) Calendario(5, 31, 4, "mayo", añoActual, diasDeSemana, diaCita);
    else if (mesCita == 6) Calendario(6, 30, 0, "junio", añoActual, diasDeSemana, diaCita);
}

// Eliminar cita médica por nombre
void cancelarCita(string nombre) {
    ifstream archivo("citas.txt");
    ofstream archivoTemporero("copia.txt");
    string lineaDtexto;
    bool nomEncontrado = false;
    if (!archivo || !archivoTemporero) {
        cout << "Error al abrir el archivo :(." << endl;
        return;
    }
    while (getline(archivo, lineaDtexto)) {
        if (lineaDtexto.find(nombre) != string::npos) {
            nomEncontrado = true;
            for (int i = 0; i < 10; i++) {
                getline(archivo, lineaDtexto);
            }
        }
        else {
            archivoTemporero << lineaDtexto << endl;
        }
    }
    archivo.close();
    archivoTemporero.close();
    remove("citas.txt");
    rename("copia.txt", "citas.txt");
    if (nomEncontrado == false) {
        cout << "Cita no se pudo encontrar con ese nombre :(." << endl;
    }
    else {
        cout << "Cita fue cancelada adecuadamente :)." << endl;
    }
}
// Función principal con menú de navegación para doctor o paciente
int main() {
    int Opcion;
    const int year = 2025;
    string mes, NomPac, NomDr, user, pass;
    bool res;
    // Vectores para mostrar especialidades y los días de la semana
    vector<string> especialidades = { "Cardiologia", "Pediatria", "Dermatologia", "Neurologia", "Otro"};
    vector<string> diasSemana = { "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado", "Domingo" };
    do {
        cout << "*********\n";
        cout << "* Login *\n";
        cout << "*********\nUsuario(Username): ";
        cin >> user;
        cout << "Contraseña(Password): ";
        cin >> pass;
        res = loginCheck(user, pass);
        if (res == false) {
            system("clear");
            cout << "\nHubo un error con sus credenciales, Favor de tratar de nuevo :)\n";
        }
    } while (res == false);
    if (res == true && user == "Admin") {
        do {
            system("clear");
            cout << "Bienvenido Doctor/a :)\nQue desea hacer?\n";
            MenuDr();
            cout << "Elija una opcion: ";
            cin >> Opcion;
            cin.ignore();
            switch (Opcion) {
            case 1:
                cout << "Ver citas por paciente\n";
                cout << "Cual es el nombre del paciente: ";
                cin.ignore();
                getline(cin, NomPac);
                verCitasPorNombre(NomPac);
                break;
            case 2:
                cout << "Ver citas por doctor\n";
                cout << "Cual es el nombre del doctor: ";
                cin.ignore();
                getline(cin, NomDr);
                verCitasPorNombre(NomDr);
                break;
            case 3:
                cout << "Calendario\n";
                cout << "Introduzca un mes entre abril y junio (en minuscula):";
                cin >> mes;
                if (mes == "abril") {
                    Calendario(4, 30, 2, mes, year, diasSemana);
                }
                else if (mes == "mayo") {
                    Calendario(5, 31, 4, mes, year, diasSemana);
                }
                else if (mes == "junio") {
                    Calendario(6, 30, 0, mes, year, diasSemana);
                }
                else {
                    cout << "Favor de entrar un mes valido de Abril a Junio.\n";
                }
                break;
            case 4:
                cout << "Eliminar citas\n";
                cout << "Introduzca el nombre del paciente: ";
                cin.ignore();
                getline(cin, NomPac);
                cancelarCita(NomPac);
                break;
            case 5:
                cout << "Saliendo del programa...\n";
                cout << "\n\nGracias por usar nuestro programa! :)\n\nBye Bye!!\n";
                break;
            default:
                cout << "Opcion no valida, favor de entrar un numero del 1-5\n";
            }
        } while (Opcion != 5);
    }
    else if (res == true && user == "Paciente") {
        do {
            cout << "Bienvenido querido paciente, que desea hacer?\n";
            MenuPac();
            cout << "Elija una opcion: ";
            cin >> Opcion;
            cin.ignore();
            switch (Opcion) {
            case 1:
                cout << "Ver citas\n";
                cout << "Cual es el nombre del paciente?\n";
                cin.ignore();
                getline(cin, NomPac);
                verCitasPorNombre(NomPac);
                break;
            case 2:
                cout << "Crear citas\n";
                crearCitaNueva(especialidades, year, diasSemana);
                break;
            case 3:
                cout << "Cancelar cita\n";
                cout << "Cual es el nombre del paciente?\n";
                cin.ignore();
                getline(cin, NomPac);
                cancelarCita(NomPac);
                break;
            case 4:
                cout << "Saliendo del programa...\n";
                cout << "Gracias por usar nuestro programa! :)\n\nBye Bye!!\n";
                break;
            default:
                cout << "Opcion no valida, favor de entrar un numero del 1-4.\n";
            }
        } while (Opcion != 4);
    }
    return 0;
}
