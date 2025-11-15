#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <cctype>
#include <fstream>
#include <cstdlib>
#include <limits>

#if defined(_WIN32)
  #include <windows.h>
  void enableVTMode() {
      HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
      if (hOut == INVALID_HANDLE_VALUE) return;
      DWORD dwMode = 0;
      if (!GetConsoleMode(hOut, &dwMode)) return;
      dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
      SetConsoleMode(hOut, dwMode);
  }
  #define CLEAR_CMD "cls"
#else
  #define CLEAR_CMD "clear"
  void enableVTMode() {}
#endif

using namespace std;

/*
=============================
Agenda Médica con Login (C++)
=============================
MediSchedule :0
Autores: Alejandro Ramirez y Pedro Gonzales
Descripción breve: consola con login (Admin/Paciente) para crear, ver y cancelar citas.
Almacena texto en 'citas.txt' y muestra un calendario simple con día resaltado.
*/

// === Colores ANSI simples ===
#define RED   "\033[31m"
#define GREEN "\033[32m"
#define YEL   "\033[33m"
#define CYAN  "\033[36m"
#define BOLD  "\033[1m"
#define RST   "\033[0m"

// Estructura para almacenar los datos de cada cita médica.
struct CitaMedica {
    string nombre;
    int    edad = 0;
    string tel;
    int    SS = 0;
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
    if ((usr == "Admin"    && cont == "admin123") ||
        (usr == "Paciente" && cont == "paciente123")) {
        return true;
    }
    return false;
}

// Buscar y mostrar citas según nombre del paciente o doctor.
void verCitasPorNombre(string nombre) {
    ifstream archivo("data/citas.txt");
    string lineaDtexto;
    bool nomEncontrado = false;

    if (!archivo) {
        cout << RED "Hubo un error al abrir el archivo, favor de intentar de nuevo :)." RST "\n";
        return;
    }
    while (getline(archivo, lineaDtexto)) {
        if (lineaDtexto.find(nombre) != string::npos) {
            nomEncontrado = true;
            cout << CYAN "-----------------" RST;
            cout << CYAN "| " << nombre << " |" RST;
            cout << CYAN "-----------------" RST "\n";
            for (int i = 0; i < 10 && getline(archivo, lineaDtexto); i++) {
                cout << lineaDtexto << '\n';
            }
        }
    }
    if (!nomEncontrado) {
        cout << YEL "El nombre entrado no se encuentra en el archivo :(." RST "\n";
    }
}

// Mostrar calendario con opción de resaltar un día en rojo
void Calendario(int /*mes*/, int diasDelMes, int primerDiaMes, string nombreDmes,
                int añoActual, vector<string>& diasDeSemana, int diaCita = -1) {
    cout << "\n" BOLD "[ " << nombreDmes << " " << añoActual << " ]" RST "\n";
    cout << "---------------------------------------\n";
    for (const string& dia : diasDeSemana) {
        cout << "| " << dia[0] << "  ";
    }
    cout << "|\n";
    cout << "---------------------------------------\n";

    int dia = 1;
    for (int i = 0; i < primerDiaMes; ++i) cout << "|    ";

    while (dia <= diasDelMes) {
        for (int i = primerDiaMes; i < 7 && dia <= diasDelMes; ++i) {
            if (dia == diaCita)
                cout << "| " RED << setw(2) << dia << RST " ";
            else
                cout << "| " << setw(2) << dia << " ";
            dia++;
        }
        cout << "|\n";
        cout << "---------------------------------------\n";
        primerDiaMes = 0;
    }
}

// Crear una nueva cita médica, guardarla en archivo y mostrar calendario
void crearCitaNueva(vector<string>& opcEsp, int añoActual, vector<string>& diasDeSemana) {
    CitaMedica cita;
    ofstream archivo("data/citas.txt", ios::app);
    if (!archivo) {
        cout << RED "Error al abrir el archivo." RST "\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Nombre del paciente: ";
    getline(cin, cita.nombre);

    cout << "Entre su edad: ";
    while (!(cin >> cita.edad)) {
        cout << YEL "Edad inválida. Intente de nuevo: " RST;
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Entre su número de teléfono: ";
    getline(cin, cita.tel);

    cout << "Entre los últimos 4 dígitos del Seguro Social: ";
    while (!(cin >> cita.SS)) {
        cout << YEL "Entrada inválida. Intente de nuevo: " RST;
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    char tieneSM;
    cout << "¿Tiene seguro médico? (s/n): ";
    cin >> tieneSM;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    tieneSM = static_cast<char>(tolower(static_cast<unsigned char>(tieneSM)));
    if (tieneSM == 's') {
        cout << "Ingrese su seguro médico: ";
        getline(cin, cita.SM);
    } else {
        cita.SM = "El paciente no tiene seguro medico";
    }

    cout << "Fecha de la cita (YYYY/MM/DD): ";
    getline(cin, cita.fecha);
    int mesCita = stoi(cita.fecha.substr(5, 2));
    int diaCita = stoi(cita.fecha.substr(8, 2));

    cout << "Hora de la cita: ";
    getline(cin, cita.hora);

    cout << "Especialidad (seleccione una): \n";
    for (int i = 0; i < static_cast<int>(opcEsp.size()); i++)
        cout << i + 1 << ". " << opcEsp[i] << '\n';

    int opt;
    cin >> opt; cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (opt >= 1 && opt <= static_cast<int>(opcEsp.size()))
        cita.especialidad = opcEsp[opt - 1];
    else
        cita.especialidad = "Otro";

    cout << "Motivo de la cita: ";
    getline(cin, cita.motivo);
    cout << "Nombre del doctor: ";
    getline(cin, cita.doctor);

    // Guardar cita en archivo
    archivo << "Nombre: " << cita.nombre << '\n';
    archivo << "Edad: " << cita.edad << '\n';
    archivo << "Teléfono: " << cita.tel << '\n';
    archivo << "SS (últimos 4): " << cita.SS << '\n';
    archivo << "Seguro Médico: " << cita.SM << '\n';
    archivo << "Fecha: " << cita.fecha << '\n';
    archivo << "Hora: " << cita.hora << '\n';
    archivo << "Especialidad: " << cita.especialidad << '\n';
    archivo << "Motivo: " << cita.motivo << '\n';
    archivo << "Doctor: " << cita.doctor << '\n';
    archivo << "------------------------\n";
    archivo.close();

    cout << GREEN "\nCita guardada correctamente.\n" RST;

    // Mostrar calendario con día resaltado
    if (mesCita == 4)      Calendario(4, 30, 2, "abril", añoActual, diasDeSemana, diaCita);
    else if (mesCita == 5) Calendario(5, 31, 4, "mayo",  añoActual, diasDeSemana, diaCita);
    else if (mesCita == 6) Calendario(6, 30, 0, "junio", añoActual, diasDeSemana, diaCita);
}

// Eliminar cita médica por nombre
void cancelarCita(string nombre) {
    ifstream archivo("data/citas.txt");
    ofstream archivoTemporero("data/copia.txt");
    string lineaDtexto;
    bool nomEncontrado = false;

    if (!archivo || !archivoTemporero) {
        cout << RED "Error al abrir el archivo :(." RST "\n";
        return;
    }
    while (getline(archivo, lineaDtexto)) {
        if (lineaDtexto.find(nombre) != string::npos) {
            nomEncontrado = true;
            for (int i = 0; i < 10 && getline(archivo, lineaDtexto); i++) {
                // saltar las siguientes líneas de la cita
            }
        } else {
            archivoTemporero << lineaDtexto << '\n';
        }
    }
    archivo.close();
    archivoTemporero.close();
    remove("data/citas.txt");
    rename("data/copia.txt", "data/citas.txt");

    if (!nomEncontrado) {
        cout << YEL "Cita no se pudo encontrar con ese nombre :(." RST "\n";
    } else {
        cout << GREEN "Cita fue cancelada adecuadamente :)." RST "\n";
    }
}

// Función principal con menú de navegación para doctor o paciente
int main() {
    enableVTMode(); // colores en Windows 10+
    int Opcion;
    const int year = 2025;
    string mes, NomPac, NomDr, user, pass;
    bool res;

    // Vectores para mostrar especialidades y los días de la semana
    vector<string> especialidades = { "Cardiologia", "Pediatria", "Dermatologia", "Neurologia", "Otro" };
    vector<string> diasSemana = { "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado", "Domingo" };

    do {
        cout << BOLD "*********\n* Login *\n*********\n" RST;
        cout << "Usuario(Username): ";
        cin >> user;
        cout << "Contraseña(Password): ";
        cin >> pass;
        res = loginCheck(user, pass);
        if (!res) {
            system(CLEAR_CMD);
            cout << RED "\nHubo un error con sus credenciales, Favor de tratar de nuevo :)\n" RST;
        }
    } while (!res);

    if (res && user == "Admin") {
        do {
            system(CLEAR_CMD);
            cout << BOLD "Bienvenido Doctor/a :)\nQue desea hacer?\n" RST;
            MenuDr();
            cout << "Elija una opcion: ";
            cin >> Opcion;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            switch (Opcion) {
            case 1:
                cout << CYAN "Ver citas por paciente\n" RST;
                cout << "Cual es el nombre del paciente: ";
                getline(cin, NomPac);
                verCitasPorNombre(NomPac);
                break;
            case 2:
                cout << CYAN "Ver citas por doctor\n" RST;
                cout << "Cual es el nombre del doctor: ";
                getline(cin, NomDr);
                verCitasPorNombre(NomDr);
                break;
            case 3:
                cout << CYAN "Calendario\n" RST;
                cout << "Introduzca un mes entre abril y junio (en minuscula): ";
                cin >> mes;
                if (mes == "abril")       Calendario(4, 30, 2, mes, year, diasSemana);
                else if (mes == "mayo")   Calendario(5, 31, 4, mes, year, diasSemana);
                else if (mes == "junio")  Calendario(6, 30, 0, mes, year, diasSemana);
                else                      cout << YEL "Favor de entrar un mes valido de Abril a Junio.\n" RST;
                break;
            case 4:
                cout << CYAN "Eliminar citas\n" RST;
                cout << "Introduzca el nombre del paciente: ";
                getline(cin, NomPac);
                cancelarCita(NomPac);
                break;
            case 5:
                cout << "Saliendo del programa...\n";
                cout << GREEN "\n\nGracias por usar nuestro programa! :)\n\nBye Bye!!\n" RST;
                break;
            default:
                cout << YEL "Opcion no valida, favor de entrar un numero del 1-5\n" RST;
            }
        } while (Opcion != 5);
    } else if (res && user == "Paciente") {
        do {
            cout << BOLD "Bienvenido querido paciente, que desea hacer?\n" RST;
            MenuPac();
            cout << "Elija una opcion: ";
            cin >> Opcion;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            switch (Opcion) {
            case 1:
                cout << CYAN "Ver citas\n" RST;
                cout << "Cual es el nombre del paciente?\n";
                getline(cin, NomPac);
                verCitasPorNombre(NomPac);
                break;
            case 2:
                cout << CYAN "Crear citas\n" RST;
                crearCitaNueva(especialidades,  year, diasSemana);
                break;
            case 3:
                cout << CYAN "Cancelar cita\n" RST;
                cout << "Cual es el nombre del paciente?\n";
                getline(cin, NomPac);
                cancelarCita(NomPac);
                break;
            case 4:
                cout << "Saliendo del programa...\n";
                cout << GREEN "Gracias por usar nuestro programa! :)\n\nBye Bye!!\n" RST;
                break;
            default:
                cout << YEL "Opcion no valida, favor de entrar un numero del 1-4.\n" RST;
            }
        } while (Opcion != 4);
    }
    return 0;
}
